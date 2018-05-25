import sys
from random import shuffle
import math
import operator

# my code
import CART.decision_tree as dTree
import random_forest.random_forest as rf
import testing.test_classifier as test
import time

# draw chart
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt


data_file_1 = str(sys.argv[1])
data_file_2 = str(sys.argv[2])
data_file_3 = str(sys.argv[3])
data_feature_list = { 'Iris': [ 'p_l', 'p_w', 's_l', 's_w' ] }
TRAIN_SET_RATIO = 0.8

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
    def __init__(self, features=None, class_name=None):
        self.features = features
        self.class_name = class_name
    def __getitem__(self, feature):
        return(self.features[feature])

class Digit(object):
    def __init__(self, features=None, class_name=None):
        self.features = features
        self.class_name = class_name
    def __getitem__(self, feature):
        return(self.features[feature])

class Cross(object):
    def __init__(self, features=None, class_name=None):
        self.features = features
        self.class_name = class_name
    def __getitem__(self, feature):
        return(self.features[feature])


def testing(data_set, train_set_ratio, rf_tree_num, tree_bag_ratio, feature_bag_ratio, classifier='CART'):
    total = len(data_set['DATA'])

    # shuffle and pick the training set
    shuffle(data_set['DATA'])
    train_set = data_set['DATA'][0:int(math.floor(train_set_ratio*total))]
    test_set = data_set['DATA'][int(math.floor(train_set_ratio*total)):total]

    if classifier == 'CART':
        # build decision tree
        decision_tree = dTree.bulid_decision_tree(train_set, data_feature_list[data_set['NAME']])
        # test
        dTree_result = test.test_classifier(test_set, decision_tree, dTree.classify)
        return dTree_result

    if classifier == 'rf':
        # bulid random forest
        random_forest = rf.build_random_forest(
            train_set,
            data_feature_list[data_set['NAME']],
            dTree.bulid_decision_tree,
            feature_bag_ratio=feature_bag_ratio,
            tree_bag_ratio=tree_bag_ratio,
            tree_num=rf_tree_num
            )
        # test
        rf_result = test.test_classifier(test_set, random_forest, rf.random_forest_classify, dTree.classify)
        return rf_result

def progress_bar(start_time, cur_progress, total):
    end = time.time()
    elapsed = end - start_time
    elapsed_s = elapsed % 60
    elapsed = elapsed / 60
    if cur_progress == total-1:
        cur_progress = total
    i_print = math.floor((cur_progress/total)*50)
    i_per = (cur_progress/total)*100
    sys.stdout.write('\r')
    sys.stdout.write('Exection time: ')
    sys.stdout.write("%d" % elapsed)
    sys.stdout.write(' m ')
    sys.stdout.write("%d" % elapsed_s)
    sys.stdout.write(' s.  ')
    sys.stdout.write("[%-50s] %.2f%%" % ('='*i_print, i_per))
    sys.stdout.flush()

def get_result_avg(log, exec_time):
    avg_log = []
    for i in range(len(log[0])):
        acc_sum = 0.0
        for exec_t in range(exec_time):
            acc_sum += log[exec_t][i]
        avg_log.append(acc_sum/exec_time)
    return avg_log

# main
iris_data = []
test_data = []

# read data files
# read Iris
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
        iris_data.append(Iris(f, line[4]))

# read digit
digit_data = []
with open(data_file_2) as data_file:
    data = data_file.read().splitlines()
data.pop()
for line in data:
    line = line.split(',')
    features = {}
    for i in range(64):
        features[i] = int(line[i])
    class_name = line[64]
    digit_data.append(Digit(features, class_name))

digit_feature = []
for i in range(64):
    digit_feature.append(i)
data_feature_list['Digit'] = digit_feature

# t = dTree.bulid_decision_tree(digit_data, data_feature_list['Digit'])
# pred = dTree.classify(digit_data[9], t)

# read cross200
cross_data = []
with open(data_file_3) as data_file:
    data = data_file.read().splitlines()
data.pop()
for line in data:
    line = line.split(' ')
    line = [col for col in line if col != '']
    features = { 'attr1': float(line[0]), 'attr2': float(line[1]) }
    cross_data.append(Digit(features, line[2]))

data_feature_list['Cross'] = [ 'attr1', 'attr2' ]


data_sets = [ {'DATA': iris_data, 'NAME': 'Iris'},
              {'DATA': cross_data, 'NAME': 'Cross'},
              {'DATA': digit_data, 'NAME': 'Digit'} ]
CART_log = {'Iris':[], 'Cross':[], 'Digit':[]}
rf_log = {'Iris':[], 'Cross':[], 'Digit':[]}

total_tree_num_log = {}
total_train_ratio_log = {}
total_tree_bag_log = {}
total_feature_bag_log = {}

total_tree_num_label = {}
total_train_ratio_label = {}
total_tree_bag_label = {}
total_feature_bag_label = {}

