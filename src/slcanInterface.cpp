#include "slcanInterface.hpp"
#include <algorithm>
#include <cerrno>
#include <cstdint>
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
 * @brief Helper function to open and bind a CAN socket
 * @param ifname Name of the CAN interface (e.g., "can0", "slcan0")
 * @return File descriptor of the opened socket
 */
static std::uint32_t openAndBindSocket(const std::string& ifname) {
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

    if (bind(s, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        int err = errno;
        close(s);
        throw std::runtime_error(std::format("Failed to bind socket to {}: {}",
                                             ifname, strerror(err)));
    }

    return s;
}

void slcanInterface::receiveLoop() {
    while (this->continueReception) {
        can_frame frame;
        ssize_t nbytes = read(this->sock, &frame, sizeof(struct can_frame));
        if (nbytes < 0) std::println("Wrong read from CAN socket");
        else if (nbytes < static_cast<ssize_t>(sizeof(struct can_frame)))
            std::println("Incomplete CAN frame read");
        else this->receiveQueue.push(frame);
    }
}

/**< Default constructor */
slcanInterface::slcanInterface()
    : ifname(defaultSlcanIfName), sock(openAndBindSocket(defaultSlcanIfName)),
      th(std::jthread(&slcanInterface::receiveLoop, this)) {
    std::println("Socket opened with fd: {} for interface {}", this->sock,
                 this->ifname);
}

/**< Constructor taking interface name */
slcanInterface::slcanInterface(std::string ifname_)
    : ifname(ifname_), sock(openAndBindSocket(ifname_)),
      th(std::jthread(&slcanInterface::receiveLoop, this)) {
    std::println("Socket opened with fd: {} for interface {}", this->sock,
                 this->ifname);
}

/**
 * @brief Emit a CAN message
 * @param msg
 */
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
        sizeof(struct can_frame))
        throw std::runtime_error("Failed to write CAN frame to socket");
}

/**
 * @brief Wait to receive a CAN message
 * @param Object where to write the received message
 * @return true if a message was received and stored in msg
 * @return false if no message was received
 */
bool slcanInterface::Receive(CanMessage& msg) {
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

/** Dtor */
slcanInterface::~slcanInterface() {
    if (this->sock != 0) close(this->sock);
}
