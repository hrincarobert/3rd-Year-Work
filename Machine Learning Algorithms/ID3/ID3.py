import numpy as np
import pandas as pd


def compute_probabilities(valori_atribute):

    total_values = len(valori_atribute)
    probabilities = {}

    for value in set(valori_atribute):
        probabilities[value] = valori_atribute.value_counts()[value] / total_values

    return probabilities

def calculate_entropy(valori_atribute):

    probabilities = compute_probabilities(valori_atribute)
    entropy = -sum(p * np.log2(p) for p in probabilities.values() if p > 0)
    return entropy

def calculate_conditional_entropy(atribut_dat, atribut_target):

    valori_unice_atribut_dat = set(atribut_dat)
    conditional_entropy = 0

    for valoare_atribut in valori_unice_atribut_dat:
        subset_pt_val_atribut = atribut_target[atribut_dat == valoare_atribut]
        conditional_entropy += (len(subset_pt_val_atribut) / len(atribut_target)) * calculate_entropy(subset_pt_val_atribut)

    return conditional_entropy

def calculate_information_gain(atribut_dat, atribut_target):

    entropy_Y = calculate_entropy(atribut_target)
    conditional_entropy_X = calculate_conditional_entropy(atribut_dat, atribut_target)
    information_gain = entropy_Y - conditional_entropy_X

    return information_gain

def find_root_nod_tree(train_data, label, valori):
    
    feature_list = train_data.columns.drop(label) 
                                       
    max_info_gain = -1
    root_nod_tree_max = None
    for i, feature in enumerate(feature_list, start=1):
        feature_info_gain = calculate_information_gain(train_data[feature], train_data[label])
        print(f"Iteration {i}: Information Gain for {feature}: {feature_info_gain}")

        if max_info_gain < feature_info_gain: 
            max_info_gain = feature_info_gain
            root_nod_tree_max = feature
            
    return root_nod_tree_max

def generate_sub_tree(atribut_n, train_data, label, valori):

    val_unice_atribut = train_data[atribut_n].value_counts(sort=False) 
    tree = {}
    
    for val_atribut, count in val_unice_atribut.items():
        val_atribut_data = train_data[train_data[atribut_n] == val_atribut] 
        
        ok_pure = False 
        for c in valori: 
            nr_valori = val_atribut_data[val_atribut_data[label] == c].shape[0] 

            if nr_valori == count: 
                tree[val_atribut] = c 
                train_data = train_data[train_data[atribut_n] != val_atribut] 
                ok_pure = True
        if not ok_pure: 
            tree[val_atribut] = "keep_going" 
                                      
            
    return tree, train_data

def make_tree(root, prev_val_atribut, train_data, label, valori):
    if train_data.shape[0] != 0: 
        root_nod_tree_max = find_root_nod_tree(train_data, label, valori) 
        tree, train_data = generate_sub_tree(root_nod_tree_max, train_data, label, valori) 
        next_root_max = None
        
        if prev_val_atribut is not None: 
            root[prev_val_atribut] = dict()
            root[prev_val_atribut][root_nod_tree_max] = tree
            next_root_max = root[prev_val_atribut][root_nod_tree_max]
        else: 
            root[root_nod_tree_max] = tree
            next_root_max = root[root_nod_tree_max]
        
        for nod_tree, is_subTree_expandable in list(next_root_max.items()): 
            if is_subTree_expandable == "keep_going": 
                val_atribut_data = train_data[train_data[root_nod_tree_max] == nod_tree] 
                make_tree(next_root_max, nod_tree, val_atribut_data, label, valori) 

def id3_discrete(train_data, label):
    train_data_copy = train_data.copy() 
    tree = {}
    valori = train_data_copy[label].unique() 
    make_tree(tree, None, train_data_copy, label, valori) 
    return tree

main_train_data = pd.read_csv("train\monks1.csv") 
main_train_data.head() 

tree = id3_discrete(main_train_data, 'label')

print(tree)