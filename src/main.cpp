#include "CanMessage.hpp"
#include "slcanInterface.hpp"
#include <iostream>
#include <poll.h>
#include <print>
#include <string>
#include <unistd.h>
#include <vector>

int main() {
    slcanInterface manager;
    struct pollfd pfd = {STDIN_FILENO, POLLIN, 0};
    while (true) {
        int ret = poll(&pfd, 1, 100); // Poll stdin 100ms
        if (ret > 0 && (pfd.revents & POLLIN)) {
            std::string idStr;
            if (!std::getline(std::cin, idStr)) break;
            std::string dataStr;
            if (!std::getline(std::cin, dataStr)) break;
            try {
                if (idStr.empty()) continue;
                uint32_t id = std::stoul(idStr, nullptr, 16);
                std::vector<uint8_t> dataBytes;
                for (size_t i = 0; i < dataStr.length(); i += 2) {
                    std::string byteStr =
                        dataStr.substr(i, (i + 1 < dataStr.length()) ? 2 : 1);
                    dataBytes.push_back(
                        static_cast<uint8_t>(std::stoul(byteStr, nullptr, 16)));
                }
                // CAN Format: Standard or Extended
                CanMessage::canFormat fmt = (id > 0x7FF)
                                                ? CanMessage::canFormat::EXT
                                                : CanMessage::canFormat::STD;
                // CAN Type: DATA or REMOTE
                size_t inputSize = dataBytes.size();
                CanMessage::canType type = CanMessage::canType::DATA;
                uint8_t dlc = 0;
                if (inputSize == 0) type = CanMessage::canType::REMOTE;
                else if (inputSize > 8) {
                    std::println(stderr,
                                 "WARN: {} bytes provided, only first 8 sent, "
                                 "remaining {} ignored",
                                 inputSize, inputSize - 8);
                    dlc = 8;
                } else dlc = static_cast<uint8_t>(inputSize);
                // Create and emit complete CAN message
                CanMessage msg(id, type, fmt, dlc);
                auto& msgData = msg.Data();
                for (size_t i = 0; i < dlc; ++i) msgData[i] = dataBytes[i];
                manager.Emit(msg);
                std::print("\n");
            } catch (const std::exception& e) {
                std::println(stderr, "Invalid Input: {}", e.what());
            }
        } else {
            CanMessage rxMsg;
            while (manager.Receive(rxMsg))
                std::print("\n{}\n\n", rxMsg.toString());
        }
    }
    return 0;
}
