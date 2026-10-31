import random
import sys


#seed = 4
#random.seed(seed)

    
path = f'tests/test_{sys.argv[1]}.txt'
type = sys.argv[2];

if (type == "rand"):
    n = int(sys.argv[3])
    q = int(sys.argv[4])

    with open(path, 'w') as f:
        arr = [random.randint(0, 10**6) for _ in range(n)]

        f.write(f"{n} {q}\n")
        f.write(" ".join(map(str, arr)) + "\n")
        for _ in range(q):
            l = random.randint(0, n - 1)
            r = random.randint(l + 1, n)
            f.write(f"{l} {r}\n")

elif (type == "range_a"):
    n = int(sys.argv[3])
    q = int(sys.argv[4])

    with open(path, 'w') as f:
        arr = [i for i in range(1, n + 1)]

        f.write(f"{n} {q}\n")
        f.write(" ".join(map(str, arr)) + "\n")
        for _ in range(q):
            l = random.randint(0, n - 1)
            r = random.randint(l + 1, n)
            f.write(f"{l} {r}\n")

else:
    n = random.randint(0, 10**6);
    q = random.randint(0, 10**6);

    with open(path, 'w') as f:
        arr = [random.randint(0, 10**6) for _ in range(n)]

        f.write(f"{n} {q}\n")
        f.write(" ".join(map(str, arr)) + "\n")
        for _ in range(q):
            l = random.randint(0, n - 1)
            r = random.randint(l + 1, n)
            f.write(f"{l} {r}\n")
