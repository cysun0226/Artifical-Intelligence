import math
import operator

# enum
LEFT = 0
RIGHT = 1

# struct
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
        if feature != None:
            self.feature = feature
        else:
            self.feature = None
        if threshold != None:
            self.threshold = threshold
        else:
            self.threshold = None


def count_class_num(data_set):
    class_list = {}
    for data in data_set:
        if data.class_name not in class_list:
            class_list[data.class_name] = 1
        else:
            class_list[data.class_name] += 1
    return class_list

def gini_index(data_set):
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

def get_feature_boundary(data_set, feature):
    # sorting
    sorted_data = sorted(data_set, key=operator.itemgetter(feature))
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

def bulid_decision_tree(data_set, feature_list, evaluation=gini_index):
    # current value
    cur_info = evaluation(data_set)
    best_gain = 0.0
    best_feature = None
    best_threshold = None
    best_split = None
    best_diff = 10000
    total = len(data_set)

    # choose threshold
    for feature in feature_list:
        # handle the continuous features
        feature_boundary = get_feature_boundary(data_set, feature)
        for feature_value in feature_boundary:
            left_list, right_list = split_data(data_set, feature, feature_value)
            p = len(left_list)/total # probability
            gain = cur_info - (p*evaluation(left_list) + (1-p)*evaluation(right_list))
            if (gain > best_gain):
                best_gain = gain
                best_feature = feature
                best_threshold = feature_value
                best_split = (left_list, right_list)

    # infomation gain (if gain = 0, stop)
    # keep spliting
    if best_gain > 0:
        left_child = bulid_decision_tree(best_split[LEFT], feature_list)
        right_child = bulid_decision_tree(best_split[RIGHT], feature_list)
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
