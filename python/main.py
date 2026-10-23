
# Airline Offer Engine - Python

# Flight data
flights = [
    {"flightNumber": "AV101", "origin": "BOG", "destination": "MIA", "baseFare": 320.0},
    {"flightNumber": "AV202", "origin": "BOG", "destination": "JFK", "baseFare": 450.0},
    {"flightNumber": "AV303", "origin": "BOG", "destination": "MAD", "baseFare": 780.0},
    {"flightNumber": "AV404", "origin": "BOG", "destination": "MEX", "baseFare": 280.0},
    {"flightNumber": "AV505", "origin": "BOG", "destination": "LIM", "baseFare": 190.0},
    {"flightNumber": "AV606", "origin": "BOG", "destination": "MIA", "baseFare": 290.0},
    {"flightNumber": "AV707", "origin": "BOG", "destination": "MIA", "baseFare": 350.0},
    {"flightNumber": "AV808", "origin": "MDE", "destination": "MIA", "baseFare": 310.0},
]


def calculate_price(flight):
    tax_rate = 0.15
    service_fee = 20.0

    return flight["baseFare"] * (1.0 + tax_rate) + service_fee


def main():
    search_origin = "BOG"
    search_destination = "MIA"

    matching_flights = []

    for flight in flights:
        if (
            flight["origin"] == search_origin
            and flight["destination"] == search_destination
        ):
            matching_flights.append(flight)

    matching_flights.sort(key=calculate_price)

    print("=== AIRLINE OFFER ENGINE ===\n")
    print(f"Searching flights: {search_origin} -> {search_destination}\n")

    for flight in matching_flights:
        total_price = calculate_price(flight)

        print(
            f"Flight: {flight['flightNumber']} "
            f"| Price: USD {total_price:.2f}"
        )

    print(f"\nTotal offers found: {len(matching_flights)}")


if __name__ == "__main__":
    main()
