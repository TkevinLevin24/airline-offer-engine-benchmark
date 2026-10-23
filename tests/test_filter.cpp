#include <iostream>
#include "flight_filter.h"

// Verify whether a flight matches a requested route
bool testRoute(
    const std::string& flightOrigin,
    const std::string& flightDestination,
    const std::string& requestedOrigin,
    const std::string& requestedDestination,
    bool expectedResult)
{
    const bool actualResult = matchesRoute(
        flightOrigin,
        flightDestination,
        requestedOrigin,
        requestedDestination
    );

    if (actualResult != expectedResult)
    {
        std::cerr << "FAILED - Route: "
                  << flightOrigin << " -> "
                  << flightDestination << "\n";

        return false;
    }

    std::cout << "PASSED - Route: "
              << flightOrigin << " -> "
              << flightDestination << "\n";

    return true;
}

int main()
{
    bool allTestsPassed = true;

    // Matching route
    if (!testRoute("BOG", "MIA", "BOG", "MIA", true))
        allTestsPassed = false;

    // Different destination
    if (!testRoute("BOG", "JFK", "BOG", "MIA", false))
        allTestsPassed = false;

    // Reversed route
    if (!testRoute("MIA", "BOG", "BOG", "MIA", false))
        allTestsPassed = false;

    // Different origin
    if (!testRoute("MDE", "MIA", "BOG", "MIA", false))
        allTestsPassed = false;

    // Another valid requested route
    if (!testRoute("MDE", "JFK", "MDE", "JFK", true))
        allTestsPassed = false;

    // Empty origin
    if (!testRoute("", "MIA", "BOG", "MIA", false))
        allTestsPassed = false;

    if (!allTestsPassed)
    {
        std::cerr << "\nSOME FILTER TESTS FAILED\n";
        return 1;
    }

    std::cout << "\nALL FILTER TESTS PASSED\n";

    return 0;
}