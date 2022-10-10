from sys import argv
from arrs import *

if __name__ == "__main__":
    n = int(argv[1])
    a = sorted(a0)
    l = []
    print("_"*20)
    for i in range(1,n+1):
        l.append(eval(f"a{i}"))
    
    for m, _ in enumerate(l):
        if a == _:
            print(f"Equal: {m + 1}")
        else:
            for k, r in zip(a, _):
                if a.count(k) != _.count(r):
                    print("Not the same number of els: ", m+1)
                    break
            else:
                print("not equal")

