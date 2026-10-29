import subprocess
import sys
from pathlib import Path


# Project paths
PROJECT_ROOT = Path(__file__).resolve().parent.parent

PYTHON_BENCHMARK = PROJECT_ROOT / "python" / "benchmark.py"
CPP_BENCHMARK = PROJECT_ROOT / "build" / "Release" / "benchmark-cpp.exe"
VALIDATOR = PROJECT_ROOT / "python" / "validate.py"


# Routes to validate
ROUTES = [
    ("BOG", "MIA"),
    ("BOG", "MAD"),
    ("MDE", "JFK"),
]


def run_command(command):
    """Execute a command and return whether it succeeded."""

    result = subprocess.run(
        command,
        cwd=PROJECT_ROOT,
        capture_output=True,
        text=True
    )

    if result.returncode != 0:
        print(result.stdout)
        print(result.stderr)
        return False

    return True


def main():
    print("=== CROSS-LANGUAGE VALIDATION ===\n")

    results = []

    for origin, destination in ROUTES:
        route = f"{origin} -> {destination}"

        # Run Python benchmark
        python_ok = run_command([
            sys.executable,
            str(PYTHON_BENCHMARK),
            origin,
            destination
        ])

        # Run C++ benchmark
        cpp_ok = False

        if python_ok:
            cpp_ok = run_command([
                str(CPP_BENCHMARK),
                origin,
                destination
            ])

        # Compare results
        validation_ok = False

        if python_ok and cpp_ok:
            validation_ok = run_command([
                sys.executable,
                str(VALIDATOR)
            ])

        passed = python_ok and cpp_ok and validation_ok

        results.append((route, passed))

        status = "PASSED" if passed else "FAILED"

        print(f"{route:<15} {status}")

    # Summary
    total = len(results)
    passed_count = sum(passed for _, passed in results)
    failed_count = total - passed_count

    print("\n=== SUMMARY ===")
    print(f"Routes tested: {total}")
    print(f"Routes passed: {passed_count}")
    print(f"Routes failed: {failed_count}")

    if failed_count == 0:
        print("\nALL VALIDATIONS PASSED")
        return 0

    print("\nSOME VALIDATIONS FAILED")
    return 1


if __name__ == "__main__":
    sys.exit(main())