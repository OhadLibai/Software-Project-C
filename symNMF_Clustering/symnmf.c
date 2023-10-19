
/*    
    C program for:
                    i. sym
                    ii. ddg
                    iii. norm

    Also play a role as an implementation of the library.
*/  


#include <stdio.h>
#include <stdlib.h>
#include "symnmf.h"



/*
    A use of a struct for reading the data
*/

typedef struct Cord Cord;

struct Cord {
    double val;
    Cord* next; 
};


static Cord* allocate_cord() {
    Cord* new_cord = malloc(sizeof(Cord));
    if (new_cord == NULL) {
        printf("An Error Has Occurred\n");
        exit(1);
    }
    return new_cord;
}



    /*  METHODS LIBRARY */


/*  allocator of a matrix  */
double** allocate_mat(int num_of_cols, int num_of_rows) {
    int row_index;

    double** mat = malloc(num_of_rows * sizeof(double*));
    if (mat == NULL) {
        printf("An Error Has Occurred\n");
        exit(1);
    }

    for(row_index= 0; row_index < num_of_rows; row_index++)
        mat[row_index] = (double*) malloc(num_of_cols * sizeof(double));

    return mat;
}


/*  GETTERS  */
int get_col_size(double** mat) {
    return sizeof(mat[0]) / sizeof(double);
}

int get_row_size(double** mat) {
    return sizeof(mat) / sizeof(double*);
}


/*  CLEANER  */
void free_mat(double** mat) {
    int rows = get_row_size(mat);
    int i;
    for (i=0; i< rows; i++) 
        free(mat[i]);
    free(mat);
}




double** mat_mult(double** A, double** B) {
    double tmp_sum;
    int i; /*rows*/
    int l; /*cols*/
    int j; /*rows with cols*/

    int num_of_cols = get_col_size(B);
    int num_of_rows = get_row_size(A);
    int common_row_col  = get_col_size(A); /*   equivalent to: get_row_size(B)   */

    double** C = allocate_mat(num_of_cols, num_of_rows);

    for (i=0; i<num_of_rows; i++) {
        for (l=0; l<num_of_cols; l++) {
            tmp_sum=0;
            for (j=0; j<common_row_col; j++ ) {
                tmp_sum += A[i][j] * B[j][l];
            }

            C[i][l] = tmp_sum; 
        }
    }

    return C;
}



double** mat_transpose(double** mat) {
    int i; /*row*/
    int j; /*col*/

    int num_of_rows = get_row_size(mat);
    int num_of_cols = get_col_size(mat);

    double** mat_T = allocate_mat(num_of_cols, num_of_rows);

    for (i=0; i<num_of_rows; i++) {
        for (j=0; j<num_of_cols; j++) {
            mat_T[j][i] = mat[i][j];
        }
    }
    return mat_T;

}



double euclidean_dist(double curr_vec[], double other_vec[]) {
    double sum;
    int j;
    int vec_len = sizeof(curr_vec) / sizeof(double); // no getter for him

    for (j=0; j<vec_len; j++) {
        sum += pow(curr_vec[j] - other_vec[j], 2);
    }

    return sqrt(sum);
}



double frob_norm(double** new_H, double** curr_H) {
    int i;
    int j;
    double res = 0;
    double curr_num;

    int num_of_rows = get_row_size(new_H);
    int num_of_cols = get_col_size(new_H);

    for (i=0; i < num_of_rows; i++) {
        for (j=0; j<num_of_cols; j++) {
            curr_num = new_H[i][j] - curr_H[i][j];
            res += pow(curr_num, 2);
        }
    }

    return res;
}



    /*  Providing the requested matrices  */

double** calculate_similarity_matrix(double** mat) {
    int i; /*curr_row*/
    int j; /*other_rows*/
    double tmp_eucl_dist;

    int num_of_rows = get_row_size(mat);
    int num_of_cols = get_col_size(mat);

    double** sim_mat = allocate_mat(num_of_cols, num_of_rows);

    for (i=0; i<num_of_rows; i++) {
        for (j=i+1; j < num_of_rows; j++) {
            tmp_eucl_dist = euclidean_dist(mat[i],mat[j]);
            sim_mat[i][j] = exp( (-tmp_eucl_dist) / 2); 
            sim_mat[j][i] = exp( (-tmp_eucl_dist) / 2);
        }
    }

    for (i=0; i < num_of_rows; i++)
        sim_mat[i][i] = 0;
    
    return sim_mat;
}



double** calculate_ddg_matrix(double** mat) {
    int i;
    int j;
    int tmp_sum;

    double** sim_mat = calculate_similarity_matrix(mat);

    int num_of_rows = get_row_size(sim_mat);
    int num_of_cols = get_col_size(sim_mat);

    double** ddg_mat = allocate_mat(num_of_cols, num_of_rows);

    for (i=0; i<num_of_rows; i++) {
        for (j=0; j<num_of_cols;j++)
            ddg_mat[i][j] = 0;
    }

    for (i=0; i<num_of_rows; i++) {
        tmp_sum = 0;
        for (j=0; j<num_of_cols; j++) {
            tmp_sum += sim_mat[i][j];
        }
        ddg_mat[i][i] = tmp_sum;
    }

    free_mat(sim_mat);

    return ddg_mat;
}



