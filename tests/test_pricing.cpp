#include <iostream>
#include "pricing.h"

// Verify that a price calculation matches the expected result
bool testPrice(
    long long baseFareCents,
    long long expectedPriceCents)
{
    const long long actualPriceCents =
        calculatePriceCents(baseFareCents);

    if (actualPriceCents != expectedPriceCents)
    {
        std::cerr << "FAILED - Base fare: "
                  << baseFareCents
                  << " | Expected: "
                  << expectedPriceCents
                  << " | Actual: "
                  << actualPriceCents
                  << "\n";

        return false;
    }

    std::cout << "PASSED - Base fare: "
              << baseFareCents
              << " | Total: "
              << actualPriceCents
              << "\n";

    return true;
}

int main()
{
    bool allTestsPassed = true;

    // Standard price
    if (!testPrice(10000, 13500))
        allTestsPassed = false;

    // Zero base fare
    if (!testPrice(0, 2000))
        allTestsPassed = false;

    // Rounding down
    if (!testPrice(10001, 13501))
        allTestsPassed = false;

    // Rounding up
    if (!testPrice(10004, 13505))
        allTestsPassed = false;

    // Small base fare
    if (!testPrice(100, 2115))
        allTestsPassed = false;

    if (!allTestsPassed)
    {
        std::cerr << "\nSOME PRICING TESTS FAILED\n";
        return 1;
    }

    std::cout << "\nALL PRICING TESTS PASSED\n";

    return 0;
}