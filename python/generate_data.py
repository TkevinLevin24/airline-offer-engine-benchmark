
import csv
import argparse
import random
from pathlib import Path

# Configuration

parser = argparse.ArgumentParser(
    description="Generate reproducible airline flight data"
)

parser.add_argument(
    "--flights",
    type=int,
    default=10000,
    help="Number of flights to generate"
)

args = parser.parse_args()

if args.flights <= 0:
    parser.error("--flights must be greater than zero")

NUMBER_OF_FLIGHTS = args.flights

RANDOM_SEED = 42

# Airport codes
airports = [
    "BOG", "MIA", "JFK", "MAD",
    "MEX", "LIM", "MDE", "LAX"
]

# Reproducible random data
random.seed(RANDOM_SEED)

# Project data folder
project_root = Path(__file__).resolve().parent.parent
output_file = project_root / "data" / "flights.csv"

output_file.parent.mkdir(parents=True, exist_ok=True)

with open(output_file, "w", newline="", encoding="utf-8") as file:
    writer = csv.writer(file)

    writer.writerow([
        "flightNumber",
        "origin",
        "destination",
        "baseFareCents"
    ])

    for i in range(NUMBER_OF_FLIGHTS):
        origin, destination = random.sample(airports, 2)

        base_fare_cents = random.randint(10000, 150000)

        writer.writerow([
            f"FL{i + 1:07d}",
            origin,
            destination,
            base_fare_cents
        ])

print(f"Generated {NUMBER_OF_FLIGHTS} flights")
print(f"Saved to: {output_file}")
