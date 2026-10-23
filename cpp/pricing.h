
#ifndef PRICING_H
#define PRICING_H

inline long long calculatePriceCents(long long baseFareCents)
{
    const long long taxCents = (baseFareCents * 15 + 50) / 100;
    const long long serviceFeeCents = 2000;

    return baseFareCents + taxCents + serviceFeeCents;
}

#endif
