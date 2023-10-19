
/* K-means clusters */


# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# define PY_SSIZE_T_CLEAN
# include <Python.h>


/*defining the structs*/

typedef struct Cord Cord;
typedef struct Vector Vector;
typedef struct Centroid Centroid;


struct Cord {
    double value;
    Cord* next; 
};


struct Vector {
    int from_matrix;
    Cord* head_cord;
    Vector* next;
    Vector* next_in_cluster;
}; 


struct Centroid {
    int cluster_size;
    Vector* first_member;
    Vector* last_to_join;
    Vector* centroid_vector;
    Centroid* next;
};


/* defining global variables */

Vector* head_vec; /*the first vector in the matrix*/
Vector* helper_new_vec_pt; /*helper pointer vector in compute_mu*/
Centroid* first_centroid; /*first centroid in linked_list of centroids*/

int K = 0;
int iter = 0;
double epsilon = 0.0;

long long num_of_vectors = 0;
long long num_of_cords = 0;
int vec_size = 0;

int num_of_iterations = 0;
int less_then_epsilon = 0;
double curr_euclidean_dist = 0.0;
double delta = 0.0;



/* allocators */

Cord* allocate_cord() {
    Cord* new_cord;
    new_cord = malloc(sizeof(Cord));
    if (new_cord == NULL) {
        printf("An Error Has Occurred\n");
        exit(1);
    }

    return new_cord;
}                    


Vector* allocate_vector() {
    Vector* new_vector;
    new_vector = malloc(sizeof(Vector));
    if (new_vector == NULL) {
        printf("An Error Has Occurred\n");
        exit(1);
    }

    return new_vector;
}        


Centroid* allocate_centroid() {
    Centroid* new_centroid;
    new_centroid = malloc(sizeof(Centroid));
    if (new_centroid == NULL) {
        printf("An Error Has Occurred\n");
        exit(1);
    }

    return new_centroid;
}


/* program functions */


void assign_initial_values(double eps, int iterations, int k, int vec_length, long long total_vec_num, long long cords_num) {
    epsilon = eps;
    iter = iterations;
    K = k;
    vec_size = vec_length;
    num_of_vectors = total_vec_num;
    num_of_cords = cords_num;
}


void load_matrix(double cords[]) {
    Vector* curr_vec;
    Cord* head_cord;
    Cord* curr_cord;

    head_vec = allocate_vector();
    curr_vec = head_vec;
    
    head_cord = allocate_cord();
    curr_cord = head_cord;

    int i = 0, j = 0;

    while(i<num_of_cords) {

        for (j=0; j<vec_size-1; j++) {
            curr_cord->value = cords[i]; 
            i++;

            curr_cord->next = allocate_cord();
            curr_cord = curr_cord->next;
            curr_cord->next = NULL;
        }
        curr_cord->value = cords[i]; 
        i++;

        curr_vec->head_cord = head_cord;
        curr_vec->next_in_cluster = NULL;
        curr_vec->from_matrix = 1;

        if (i==num_of_cords)
            break;

        curr_vec->next = allocate_vector();
        curr_vec = curr_vec->next;

        curr_vec->next = NULL;
        head_cord = allocate_cord();
        curr_cord = head_cord;
        curr_cord->next = NULL;
    }
}


void initialize_first_K_centroids(int indices[]) {
    Vector* curr_vec;
    Centroid* curr_centroid;
    
    first_centroid = allocate_centroid();
    curr_centroid = first_centroid;

    int i, j; /* i for centroids and indices list. j is for number of nexts */

    for (i=0; i<K; i++) {
        curr_vec = head_vec;

        for (j=0; j<(indices[i]); j++) {
            curr_vec = curr_vec->next;
        }

        curr_centroid->cluster_size = 0;
        curr_centroid->first_member = NULL;
        curr_centroid->last_to_join = NULL;
        curr_centroid->centroid_vector = curr_vec;
        
        curr_centroid->next = allocate_centroid();
        curr_centroid = curr_centroid->next;
        curr_centroid->next = NULL;
    }
}

void euclidean_dist(Vector* mat_vec, Vector* cen_vec, int is_delta) {
    Cord* curr_mat_cord;
    Cord* curr_cen_cord;
    double val_mat_vec = 0.0, val_cen_vec = 0.0;
    double num = 0.0;
    int i;

    curr_mat_cord = mat_vec->head_cord;
    curr_cen_cord = cen_vec->head_cord;

    for (i=0; i<vec_size; i++ ) {
        val_mat_vec = curr_mat_cord->value;
        val_cen_vec = curr_cen_cord->value;
        num += pow(val_cen_vec - val_mat_vec ,2);
        curr_mat_cord = curr_mat_cord->next;
        curr_cen_cord = curr_cen_cord->next;
    }
    num = sqrt(num);

    if (is_delta == 1) 
        delta = num;

    else
        curr_euclidean_dist = num;
}



Vector* compute_mu_cluster_size_one(Centroid* centroid) {
    return centroid->centroid_vector;
}


