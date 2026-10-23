#ifndef OFFER_SORT_H
#define OFFER_SORT_H

#include <string>

// Compare two offers by price and flight number
inline bool offerComesFirst(
    long long priceA,
    const std::string& flightNumberA,
    long long priceB,
    const std::string& flightNumberB)
{
    if (priceA != priceB)
    {
        return priceA < priceB;
    }

    return flightNumberA < flightNumberB;
}

#endif