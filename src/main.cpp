#include "CanMessage.hpp"
#include "slcanInterface.hpp"
#include <iostream>
#include <poll.h>
#include <print>
#include <string>
#include <vector>

int main() {
    // try {
    slcanInterface manager;
    struct pollfd pfd = {STDIN_FILENO, POLLIN, 0};
    while (true) {
        int ret = poll(&pfd, 1, 100); // Poll stdin for 100ms
        if (ret > 0 && (pfd.revents & POLLIN)) {
            std::string idStr; // Data entered in stdin, try to read CAN ID
            if (!std::getline(std::cin, idStr)) break; // EOF or error
            std::string dataStr; // Block execution to read eventual data
            if (!std::getline(std::cin, dataStr)) break; // Exit on EOF
            try {
                if (idStr.empty()) continue; // Restart read loop
                // Parse CAN ID
                uint32_t id = std::stoul(idStr, nullptr, 16);
                std::vector<uint8_t> dataBytes; // Parse CAN data
                // Hex parsing byte by byte
                for (size_t i = 0; i < dataStr.length(); i += 2) {
                    std::string byteStr =
                        dataStr.substr(i, (i + 1 < dataStr.length()) ? 2 : 1);
                    dataBytes.push_back(
                        static_cast<uint8_t>(std::stoul(byteStr, nullptr, 16)));
                }
                // Determine CAN Standard or Extended
                CanMessage::canFormat fmt = (id > 0x7FF)
                                                ? CanMessage::canFormat::EXT
                                                : CanMessage::canFormat::STD;
                // Determine CAN Data or Remote
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
                CanMessage msg(id, type, fmt, dlc);
                // Copy data to message
                auto& msgData = msg.Data();
                for (size_t i = 0; i < dlc; ++i) msgData[i] = dataBytes[i];
                manager.Emit(msg); // Emit CAN message
            } catch (const std::exception& e) {
                // Ignore parsing errors to keep CLI alive
                std::println(stderr, "Invalid Input: {}", e.what());
            }
        } else {
            CanMessage rxMsg; // No input, check for received CAN messages
            // Process all available messages unless input arrives
            while (manager.Receive(rxMsg))
                std::print("\n\n{}\n\n", rxMsg.toString());
            // if (poll(&pfd, 1, 0) > 0) break; // Key pressed ? Emit
        }
    }
    // } catch (const std::exception& e) {
    //     std::println(stderr, "Fatal Error: {}", e.what());
    //     return 1;
    // }
    return 0;
}
