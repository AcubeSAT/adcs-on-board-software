#include <catch2/catch.hpp>
#include "MathFunctions.hpp"

TEST_CASE("Date to decimal date test") {
    REQUIRE(date2decimal(2024, 1, 1, 0, 0, 0) == Approx(2024).epsilon(0.000001));
    REQUIRE(date2decimal(2024, 12, 32, 0, 0, 0) == Approx(2025).epsilon(0.000001));
    REQUIRE(date2decimal(2024, 7, 24, 7, 23, 28) == Approx(2024.5609507185).epsilon(0.000001));
    REQUIRE(date2decimal(2024, 1, 10, 11, 54, 26) == Approx(2024.0259457220).epsilon(0.000001));
    REQUIRE(date2decimal(2024, 9, 11, 9, 27, 16) == Approx(2024.6950653967).epsilon(0.000001));
    REQUIRE(date2decimal(2024, 5, 30, 10, 52, 58) == Approx(2024.4110749975).epsilon(0.000001));
    REQUIRE(date2decimal(2023, 6, 9, 6, 9, 6) == Approx(2023.4363186834).epsilon(0.000001));
    REQUIRE(date2decimal(2100, 6, 9, 6, 9, 6) == Approx(2100.4363186834).epsilon(0.000001));
    REQUIRE(date2decimal(2400, 9, 11, 9, 27, 16) == Approx(2400.6950653967).epsilon(0.000001));
}