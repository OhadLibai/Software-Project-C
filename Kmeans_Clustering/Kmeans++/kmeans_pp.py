
""" K-means clusters """


import sys
import numpy as np
import pandas as pd

import mykmeanssp


# defining the global variables
global eps, iter, K    # from user
global all_cords, initialized_vectors_indices, all_vectors   # lists
global num_of_vectors, num_of_cords, vec_length    # ints

# initialized and default values
eps = 0
iter = 300
K = 0

all_cords = []
initialized_vectors_indices = []
all_vectors = []

num_of_vectors = 0
num_of_cords = 0
vec_length = 0


# functions #

def assign_args():
    global eps, iter, K
    
    if not (len(sys.argv)==5 or len(sys.argv)==6):
        print("An Error Has Occurred")
        exit(1)

    if len(sys.argv) == 6:
        iter = int(sys.argv[2])

    eps = float(sys.argv[-3])
    K = int(sys.argv[1])


def load_data():
    global num_of_vectors, num_of_cords, vec_length
    global all_cords, all_vectors

    file_1 = pd.read_csv(sys.argv[-2])
    file_2 = pd.read_csv(sys.argv[-1])

    # fixing first row unreading problem #
    first_row_of_file_1 = file_1.columns
    
    build_first_row_file_1=[0 for i in range(len(file_1.columns))]
    for i in range(len(file_1.columns)):
        build_first_row_file_1[i] = float(first_row_of_file_1[i])

    file_1.loc[-1] = build_first_row_file_1
    file_1.index = file_1.index + 1  
    file_1.sort_index(inplace=True)

    first_row_of_file_2 = file_2.columns

    build_first_row_file_2=[0 for i in range(len(file_2.columns))]
    for i in range(len(file_2.columns)):
        build_first_row_file_2[i] = float(first_row_of_file_2[i])

    file_2.loc[-1] = build_first_row_file_2
    file_2.index = file_2.index + 1  
    file_2.sort_index(inplace=True)

    # merging the 2 files #
    file_1.columns = [str(i) for i in range(len(file_1.columns))]
    file_2.columns = [str(i) for i in range(len(file_1.columns))]

    combined = pd.merge(file_1, file_2, on='0', how="inner")
    combined.sort_values(by=combined.columns[0], inplace=True)

    for i in range(0, len(combined)):
        curr_row_redundant_cord = combined.iloc[i]
        curr_row_redundant_cord_as_np_array = np.array(curr_row_redundant_cord)
        curr_row_as_np_array = np.delete(curr_row_redundant_cord_as_np_array, [0]) 

        all_vectors.append(curr_row_as_np_array)

        for j in range(len(curr_row_as_np_array)):
            all_cords.append(float(curr_row_as_np_array[j]))

    num_of_vectors = len(combined)
    num_of_cords = len(all_cords)
    vec_length = len(combined.columns) - 1


def check_K_and_iter():
    K_violates, iter_viloates = False, False

    if (1>=K) or (K>=len(all_vectors)):
        K_violates = True
    if (1>=iter) or (iter>=1000):
        iter_viloates = True

    if K_violates or iter_viloates:
        if K_violates:
            print("Invalid number of clusters!")
        if iter_viloates:
            print("Invalid maximum iteration!")
        exit(1)


def euclidean_dist(vec_1, vec_2):
    num = 0
    for i in range(len(vec_1)):
        num += ((vec_1[i] - vec_2[i])**2)
    return num**0.5


def initialize_first_vectors_indices():
    global initialized_vectors_indices, all_vectors

    all_vectors = np.array(all_vectors)

    distances = [ sys.maxsize for i in range(num_of_vectors)]
    probability_distances = [0 for i in range(num_of_vectors)] 

    curr_rand_vec_indx = 0

    curr_rand_vec = []
    curr_dist=0
    sum_of_all_dist = 0

    np.random.seed(0)

    curr_rand_vec_indx = np.random.choice(num_of_vectors)

    for i in range(K):
        initialized_vectors_indices.append(curr_rand_vec_indx)
        curr_rand_vec = all_vectors[curr_rand_vec_indx]
        
        for j in range(num_of_vectors):
            curr_dist = euclidean_dist(curr_rand_vec, all_vectors[j])
            distances[j] = min(curr_dist, distances[j])  

        sum_of_all_dist = sum(distances)
        for j in range(num_of_vectors):
            probability_distances[j] = distances[j]/sum_of_all_dist

        curr_rand_vec_indx = np.random.choice(num_of_vectors, p=probability_distances)  


def printing(final_cords):
    i=0
    print(initialized_vectors_indices[0], end='')
    for i in range(1, len(initialized_vectors_indices)):
        print("," + str(initialized_vectors_indices[i]), end = '')
    print()

    cord_indx = 0   
    i=0
    j=0

    for i in range(K):
        print('{:.4f}'.format(final_cords[cord_indx]), end = '')
        cord_indx+=1
        if vec_length == 1: 
            continue

        for j in range(vec_length-1):
            print("," + str('{:.4f}'.format((final_cords[cord_indx]))), end = '')
            cord_indx+=1
        print()

    print() # they asked for an empty line at the end of the output



############################
### start of the program ###
############################


# assigns for the arguments
assign_args()

# constructing all_cords list
load_data()

# checking for K and iter
check_K_and_iter()

# initializing first K vectors
initialize_first_vectors_indices()

# getting the list of final cords
final_cords = mykmeanssp.fit(all_cords, initialized_vectors_indices,
                             eps, iter, K,
                             vec_length, num_of_vectors, num_of_cords)

# printing the results
printing(final_cords)