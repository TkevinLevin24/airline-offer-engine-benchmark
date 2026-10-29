# Airline Offer Engine Benchmark

**A reproducible C++17 vs. Python performance benchmark for airline offer processing, featuring configurable flight routes, automated testing, and cross-language result validation.**

## Overview

Airline retailing and reservation systems must efficiently process large volumes of flight data while maintaining pricing accuracy and consistent results.

This project implements a simplified airline offer processing engine in **C++17 and Python**, using identical datasets and business rules to compare execution performance.

The engine performs three core operations:

1. **Route filtering:** Select flights matching a requested origin and destination.
2. **Offer pricing:** Calculate the final ticket price using taxes and service fees.
3. **Offer sorting:** Rank matching offers by total price, using the flight number as a secondary sorting key.

The project emphasizes:

- Performance benchmarking
- Algorithmic efficiency
- Reproducible experiments
- Automated unit testing
- Cross-language correctness validation
- Modular C++ development

The objective is to demonstrate how C++ can improve processing performance while preserving the business logic and output consistency of a Python reference implementation.

---

## Technology Stack

| Component | Technology |
|---|---|
| High-performance implementation | C++17 |
| Reference implementation | Python 3.11 |
| C++ compiler | Microsoft Visual C++ (MSVC) |
| Build system | CMake |
| Unit testing | CTest and custom C++ tests |
| Data generation | Python |
| Data format | CSV |
| Benchmark automation | Python |
| Correctness validation | CSV comparison and SHA-256 |
| Data visualization | Matplotlib |
| Development environment | Visual Studio Code |
| Operating system | Windows |

---

## Project Structure

```text
airline-offer-engine-benchmark/
|
|-- cpp/
|   |-- main.cpp
|   |-- benchmark.cpp
|   |-- pricing.h
|   |-- flight_filter.h
|   `-- offer_sort.h
|
|-- python/
|   |-- main.py
|   |-- benchmark.py
|   |-- generate_data.py
|   |-- validate.py
|   |-- validate_routes.py
|   |-- run_benchmarks.py
|   `-- plot_results.py
|
|-- tests/
|   |-- test_pricing.cpp
|   |-- test_filter.cpp
|   `-- test_sort.cpp
|
|-- data/
|   `-- (generated CSV files)
|
|-- charts/
|   |-- processing_time.png
|   `-- speedup.png
|
|-- CMakeLists.txt
|-- .gitignore
|-- LICENSE
`-- README.md
```

Generated datasets, benchmark outputs, and compiled binaries are excluded from version control.

---

## Processing Workflow

```text
           CSV Flight Dataset
                    |
                    v
               Load Flights
                    |
                    v
              Select Route
             (Origin / Destination)
                    |
                    v
               Filter Flights
                    |
                    v
              Calculate Prices
                    |
                    v
                Sort Offers
                    |
                    v
               Export Results
                    |
                    v
           Cross-Language Validation
              Python vs. C++
```

Both implementations process the same flight data using equivalent pricing and sorting rules.

### Configurable Flight Routes

The engine supports route selection through command-line arguments.

Examples:

```text
BOG -> MIA
BOG -> MAD
MDE -> JFK
```

If no route is provided, the engine uses the default route:

```text
BOG -> MIA
```

This functionality allows the same implementation to process multiple origin-destination combinations without modifying the source code.

---

## Pricing Model

The final ticket price is calculated using:

```text
Total Price = Base Fare + Tax + Service Fee
```

### Business Rules

| Component | Rule |
|---|---|
| Base fare | Flight-specific |
| Tax | 15% of base fare |
| Service fee | USD 20.00 |
| Monetary representation | Integer cents |
| Tax rounding | Nearest cent, half-up for nonnegative amounts |

Using integer cents avoids floating-point inconsistencies in monetary calculations.

### Pricing Example

For a base fare of USD 100.00:

```text
Base Fare:    USD 100.00
Tax (15%):    USD  15.00
Service Fee:  USD  20.00
-------------------------
Total Price:  USD 135.00
```

Both implementations use the same pricing rules.

---

## Benchmark Methodology

Three dataset sizes are evaluated:

- 10,000 flights
- 100,000 flights
- 1,000,000 flights

Synthetic flight data is generated using a fixed random seed (`42`) to support reproducibility.

### Measurement Procedure

For each dataset size:

