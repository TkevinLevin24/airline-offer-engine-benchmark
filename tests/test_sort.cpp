#include <iostream>
#include "offer_sort.h"
#include <algorithm>
#include <vector>

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
struct TestOffer
{
    long long totalPriceCents;
    std::string flightNumber;
};

bool testCompleteSorting()
{
    std::vector<TestOffer> offers = {
        {20000, "FL003"},
        {15000, "FL002"},
        {15000, "FL001"},
        {10000, "FL004"}
    };

    sortOffers(offers);

    const std::vector<std::string> expected = {
        "FL004",
        "FL001",
        "FL002",
        "FL003"
    };

    for (size_t i = 0; i < offers.size(); ++i)
    {
        if (offers[i].flightNumber != expected[i])
        {
            std::cerr
                << "FAILED - Complete sorting at position "
                << i << "\n";

            return false;
        }
    }

    std::cout << "PASSED - Complete offer sorting\n";

    return true;
}

bool testEmptySorting()
{
    std::vector<TestOffer> offers;

    sortOffers(offers);

    if (!offers.empty())
    {
        std::cerr << "FAILED - Empty vector sorting\n";
        return false;
    }

    std::cout << "PASSED - Empty vector sorting\n";
    return true;
}

bool testSingleOfferSorting()
{
    std::vector<TestOffer> offers = {
        {15000, "FL001"}
    };

    sortOffers(offers);

    if (offers.size() != 1 ||
        offers[0].flightNumber != "FL001" ||
        offers[0].totalPriceCents != 15000)
    {
        std::cerr << "FAILED - Single offer sorting\n";
        return false;
    }

    std::cout << "PASSED - Single offer sorting\n";
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
    if (!testCompleteSorting())
{
    allTestsPassed = false;
}
if (!testEmptySorting())
{
    allTestsPassed = false;
}

if (!testSingleOfferSorting())
{
    allTestsPassed = false;
}

    if (!allTestsPassed)
    {
        std::cerr << "\nSOME SORT TESTS FAILED\n";
        return 1;
    }
    

    std::cout << "\nALL SORT TESTS PASSED\n";

    return 0;
}