#ifndef OFFER_SORT_H
#define OFFER_SORT_H
#include <algorithm>
#include <string>
#include <vector>

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
// Sort offers by total price and flight number
template <typename OfferType>
void sortOffers(std::vector<OfferType>& offers)
{
    std::sort(
        offers.begin(),
        offers.end(),
        [](const OfferType& a, const OfferType& b)
        {
            return offerComesFirst(
                a.totalPriceCents,
                a.flightNumber,
                b.totalPriceCents,
                b.flightNumber
            );
        }
    );
}

#endif