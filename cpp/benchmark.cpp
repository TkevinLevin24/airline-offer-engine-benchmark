
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <iomanip>
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

// Calculate price in cents
long long calculatePriceCents(const Flight& flight) {
    long long taxes =
        (flight.baseFareCents * 15 + 50) / 100;

    const long long serviceFee = 2000;

    return flight.baseFareCents + taxes + serviceFee;
}

// Filter, calculate and sort offers
std::vector<Offer> processOffers(
    const std::vector<Flight>& flights
) {
    std::vector<Offer> matchingOffers;

    for (const Flight& flight : flights) {
        if (flight.origin == "BOG" &&
            flight.destination == "MIA") {

            matchingOffers.push_back({
                calculatePriceCents(flight),
                flight.flightNumber
            });
        }
    }

    std::sort(
        matchingOffers.begin(),
        matchingOffers.end(),
        [](const Offer& a, const Offer& b) {
            if (a.totalPriceCents != b.totalPriceCents) {
                return a.totalPriceCents < b.totalPriceCents;
            }

            return a.flightNumber < b.flightNumber;
        }
    );

    return matchingOffers;
}

int main() {
    try {
        const int repetitions = 10;

        std::vector<Flight> flights =
            loadFlights("data/flights.csv");

        std::cout << "=== C++ AIRLINE OFFER BENCHMARK ===\n";
        std::cout << "Flights loaded: " << flights.size() << "\n";
        std::cout << "Route: BOG -> MIA\n";

        // Warm-up
        std::vector<Offer> result = processOffers(flights);

        std::vector<double> executionTimes;

        for (int i = 0; i < repetitions; i++) {
            auto start = std::chrono::steady_clock::now();

            result = processOffers(flights);

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