double** calculate_normalized_matrix(double** mat) {
    int i;

    double** A = calculate_similarity_matrix(mat);
    double** D = calculate_ddg_matrix(mat);

    int num_of_rows = get_row_size(D);

    for(i=0 ; i < num_of_rows; i++)
        D[i][i] = pow(D[i][i], -0.5);
    
    double** left_multiplication = mat_mult(D, A);

    double** norm_mat = mat_mult(left_multiplication, D);

    free_mat(A);
    free_mat(D);

    return norm_mat; 
}



/*  Final step for symnmfmodule only  */

double** H_optimization(double** H, double** W) {
    const int max_iter = 300;
    const double eps = 0.0001;
    int iter = 0;

    double** curr_H;
    double** new_H;
    double** curr_H_transpose;
    double** curr_numerator_mult;
    double** curr_denominator_mult;
    double** curr_left_denominator_mult;
    double curr_frob_norm_dist;

    int i; /*rows*/
    int j; /*cols*/

    int H_vec_len = get_col_size(H);
    int H_num_of_vectors = get_row_size(H); /* H_num_of_vectors == W_num_of_vectors == num_of_vectors */

    curr_H = H; 

    while (iter<max_iter) {
        new_H = allocate_mat(H_vec_len, H_num_of_vectors);

        curr_numerator_mult = mat_mult(W, curr_H);
        curr_H_transpose = mat_transpose(curr_H);
        curr_left_denominator_mult = mat_mult(curr_H, curr_H_transpose);
        curr_denominator_mult = mat_mult(curr_left_denominator_mult, curr_H);
            
        for (i=0; i < H_num_of_vectors; i++) {
            for (j=0; j < H_vec_len; j++) {
                new_H[i][j] = curr_H[i][j] * (0.5*(1 + (curr_numerator_mult[i][j] / curr_denominator_mult[i][j])));
            }
        }

        curr_frob_norm_dist = frob_norm(new_H, curr_H);

        free_mat(curr_numerator_mult);
        free_mat(curr_H_transpose);
        free_mat(curr_left_denominator_mult);
        free_mat(curr_denominator_mult);
        free_mat(curr_H);

        if (curr_frob_norm_dist < eps)
            break;    
        else 
            curr_H = new_H;

        iter++;
    }

    return new_H;
}




/* 
*
    symnmf.c's static functions, for its own running  
*    
*/

static double** load_data(char* path) {
   
    Cord* curr_cord;
    Cord* first_cord;
    Cord* prev_cord;

    double num;
    char ch;
    int vec_len_checked = 0; /*as boolean*/
    double vec_len = 0;
    double num_of_vectors = 0;

    curr_cord = allocate_cord();
    first_cord = curr_cord;
    prev_cord = curr_cord;

    FILE* f;
    f = fopen(path, "r" ); 

    while ((fscanf(f, "%f%c", &num, &ch) == 2)) {
        curr_cord->val = num;
        prev_cord = curr_cord;
        curr_cord->next = allocate_cord();
        curr_cord = curr_cord->next;

        if (!vec_len_checked)
            vec_len++;
        
        if (ch = '\n') {
            vec_len_checked = 1;
            num_of_vectors++;
        }
    }
    fclose(f);
    prev_cord->next = NULL;
    free(curr_cord);

    /*  building a matrix from Cords  */
    double** mat_data = allocate_mat(vec_len, num_of_vectors);
    curr_cord = first_cord;

    int i; /*rows*/
    int j; /*cols*/
    for (i=0; i< num_of_vectors; i++) {
        for (j=0; j< vec_len; j++) {
            mat_data[i][j] = curr_cord->val;
            prev_cord = curr_cord;
            curr_cord = curr_cord->next;
            free(prev_cord);
        }
    }

    return mat_data;
}



static print_mat(double** mat) {
    int i;
    int j;
    int vec_len = get_col_size(mat);
    int num_of_rows = get_row_size(mat);

    for (i=0; i<num_of_rows; i++) {
        printf("%.4f", mat[i][0]);
        
        if ((vec_len) > 1) {
            for(j=1; j < (vec_len) ;j++)
                printf(",%.4f", mat[i][j]);
        }
        
        printf("\n");
    }

    printf("\n"); /* they asked for an empty line at the end of the output */
}



    /*  main  */

int main(int argc, char* argv[]) {
    char* path = argv[2];
    const char arg = argv[1];

    const char sym[] = "sym";
    const char ddg[] = "ddg";
    const char norm[] = "norm";

    double** mat_data = load_data(path);

    double** requested_mat;
  
    if (strcmp(sym,arg) == 0) 
        requested_mat = calculate_similarity_matrix(mat_data);

    if (strcmp(ddg,arg) == 0) 
        requested_mat = calculate_ddg_matrix(mat_data);

    if (strcmp(norm,arg) == 0) 
        requested_mat = calculate_normalized_matrix(mat_data);

    print_mat(requested_mat);

    free_mat(requested_mat);
    free_mat(mat_data);
    
    return 0;

}