Vector* compute_new_mu(Centroid* centroid) { 
    Vector* curr_vec_in_cluster;
    Cord* curr_cord;
    Cord* curr_cord_new_vec;
    Cord* head_cord_of_new_vec;
    int i,j;

    /* constructing the vector */
    helper_new_vec_pt = allocate_vector();

    helper_new_vec_pt->next = NULL;

    head_cord_of_new_vec = allocate_cord();
    curr_cord_new_vec = head_cord_of_new_vec;

    for (j=0; j<(vec_size-1); j++) {
        curr_cord_new_vec->value = 0.0;
        curr_cord_new_vec->next = allocate_cord();
        curr_cord_new_vec = curr_cord_new_vec->next;
    }
    curr_cord_new_vec->value = 0.0;
    curr_cord_new_vec->next = NULL;

    helper_new_vec_pt->head_cord = head_cord_of_new_vec;
    helper_new_vec_pt->from_matrix = 0;

    /* iterating over the vectors in the cluster */
    curr_vec_in_cluster = centroid->first_member;

    for (i=0; i < (centroid->cluster_size); i++) {
        curr_cord = curr_vec_in_cluster->head_cord;
        curr_cord_new_vec = head_cord_of_new_vec;

        for (j=0 ; j<vec_size; j++) {
            curr_cord_new_vec->value += curr_cord->value;
            curr_cord = curr_cord->next;
            curr_cord_new_vec = curr_cord_new_vec->next;
        }

        curr_vec_in_cluster = curr_vec_in_cluster->next_in_cluster;    
    }
    /* averaging */
    curr_cord_new_vec = head_cord_of_new_vec;
    for (i=0; i < (vec_size) ; i++) { 
        curr_cord_new_vec->value = (curr_cord_new_vec->value)/(centroid->cluster_size);
        curr_cord_new_vec = curr_cord_new_vec->next;
    }
    
    return helper_new_vec_pt; 
}


Vector* compute_mu(Centroid* centroid) {              
    
    if (centroid->cluster_size == 1) 
        return compute_mu_cluster_size_one(centroid);
    
    return compute_new_mu(centroid);
}


void constructing_clusters() {
    Vector* curr_vec;
    Centroid* closest_centroid;
    Centroid* curr_centroid;
    int i,j;

    double min_dist;
    
    curr_vec = head_vec;

    for (i=0; i<num_of_vectors; i++) {
        min_dist = __INT_MAX__; 
        closest_centroid = NULL;
        curr_centroid = first_centroid;

        /* finiding which centroid the vector belongs to */
        for (j=0; j<K; j++) {
            
            euclidean_dist(curr_vec, curr_centroid->centroid_vector, 0); /* 0 is for assigning to curr_euclidean_dist */

            if (curr_euclidean_dist<min_dist) {
                closest_centroid = curr_centroid;
                min_dist = curr_euclidean_dist; 
            }

            curr_centroid = curr_centroid->next;
        }
        
        /* adding the vector to the centroid cluster */
        if (closest_centroid->cluster_size == 0 ){
            closest_centroid->first_member = curr_vec;
            closest_centroid->last_to_join = curr_vec;
        }
        else {
            closest_centroid->last_to_join->next_in_cluster = curr_vec;
            closest_centroid->last_to_join = curr_vec;
            closest_centroid->last_to_join->next_in_cluster = NULL;
        }
        closest_centroid->cluster_size++;

        curr_vec = curr_vec->next;
    }
}


void checking_convergence_for_every_centroid() {
    Vector* new_vec_cen;
    Centroid* curr_centroid;
    int i;

    curr_centroid = first_centroid;

    for (i=0; i<K ; i++) {

        new_vec_cen = compute_mu(curr_centroid);

        euclidean_dist(new_vec_cen, curr_centroid->centroid_vector, 1); /* 1 is for assining to delta */

        if (! (delta<epsilon))
            less_then_epsilon = 0;
        
        if ((curr_centroid->cluster_size > 1) && !(curr_centroid->centroid_vector->from_matrix)) /* every centroid has at least one vector in his list, the vector he was initialized with */
            free(curr_centroid->centroid_vector);

        curr_centroid->centroid_vector = new_vec_cen;
        curr_centroid->cluster_size = 0;
        curr_centroid->last_to_join = NULL;
        curr_centroid->first_member = NULL;

        curr_centroid = curr_centroid->next;
    }
}


void convergence() {

    while ((!(less_then_epsilon == 1)) && (num_of_iterations < iter)) { 
        less_then_epsilon = 1;

        constructing_clusters();

        checking_convergence_for_every_centroid();
    
        num_of_iterations++;
    }
}



