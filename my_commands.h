// Prunescu Bogdan-Andrei 311CA
#ifndef _COMMANDS_
#define _COMMANDS_

#define MOD 10007

typedef struct {
	// Use the sum variable for the 'O' command
	int rows, cols, sum;
	int **mat;
} octave;

void print_error(int id);
void free_matrix(int rows, int **a);
void free_myoctave(octave **data, int size);
int **allocate_matrix(int n, int m);
void read_matrix(int n, int m, int **a);
int check_index(int index, int size);
void print_size(octave *data, int index, int size);
void check_space(octave **data, int size, int *cap);
void copy_matrix(octave *dest, int **matrix, int rows, int cols, int sum);
void insert_matrix(int **mat, octave *data, int r, int c, int *size, int s);
void print_matrix(const octave *data, int index, int size);
int sum_elements(int n, int m, int **a);
void resize_matrix(octave *data, int index, int size);
int **multiply_matrix(octave *a, octave *b);
void swap_octave(octave *a, octave *b);
void sort_storage(octave *data, int size);
void transpose(octave *data, int index, int size);
void erase_matrix(octave *data, int index, int *size);
int **add_matrix(int **a, int **b, int size);
int **subtract_matrix(int **a, int **b, int size);
void insert_cropped_matrix(octave **dest, octave *src);
void free_strassen_failed(octave **a, octave **b, octave **c);
int **s(octave *a, octave *b);
void create_intermediate_values(octave *a, octave *b, octave **c, int size);
void multiplication(octave **data, int a, int b, char id, int *size, int *cap);
#endif
