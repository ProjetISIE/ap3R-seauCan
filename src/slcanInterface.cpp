#include "slcanInterface.hpp"
#include <algorithm>
#include <cerrno>
#include <cstring>
#include <format>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <print>
#include <stdexcept>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

/**
 * @brief Opens and binds CAN socket with timeout
 * @param ifname Interface name
 * @return Socket file descriptor
 * Official documentation:
 * https://www.kernel.org/doc/html/latest/networking/can.html#how-to-use-socketcan
 */
static int openAndBindSocket(const std::string& ifname) {
    // Open a SocketCAN Linux socket
    int s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (s < 0) throw std::runtime_error("Failed to open CAN socket");
    struct ifreq ifr; // Interface request with safe copy of interface name
    std::strncpy(ifr.ifr_name, ifname.c_str(), IFNAMSIZ - 1);
    ifr.ifr_name[IFNAMSIZ - 1] = '\0'; // Ensure interface name null-terminated
    // Retrieve the interface index (required for binding) from system
    if (ioctl(s, SIOCGIFINDEX, &ifr) < 0) {
        int err = errno;
        close(s);
        throw std::runtime_error(std::format("Failed to find interface {}: {}",
                                             ifname, strerror(err)));
    }
    // Initialise a clean socket address
    struct sockaddr_can addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    // Set receive timeout to 100ms so 'read' doesn't block forever
    struct timeval tv{0, 100000};
    setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
    // Bind the socket to the actual CAN interface
    if (bind(s, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) < 0) {
        int err = errno;
        close(s);
        throw std::runtime_error(std::format("Failed to bind socket to {}: {}",
                                             ifname, strerror(err)));
    }
    return s; // Return the successfully opened and bound socket
}

void slcanInterface::receiveLoop(std::stop_token stoken) {
    // Continuously read CAN frames from socket until (C++20) stop is requested
    while (!stoken.stop_requested()) {
        struct can_frame frame;
        ssize_t nbytes = read(this->sock, &frame, sizeof(struct can_frame));
        if (nbytes < 0) {
            std::println(stderr, "Error reading from CAN socket: {}",
                         strerror(errno));
        } else if (nbytes < static_cast<ssize_t>(sizeof(struct can_frame))) {
            std::println(stderr, "Incomplete CAN frame read");
        } else {
            std::lock_guard<std::mutex> lock(this->queueMutex);
            this->receiveQueue.push(frame);
        }
    }
}

slcanInterface::slcanInterface()
    : ifname(defaultSlcanIfName), sock(openAndBindSocket(defaultSlcanIfName)),
      // Start thread with lambda to capture 'this' and pass stop_token
      th([this](std::stop_token st) { this->receiveLoop(st); }) {
    std::println("Socket opened with fd: {} for interface {}", this->sock,
                 this->ifname);
}

slcanInterface::slcanInterface(std::string ifname_)
    : ifname(ifname_), sock(openAndBindSocket(ifname_)),
      // Start thread with lambda to capture 'this' and pass stop_token
      th([this](std::stop_token st) { this->receiveLoop(st); }) {
    std::println("Socket opened with fd: {} for interface {}", this->sock,
                 this->ifname);
}

// More documentation:
// https://www.kernel.org/doc/html/latest/networking/can.html#can-filter-usage-optimisation
void slcanInterface::Emit(CanMessage& msg) {
    struct can_frame frame; // Initialise a clean CAN frame
    std::memset(&frame, 0, sizeof(frame));
    frame.can_id = msg.getId(); // Copy CanMessage ID to frame
    if (msg.getFormat() == CanMessage::canFormat::EXT)
        frame.can_id |= CAN_EFF_FLAG; // Set Extended Frame Flag of frame
    if (msg.getType() == CanMessage::canType::REMOTE)
        frame.can_id |= CAN_RTR_FLAG; // Set Remote Transmission Request Flag
    frame.can_dlc = msg.getDlc();     // Copy CanMessage DLC to frame
    // Copy CanMessage data to frame
    std::copy(msg.Data().begin(), msg.Data().end(), frame.data);
    if (write(this->sock, &frame, sizeof(struct can_frame)) !=
        sizeof(struct can_frame))
        throw std::runtime_error("Failed to write CAN frame to socket");
}

bool slcanInterface::Receive(CanMessage& msg) {
    // Do not write to the queue while another thread is writing to it
    std::lock_guard<std::mutex> lock(this->queueMutex);
    if (this->receiveQueue.empty()) return false; // No message available
    auto oldestMsg = this->receiveQueue.front();
    // Convert received CAN frame format to internal CanMessage format
    if (oldestMsg.can_id & CAN_EFF_FLAG) { // Extended CAN
        msg.setFormat(CanMessage::canFormat::EXT);
        msg.setId(oldestMsg.can_id &
                  CAN_EFF_MASK); // Mask out flags to get bare CAN ID
    } else {
        msg.setFormat(CanMessage::canFormat::STD);
        msg.setId(oldestMsg.can_id & CAN_SFF_MASK); // Standard CAN
    }
    // Convert received CAN frame type to internal CanMessage type
    if (oldestMsg.can_id & CAN_RTR_FLAG)
        msg.setType(CanMessage::canType::REMOTE); // Remote
    else msg.setType(CanMessage::canType::DATA);  // Data
    msg.setDlc(oldestMsg.can_dlc);                // Copy CAN DLC
    std::copy(std::begin(oldestMsg.data), std::end(oldestMsg.data),
              msg.Data().begin());
    this->receiveQueue.pop(); // Remove the processed message from the queue
    return true;              // Message sucessfully read
}

slcanInterface::~slcanInterface() {
    if (this->sock != 0) close(this->sock);
}
