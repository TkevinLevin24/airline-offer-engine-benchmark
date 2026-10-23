
import csv
from pathlib import Path

import matplotlib.pyplot as plt

PROJECT_ROOT = Path(__file__).resolve().parent.parent
RESULTS_FILE = PROJECT_ROOT / "data" / "benchmark_results.csv"
CHARTS_DIR = PROJECT_ROOT / "charts"

CHARTS_DIR.mkdir(exist_ok=True)

# Read benchmark results
with open(RESULTS_FILE, newline="", encoding="utf-8") as file:
    results = list(csv.DictReader(file))

labels = [f"{int(row['flights']):,}" for row in results]
python_times = [float(row["python_ms"]) for row in results]
cpp_times = [float(row["cpp_ms"]) for row in results]
speedups = [float(row["speedup"]) for row in results]

positions = list(range(len(labels)))
bar_width = 0.35

# Chart 1: Processing time comparison
fig, ax = plt.subplots(figsize=(9, 5))

ax.bar(
    [x - bar_width / 2 for x in positions],
    python_times,
    width=bar_width,
    label="Python"
)

ax.bar(
    [x + bar_width / 2 for x in positions],
    cpp_times,
    width=bar_width,
    label="C++"
)

ax.set_xticks(positions)
ax.set_xticklabels(labels)
ax.set_xlabel("Number of flights")
ax.set_ylabel("Processing time (ms)")
ax.set_title("Airline Offer Engine: Python vs C++")
ax.legend()
ax.grid(axis="y", alpha=0.25)

fig.tight_layout()
fig.savefig(CHARTS_DIR / "processing_time.png", dpi=200)
plt.close(fig)

# Chart 2: Speedup comparison
fig, ax = plt.subplots(figsize=(9, 5))

ax.bar(labels, speedups)

ax.set_xlabel("Number of flights")
ax.set_ylabel("Speedup (Python / C++)")
ax.set_title("C++ Speedup over Python")
ax.grid(axis="y", alpha=0.25)

for index, speedup in enumerate(speedups):
    ax.text(
        index,
        speedup + 0.15,
        f"{speedup:.2f}x",
        ha="center"
    )

ax.set_ylim(0, max(speedups) * 1.2)

fig.tight_layout()
fig.savefig(CHARTS_DIR / "speedup.png", dpi=200)
plt.close(fig)

print("Charts generated successfully:")
print(CHARTS_DIR / "processing_time.png")
print(CHARTS_DIR / "speedup.png")
