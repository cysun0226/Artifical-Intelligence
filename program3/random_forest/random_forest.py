from random import shuffle
import math
import operator


def build_random_forest(data_set, feature_list, bulid_decision_tree, tree_bag_ratio=0.7, feature_bag_ratio=0.75, tree_num=10):
    random_forest = []
    total = len(data_set)
    for i in range(tree_num):
        # create tree bag
        shuffle(data_set)
        tree_bag = data_set[0:int(math.floor(tree_bag_ratio*total))]
        # create feature bag
        shuffle(feature_list)
        feature_bag = feature_list[0:int(math.floor(feature_bag_ratio*len(feature_list)))]
        # bulid_decision_tree
        tree = bulid_decision_tree(tree_bag, feature_bag)
        random_forest.append(tree)
    return random_forest

def random_forest_classify(data, random_forest, classify):
    voting = {}
    for tree in random_forest:
        data_class = classify(data, tree)
        if data_class not in voting:
            voting[data_class] = 1
        else:
            voting[data_class] += 1

    # return the class that has most voting
    predict_class = max(voting.items(), key=operator.itemgetter(1))[0]
    return predict_class
