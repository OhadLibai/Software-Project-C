
/* K-means clusters */


# include <stdio.h>
# include <stdlib.h>
# include <math.h>


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

long long N = 0; /* N is the number of vectors */
int K = 0; 
int vec_size = 0;
int num_of_iterations = 0;
int less_then_epsilon = 0;
int iter = 200;
double epsilon = 0.001;
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

void load_matrix() {
    Vector* curr_vec;
    Vector* prev_vec;
    Cord* head_cord;
    Cord* curr_cord;

    double cord_val; 
    char c;

    head_vec = allocate_vector();
    curr_vec = head_vec;
    
    head_cord = allocate_cord();
    curr_cord = head_cord;

   while (scanf("%lf%c", &cord_val, &c) == 2) {
        if (c == '\n') {
            curr_cord->value = cord_val;

            curr_vec->head_cord = head_cord;
            curr_vec->next_in_cluster = NULL;
            curr_vec->from_matrix = 1;

            prev_vec = curr_vec; 
            curr_vec->next = allocate_vector(); 
            curr_vec = curr_vec->next;

            curr_vec->next = NULL;
            head_cord = allocate_cord();
            curr_cord = head_cord;
            curr_cord->next = NULL;

            N++;
            continue;
        }

        curr_cord->value = cord_val;
        curr_cord->next = allocate_cord();
        curr_cord = curr_cord->next;
        curr_cord->next = NULL;
    }
    prev_vec->next = NULL; 

    free(curr_vec);
    free(head_cord);
        
    /* checking the size of a vector in regard to the current input */
    curr_cord = head_vec->head_cord;
    while (curr_cord != NULL) {
        vec_size++;
        curr_cord = curr_cord->next;
    }
}


int check_K_and_iter() { 
    int K_violates = 0 , iter_viloates = 0;

    if ((1>=K) || (K>=N))
        K_violates = 1;
    if ((1>=iter) || (iter>=1000))
        iter_viloates = 1;

    if (K_violates || iter_viloates){
        if (K_violates == 1)
            printf("Invalid number of clusters!\n");
        if (iter_viloates == 1)
            printf("Invalid maximum iteration!\n");
        exit(1);
    }

    return 0;
}


void initialize_first_K_centroids() {
    Vector* curr_vec;
    Centroid* curr_centroid;
    int i;

    first_centroid = allocate_centroid();
    curr_centroid = first_centroid;

    curr_vec = head_vec;
   
    for (i = 0; i<K; i++) {
        curr_centroid->cluster_size = 0;
        curr_centroid->first_member = NULL;
        curr_centroid->last_to_join = NULL;
        curr_centroid->centroid_vector = curr_vec;
        
        curr_centroid->next = allocate_centroid();
        curr_centroid = curr_centroid->next;
        curr_centroid->next = NULL;
        
        curr_vec = curr_vec->next;
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

    for (i=0; i<N; i++) {
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


void printing_all_centroids_vectors() {
    Cord* curr_cord;
    Centroid* curr_centroid;
    int i,j;

    curr_centroid = first_centroid;

    for (i=0; i<K; i++) {
        curr_cord = curr_centroid->centroid_vector->head_cord;

        printf("%.4f", curr_cord->value);
        
        if ((vec_size) > 1) {
            curr_cord = curr_cord->next;
            for(j=1; j < (vec_size) ;j++) {
                printf(",%.4f", curr_cord->value);
                curr_cord = curr_cord->next;
            }
        }
        
        printf("\n");

        curr_centroid = curr_centroid->next;
    }

    printf("\n"); /* they asked for an empty line at the end of the output */
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


int main(int argc, char *argv[]) {
    
    if (!(argc==2 || argc==3)) {
        printf("An Error Has Occurred\n");
        exit(1);
    }

    if (argc == 3)
        iter = atoi(*(argv+2));
    
    K = atoi(*(argv+1));
    
    load_matrix();

    check_K_and_iter(); 
       
    initialize_first_K_centroids();

    convergence();

    printing_all_centroids_vectors();

    free_memory();

    return 0;
}
