
import csv
import re
import subprocess
import sys
from pathlib import Path

# Project configuration
PROJECT_ROOT = Path(__file__).resolve().parent.parent
PYTHON_DIR = PROJECT_ROOT / "python"
CPP_EXECUTABLE = PROJECT_ROOT / "build" / "Release" / "benchmark-cpp.exe"

DATASET_SIZES = [10000, 100000, 1000000]
SESSIONS = 5

RESULTS_FILE = PROJECT_ROOT / "data" / "benchmark_results.csv"


def run_command(command):
    """Execute a command and return its output."""

    result = subprocess.run(
        command,
        cwd=PROJECT_ROOT,
        capture_output=True,
        text=True,
        check=True
    )

    return result.stdout


def extract_metrics(output):
    """Extract measurements from benchmark output."""

    patterns = {
        "flights": r"Flights loaded:\s*(\d+)",
        "offers": r"Matching offers:\s*(\d+)",
        "time_ms": r"Median processing time:\s*([\d.]+)"
    }

    metrics = {}

    for name, pattern in patterns.items():
        match = re.search(pattern, output)

        if not match:
            raise ValueError(f"Missing metric: {name}")

        metrics[name] = (
            float(match.group(1))
            if name == "time_ms"
            else int(match.group(1))
        )

    return metrics


def main():

    if not CPP_EXECUTABLE.exists():
        print("ERROR: C++ executable not found.")
        print("Compile benchmark.cpp before continuing.")
        sys.exit(1)

    RESULTS_FILE.parent.mkdir(parents=True, exist_ok=True)

    results = []

    for size in DATASET_SIZES:

        print(f"\n=== BENCHMARK: {size:,} FLIGHTS ===")

        # Generate dataset
        print("Generating flight data...")

        run_command([
            sys.executable,
            str(PYTHON_DIR / "generate_data.py"),
            "--flights",
            str(size)
        ])

        
        session_results = []

        for session in range(SESSIONS):
            print(f"\nSession {session + 1}/{SESSIONS}")

            # Alternate execution order
            if session % 2 == 0:
                languages = ["python", "cpp"]
            else:
                languages = ["cpp", "python"]

            metrics = {}

            for language in languages:
                print(f"Running {language} benchmark...")

                if language == "python":
                    output = run_command([
                        sys.executable,
                        str(PYTHON_DIR / "benchmark.py")
                    ])
                else:
                    output = run_command([
                        str(CPP_EXECUTABLE)
                    ])

                metrics[language] = extract_metrics(output)

            # Validate results after both executions
            validation_output = run_command([
                sys.executable,
                str(PYTHON_DIR / "validate.py")
            ])

            if "VALIDATION PASSED" not in validation_output:
                raise RuntimeError("Validation failed")

            if (
                metrics["python"]["flights"] != size
                or metrics["cpp"]["flights"] != size
                or metrics["python"]["offers"] != metrics["cpp"]["offers"]
            ):
                raise RuntimeError("Benchmark metrics do not match")

            python_time = metrics["python"]["time_ms"]
            cpp_time = metrics["cpp"]["time_ms"]

            session_results.append({
                "python_ms": python_time,
                "cpp_ms": cpp_time
            })

            print(f"Python: {python_time:.4f} ms")
            print(f"C++: {cpp_time:.4f} ms")
            print("Validation: PASSED")

        # Median across independent sessions
        import statistics

        python_median = statistics.median(
            item["python_ms"] for item in session_results
        )

        cpp_median = statistics.median(
            item["cpp_ms"] for item in session_results
        )

        speedup = python_median / cpp_median

        results.append({
            "flights": size,
            "matching_offers": metrics["python"]["offers"],
            "python_ms": round(python_median, 4),
            "cpp_ms": round(cpp_median, 4),
            "speedup": round(speedup, 2)
        })

        print(f"\n=== SUMMARY: {size:,} FLIGHTS ===")
        print(f"Python median: {python_median:.4f} ms")
        print(f"C++ median: {cpp_median:.4f} ms")
        print(f"Speedup: {speedup:.2f}x")



    # Save benchmark summary
    with open(RESULTS_FILE, "w", newline="", encoding="utf-8") as file:
        writer = csv.DictWriter(
            file,
            fieldnames=[
                "flights",
                "matching_offers",
                "python_ms",
                "cpp_ms",
                "speedup"
            ]
        )

        writer.writeheader()
        writer.writerows(results)

    print("\n=== BENCHMARK COMPLETED ===")
    print(f"Results saved to: {RESULTS_FILE}")


if __name__ == "__main__":
    main()
