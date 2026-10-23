
import csv
import hashlib
from pathlib import Path

project_root = Path(__file__).resolve().parent.parent
data_dir = project_root / "data"

python_file = data_dir / "python_results.csv"
cpp_file = data_dir / "cpp_results.csv"


def calculate_sha256(file_path):
    sha256 = hashlib.sha256()

    with open(file_path, "rb") as file:
        for chunk in iter(lambda: file.read(65536), b""):
            sha256.update(chunk)

    return sha256.hexdigest()


def load_results(file_path):
    with open(file_path, newline="", encoding="utf-8") as file:
        return list(csv.DictReader(file))


def main():
    print("=== AIRLINE OFFER VALIDATION ===")

    if not python_file.exists() or not cpp_file.exists():
        print("VALIDATION FAILED: Missing result files")
        raise SystemExit(1)

    python_results = load_results(python_file)
    cpp_results = load_results(cpp_file)

    print(f"Python offers: {len(python_results)}")
    print(f"C++ offers: {len(cpp_results)}")

    if python_results != cpp_results:
        print("VALIDATION FAILED: Results differ")
        raise SystemExit(1)

    python_hash = calculate_sha256(python_file)
    cpp_hash = calculate_sha256(cpp_file)

    if python_hash == cpp_hash:
        print("SHA-256: IDENTICAL")
    else:
        print("SHA-256: Different file encoding or formatting")

    print("VALIDATION PASSED: All offers match")


if __name__ == "__main__":
    main()
