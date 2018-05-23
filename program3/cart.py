import sys

data_file_name = str(sys.argv[1])
feature_list = { 'Iris': [ 's_l', 's_w', 'p_l', 'p_w' ] }

# struct
class Iris(object):
    def __init__(self, id=None, sepal_l=None, sepal_w=None, petal_l=None, petal_w=None, class_name=None, features=None):
        self.id = id
        self.sepal_l = sepal_l
        self.sepal_w = sepal_w
        self.petal_l = petal_l
        self.petal_w = petal_w
        self.class_name = class_name
        self.features = features
    def __str__(self):
        out = 'id: {:<3} | '.format(self.id)
        out = out + 's_l = ' + str(self.sepal_l) + ', '
        out = out + 's_w = ' + str(self.sepal_w) + ', '
        out = out + 'p_l = ' + str(self.petal_l) + ', '
        out = out + 'p_w = ' + str(self.petal_w) + ' | '
        out = out + 'class: ' + self.iris_class
        return out

class Node(object):
    def __init__(self, left_child=None, right_child=None, data = None, feature=None, threshold=None, gini_idx=None):
        if left_child != None:
            self.left_child = left_child
        if right_child != None:
            self.right_child = right_child
        if data != None:
            self.data = data
        if feature != None:
            self.feature = feature
        if threshold != None:
            self.threshold = threshold
        if gini_idx != None:
            self.gini_idx = gini_idx

def count_class_num(data_set):
    class_list = {}
    for data in data_set:
        if data.class_name not in class_list:
            class_list[data.class_name] = 1
        else:
            class_list[data.class_name] += 1
    return class_list

def calculate_gini(data_set):
    total = len(data_set)
    num_of_each_class = count_class_num(data_set)
    impurity = 0.0
    for count in num_of_each_class:
        impurity += count/total * count/total
    return 1 - impurity

def split_data(data_set, feature, threshold):
    left_list = []
    right_list = []
    for data in data_set:
        if data.features[feature] < threshold:
            left_list.append(data)
        else:
            right_list.append(data)
    return (left_list, right_list)

def get_feature_values(data_set, feature):
    feature_values = []
    for data in data_set:
        feature_values.append(data.features[feature])
    return feature_values

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
        f = { 's_l': float(line[0]), 's_w':float(line[1]), 'p_l': float(line[2]), 'p_w': float(line[3]) }
        if line[4].endswith('*'):
            line[4] = line[4][:-1]
        iris_data.append(Iris(idx, float(line[0]), float(line[1]), float(line[2]), float(line[3]), line[4], f))
    idx += 1

print(count_class_num(iris_data))
