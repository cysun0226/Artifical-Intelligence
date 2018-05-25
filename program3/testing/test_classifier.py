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
