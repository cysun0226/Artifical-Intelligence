import sys

data_file_name = str(sys.argv[1])

# struct
class Iris(object):
    def __init__(self, id=None, sepal_l=None, sepal_w=None, petal_l=None, petal_w=None, iris_class=None):
        self.id = id
        self.sepal_l = sepal_l
        self.sepal_w = sepal_w
        self.petal_l = petal_l
        self.petal_w = petal_w
        self.iris_class = iris_class
    def __str__(self):
        out = 'id: {:<3} | '.format(self.id)
        out = out + 's_l = ' + str(self.sepal_l) + ', '
        out = out + 's_w = ' + str(self.sepal_w) + ', '
        out = out + 'p_l = ' + str(self.petal_l) + ', '
        out = out + 'p_w = ' + str(self.petal_w) + ' | '
        out = out + 'class: ' + self.iris_class
        return out

class Node(object):
    def __init__(self, left_child=None, right_child=None, feature=None, threshold=None, gini_idx=None):
        if left_child != None:
            self.left_child = left_child
        if right_child != None:
            self.right_child = right_child
        if feature != None:
            self.feature = feature
        if threshold != None:
            self.threshold = threshold
        if gini_idx != None:
            self.gini_idx = gini_idx




iris_data = []

# data_file = open(data_file_name)
# for line in data_file:
     # line = line.split(',')

with open(data_file_name) as data_file:
    data = data_file.read().splitlines()
idx = 0
for line in data:
    line = line.split(',')
    # print(line)
    if line[0] != '':
        if line[4].endswith('*'):
            line[4] = line[4][:-1]
        iris_data.append(Iris(idx, float(line[0]), float(line[1]), float(line[2]), float(line[3]), line[4]))
    idx += 1
