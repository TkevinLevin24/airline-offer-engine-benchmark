
import csv
import time
import statistics
from pathlib import Path

# Configuration
SEARCH_ORIGIN = "BOG"
SEARCH_DESTINATION = "MIA"
REPETITIONS = 10

# CSV file path
project_root = Path(__file__).resolve().parent.parent
csv_file = project_root / "data" / "flights.csv"


def load_flights():
    flights = []

    with open(csv_file, newline="", encoding="utf-8") as file:
        reader = csv.DictReader(file)

        for row in reader:
            flights.append({
                "flightNumber": row["flightNumber"],
                "origin": row["origin"],
                "destination": row["destination"],
                "baseFareCents": int(row["baseFareCents"])
            })

    return flights


def calculate_price_cents(flight):
    base_fare = flight["baseFareCents"]

    # 15% tax, rounded to nearest cent
    taxes = (base_fare * 15 + 50) // 100

    # USD 20 service fee
    service_fee = 2000

    return base_fare + taxes + service_fee


def process_offers(flights):
    matching_offers = []

    for flight in flights:
        if (
            flight["origin"] == SEARCH_ORIGIN
            and flight["destination"] == SEARCH_DESTINATION
        ):
            total_price = calculate_price_cents(flight)

            matching_offers.append((
                total_price,
                flight["flightNumber"]
            ))

    # Sort by price, then flight number
    matching_offers.sort()

    return matching_offers


def main():
    flights = load_flights()

    print("=== PYTHON AIRLINE OFFER BENCHMARK ===")
    print(f"Flights loaded: {len(flights)}")
    print(f"Route: {SEARCH_ORIGIN} -> {SEARCH_DESTINATION}")

    # Warm-up
    process_offers(flights)

    execution_times = []
    result = []

    for _ in range(REPETITIONS):
        start = time.perf_counter()

        result = process_offers(flights)

        end = time.perf_counter()

        execution_times.append((end - start) * 1000)

    median_time = statistics.median(execution_times)

    print(f"Matching offers: {len(result)}")
    print(f"Repetitions: {REPETITIONS}")
    print(f"Median processing time: {median_time:.4f} ms")

    print("\nCheapest 5 offers:")

    for price_cents, flight_number in result[:5]:
        print(f"{flight_number}: USD {price_cents / 100:.2f}")

        
    # Export all processed offers for validation
    output_file = project_root / "data" / "python_results.csv"

    with open(output_file, "w", newline="", encoding="utf-8") as file:
        writer = csv.writer(file)
        writer.writerow(["flightNumber", "totalPriceCents"])

        for price_cents, flight_number in result:
            writer.writerow([flight_number, price_cents])

    print(f"\nValidation file: {output_file}")



if __name__ == "__main__":
    main()
