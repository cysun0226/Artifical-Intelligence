# AI program 1
# BFS, IDS, A*
# by 0416045 cysun

import sys
from collections import namedtuple

# test data
# seq_filename = str(sys.argv[1])
seq_filename = "IntroAI_PR1_test.txt"
sequence = []

# struct
def getDirName(num):
    return {
        0 : 'x+',
        1 : 'y+',
        2 : 'x-',
        3 : 'y-',
        4 : 'S',
    }[num]

class Point(object):
    def __init__(self, x, y):
        self.x = x
        self.y = y
    def __str__(self):
        return '(' + str(self.x) + ', ' + str(self.y) + ')'
    def __eq__(self, other):
        if isinstance(other, self.__class__):
            return (self.x == other.x) and (self.y == other.y)
        else:
            return False
    def __hash__(self):
        return 41 * (41 + self.x) + self.y


class Node(object):
    def __init__(self, dir, dist, point):
        self.dir = dir
        self.dist = dist
        self.pos = point
    def __str__(self):
        new_pos = move(self.dir, self.dist, self.pos)
        return '(' + self.dir + ') ' + str(self.dist) + '\t\t' + str(new_pos)

class Sequence(object):
    def __init__(self, id, dist):
        self.dir = 'S'
        self.id = id
        self.dist = dist
    def __str__(self):
        return '(' + self.dir + ') ' + str(self.dist)

# functions
def move(dir, dist, pos):
    new_pos = Point(pos.x, pos.y)
    if dir == 'x+':
        new_pos.x += dist
    elif dir == 'y+':
        new_pos.y += dist
    elif dir == 'x-':
        new_pos.x -= dist
    elif dir == 'y-':
        new_pos.y -= dist
    return new_pos

def print_result(sol, cnt):
    print('initial\t\t(0,0)')

    for i in range(len(sol)-1):
        print(sol[i])

    print(sol[-1], end='')
    print(' Goal\n')
    print('step count = ' + str(cnt) + '\n')



# BFS
def BFS(seq, target):
    pos = Point(0,0)
    # start = seq[0]


    # keep track of all visited nodes
    explored = []

    # keep track of nodes to be checked
    frontier = []
    for i in range(5):
        frontier.append(Node(getDirName(i), seq[0].dist, pos))
    # frontier = [ Node('x+', seq[0].dist, pos), Node('y+', seq[0].dist, pos),
    #              Node('x-', seq[0].dist, pos), Node('y-', seq[0].dist, pos),
    #              Node('skip', seq[0].dist)]

    level = 0
    track = {pos:[]}         # this dict keeps track of levels
    # levels[start] = 0    # depth of start node is 0

    visited = [pos]     # to avoid inserting the same node twice into the queue

    # keep looping until there are nodes still to be checked
    step = 0
    while frontier:
        #print('\ncurrent frontier : \n')
        #for n in frontier:
        #    print(n)
        step += 1



        # pop shallowest node (first node) from queue
        node = frontier.pop(0)
        explored.append(node)

        new_pos = move(node.dir, node.dist, node.pos)

        if new_pos not in track:
            visited.append(new_pos)
            # print(track[node.pos])
            track[new_pos] = [e for e in track[node.pos]]
            track[new_pos].append(node)

            # print(track[new_pos])

            if new_pos == target:
            # if new_pos == Point(-7, -2):
                break

            for i in range(5):
                # next seq
                next = move(getDirName(i), node.dist, node.pos)
                # print(len(track[new_pos]))
                frontier.append(Node(getDirName(i), seq[len(track[new_pos])].dist, new_pos))

        #wait = input("\n(press anykey to contunine...)")

    # print solution
    print_result(track[target], step)
    #print('initial\t\t(0,0)')

    #for i in range(len(track[target])-1):
    #    print(track[target][i])

    #print(track[target][-1], end='')
    #print(' Goal\n')
    #print('step count = ' + str(step) + '\n')

    return

# IDS (iterative deepening search)
# DLS
IDS_step = 0

def DLS(seq, node, level, target, limit, track):
    global IDS_step
    IDS_step += 1
    level += 1
    if level > limit:
        return False

    update_track = [e for e in track]
    update_track.append(node)

    # print('\npos = ' + str(node.pos))
    # print('\nlevel = ' + str(level))
    #print('\n\n-- track (lv = ' + str(level) + ', limit = ' + str(limit) + ') --\n')
    #for n in update_track:
    #    print(n)

    # print('new_pos = ' + str(new_pos))
    # wait = input("\n(press anykey to contunine...)")

    if node.pos == target:
        print_result(track, IDS_step)
        #print('initial\t\t(0,0)')
        #for i in range(len(track)-1):
        #    print(track[i])

        #print(track[-1], end='')
        #print(' Goal\n')
        return True

    new_pos = move(node.dir, node.dist, node.pos)

    for i in range(5):
        new_node = Node(getDirName(i), seq[level].dist, new_pos)
        if DLS(seq, new_node, level, target, limit, update_track):
            return True



def IDS(seq, target):
    IDS_step = 0
    pos = Point(0,0)
    goal = False
    for level in range(len(seq)):
        track = []
        for i in range(5):
            node = Node(getDirName(i), seq[0].dist, pos)
            if DLS(seq, node, 0, target, level, track):
                goal = True
                break
        if goal:
            break



# main
seq_file = open(seq_filename, "r")
lines = seq_file.readlines()
seq_file.close()
test_data_id = 0

for line in lines:
    test_data_id += 1
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

    print('\n=== test ' + str(test_data_id) + ' === \n')
    print('search strategy = ' + strategy + ' \n')

    if strategy == 'BFS':
        BFS(seq, target)
    elif strategy == 'IDS':
        IDS(seq, target)