1. Generate the flight dataset.
2. Execute both implementations using identical input data.
3. Perform a warm-up run before collecting measurements.
4. Measure 10 processing repetitions per execution.
5. Repeat the benchmark across 5 independent process sessions.
6. Alternate the execution order between Python and C++.
7. Validate the generated output files.
8. Report the median of the session medians.

### Timing Scope

The timed processing section includes:

- Route filtering
- Price calculation
- Offer sorting

The following operations are excluded:

- CSV loading
- Result export
- Cross-language validation

The C++ implementation is compiled in Release mode with compiler optimizations enabled.

---

## Benchmark Results

The following historical benchmark measurements were obtained on a Windows development machine using MSVC and Python 3.11.

| Flights | Python (ms) | C++ (ms) | C++ Speedup |
|---:|---:|---:|---:|
| 10,000 | 0.5635 | 0.0582 | 9.68x |
| 100,000 | 7.3917 | 1.1220 | 6.59x |
| 1,000,000 | 71.6215 | 12.5204 | 5.72x |

Speedup is calculated as:

```text
Speedup = Python Processing Time / C++ Processing Time
```

### Processing Time Comparison

![Python vs C++ Processing Time](charts/processing_time.png)

### C++ Speedup

![C++ Speedup over Python](charts/speedup.png)

### Key Observations

- C++ achieved lower processing times across all three dataset sizes.
- The observed speedup ranged from 5.72x to 9.68x in these consolidated measurements.
- Performance differences varied depending on dataset size.
- Repeated measurements help reduce the influence of execution variability.
- Results are specific to the implementation, workload, hardware, and software environment.

These measurements are historical benchmark results and should not be interpreted as universal performance guarantees.

---

## Correctness Validation

Performance improvements are meaningful only when both implementations produce equivalent results.

The project validates the complete sorted offer lists generated by Python and C++.

The comparison verifies:

- Matching flight numbers
- Final prices in integer cents
- Number of matching offers
- Offer ordering
- SHA-256 file hashes

### Example Validation

For the default route `BOG -> MIA`, using the one-million-flight dataset:

```text
=== AIRLINE OFFER VALIDATION ===
Python offers: 17950
C++ offers: 17950
SHA-256: IDENTICAL
VALIDATION PASSED: All offers match
```

The identical SHA-256 hashes confirm that the generated result files are byte-for-byte identical.

---

## Automated Multi-Route Validation

The project includes an automated validation script that tests multiple routes without requiring manual execution of each benchmark.

The script:

1. Executes the Python implementation.
2. Executes the C++ implementation using the same route.
3. Compares both output files.
4. Reports whether the route passed validation.
5. Produces a consolidated summary.

### Supported Validation Scenarios

| Route | Matching Offers | Validation |
|---|---:|---|
| BOG -> MIA | 17,950 | PASSED |
| BOG -> MAD | 17,852 | PASSED |
| MDE -> JFK | 17,777 | PASSED |

These results correspond to the deterministic one-million-flight dataset.

### Run Automated Validation

```powershell
python .\python\validate_routes.py
```

Expected output:

```text
=== CROSS-LANGUAGE VALIDATION ===

BOG -> MIA      PASSED
BOG -> MAD      PASSED
MDE -> JFK      PASSED

=== SUMMARY ===
Routes tested: 3
Routes passed: 3
Routes failed: 0

ALL VALIDATIONS PASSED
```

This provides a repeatable regression check when modifying the processing logic.

---

## C++ Unit Testing

The project includes automated C++ tests organized into three categories.

### Pricing Tests

Verify:

- Standard fare calculations
- Zero-value fares
- Tax rounding behavior
- Small monetary values

### Flight Filtering Tests

Verify:

- Correct route matching
- Incorrect origins
- Incorrect destinations
- Reversed routes
- Empty origin values

### Offer Sorting Tests

Verify:

- Ascending price ordering
- Flight-number tie-breaking
- Sorting multiple offers
- Empty collections
- Single-element collections

The tests cover reusable functions defined in:

```text
cpp/pricing.h
cpp/flight_filter.h
cpp/offer_sort.h
```

### Run Unit Tests

After building the project:

```powershell
ctest --test-dir build -C Release
```

Expected output:

```text
Start 1: PricingTests
1/3 Test #1: PricingTests .......... Passed

Start 2: FlightFilterTests
2/3 Test #2: FlightFilterTests ..... Passed

Start 3: OfferSortTests
3/3 Test #3: OfferSortTests ........ Passed

100% tests passed out of 3
```

