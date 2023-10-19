
#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <stdio.h>
#include <stdlib.h>
#include "symnmf.h"



/*
*
    Module's static functions for formatting the data 
*    
*/

static double** build_mat_from_array_of_cords(double* all_cords, int vec_len, int num_of_vectors) {
    int i;  /*  rows  */
    int j;  /*  cols  */
    int curr_num = 0;   /*  index in all_cords  */

    double** mat = allocate_mat(vec_len, num_of_vectors);

    for (i=0 ; i < num_of_vectors; i++) {
        for (j=0 ; j < vec_len; j++) {  
            mat[i][j] = all_cords[curr_num];
            curr_num++;
        }
    }

    return mat;
}


static double* defragment_mat_to_array_of_cords(double** mat) { 
    int i;
    int j;
    int index = 0;

    int num_of_rows = get_row_size(mat);
    int col_size = get_col_size(mat);

    double* list_of_cords = malloc(sizeof(double)*num_of_rows*col_size);

    for(i=0; i<num_of_rows; i++) {
        for(j=0; j<col_size; j++) {
            list_of_cords[index] = mat[i][j];
            index++;
        }
    }

    return list_of_cords;    
}




/*
*
*
    C-Python API
*
*
*/



/*  similarity matrix  */

static PyObject* sym(PyObject* self, PyObject* args) {
    PyObject* all_cords_list;
    PyObject* item;

    double curr_num;

    int vec_len;
    long long num_of_vectors; 

    int i; /* index runner */

    if(!PyArg_ParseTuple(args, "OiL", &all_cords_list, &vec_len, &num_of_vectors)) {
        printf("An Error Has Occurred\n");
        return NULL;
    }

    /*  transforming the python list into array in C  */
    double* all_cords_as_array = (double*) malloc(vec_len*num_of_vectors*sizeof(double));
    if (all_cords_as_array == NULL) {
        printf("An Error Has Occurred\n");
        return NULL;
    }
    for (i=0; i<(vec_len*num_of_vectors); i++) {
        item = PyList_GetItem(all_cords_list, i);
        curr_num = PyFloat_AsDouble(item);
        all_cords_as_array[i] = curr_num;
    }

    /*  build matrix from array  */
    double** all_cords_as_mat = build_mat_from_array_of_cords(all_cords_as_array, vec_len, num_of_vectors);

    /*  calculating  */
    double** sim_mat = calculate_similarity_matrix(all_cords_as_mat); 
    double* sim_mat_as_array = defragment_mat_to_array_of_cords(sim_mat);

    PyObject* python_val;
    PyObject* python_float;    

    python_val = PyList_New(num_of_vectors*vec_len);

    for (i=0; i<(num_of_vectors*vec_len) ; i++) {
        python_float = Py_BuildValue("d", sim_mat_as_array[i]);
        PyList_SetItem(python_val, i, python_float);
    }
    
    free(all_cords_as_array);
    free(all_cords_as_mat);
    free_mat(sim_mat);

    return python_val;
}





/*  ddg matrix  */

static PyObject* ddg(PyObject* self, PyObject* args) {
    PyObject* all_cords_list;
    PyObject* item;

    double curr_num;
    
    int vec_len;
    long long num_of_vectors; 

    int i; /* index runner */

    if(!PyArg_ParseTuple(args, "OiL", &all_cords_list, &vec_len, &num_of_vectors)) {
        printf("An Error Has Occurred\n");
        return NULL;
    }

    /*  transforming the python list into array in C  */
    double* all_cords_as_array = (double*) malloc(vec_len*num_of_vectors*sizeof(double));
    if (all_cords == NULL) {
        printf("An Error Has Occurred\n");
        return NULL;
    }
    for (i=0; i<(vec_len*num_of_vectors); i++) {
        item = PyList_GetItem(all_cords_list, i);
        curr_num = PyFloat_AsDouble(item);
        all_cords_as_array[i] = curr_num;
    }

    /*  build matrix from array  */
    double** all_cords_as_mat = build_mat_from_array_of_cords(all_cords_as_array, vec_len, num_of_vectors);

    /*  calculating  */
    double** ddg_mat = calculate_ddg_matrix(all_cords_as_mat); 
    double* ddg_mat_as_array = defragment_mat_to_array_of_cords(ddg_mat);

    PyObject* python_val;
    PyObject* python_float;    

    python_val = PyList_New(num_of_vectors*vec_length);

    for (i=0; i<(num_of_vectors*vec_len) ; i++) {
        python_float = Py_BuildValue("d", ddg_mat_as_array[i]);
        PyList_SetItem(python_val, i, python_float);
    }
    
    free(all_cords_as_array);
    free(all_cords_as_mat);
    free_mat(ddg_mat);

    return python_val;
}





/*  norm matrix  */

