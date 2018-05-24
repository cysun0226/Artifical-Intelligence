import sys
from random import shuffle
import math
import operator
import CART.decision_tree as dTree
import random_forest.random_forest as rf

data_file_1 = str(sys.argv[1])
data_file_2 = str(sys.argv[2])
data_feature_list = { 'Iris': [ 'p_l', 'p_w', 's_l', 's_w' ] }
train_set_ratio = 0.3

# for debug
def wait_key():
    key = input("Press Enter to continue...")
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

# testing
def test_classifier(test_set, decision_tree, classify_function, forest_classify=None, log='n', classifier_name='result'):
    if log == 'y':
        print('\n-- ' + classifier_name + ' --\n')
    correct = 0.0
    for data in test_set:
        if forest_classify == None:
            predict = classify_function(data, decision_tree)
        else:
            predict = classify_function(data, decision_tree, forest_classify)

        if predict == data.class_name:
            correct += 1
            if log == 'y':
                print('[correct] data = {0:15}'.format(data.class_name) + ' | predict = ' + predict)
        else:
            if log == 'y':
                print('[ wrong ] data = {0:15}'.format(data.class_name) + ' | predict = ' + predict)

    accuracy = correct / len(test_set)
    if log == 'y':
        print('\n\naccuracy = %.3f' % accuracy)
    return accuracy

# main
iris_data = []
test_data = []
acc_avg_sum = 0
rf_acc_avg_sum = 0

# read data files
with open(data_file_1) as data_file:
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

# with open(data_file_2) as data_file:
#     data = data_file.read().splitlines()

# print result header
print('\n\n===== ' + data_file_1 + ' data set =====\n')
print('train_set = %d' % int(math.floor(train_set_ratio*len(iris_data))))
print('test_set = %d' % (len(iris_data) - math.floor(train_set_ratio*len(iris_data))))

time = int(input("\ntest time: "))
print_result = input("print detailed results?(y/n): ")
print('')

# progress bar
pbar_width = time
# setup toolbar
sys.stdout.write("> progress: [%s]" % (" " * pbar_width))
sys.stdout.flush()
sys.stdout.write("\b" * (pbar_width+1)) # return to start of line, after '['

# pbar = tqdm(total=time, bar_format='{l_bar}{bar}', position=0)
for x in range(time):
    if print_result=='y':
        print('\n\n== test %d ==' % (x+1))
    # shuffle and pick the training set
    shuffle(iris_data)
    train_set = iris_data[0:int(math.floor(0.7*total))]
    test_set = iris_data[int(math.floor(0.7*total)):total]

    # build decision tree
    decision_tree = dTree.bulid_decision_tree(train_set, data_feature_list['Iris'])

    # test
    acc_avg_sum += test_classifier(test_set, decision_tree, dTree.classify, log=print_result)

    # bulid random forest
    random_forest = rf.build_random_forest(
        train_set,
        data_feature_list['Iris'],
        dTree.bulid_decision_tree,
        tree_num=30
        )

    # test
    rf_acc_avg_sum += test_classifier(test_set, random_forest, rf.random_forest_classify, dTree.classify, log=print_result)

    # progress bar
    sys.stdout.write("-")
    sys.stdout.flush()
    # pbar.update(1)

# avg test result
# pbar.close()
print('\n\n=== test results ===')
print('\nCARF avg accuracy = %.3f' % (acc_avg_sum / time))
print('random forest avg accuracy = %.3f' % (rf_acc_avg_sum / time))
print('')
