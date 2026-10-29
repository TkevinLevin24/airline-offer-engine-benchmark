
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include "pricing.h"
#include "flight_filter.h"
#include "offer_sort.h"
#include <stdexcept>


// Flight information
struct Flight {
    std::string flightNumber;
    std::string origin;
    std::string destination;
    long long baseFareCents;
};

// Processed offer
struct Offer {
    long long totalPriceCents;
    std::string flightNumber;
};

// Load flights from CSV
std::vector<Flight> loadFlights(const std::string& filename) {
    std::vector<Flight> flights;

    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Cannot open CSV file");
    }

    std::string line;

    // Skip header
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        Flight flight;
        std::string fareText;

        std::getline(ss, flight.flightNumber, ',');
        std::getline(ss, flight.origin, ',');
        std::getline(ss, flight.destination, ',');
        std::getline(ss, fareText, ',');

        flight.baseFareCents = std::stoll(fareText);

        flights.push_back(flight);
    }

    return flights;
}

// Calculate price in cents using the shared pricing function
long long calculatePriceCents(const Flight& flight) {
    return calculatePriceCents(flight.baseFareCents);
}

// Filter, calculate and sort offers
std::vector<Offer> processOffers(
    const std::vector<Flight>& flights,
    const std::string& requestedOrigin,
    const std::string& requestedDestination
) {
    std::vector<Offer> matchingOffers;

   for (const Flight& flight : flights) {
    if (matchesRoute(
        flight.origin,
        flight.destination,
        requestedOrigin,
        requestedDestination)) {

        matchingOffers.push_back({
            calculatePriceCents(flight),
            flight.flightNumber
        });
    }
}

    sortOffers(matchingOffers);

    return matchingOffers;
}

int main(int argc, char* argv[]) {
    try {
        std::string requestedOrigin = "BOG";
std::string requestedDestination = "MIA";

if (argc == 3)
{
    requestedOrigin = argv[1];
    requestedDestination = argv[2];
}
else if (argc != 1)
{
    throw std::invalid_argument(
        "Usage: benchmark-cpp [ORIGIN DESTINATION]"
    );
}
        const int repetitions = 10;

        std::vector<Flight> flights =
            loadFlights("data/flights.csv");

        std::cout << "=== C++ AIRLINE OFFER BENCHMARK ===\n";
        std::cout << "Flights loaded: " << flights.size() << "\n";
        std::cout << "Route: "
          << requestedOrigin
          << " -> "
          << requestedDestination
          << "\n";

        // Warm-up
        std::vector<Offer> result = processOffers(
    flights,
    requestedOrigin,
    requestedDestination
);

        std::vector<double> executionTimes;

        for (int i = 0; i < repetitions; i++) {
            auto start = std::chrono::steady_clock::now();

            result = processOffers(
    flights,
    requestedOrigin,
    requestedDestination
);

            auto end = std::chrono::steady_clock::now();

            double milliseconds =
                std::chrono::duration<double, std::milli>(
                    end - start
                ).count();

            executionTimes.push_back(milliseconds);
        }

        // Calculate median
        std::sort(
            executionTimes.begin(),
            executionTimes.end()
        );

        double medianTime =
            (executionTimes[4] + executionTimes[5]) / 2.0;

        std::cout << "Matching offers: "
                  << result.size() << "\n";

        std::cout << "Repetitions: "
                  << repetitions << "\n";

        std::cout << std::fixed << std::setprecision(4);

        std::cout << "Median processing time: "
                  << medianTime << " ms\n";

        std::cout << "\nCheapest 5 offers:\n";

        for (size_t i = 0;
             i < std::min<size_t>(5, result.size());
             i++) {

            std::cout << result[i].flightNumber
                      << ": USD "
                      << std::setprecision(2)
                      << result[i].totalPriceCents / 100.0
                      << "\n";
        }

        
        // Export all processed offers for validation
        std::ofstream outputFile("data/cpp_results.csv");

        if (!outputFile.is_open()) {
            throw std::runtime_error(
                "Cannot create C++ validation file"
            );
        }

        outputFile << "flightNumber,totalPriceCents\n";

        for (const Offer& offer : result) {
            outputFile << offer.flightNumber << ","
                       << offer.totalPriceCents << "\n";
        }

        std::cout << "\nValidation file: data/cpp_results.csv\n";


    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << "\n";
        return 1;
    }

    return 0;
}
