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
 */
static int openAndBindSocket(const std::string& ifname) {
    int s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (s < 0) throw std::runtime_error("Failed to open CAN socket");

    struct ifreq ifr;
    std::strncpy(ifr.ifr_name, ifname.c_str(), IFNAMSIZ - 1);
    ifr.ifr_name[IFNAMSIZ - 1] = '\0';
    if (ioctl(s, SIOCGIFINDEX, &ifr) < 0) {
        int err = errno;
        close(s);
        throw std::runtime_error(std::format("Failed to find interface {}: {}",
                                             ifname, strerror(err)));
    }

    struct sockaddr_can addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    // Set receive timeout to 100ms
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 100000;
    setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

    if (bind(s, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) < 0) {
        int err = errno;
        close(s);
        throw std::runtime_error(std::format("Failed to bind socket to {}: {}",
                                             ifname, strerror(err)));
    }

    return s;
}

void slcanInterface::receiveLoop(std::stop_token stoken) {
    while (!stoken.stop_requested()) {
        struct can_frame frame;
        ssize_t nbytes = read(this->sock, &frame, sizeof(struct can_frame));
        if (nbytes < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) continue;
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
      th([this](std::stop_token st) { this->receiveLoop(st); }) {
    std::println("Socket opened with fd: {} for interface {}", this->sock,
                 this->ifname);
}

slcanInterface::slcanInterface(std::string ifname_)
    : ifname(ifname_), sock(openAndBindSocket(ifname_)),
      th([this](std::stop_token st) { this->receiveLoop(st); }) {
    std::println("Socket opened with fd: {} for interface {}", this->sock,
                 this->ifname);
}

void slcanInterface::Emit(CanMessage& msg) {
    struct can_frame frame;
    std::memset(&frame, 0, sizeof(frame));

    frame.can_id = msg.getId();
    if (msg.getFormat() == CanMessage::canFormat::EXT)
        frame.can_id |= CAN_EFF_FLAG;

    if (msg.getType() == CanMessage::canType::REMOTE)
        frame.can_id |= CAN_RTR_FLAG;

    frame.can_dlc = msg.getDlc();
    std::copy(msg.Data().begin(), msg.Data().end(), frame.data);

    if (write(this->sock, &frame, sizeof(struct can_frame)) !=
        sizeof(struct can_frame)) {
        throw std::runtime_error("Failed to write CAN frame to socket");
    }
}

bool slcanInterface::Receive(CanMessage& msg) {
    std::lock_guard<std::mutex> lock(this->queueMutex);
    if (this->receiveQueue.empty()) return false;

    auto received = this->receiveQueue.front();
    if (received.can_id & CAN_EFF_FLAG) {
        msg.setFormat(CanMessage::canFormat::EXT);
        msg.setId(received.can_id & CAN_EFF_MASK);
    } else {
        msg.setFormat(CanMessage::canFormat::STD);
        msg.setId(received.can_id & CAN_SFF_MASK);
    }

    if (received.can_id & CAN_RTR_FLAG)
        msg.setType(CanMessage::canType::REMOTE);
    else msg.setType(CanMessage::canType::DATA);

    msg.setDlc(received.can_dlc);
    std::copy(std::begin(received.data), std::end(received.data),
              msg.Data().begin());
    this->receiveQueue.pop();
    return true;
}

slcanInterface::~slcanInterface() {
    if (this->sock != 0) close(this->sock);
}
