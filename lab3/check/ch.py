from check import *

a = sorted(a0)
l = []
for i in range(1,10):
    try:
        l.append(eval(f"a{i}"))
    except:
        break

for m, _ in enumerate(l):
    if a == _:
        print(m, end=" ", flush=True)

