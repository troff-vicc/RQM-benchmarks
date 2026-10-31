import subprocess, sys
import csv

cnt_test = 10
cnt_struct = 5

names = ["SegmentTree", "SparseTable", "Treap", "SplayTree", "SqrtTree"]

def F(numbers):
    return sum(numbers) / len(numbers)

files = sys.argv[1:]


structs = [[] for _ in range(cnt_struct)];

with open('results.csv', 'w', newline='') as csvf:
    w = csv.writer(csvf)
    w.writerow(['file', 'n', 'q', 'algo', 'time_ms'])

    for file in files:
        with open(file) as f:
            n, q = map(int, f.readline().split())

        sum = [0] * cnt_struct
        for _ in range(cnt_test):
            times = [float(x) for x in subprocess.run(['./build/rmq_benchmark'],
                                    stdin=open(file),
                                    capture_output=True, text=True).stdout.split()]

            for i in range(cnt_struct):
                sum[i] += times[i]

        for i in range(cnt_struct):
            structs[i].append(sum[i] / cnt_test)
            w.writerow([file, n, q, names[i], sum[i] / cnt_test])


print("Segment Tree ", *structs[0])
print("Sparse Table ", *structs[1])
print("Treap        ", *structs[2])
print("Splay Tree   ", *structs[3])
print("Sqrt Tree    ", *structs[4])
