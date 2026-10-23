#ifndef FLIGHT_FILTER_H
#define FLIGHT_FILTER_H

#include <string>

// Check whether a flight matches the requested route
inline bool matchesRoute(
    const std::string& flightOrigin,
    const std::string& flightDestination,
    const std::string& requestedOrigin,
    const std::string& requestedDestination)
{
    return flightOrigin == requestedOrigin &&
           flightDestination == requestedDestination;
}

#endif