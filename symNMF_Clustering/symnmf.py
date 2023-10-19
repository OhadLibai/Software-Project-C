"""
Python interface

"""

import sys
import numpy as np
import SymNMF

np.random.seed(0)



"""
    Program functions

"""

def load_data(path):
    all_cords_as_list = [] # coordinate is one value of the d-dimension of the vector
    all_vectors_as_list = [] # later for analysis and silhouette_score

    file = open(path, "r")
    curr_vec = file.readline().rstrip('\n').split(",")

    vec_len = len(curr_vec)

    while curr_vec[0]!='':
        for i in range(len(vec_len)):
            curr_vec[i] = float(curr_vec[i])
            all_cords_as_list.append(curr_vec[i])
        all_vectors_as_list.append(curr_vec)

        num_of_vectors += 1
        curr_vec = file.readline().rstrip('\n').split(",")

    file.close()

    data_inf = [all_cords_as_list, vec_len, num_of_vectors, all_vectors_as_list]
    return data_inf



def create_H(norm_mat_as_list):
    H_cords = []
    avg = sum(norm_mat_as_list)/len(norm_mat_as_list)
    for i in range(num_of_clusters*num_of_vectors):
        num = np.random.uniform(0, 2*((avg/num_of_clusters)**0.5))
        H_cords.append(num)
    return H_cords



def deriving_clusters_in_labels(H_as_list, num_of_clusters, num_of_vectors):
    """ matching each vector to which cluster he belongs to by labeling """
    labels = []

    for i in range(num_of_vectors):
        curr_num = H_as_list[i*num_of_clusters] 

        curr_max= curr_num # start with first element in the row
        max_cluster_index = 0 # to which cluster vector belongs to

        for j in range(1, num_of_clusters): # num_of_cluster == col_size
            curr_num = H_as_list[(i*num_of_clusters) + j]

            if curr_num > curr_max: # check later what happens in case of evening with 2 columns
               curr_max = curr_num
               max_cluster_index = j

        labels.append(max_cluster_index)

    return labels



    # for the use of analysis
def symNMF_clustering(path, num_of_clusters): 
    data_inf = load_data(path)

    all_cords_as_list = data_inf[0]
    vec_len = data_inf[1]
    num_of_vectors = data_inf[2]
    all_vectors_as_list = data_inf[3]
    
    W_mat_as_list = SymNMF.norm(all_cords_as_list, vec_len, num_of_vectors)
    H_mat_as_list = create_H(W_mat_as_list)
    final_H_as_list = SymNMF.symnmf(H_mat_as_list, W_mat_as_list, num_of_clusters, vec_len, num_of_vectors)

    labels = deriving_clusters_in_labels(final_H_as_list, num_of_clusters, num_of_vectors)

    clustering = [all_vectors_as_list, labels]

    return clustering



    # function for a symnmf.py run 
def print_mat(mat_as_list, vec_len, num_of_vectors):
    num_index = 0

    for i in range(num_of_vectors):
        print('{:.4f}'.format(mat_as_list[num_index]), end = '')
        num_index+=1
        if vec_len == 1: 
            continue

        for j in range(vec_len-1):
            print("," + str('{:.4f}'.format((mat_as_list[num_index]))), end = '')
            num_index+=1
        print()

    print() # they asked for an empty line at the end of the output





############################    
         ########

### program execution ###

         ########
############################


#   collecting the inputs from CMD
num_of_clusters = sys.argv[1]
goal = sys.argv[2]
path = sys.argv[3]

#   asking for the data
data_inf = load_data(path)
all_cords_as_list = data_inf[0]
vec_len = data_inf[1]
num_of_vectors = data_inf[2]

#   supply the request
mat_as_list = []

if not goal == 'symnmf' :

    if goal == 'sym' :
        mat_as_list = SymNMF.sym(all_cords_as_list, vec_len, num_of_vectors)

    if goal == 'ddg' :
        mat_as_list = SymNMF.ddg(all_cords_as_list, vec_len, num_of_vectors)

    if goal == 'norm' :
        mat_as_list = SymNMF.norm(all_cords_as_list, vec_len, num_of_vectors)

    print_mat(mat_as_list, vec_len, num_of_vectors)

else:
    normalized_mat_as_list = SymNMF.norm(all_cords_as_list, vec_len, num_of_vectors)
    H_mat_as_list = create_H(normalized_mat_as_list)

    symnmf_mat_as_list = SymNMF.symnmf(H_mat_as_list, normalized_mat_as_list,    # num_of_clusters = H_vec_len 
                                       num_of_clusters, vec_len, num_of_vectors) # vec_len = norm_vec_len  
    print_mat(symnmf_mat_as_list, num_of_clusters, num_of_vectors)




