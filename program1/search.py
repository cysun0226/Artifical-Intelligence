# AI program 1
# BFS, IDS, A*
# by 0416045 cysun

import sys
from collections import namedtuple

# test data
seq_filename = str(sys.argv[1])
sequence = []

# struct
Point = namedtuple("Point", "x y")

# BFS


# main
seq_file = open(seq_filename, "r")
lines = seq_file.readlines()
seq_file.close()

for line in lines:
    line = line.split()
    print(line)
    strategy = line[0]
    del line[0]
    print("strategy = " + strategy)
    target = Point(int(line[0]), int(line[1]))
    print("target = ", end="")
    print(target)
    print(target.x)
    print(target.y)
    del line[0:2]
    # print(line)
    seq = []
    print("seq = ")
    for num in line:
        seq.append(int(num))
    print(seq)
    print()
