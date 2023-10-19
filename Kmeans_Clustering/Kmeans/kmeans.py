
""" K-means clusters """


import sys


# defining the global variables
global less_then_epsilon, epsilon, num_of_iterations, iter, K, N, all_vectors, all_centroids, vec_width

epsilon = 0.001
less_then_epsilon = False
num_of_iterations = 0
iter = 200
K = 0
N = 0
all_vectors = []
all_centroids = []
vec_width = 0


class Centroid:
    def __init__(self, centroid_val, N):
        self.end_index = 0
        self.included_vectors = [0 for i in range(N)] # each centroid will actually have a list of index instead of containing all the vectors themselves.
                                                      # potentially a centroid can have a N vetors
        self.cenroid_val = centroid_val


def load_file():
    global N, vec_width

    file = open(sys.argv[-1], "r")
    curr_vector = file.readline().rstrip('\n').split(",")
    while curr_vector[0]!='':
        for i in range(len(curr_vector)):
            curr_vector[i] = float(curr_vector[i])
        all_vectors.append(curr_vector)
        curr_vector = file.readline().rstrip('\n').split(",")
    file.close()
    vec_width = len(all_vectors[0])
    N = len(all_vectors)

    #del curr_vector - optional -


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


def initialize_first_centroids_by_order():
    for i in range(K):
        curr_centroid = Centroid(all_vectors[i], N)
        all_centroids.append(curr_centroid)


def euclidean_dist(vector, centroid_vec):
    num = 0
    for i in range(len(vector)):
        num += (vector[i] - centroid_vec[i])**2
    return num**0.5


def compute_mu(centroid):
    updated_centroid_val = []

    for j in range(vec_width): #j moves from 0 coordinate to the m-1 coordinate, where m is the width of the vectors
        cord_val = 0
        for i in range(centroid.end_index):
            index = centroid.included_vectors[i]
            cord_val += all_vectors[index][j]
        cord_val = cord_val/centroid.end_index
        updated_centroid_val.append(cord_val)

    return updated_centroid_val


############################
### start of the program ###
############################


if not (len(sys.argv)==3 or len(sys.argv)==4):
    print("An Error Has Occurred")
    exit(1)
if len(sys.argv) == 4:
    iter = int(sys.argv[2])
K = int(sys.argv[1])

# constructing all_vectors list
load_file()

#checking for K and iter
check_K_and_iter()

#initializing first K centroids
initialize_first_centroids_by_order()


# finding the centroids
while (not less_then_epsilon) and (num_of_iterations < iter):
    less_then_epsilon = True

    for i in range(len(all_vectors)):
        min_dist = int(sys.maxsize)
        closest_centroid = None
        for centroid in all_centroids:
            curr_dist = euclidean_dist(all_vectors[i], centroid.cenroid_val)
            if curr_dist<min_dist:
                closest_centroid = centroid
                min_dist = curr_dist

        closest_centroid.included_vectors[closest_centroid.end_index] = i
        closest_centroid.end_index = closest_centroid.end_index + 1

    for centroid in all_centroids:
        updated_centroid_val = compute_mu(centroid) # the return value is of course a vector and it is the new centroid_val.
                                                    # NOTE: calculated mu according the classical definition of mean in n-dimension.
        delta = euclidean_dist(updated_centroid_val, centroid.cenroid_val)
        if not delta<epsilon:
            less_then_epsilon = False

        del centroid.cenroid_val
        centroid.cenroid_val = updated_centroid_val
        centroid.end_index = 0

    num_of_iterations += 1

#printing
for centroid in all_centroids:
    print('{:.4f}'.format(centroid.cenroid_val[0]), end = '')
    if len(centroid.cenroid_val) == 1:
        print()
        continue

    for i in range(1,len(centroid.cenroid_val)):
        print("," + str('{:.4f}'.format((centroid.cenroid_val[i]))), end = '')
    print() 
print() #they asked for an empty line at the end of the output

del all_vectors
del all_centroids