static PyObject* norm(PyObject* self, PyObject* args) {
    PyObject* all_cords_list;
    PyObject* item;

    double curr_num;

    int vec_len;
    long long num_of_vectors; 

    int i; /* index runner */

    if(!PyArg_ParseTuple(args, "OiL", &all_cords_list, &vec_len, &num_of_vectors)) {
        printf("An Error Has Occurred\n");
        return NULL;
    }

    /*  transforming the python list into array in C  */
    double* all_cords_as_array = (double*) malloc(vec_len*num_of_vectors*sizeof(double));
    if (all_cords == NULL) {
        printf("An Error Has Occurred\n");
        return NULL;
    }
    for (i=0; i<(vec_len*num_of_vectors); i++) {
        item = PyList_GetItem(all_cords_list, i);
        curr_num = PyFloat_AsDouble(item);
        all_cords_as_array[i] = curr_num;
    }

    /*  build matrix from array  */
    double** all_cords_as_mat = build_mat_from_array_of_cords(all_cords_as_array, vec_len, num_of_vectors);

    /*  calculating  */
    double** norm_mat = calculate_normalized_matrix(all_cords_as_mat); 
    double* norm_mat_as_array = defragment_mat_to_array_of_cords(norm_mat);

    PyObject* python_val;
    PyObject* python_float;    

    python_val = PyList_New(num_of_vectors*vec_length);

    for (i=0; i<(num_of_vectors*vec_len) ; i++) {
        python_float = Py_BuildValue("d", norm_mat_as_array[i]);
        PyList_SetItem(python_val, i, python_float);
    }
    
    free(all_cords_as_array);
    free(all_cords_as_mat);
    free_mat(norm_mat);

    return python_val;
}




/*  symnmf matrix  */

static PyObject* symnmf(PyObject* self, PyObject* args) {
    PyObject* H_all_cords_list;
    PyObject* W_all_cords_list;

    int H_vec_len; /* equivalent to num_of_clusters */
    int W_vec_len; /* is norm_vec_len vec_len */
    long long num_of_vectors;

    PyObject* item;
    double curr_num;
   
    int i; /* index runner */

    if(!PyArg_ParseTuple(args, "OOiiL", &H_all_cords_list, &W_all_cords_list,
                                        &H_vec_len, &W_vec_len, &num_of_vectors)
                                          ) {
        printf("An Error Has Occurred\n");
        return NULL;
    }

        /*  transforming H python list to array in C  */
    double* H_all_cords_as_array = (double*) malloc(H_vec_len*num_of_vectors*sizeof(double));
    if (all_cords == NULL) {
        printf("An Error Has Occurred\n");
        return NULL;
    }
    for (i=0; i<(num_of_vectors*H_vec_len); i++) {
        item = PyList_GetItem(H_all_cords_list, i);
        curr_num = PyFloat_AsDouble(item);
        H_all_cords_as_array[i] = curr_num;
    }

        /*  transforming W python list to array in C  */
    double* W_all_cords_as_array = (double*) malloc(W_vec_len*num_of_vectors*sizeof(double));
    if (all_cords == NULL) {
        printf("An Error Has Occurred\n");
        return NULL;
    }
    for (i=0; i<(W_vec_len*num_of_vectors); i++) {
        item = PyList_GetItem(W_all_cords_list, i);
        curr_num = PyFloat_AsDouble(item);
        W_all_cords_as_array[i] = curr_num;
    }

        /*  build matrices from arrays  */
    double** H_mat = build_mat_from_array_of_cords(H_all_cords_as_array, H_vec_len, num_of_vectors);
    double** W_mat = build_mat_from_array_of_cords(W_all_cords_as_array, W_vec_len, num_of_vectors);

        /*  calculating  */
    double** symnmf_mat = H_optimization(H_mat, W_mat); 
    double* symnmf_mat_as_array = defragment_mat_to_array_of_cords(symnmf_mat);

    PyObject* python_val;
    PyObject* python_float;    

    python_val = PyList_New(num_of_vectors*H_vec_len);

    for (i=0; i<(num_of_vectors*H_vec_len) ; i++) {
        python_float = Py_BuildValue("d", symnmf_mat_as_array[i]);
        PyList_SetItem(python_val, i, python_float);
    }
    
    free(H_all_cords_as_array);
    free(W_all_cords_as_array);
    free_mat(H_mat);
    free_mat(W_mat);
    free_mat(symnmf_mat);

    return python_val;
}





static PyMethodDef symnmf_methods[] = {
    { 
        "sym",                   
      (PyCFunction) sym, 
      METH_VARARGS,     
      PyDoc_STR("Similarity matrix") 

    } , { 
        "ddg",
        (PyCFunction) ddg,
        METH_VARARGS,
        PyDoc_STR("Diagonal degree matrix"),

    } , { 
        "norm",
        (PyCFunction) norm,
        PyDoc_STR("Normalized similarity matrix")

    } , {
        "symnmf",
        (PyCFunction) symnmf,
        PyDoc_STR("Performing symNMF optimization")
        }

        {NULL, NULL, 0, NULL}     
                                     };



static struct PyModuleDef symnmfmodule = {
    PyModuleDef_HEAD_INIT,
    "SymNMF", 
    NULL, 
    -1,  
    symnmf_methods
};



PyMODINIT_FUNC PyInit_SymNMF(void) {
    PyObject *m;
    m = PyModule_Create(&symnmfmodule);
    if (!m) {
        return NULL;
    }
    return m;
}