# testing each data_set
for data_set in data_sets:
    # print result header
    total = len(data_set['DATA'])
    print('\n\n===== ' + data_set['NAME'] + ' dataset =====\n')
    print('data num = %d' % total)

    exec_time = int(input("test time: "))
    if exec_time == -1:
        continue
    print_result = input("print detailed results?(y/n): ")

    # init
    CART_acc_sum = 0
    rf_acc_sum = 0
    train_set_ratio = 0.8
    rf_tree_num = 10
    max_tree_num = int(input("\nmax tree num: "))
    tree_bag_ratio = float(input("default tree bag ratio: "))
    feature_bag_ratio = float(input("default feature bag ratio: "))
    only_tree = input("only tree num?(y/n)")
    start = time.time()

    tree_num_log = []
    train_ratio_log = []
    tree_bag_log = []
    feature_bag_log = []

    tree_num_label = []
    train_ratio_label = []
    tree_bag_label = []
    feature_bag_label = []

    for t in range(exec_time):
        print('\n\n== test %d ==' % (t+1))

        CART_acc = []
        rf_acc = []

        if only_tree != 'y':
            # test trainning_set ratio
            start = time.time()
            if print_result == 'y':
                print('\n\n-- test trainning set ratio --\n')
            for r in range(10):
                CART_result = testing(data_set, 0.5+0.05*r, rf_tree_num, tree_bag_ratio, feature_bag_ratio, 'CART')
                CART_acc.append(CART_result)
                if t == 0:
                    train_ratio_label.append(0.5+0.05*r)
                if print_result == 'y':
                    progress_bar(start, r, 10)
            train_ratio_log.append(CART_acc)

        # test tree num in random_forest
        print('\n\n-- test random forest tree num --\n')
        start = time.time()
        rf_acc = []
        for tree_num in range(1,max_tree_num+1):
            rf_result = testing(data_set, train_set_ratio, tree_num, tree_bag_ratio, feature_bag_ratio, 'rf')
            rf_acc.append(rf_result)
            if t == 0:
                tree_num_label.append(tree_num)
            # if print_result == 'y':
            progress_bar(start, tree_num, max_tree_num)
        tree_num_log.append(rf_acc)

        if only_tree != 'y':
            # test tree bag ratio
            start = time.time()
            if print_result == 'y':
                print('\n\n-- test tree bag ratio --\n')
            rf_acc = []
            for r in range(1,20+1):
                rf_result = testing(data_set, train_set_ratio, rf_tree_num, float(r)/20, feature_bag_ratio, 'rf')
                rf_acc.append(rf_result)
                if t == 0:
                    tree_bag_label.append(float(r)/20)
                if print_result == 'y':
                    progress_bar(start, r, 20)
            tree_bag_log.append(rf_acc)

        if only_tree != 'y':
            # test feature bag ratio
            start = time.time()
            if print_result == 'y':
                print('\n\n-- test feature bag ratio --\n')
            rf_acc = []
            for r in range(1,20+1):
                rf_result = testing(data_set, train_set_ratio, rf_tree_num, tree_bag_ratio, float(r)/20, 'rf')
                rf_acc.append(rf_result)
                if t == 0:
                    feature_bag_label.append(float(r)/20)
                if print_result == 'y':
                    progress_bar(start, r, 20)
            feature_bag_log.append(rf_acc)

    # avg test result
    # total_tree_num_log[data_set['NAME']] =  get_result_avg(tree_num_log, exec_time)
    # total_tree_num_label[data_set['NAME']] =  tree_num_label

    plt.plot(tree_num_label, get_result_avg(tree_num_log, exec_time))
    plt.ylim((0.5,1))
    plt.savefig( data_set['NAME'] + '_tree_num.png')
    plt.close()

    if only_tree == 'y':
        continue

    plt.plot(train_ratio_label, get_result_avg(train_ratio_log, exec_time))
    plt.ylim((0.5,1))
    plt.savefig( data_set['NAME'] + '_train_ratio.png')
    plt.close()

    plt.plot(tree_bag_label, get_result_avg(tree_bag_log, exec_time))
    plt.ylim((0,1))
    plt.savefig( data_set['NAME'] + '_tree_bag.png')
    plt.close()

    plt.plot(feature_bag_label, get_result_avg(feature_bag_log, exec_time))
    plt.ylim((0,1))
    plt.savefig( data_set['NAME'] + '_feature_bag.png')
    plt.close()





    # total_train_ratio_log[data_set['NAME']] = get_result_avg(train_ratio_log, exec_time)
    # total_tree_bag_log[data_set['NAME']] = get_result_avg(tree_bag_log, exec_time)
    # total_feature_bag_log[data_set['NAME']] = get_result_avg(feature_bag_log, exec_time)
    #
    # total_train_ratio_label[data_set['NAME']] = train_ratio_label
    # total_tree_bag_label[data_set['NAME']] = tree_bag_label
    # total_feature_bag_label[data_set['NAME']] = feature_bag_label



    # print('\n\n=== test results ===')
    # print('\nCARF avg accuracy = %.3f' % (CART_acc_sum / exec_time))
    # print('random forest avg accuracy = %.3f' % (rf_acc_sum / exec_time))
