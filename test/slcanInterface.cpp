#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "slcanInterface.hpp"
#include <doctest/doctest.h>
#include <stdexcept>
#include <type_traits>
// #include "NoBitRateManagementException.hpp"

TEST_CASE("slcanInterface ctor") {
    CHECK_THROWS_AS(slcanInterface("invalid_interface_name"),
                    std::runtime_error);
    CHECK_FALSE(std::is_copy_constructible_v<slcanInterface>);
    CHECK_FALSE(std::is_move_constructible_v<slcanInterface>);
}

// TODO Mock tests
