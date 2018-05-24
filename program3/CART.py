import sys
from random import shuffle
import math
import operator

data_file_name = str(sys.argv[1])
feature_list = { 'Iris': [ 'p_l', 'p_w', 's_l', 's_w' ] }
LEFT = 0
RIGHT = 1

# for debug
def wait_key():
    key = raw_input("Press Enter to continue...")
    return

def print_list(array):
    for member in array:
        print(member)

# struct
# LABEL Iris
class Iris(object):
    def __init__(self, id=None, s_l=None, s_w=None, p_l=None, p_w=None, class_name=None, features=None):
        self.id = id
        self.s_l = s_l
        self.s_w = s_w
        self.p_l = p_l
        self.p_w = p_w
        self.class_name = class_name
        self.features = features
    def __str__(self):
        out = 'id: {:<3} | '.format(self.id)
        out = out + 's_l = ' + str(self.s_l) + ', '
        out = out + 's_w = ' + str(self.s_w) + ', '
        out = out + 'p_l = ' + str(self.p_l) + ', '
        out = out + 'p_w = ' + str(self.p_w) + ' | '
        out = out + 'class: ' + self.class_name
        return out

class Node(object):
    def __init__(self, left=None, right=None, feature=None, threshold=None):
        if left != None:
            self.left = left
        else:
            self.left = None

        if right != None:
            self.right = right
        else:
            self.right = None
        # if data != None:
        #     self.data = data
        if feature != None:
            self.feature = feature
        else:
            self.feature = None
        if threshold != None:
            self.threshold = threshold
        else:
            self.threshold = None
        # if gini_idx != None:
        #     self.gini_idx = gini_idx

def count_class_num(data_set):
    class_list = {}
    for data in data_set:
        if data.class_name not in class_list:
            class_list[data.class_name] = 1
        else:
            class_list[data.class_name] += 1
    return class_list

def gini(data_set):
    total = len(data_set)
    num_of_each_class = count_class_num(data_set)
    impurity = 0.0
    for class_name in num_of_each_class:
        count = num_of_each_class[class_name]
        impurity += float(count)/total * float(count)/total
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

def bulid_decision_tree(data_set, evaluation=gini):
    # wait_key()
    print('\n --- new node ---\n')
    print('data_set = ' + str(len(data_set)))
    print_list(data_set)

    # current gini
    cur_gini = evaluation(data_set)
    best_gain = 0.0
    best_feature = None
    best_threshold = None
    best_split = None
    best_diff = 10000
    total = len(data_set)

    # choose threshold
    for feature in feature_list['Iris']:
        # handle the continuous features
        feature_boundary = get_feature_boundary(data_set, feature)
        for feature_value in feature_boundary:
            left_list, right_list = split_data(data_set, feature, feature_value)
            p = len(left_list)/total # probability
            gain = cur_gini - (p*evaluation(left_list) + (1-p)*evaluation(right_list))
            diff = abs(len(left_list) - len(right_list))
            # if ((gain > best_gain) or (gain >= best_gain and diff < best_diff)):
            print('new_gain = ' + str(gain))
            if (gain > best_gain):
                best_gain = gain
                best_feature = feature
                best_threshold = feature_value
                best_diff = diff
                best_split = (left_list, right_list)

    print('cur_gini = ' + str(cur_gini))
    print('best_gain = ' + str(best_gain))
    print('best_feature = ' + str(best_feature))
    print('best_threshold = ' + str(best_threshold))
    if best_split != None:
        print('left_list = ' + str(len(best_split[LEFT])))
        print_list(best_split[LEFT])
        print('right_list = ' + str(len(best_split[RIGHT])))
        print_list(best_split[RIGHT])

    # gini gain (if gain = 0, stop)
    # keep spliting
    if best_gain > 0:
        left_child = bulid_decision_tree(best_split[LEFT])
        right_child = bulid_decision_tree(best_split[RIGHT])
        return Node(left_child, right_child, best_feature, best_threshold)
    # reach leaf node
    else:
        return Node(left=None, right=None, feature=data_set[0].class_name)


def classify(data, decision_tree):
    # reach leaf node
    if (decision_tree.left == None and decision_tree.right == None):
        return decision_tree.feature
    # keep traversing the tree
    if data.features[decision_tree.feature] < decision_tree.threshold:
        data_class = classify(data, decision_tree.left)
    else:
        data_class = classify(data, decision_tree.right)

    return data_class


def get_feature_boundary(data_set, feature):
    # sorting
    sorted_data = sorted(data_set, key=operator.attrgetter(feature))
    # find boundary (class change)
    boundary = []
    prev_class = sorted_data[0].class_name
    prev_feature_value = None
    for data in sorted_data:
        cur_feature_value = data.features[feature]
        if data.class_name != prev_class and (cur_feature_value not in boundary):
            prev_class = data.class_name
            boundary.append((cur_feature_value+prev_feature_value)/2)
        prev_feature_value = data.features[feature]
    return boundary


# def find_feature_boundary(feature_values):

    # pick up boundary




# main
iris_data = []
test_data = []
acc_avg_sum = 0

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
total = len(iris_data)

# print result header
print('\n\n===== ' + data_file_name + ' data set =====\n')
print('train_set = %d' % int(math.floor(0.7*len(iris_data))))
print('test_set = %d' % (len(iris_data) - math.floor(0.7*len(iris_data))))

time = int(input("\ntest time: "))
print_result = raw_input("print detailed results?(y/n): ")

for x in range(time):
    print('\n\n===== test %d =====' % (x+1))
    # shuffle and pick the training set
    shuffle(iris_data)
    train_set = iris_data[0:int(math.floor(0.7*total))]
    test_set = iris_data[int(math.floor(0.7*total)):total]

    # handle the continuous features
    # boundary = get_feature_boundary(train_set)
    # print(boundary['p_w'])

    # build decision tree
    decision_tree = bulid_decision_tree(train_set)

    # test
    correct = 0.0

    for data in test_set:
        predict = classify(data, decision_tree)
        if predict == data.class_name:
            correct += 1
            if print_result == 'y':
                print('[correct] data = {0:15}'.format(data.class_name) + ' | Predict = ' + predict)
        else:
            if print_result == 'y':
                print('[ wrong ] data = {0:15}'.format(data.class_name) + ' | Predict = ' + predict)

    accuracy = correct / len(test_set)
    acc_avg_sum += accuracy
    print('\naccuracy = ' + str(accuracy))
    print(count_class_num(test_set))
print('')

# avg test result
print('\n\n===== test results =====')
print('\ntest = %d' % time)
print('\navg accuracy = %f' % (acc_avg_sum / time))
print('')