double* organizing_final_cords() {
    Centroid* curr_cen;
    Vector* curr_vec;
    Cord* curr_cord;

    double* final_cords = (double*) malloc(K*vec_size*sizeof(double));
    if (final_cords == NULL) {
        printf("An Error Has Occurred\n");
        return NULL;
    }

    curr_cen = first_centroid;    

    int cord_indx = 0;
    int i, j;
    
    for (i=0; i<K; i++) {
        curr_vec = curr_cen->centroid_vector;
        curr_cord = curr_vec->head_cord;
        for (j=0; j<vec_size;j++) {
            final_cords[cord_indx] = curr_cord->value;
            cord_indx++;
            curr_cord = curr_cord->next;
        }
        curr_cen = curr_cen->next;
    }

    return final_cords;
}


void free_vectors_and_cords_matrix() { 
    Vector* curr_vec;
    Vector* vec_to_be_del;
    Cord* curr_cord;
    Cord* cord_to_be_del;

    curr_vec = head_vec;

    while (curr_vec != NULL) {
        curr_cord = curr_vec->head_cord;

        while (curr_cord != NULL) {
            cord_to_be_del = curr_cord;
            curr_cord = curr_cord->next;
            free(cord_to_be_del);  
        }
        
        vec_to_be_del = curr_vec;
        curr_vec = curr_vec->next;
        free(vec_to_be_del);
    }
}


void free_centroids() {
    Centroid* curr_centroid;
    Centroid* centroid_to_be_del;

    curr_centroid = first_centroid;
    while (curr_centroid != NULL) {
        centroid_to_be_del = curr_centroid;
        curr_centroid = curr_centroid->next;
        free(centroid_to_be_del);
    }
}


void free_memory() {

    free_vectors_and_cords_matrix();
    free_centroids();

}


double* kmeans_imp_C(double cords[], int first_indices[], double eps, int iterations, int k, int vec_length, long long total_vec_num, long long cords_num) {

    /* the function returns the final cords through pointer */ 

    assign_initial_values(eps, iterations, k, vec_length, total_vec_num, cords_num);
    
    load_matrix(cords);

    initialize_first_K_centroids(first_indices);

    convergence();

    double* final_cords = organizing_final_cords();

    free_memory();

    return final_cords;
    
}



/*
*
*
    C-Python API
*
*
*/


static PyObject* fit(PyObject* self, PyObject* args) {
    PyObject* all_cords_list;
    PyObject* initialized_vectors_indices_list;
    PyObject* item;

    double curr_num;

    double eps;
    int iterations;
    int k;
    
    int vec_length;
    long long total_vec_num; 
    long long cords_num; 

    int i; /* index runner */

    if(!PyArg_ParseTuple(args, "OOdiiiLL", &all_cords_list, &initialized_vectors_indices_list,
                                           &eps, &iterations, &k,  
                                           &vec_length, &total_vec_num, &cords_num)) {
        return NULL;
    }

    double* all_cords = (double*) malloc(cords_num*sizeof(double));
    if (all_cords == NULL) {
        printf("An Error Has Occurred\n");
        return NULL;
    }
    for (i=0; i<cords_num; i++) {
        item = PyList_GetItem(all_cords_list, i);
        curr_num = PyFloat_AsDouble(item);
        all_cords[i] = curr_num;
    }

    int* initialized_vectors_indices = (int*) malloc(k*sizeof(int));
    if (initialized_vectors_indices == NULL) {
        printf("An Error Has Occurred\n");
        return NULL;
    }
    for (i=0; i<k; i++) {
        item = PyList_GetItem(initialized_vectors_indices_list, i);
        curr_num = (int) PyLong_AsLong(item);
        initialized_vectors_indices[i] = curr_num;
    }

    double* final_cords = kmeans_imp_C(all_cords, initialized_vectors_indices,
                                        eps, iterations, k,
                                        vec_length, total_vec_num, cords_num); 


    PyObject* python_val;
    PyObject* python_float;    

    python_val = PyList_New(k*vec_length);

    for (i=0; i<(k*vec_length) ; i++) {
        python_float = Py_BuildValue("d", final_cords[i]);
        PyList_SetItem(python_val, i, python_float);
    }
    
    free(all_cords);
    free(initialized_vectors_indices);

    return python_val;
}



static PyMethodDef kmeansMethods[] = {
    { "fit",                   
      (PyCFunction) fit, 
      METH_VARARGS,     
      PyDoc_STR("Evaluting K centroids as described in the K-means clustering method. The function recieves the data points already initialied with the first K centroids and paramaters of convergence."         
                    "To be precised, it expects parameters in this order: full list of coordinate of shape 1 given by the datapoints, initialized first indices, "
                    "eps, iter, K, a data point dimension, total number of datapoints, total number of coordinates of shape 1.") },
        {NULL, NULL, 0, NULL}     
                                     };



static struct PyModuleDef kmeansmodule = {
    PyModuleDef_HEAD_INIT,
    "mykmeanssp", 
    NULL, 
    -1,  
    kmeansMethods 
};



PyMODINIT_FUNC PyInit_mykmeanssp(void) {
    PyObject *m;
    m = PyModule_Create(&kmeansmodule);
    if (!m) {
        return NULL;
    }
    return m;
}