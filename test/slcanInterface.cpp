#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "slcanInterface.hpp"
#include "NoBitRateManagementException.hpp"
#include <doctest/doctest.h>
#include <type_traits>

TEST_CASE("slcanInterface ctor") {
    CHECK_THROWS_AS(slcanInterface("invalid_interface_name"),
                    std::runtime_error);
    CHECK_FALSE(std::is_copy_constructible_v<slcanInterface>);
    CHECK_FALSE(std::is_move_constructible_v<slcanInterface>);
}

TEST_CASE("slcanInterface NoBitRateManagement") {
    slcanInterface slcan("vcan0");
    SUBCASE("BitRate Management throws") {
        CHECK_THROWS_AS(slcan.setbitRate(bitRate_t::KBPS125),
                        NoBitRateManagementException);
        CHECK_THROWS_AS(slcan.getbitRate(), NoBitRateManagementException);
    }
}
