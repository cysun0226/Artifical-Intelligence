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

class Sequence(object):
    def __init__(self, id, dist):
        self.direction = 'SKIP'
        self.id = id
        self.distance = dist
    def __str__(self):
        output = '(' + self.direction
        if self.direction != 'SKIP':
            if self.distance >= 0:
                output += '+'
            else:
                output += '-'
        output += ') ' + str(self.distance)

        return output



# BFS
# def BFS(seq, target):
#     start = Point(0,0)
#
#     # keep track of all visited nodes
#     explored = []
#     # keep track of nodes to be checked
#     queue = [start]
#
#     levels = {}         # this dict keeps track of levels
#     levels[start] = 0    # depth of start node is 0
#
#     visited = [start]     # to avoid inserting the same node twice into the queue
#
#     # keep looping until there are nodes still to be checked
#     while queue:
#        # pop shallowest node (first node) from queue
#         node = queue.pop(0)
#         explored.append(node)
#         neighbours = graph[node]
#
#         # add neighbours of node to queue
#         for neighbour in neighbours:
#             if neighbour not in visited:
#                 queue.append(neighbour)
#                 visited.append(neighbour)
#
#     return

# main
seq_file = open(seq_filename, "r")
lines = seq_file.readlines()
seq_file.close()

for line in lines:
    line = line.split()
    strategy = line[0]
    del line[0]
    target = Point(int(line[0]), int(line[1]))
    #print(target.x)
    #print(target.y)
    del line[0:2]
    seq = []
    for i in range(len(line)):
        seq.append(Sequence(i,int(line[i])))
