#include <format>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "CanMessage.hpp"
#include "NoBitRateManagementException.hpp"
#include "slcanInterface.hpp"
#include <doctest/doctest.h>
#include <stdexcept>
#include <thread>
#include <type_traits>

using namespace std::chrono_literals;

TEST_CASE("slcanInterface ctor") {
    CHECK_THROWS_AS(slcanInterface("invalid_interface_name"),
                    std::runtime_error);
    CHECK_FALSE(std::is_copy_constructible_v<slcanInterface>);
    CHECK_FALSE(std::is_move_constructible_v<slcanInterface>);
    CHECK_FALSE(std::is_constructible_v<slcanInterface, CanManager::bitRate_t>);
}

TEST_CASE("slcanInterface integration with slcan0") {
    // require 'slcan0' interface to be up (with vcan.sh…)
    try {
        slcanInterface txIf("slcan0");
        slcanInterface rxIf("slcan0");
        CanMessage junk; // Ensure clean state
        while (txIf.Receive(junk)) {}
        while (rxIf.Receive(junk)) {}
        SUBCASE("Bitrate management throws") {
            CHECK_THROWS_AS(txIf.setbitRate(CanManager::bitRate_t::KBPS125),
                            NoBitRateManagementException);
            CHECK_THROWS_AS(txIf.getbitRate(), NoBitRateManagementException);
        }
        SUBCASE("Send and Receive Standard Frame") {
            CanMessage msgToSend(0x123, CanMessage::canType::DATA,
                                 CanMessage::canFormat::STD, 4);
            auto& data = msgToSend.Data();
            data[0] = 0xDE;
            data[1] = 0xAD;
            data[2] = 0xBE;
            data[3] = 0xEF;

            txIf.Emit(msgToSend);
            std::this_thread::sleep_for(50ms); // Time for socket processing
            CanMessage receivedMsg;
            bool gotMessage = rxIf.Receive(receivedMsg);

            REQUIRE(gotMessage);
            CHECK(receivedMsg.getId() == 0x123);
            CHECK(receivedMsg.getType() == CanMessage::canType::DATA);
            CHECK(receivedMsg.getFormat() == CanMessage::canFormat::STD);
            CHECK(receivedMsg.getDlc() == 4);
            CHECK(receivedMsg.Data()[0] == 0xDE);
            CHECK(receivedMsg.Data()[3] == 0xEF);
        }

        SUBCASE("Send and Receive Extended Frame") {
            CanMessage msgToSend(0x1ABCDEF, CanMessage::canType::DATA,
                                 CanMessage::canFormat::EXT, 8);
            msgToSend.Data().fill(0xAA);
            txIf.Emit(msgToSend);
            std::this_thread::sleep_for(50ms);

            CanMessage receivedMsg;
            REQUIRE(rxIf.Receive(receivedMsg));
            CHECK(receivedMsg.getId() == 0x1ABCDEF);
            CHECK(receivedMsg.getFormat() == CanMessage::canFormat::EXT);
            CHECK(receivedMsg.Data()[0] == 0xAA);
        }

        SUBCASE("Send and Receive Remote Frame") {
            CanMessage msgToSend(0x456, CanMessage::canType::REMOTE,
                                 CanMessage::canFormat::STD, 0);
            txIf.Emit(msgToSend);
            std::this_thread::sleep_for(50ms);

            CanMessage receivedMsg;
            REQUIRE(rxIf.Receive(receivedMsg));
            CHECK(receivedMsg.getId() == 0x456);
            CHECK(receivedMsg.getType() == CanMessage::canType::REMOTE);
        }

        SUBCASE("Loopback check (Sender does NOT receive its own message by "
                "default)") {
            CanMessage msgToSend(0x789, CanMessage::canType::DATA,
                                 CanMessage::canFormat::STD, 1);
            msgToSend.Data()[0] = 0x55;

            txIf.Emit(msgToSend);
            std::this_thread::sleep_for(50ms);
            CanMessage selfReceived;
            // The sender interface should NOT receive it because default
            // CAN_RAW_RECV_OWN_MSGS is off
            bool gotIt = txIf.Receive(selfReceived);
            CHECK_FALSE(gotIt);

            // But rxIf SHOULD get it because CAN_RAW_LOOPBACK is on by default
            CanMessage otherReceived;
            bool rxGotIt = rxIf.Receive(otherReceived);
            REQUIRE(rxGotIt);
            CHECK(otherReceived.getId() == 0x789);
        }
    } catch (const std::runtime_error& e) {
        FAIL(std::format("Could not open slcan0 (vcan setup?): {}", e.what()));
    }
}
