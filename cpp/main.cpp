
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>

// Flight data
struct Flight {
    std::string flightNumber;
    std::string origin;
    std::string destination;
    double baseFare;
};

// Calculate final price
double calculatePrice(const Flight& flight) {
    const double taxRate = 0.15;
    const double serviceFee = 20.0;

    return flight.baseFare * (1.0 + taxRate) + serviceFee;
}

int main() {

    std::vector<Flight> flights = {
        {"AV101", "BOG", "MIA", 320.0},
        {"AV202", "BOG", "JFK", 450.0},
        {"AV303", "BOG", "MAD", 780.0},
        {"AV404", "BOG", "MEX", 280.0},
        {"AV505", "BOG", "LIM", 190.0},
        {"AV606", "BOG", "MIA", 290.0},
        {"AV707", "BOG", "MIA", 350.0},
        {"AV808", "MDE", "MIA", 310.0}
    };

    // Search criteria
    std::string searchOrigin = "BOG";
    std::string searchDestination = "MIA";

    // Store matching flights
    std::vector<Flight> matchingFlights;

    for (const Flight& flight : flights) {
        if (flight.origin == searchOrigin &&
            flight.destination == searchDestination) {

            matchingFlights.push_back(flight);
        }
    }

    // Sort matching flights by final price
    std::sort(
        matchingFlights.begin(),
        matchingFlights.end(),
        [](const Flight& a, const Flight& b) {
            return calculatePrice(a) < calculatePrice(b);
        }
    );

    // Display results
    std::cout << "=== AIRLINE OFFER ENGINE ===\n\n";

    std::cout << "Searching flights: "
              << searchOrigin << " -> "
              << searchDestination << "\n\n";

    std::cout << std::fixed << std::setprecision(2);

    for (const Flight& flight : matchingFlights) {
        std::cout << "Flight: " << flight.flightNumber
                  << " | Price: USD "
                  << calculatePrice(flight) << "\n";
    }

    std::cout << "\nTotal offers found: "
              << matchingFlights.size() << "\n";

    return 0;
}
