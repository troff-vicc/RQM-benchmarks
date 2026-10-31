import csv
import sys
import matplotlib.pyplot as plt
from collections import defaultdict

COLORS = {
    "SegmentTree": "#1f77b4",
    "SparseTable": "#ff7f0e",
    "SqrtTree":    "#2ca02c",
    "Treap":       "#d62728",
    "SplayTree":   "#9467bd",
}


data = defaultdict(dict)
meta = {}
with open(sys.argv[1] if len(sys.argv) > 1 else "results.csv") as f:
    for row in csv.DictReader(f):
        fname = row["file"]
        data[fname][row["algo"]] = float(row["time_ms"])
        meta[fname] = (int(row["n"]), int(row["q"]))

files = list(data.keys())
algos = [a for a in COLORS if any(a in data[f] for f in files)]


fig, ax = plt.subplots(figsize=(11, 5))
x = range(len(files))
w = 0.8 / len(algos)
for i, algo in enumerate(algos):
    vals = [data[f].get(algo, 0) for f in files]
    ax.bar([xi + i * w for xi in x], vals, w, label=algo, color=COLORS[algo])

def label(f):
    n, q = meta[f]
    return f"n={n}\nq={q}"

ax.set_xticks([xi + 0.4 for xi in x])
ax.set_xticklabels([label(f) for f in files], fontsize=8)
ax.set_ylabel("total time, ms")
ax.set_yscale("log")
ax.legend()
ax.set_title("Total time per test")
ax.grid(True, axis="y", which="both", alpha=0.3)
plt.tight_layout()
plt.savefig("results_bar.png", dpi=150)
print("→ results_bar.png")


fig, ax = plt.subplots(figsize=(8, 5))
for algo in algos:
    pts = []
    for f in files:
        n, q = meta[f]
        if n == 0:
            continue
        pts.append((q / n, data[f][algo]))
    pts.sort()
    if not pts:
        continue
    xs, ys = zip(*pts)
    ax.plot(xs, ys, "o-", label=algo, color=COLORS[algo], markersize=8)

ax.set_xscale("log")
ax.set_yscale("log")
ax.set_xlabel("q / n")
ax.set_ylabel("total time, ms")
ax.axvline(1.0, color="gray", linestyle="--", alpha=0.5, label="q = n")
ax.legend()
ax.grid(True, which="both", alpha=0.3)
ax.set_title("Crossover: total time vs q/n")
plt.tight_layout()
plt.savefig("results_crossover.png", dpi=150)
print("→ results_crossover.png")
