#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "CanMessage.hpp"
#include <doctest/doctest.h>
#include <print>

TEST_CASE("CanMessage 2 arg ctor") {
    std::println("Testing CanMessage() 2 arg ctor");
    CanMessage msg =
        CanMessage(CanMessage::canType::DATA, CanMessage::canFormat::STD);
    CHECK_EQ(msg.getType(), CanMessage::canType::DATA);
    CHECK_EQ(msg.getFormat(), CanMessage::canFormat::STD);
    CHECK_EQ(msg.getId(), 0);
    CHECK_EQ(msg.getDlc(), 0);
    msg = CanMessage(CanMessage::canType::REMOTE, CanMessage::canFormat::EXT);
    CHECK_EQ(msg.getType(), CanMessage::canType::REMOTE);
    CHECK_EQ(msg.getFormat(), CanMessage::canFormat::EXT);
}

TEST_CASE("CanMessage 4 args ctor") {
    std::println("Testing CanMessage() 3 args ctor");
    CanMessage msg =
        CanMessage(1, CanMessage::canType::DATA, CanMessage::canFormat::STD);
    CHECK_EQ(msg.getType(), CanMessage::canType::DATA);
    CHECK_EQ(msg.getFormat(), CanMessage::canFormat::STD);
    CHECK_EQ(msg.getId(), 1);
    CHECK_EQ(msg.getDlc(), 0);
}

TEST_CASE("CanMessage 5 args ctor") {
    std::println("Testing CanMessage() 5 args ctor");
    CanMessage msg = CanMessage(1, CanMessage::canType::DATA,
                                CanMessage::canFormat::STD, MAX_DLC);
    CHECK_EQ(msg.getType(), CanMessage::canType::DATA);
    CHECK_EQ(msg.getFormat(), CanMessage::canFormat::STD);
    CHECK_EQ(msg.getId(), 1);
    CHECK_EQ(msg.getDlc(), MAX_DLC);
}

TEST_CASE("CanMessage toString()") {
    std::println("Testing CanMessage::Data()");
    CanMessage msg = CanMessage(1, CanMessage::canType::DATA,
                                CanMessage::canFormat::STD, MAX_DLC);
    CHECK_EQ(msg.Data(), std::array<uint8_t, MAX_DLC>());
    std::array<uint8_t, MAX_DLC> data = {1, 2, 3, 4, 5, 6, 7, 8};
    msg.Data() = data;
    CHECK_EQ(msg.Data(), data);
}

TEST_CASE("CanMessage isMessageValid()") {
    std::println("Testing CanMessage::isMessageValid()");
    CanMessage msg = CanMessage(1, CanMessage::canType::DATA,
                                CanMessage::canFormat::STD, MAX_DLC);
    CHECK(msg.isMessageValid());
    msg = CanMessage(1, CanMessage::canType::DATA, CanMessage::canFormat::STD,
                     MAX_DLC + 1);
    CHECK_FALSE(msg.isMessageValid());
}

TEST_CASE("CanMessage toString()") {
    std::println("Testing CanMessage::toString()");
    CanMessage msg = CanMessage(1, CanMessage::canType::DATA,
                                CanMessage::canFormat::STD, MAX_DLC);
    // TODO à voir selon implémentation
    CHECK_EQ(msg.toString(), "ID: 1, Type: DATA, Format: STD, DLC: 8");
}
