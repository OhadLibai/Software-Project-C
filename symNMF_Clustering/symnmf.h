
/*    Library methods    */


double** allocate_mat(int num_of_cols, int num_of_rows);
int get_col_size(double** mat);
int get_row_size(double** mat);
void free_mat(double** mat);

double** mat_mult(double** A, double** B);
double** mat_transpose(double** mat);

double euclidean_dist(double curr_vec[], double other_vec[]); 
double frob_norm(double** new_H, double** curr_H);

double** calculate_similarity_matrix(double** mat);
double** calculate_ddg_matrix(double** mat);
double** calculate_normalized_matrix(double** mat);
double** H_optimization(double** H, double** W);

