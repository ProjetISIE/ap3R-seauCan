#include "slcanInterface.hpp"
#include <cstdint>
#include <format>
#include <linux/can.h>
#include <print>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

std::uint32_t openSocket() {
    int fd = socket(AF_INET, SOCK_STREAM, CAN_RAW);
    if (fd == -1) throw std::runtime_error("Failed to open socket");
    return fd;
}

/**< Default constructor */
slcanInterface::slcanInterface() : sock(openSocket()) {
    std::println(
        "%s", std::format("Socket opened with fd: {}\n", this->sock).c_str());
};

/**< Constructor taking interface name */
slcanInterface::slcanInterface(std::string ifname_) : sock(openSocket()) {
    std::println(
        "%s", std::format("Socket opened with fd: {}\n", this->sock).c_str());
};

/**
 * @brief Emit a CAN message
 * @param msg
 */
void slcanInterface::Emit(CanMessage& msg) { return; };

/**
 * @brief Wait to receive a CAN message
 *
 * @param msg
 * @return true if a message was received and stored in msg
 * @return false if no message was received
 */
[[nodiscard]] bool Receive(CanMessage& msg) { return false; };

/** dtor */
slcanInterface::~slcanInterface() {
    if (this->sock != 0) close(this->sock);
}
