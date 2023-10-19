
"""
Analysis program for the 2 methods: K-means and symNMF

"""

import sys

from sklearn.metrics import silhouette_score

import symnmf as symnmf_py
import kmeans as kmeans_py


############################    
         ########

### program execution ###

         ########
############################


# collecting the inputs from CMD
num_of_clusters = sys.argv[1]
path = sys.argv[2]

# part one: clusters according to symNMF
symNMF_clustering = symnmf_py.symNMF_clustering(path, num_of_clusters) # symNMF_clustering[0] == all_vectors_as_list
                                                                       # symNMF_clustering[1] == labels                                                                         
symNMF_score = silhouette_score(symNMF_clustering[0], symNMF_clustering[1])

# part two: clusters according to K-means
Kmeans_clustering = kmeans_py.kmeans_clustering(path, num_of_clusters) # Kmeans_clustering[0] == all_vectors_as_list
                                                                       # Kmeans_clustering[1] == lables 
Kmeans_score = silhouette_score(Kmeans_clustering[0], Kmeans_clustering[1]) 


print("nmf: " , symNMF_score)
print("kmeans: " , Kmeans_score)