The current test suite contains 19 individual checks across three test executables.

---

## Getting Started

### Requirements

- Windows 10 or Windows 11
- Python 3.11 or another compatible Python 3 version
- Visual Studio 2022 with C++ development tools
- CMake
- Git
- Matplotlib (optional, for charts)

The following instructions use Windows PowerShell.

### 1. Clone the Repository

```powershell
git clone https://github.com/TkevinLevin24/airline-offer-engine-benchmark.git

cd airline-offer-engine-benchmark
```

### 2. Configure CMake

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
```

### 3. Build the C++ Project

```powershell
cmake --build build --config Release
```

This generates the executables:

```text
build/Release/airline-engine.exe
build/Release/benchmark-cpp.exe
build/Release/test-pricing.exe
build/Release/test-filter.exe
build/Release/test-sort.exe
```

### 4. Generate Flight Data

Generate one million synthetic flights:

```powershell
python .\python\generate_data.py --flights 1000000
```

The generated dataset is saved in:

```text
data/flights.csv
```

### 5. Run the Python Engine

Default route:

```powershell
python .\python\benchmark.py
```

Custom route:

```powershell
python .\python\benchmark.py BOG MAD
```

### 6. Run the C++ Engine

Default route:

```powershell
.\build\Release\benchmark-cpp.exe
```

Custom route:

```powershell
.\build\Release\benchmark-cpp.exe BOG MAD
```

### 7. Validate Results

Run both implementations using the same route:

```powershell
python .\python\benchmark.py BOG MAD

.\build\Release\benchmark-cpp.exe BOG MAD

python .\python\validate.py
```

Both implementations write their results to CSV files.

The validation script compares those files and reports whether they match.

**Important:** Each benchmark execution overwrites its corresponding output file. Always run both implementations with the same route before validating.

### 8. Run Automated Multi-Route Validation

```powershell
python .\python\validate_routes.py
```

This executes all three configured validation scenarios.

### 9. Run C++ Unit Tests

```powershell
ctest --test-dir build -C Release
```

---

## Running the Complete Benchmark

To reproduce the benchmark across different dataset sizes:

```powershell
python .\python\run_benchmarks.py
```

The benchmark automation generates datasets, executes both implementations, validates outputs, and writes a results summary to:

```text
data/benchmark_results.csv
```

### Generate Performance Charts

Install Matplotlib:

```powershell
python -m pip install matplotlib
```

Generate the charts:

```powershell
python .\python\plot_results.py
```

The generated images are saved to:

```text
charts/processing_time.png
charts/speedup.png
```

---

## Computational Complexity

Let:

- **N** = total number of flights
- **M** = number of flights matching the requested route

The main processing operations have the following time complexities:

| Operation | Time Complexity |
|---|---|
| Route filtering | O(N) |
| Price calculation | O(M) |
| Offer sorting | O(M log M) |

Overall time complexity:

**O(N + M log M)**

Both implementations follow the same general algorithmic approach, enabling a comparison of their execution performance.

---

## Software Engineering Practices

This project demonstrates several software engineering practices:

**Modular design**

Reusable C++ functionality is separated into dedicated headers for pricing, filtering, and sorting.

**Automated testing**

CTest executes unit tests for the main processing components.

**Regression validation**

A Python script compares the results of both implementations across multiple routes.

**Reproducibility**

Synthetic datasets use a deterministic random seed.

**Performance measurement**

Benchmarks use warm-up executions, repeated measurements, and median-based reporting.

**Version control**

Git is used to track development and maintain the project on GitHub.

---

## Limitations

This project is an educational performance engineering exercise, not a production airline pricing or reservation system.

Current limitations include:

- Synthetic flight data rather than real airline inventory.
- Simplified pricing rules without dynamic fares, fare families, or ancillary services.
- A limited set of predefined automated validation routes.
- Benchmarks conducted in a Windows development environment.
- No comprehensive memory usage analysis.
- No end-to-end performance comparison including CSV loading.
- No concurrency or parallel processing implementation.

Performance results may vary depending on hardware, compiler configuration, Python version, operating system, and background system activity.

---



## Author

**Kevin Andres Castaneda Acevedo**

Aeronautical Engineer | C++ and Python Software Development | Aviation Technology

GitHub: TkevinLevin24

---

## License

This project is licensed under the MIT License.

See the `LICENSE` file for details.
