#include <iostream>
#include "offer_sort.h"

bool testSort(
    long long priceA,
    const std::string& flightA,
    long long priceB,
    const std::string& flightB,
    bool expected)
{
    bool actual = offerComesFirst(
        priceA,
        flightA,
        priceB,
        flightB
    );

    if (actual != expected)
    {
        std::cerr << "FAILED - "
                  << flightA << " vs "
                  << flightB << "\n";

        return false;
    }

    std::cout << "PASSED - "
              << flightA << " vs "
              << flightB << "\n";

    return true;
}

int main()
{
    bool allTestsPassed = true;

    // Lower price comes first
    if (!testSort(15000, "FL001", 20000, "FL002", true))
        allTestsPassed = false;

    // Higher price comes later
    if (!testSort(20000, "FL001", 15000, "FL002", false))
        allTestsPassed = false;

    // Same price: lower flight number comes first
    if (!testSort(15000, "FL001", 15000, "FL002", true))
        allTestsPassed = false;

    // Same price: higher flight number comes later
    if (!testSort(15000, "FL002", 15000, "FL001", false))
        allTestsPassed = false;

    // Identical offers do not precede each other
    if (!testSort(15000, "FL001", 15000, "FL001", false))
        allTestsPassed = false;

    if (!allTestsPassed)
    {
        std::cerr << "\nSOME SORT TESTS FAILED\n";
        return 1;
    }

    std::cout << "\nALL SORT TESTS PASSED\n";

    return 0;
}