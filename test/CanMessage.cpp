#include "CanMessage.hpp"
#include <doctest/doctest.h>
#include <print>

TEST_CASE("CanMessage 2 arg ctor") {
    std::println("Testing CanMessage() 2 arg ctor");
    {
        CanMessage msg =
            CanMessage(CanMessage::canType::DATA, CanMessage::canFormat::STD);
        CHECK_EQ(msg.getType(), CanMessage::canType::DATA);
        CHECK_EQ(msg.getFormat(), CanMessage::canFormat::STD);
        CHECK_EQ(msg.getId(), 0);
        CHECK_EQ(msg.getDlc(), 0);
    }
    CanMessage msg =
        CanMessage(CanMessage::canType::REMOTE, CanMessage::canFormat::EXT);
    CHECK_EQ(msg.getType(), CanMessage::canType::REMOTE);
    CHECK_EQ(msg.getFormat(), CanMessage::canFormat::EXT);
}
