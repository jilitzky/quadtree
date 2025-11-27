import random

with open("Points.txt", "w") as f:
    for _ in range(10000):
        x = round(random.uniform(-1000, 1000), 2)
        y = round(random.uniform(-1000, 1000), 2)
        f.write(f"{x},{y}\n")

print("Done!")
