# AI program 1
# BFS, IDS, A*
# by 0416045 cysun

import sys
import time
import math
from heapq import *
# from collections import namedtuple

# test data
seq_filename = str(sys.argv[1])
# seq_filename = "IntroAI_PR1_test.txt"
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
    # def __eq__(self, other):
    #     if isinstance(other, self.__class__):
    #         return (self.x == other.x) and (self.y == other.y)
    #     else:
    #         return False
    # def __hash__(self):
    #     return 41 * (41 + self.x) + self.y

class Node(object):
    def __init__(self, dir, dist, point):
        self.dir = dir
        self.dist = dist
        self.pos = point
        # self.predecessor = Node('S', 0, Point(0,0))
        self.cost = 0
        self.heur = sys.maxsize
    def __str__(self):
        new_pos = move(self.dir, self.dist, self.pos)
        return '(' + self.dir + ') ' + str(self.dist) + '\t\t' + str(new_pos)
    def __lt__(self, other):
        return self.heur < other.heur

class Sequence(object):
    def __init__(self, id, dist):
        self.dir = 'S'
        self.id = id
        self.dist = dist
    def __str__(self):
        return '(' + self.dir + ') ' + str(self.dist)

# functions
def waitkey():
    wait = input("\n(press anykey to contunine...)")

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

def print_result(sol, cnt, t, max_f = 0):
    print('initial\t\t(0, 0)')

    for i in range(len(sol)-1):
        print(sol[i])

    print(sol[-1], end='')
    print(' Goal\n')
    print('step count = ' + str(cnt))
    print('cost time = ' + str(round(t*1000, 5)) + ' ms')
    if max_f != 0:
        print('max queue = ' + str(max_f))
    print()

# BFS
def BFS(seq, target):
    start = time.time()
    pos = Point(0,0)
    goal = Point(0,0)
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
    max_frontier = 0
    while frontier:
        # print('\ncurrent frontier : \n')
        # for n in frontier:
        #    print(n)
        # waitkey()


        if len(frontier) >= max_frontier:
            max_frontier = len(frontier)
        step += 1
        # pop the first frontier
        node = frontier.pop(0)
        explored.append(node)

        new_pos = move(node.dir, node.dist, node.pos)

        # visited.append(new_pos)
        track[new_pos] = [e for e in track[node.pos]]
        track[new_pos].append(node)

        if new_pos.x == target.x and new_pos.y == target.y:
            goal = new_pos
            break

        if len(track[new_pos]) > len(seq)-1:
            continue

        for i in range(5):
            # next seq
            next = move(getDirName(i), node.dist, node.pos)
            # print(len(track[new_pos]))
            frontier.append(Node(getDirName(i), seq[len(track[new_pos])].dist, new_pos))

    # print solution
    end = time.time()
    print_result(track[goal], step, end-start, max_frontier)

    return

# IDS (iterative deepening search)
# DLS
IDS_step = 0
start = 0

def DLS(seq, node, level, target, limit, track):
    global start
    global IDS_step
    level += 1
    if level > limit:
        return False

    IDS_step += 1

    # update_track = [e for e in track[node.pos]]
    update_track = [e for e in track]
    update_track.append(node)

    # print('\nIDS step = ' + str(IDS_step))
    # print('\npos = ' + str(node.pos))
    # print('\nlevel = ' + str(level))
    # print('\n\n-- track (lv = ' + str(level) + ', limit = ' + str(limit) + ') --\n')
    # for n in update_track:
    #    print(n)
    #
    # waitkey()

    new_pos = move(node.dir, node.dist, node.pos)

    if new_pos.x == target.x and new_pos.y == target.y:
        end = time.time()
        print_result(update_track, IDS_step, end-start)
        return True

    # if new_pos in track:
    #     if len(update_track) < len(track[new_pos]):
    #         return False
    #
    # track[new_pos] = update_track


    for i in range(5):
        new_node = Node(getDirName(i), seq[level].dist, new_pos)
        # if DLS(seq, new_node, level, target, limit, track):
        if DLS(seq, new_node, level, target, limit, update_track):
            return True



def IDS(seq, target):
    global start
    global IDS_step
    start = time.time()
    pos = Point(0,0)
    IDS_step = 0
    goal = False
    for level in range(len(seq)):
        # track = {pos:[]}
        track = []
        for i in range(5):
            node = Node(getDirName(i), seq[0].dist, pos)
            if DLS(seq, node, 0, target, level, track):
                goal = True
                break
        if goal:
            break

# A*
def print_queue(queue):
    out_queue = nsmallest(len(queue), queue)
    for n in out_queue:
        print(str(n) + ' heur = ' + str(n.heur))

def heuristic(p1, p2):
    return math.floor(abs(p1.x - p2.x)/9) + math.floor(abs(p1.y - p2.y)/9)

def my_heuristic(p1, p2):
    dist = (p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y)
    dist = dist ** 0.5 # sqrt
    return dist

def A_star(seq, target, heur = 'default'):
    start = time.time()
    pos = Point(0,0)
    root = Point(0,0)
    goal = Point(0,0)
    pri_queue = []
    step = 0
    max_queue = 0

    # insert start point
    for i in range(5):
        next = Node(getDirName(i), seq[0].dist, pos)
        new_pos = move(next.dir, next.dist, next.pos)
        if heur == 'default':
            next.heur = heuristic(new_pos, target)
        else:
            next.heur = my_heuristic(new_pos, target)
        heappush(pri_queue, next)

    track = {pos:[]}

    while pri_queue:
        if len(pri_queue) >= max_queue:
            max_queue = len(pri_queue)
        step += 1
        node = heappop(pri_queue)
        new_pos = move(node.dir, node.dist, node.pos)

        # print('\n-- pri_queue | pos = ' + str(new_pos) + ' -- \n')
        # print_queue(pri_queue)


        if (new_pos in track) and (len(track[new_pos]) < len(track[node.pos])+1):
           continue
        else:
           track[new_pos] = [e for e in track[node.pos]]
           track[new_pos].append(node)

        if new_pos.x == target.x and new_pos.y == target.y:
            goal = new_pos
            break

        # print('\nlen(track[new_pos] = ' + str(len(track[new_pos])) + '\n')
        # wait = input("\n(press anykey to contunine...)")

        if len(track[new_pos]) >= len(seq)-1:
            continue

        for i in range(5): # add new frontier
            next = Node(getDirName(i), seq[len(track[new_pos])].dist, new_pos)
            next_pos = move(next.dir, next.dist, next.pos)
            if heur == 'default':
                next.heur = heuristic(next_pos, target)
            else:
                next.heur = my_heuristic(next_pos, target)
            heappush(pri_queue, next)

    # print result
    end = time.time()
    print_result(track[goal], step, end-start, max_queue)



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
    elif strategy == 'A*':
        A_star(seq, target)
        print('\nsearch strategy = ' + strategy + '(my) \n')
        A_star(seq, target, 'my')
