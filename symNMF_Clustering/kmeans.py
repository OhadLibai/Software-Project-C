
""" K-means clusters """


"""
This should serve analysis and suffices her with final solution for the clusters

"""

import sys


# defining the global variables

# constants
global epsilon, max_iter
epsilon = 0.0001
max_iter = 300

# shared across functions
global all_vectors   # all_vectors represented as mat    
global vectors_labels



class Centroid:
    def __init__(self, centroid_val, N):
        self.end_index = 0
        self.included_vectors = [0 for i in range(N)] # each centroid will actually have a list of index instead of containing all the vectors themselves.
                                                      # potentially a centroid can have a N vetors
        self.centroid_val = centroid_val    # a vector


def load_data(path):
    global all_vectors

    file = open(path, "r")
    curr_vector = file.readline().rstrip('\n').split(",")

    all_vectors = []

    while curr_vector[0]!='':
        for i in range(len(curr_vector)):
            curr_vector[i] = float(curr_vector[i])

        all_vectors.append(curr_vector)
        curr_vector = file.readline().rstrip('\n').split(",")

    file.close()



def initialize_first_centroids_by_order(num_of_clusters):
    initialized_centroids = []

    for i in range(num_of_clusters):
        curr_centroid = Centroid(all_vectors[i], len(all_vectors))
        initialized_centroids.append(curr_centroid)

    return initialized_centroids


def euclidean_dist(vector, centroid_vec):
    num = 0
    for i in range(len(vector)):
        num += (vector[i] - centroid_vec[i])**2
    return num**0.5


def compute_mu(centroid):
    updated_centroid_val = []

    for j in range(len(centroid.centroid_val)): #j moves from 0 coordinate to the m-1 coordinate, where m is the width of the vectors
        cord_val = 0
        for i in range(centroid.end_index):
            index = centroid.included_vectors[i]
            cord_val += all_vectors[index][j]
        cord_val = cord_val/centroid.end_index
        updated_centroid_val.append(cord_val)

    return updated_centroid_val




def finding_clusters(initialized_centroids):
    """ this function also finds the centroids of course """
    global vectors_labels
    vectors_labels = [ -1  for i in range(len(all_vectors))] # initialize lables

    tmp_centroids_list = initialized_centroids
    less_then_epsilon = False
    iter = 0
    centroid_index = -1 # for the labling. To which index the vectors belongs to

    while (not less_then_epsilon) and (iter < max_iter):
        less_then_epsilon = True

        for i in range(len(all_vectors)):
            min_dist = int(sys.maxsize)
            closest_centroid = None
            centroid_index = -1 

            for j in range(len(tmp_centroids_list)): 
                curr_dist = euclidean_dist(all_vectors[i], tmp_centroids_list[j].centroid_val)
                if curr_dist<min_dist:
                    closest_centroid = tmp_centroids_list[j]
                    min_dist = curr_dist
                    centroid_index = j

            closest_centroid.included_vectors[closest_centroid.end_index] = i
            closest_centroid.end_index = closest_centroid.end_index + 1
            vectors_labels[i] = centroid_index

        for centroid in tmp_centroids_list:
            updated_centroid_val = compute_mu(centroid) # the return value is of course a vector and it is the new centroid_val.
                                                        # NOTE: calculated mu according the classical definition of mean in n-dimension.
            delta = euclidean_dist(updated_centroid_val, centroid.centroid_val)
            if not delta<epsilon:
                less_then_epsilon = False

            centroid.centroid_val = updated_centroid_val
            centroid.end_index = 0

        iter += 1




# for the use of analysis
def kmeans_clustering(path, num_of_clusters):

    # reading the data
    load_data(path)

    #initializing first K centroids
    initialized_centroids = initialize_first_centroids_by_order(num_of_clusters)
    
    #final_centroids_list = finding_final_centroids(initialized_centroids)

    # finding the centroids (and the lables)
    finding_clusters(initialized_centroids)

    clustering = [all_vectors, vectors_labels]

    return clustering









































