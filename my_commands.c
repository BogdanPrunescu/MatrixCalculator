// Prunescu Bogdan-Andrei 311CA
#include <stdio.h>
#include <stdlib.h>

#define MOD 10007

typedef struct {
	// Use the sum variable for the 'O' command
	int rows, cols, sum;
	int **mat;
} octave;

// function used when an error occurs
// id = 1: No matrix with the given index
// id = 2: Unrecognized command
// id = 3: Cannot perform matrix multiplication
void print_error(int id)
{
	if (id == 1)
		printf("No matrix with the given index\n");
	else if (id == 2)
		printf("Unrecognized command\n");
	else if (id == 3)
		printf("Cannot perform matrix multiplication\n");
}

// Free memory allocated in a matrix
void free_matrix(int rows, int **a)
{
	for (int i = 0; i < rows; i++)
		free(a[i]);
	free(a);
	a = NULL;
}

// Free an octave-type array
void free_myoctave(octave **data, int size)
{
	for (int k = 0; k < size; k++)
		free_matrix((*data + k)->rows, (*data + k)->mat);
	free(*data);
	*data = NULL;
}

// Dinamically allocate a matrix with n rows and m columns
// and return a pointer to that matrix
int **allocate_matrix(int n, int m)
{
	int **matrix = (int **)malloc(n * sizeof(int *));
	if (!matrix) {
		fprintf(stderr, "malloc() failed\n");
		return NULL;
	}
	for (int i = 0; i < n; i++) {
		matrix[i] = (int *)malloc(m * sizeof(int));
		if (!matrix[i]) {
			fprintf(stderr, "malloc() failed\n");
			return NULL;
		}
	}
	return matrix;
}

// Read a matrix
void read_matrix(int n, int m, int **a)
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			scanf("%d", &a[i][j]);
}

// Check if the given index is inside [0, size - 1]
int check_index(int index, int size)
{
	if (index < size && index >= 0)
		return 0;
	return 1;
}

// Print the number of rows and columns of a matrix
// located at index in an octave-type array
void print_size(octave *data, int index, int size)
{
	if (check_index(index, size)) {
		print_error(1);
	} else {
		int n = (data + index)->rows;
		int m = (data + index)->cols;
		printf("%d %d\n", n, m);
	}
}

// Check if we need to reallocate the memory of an octave array
void check_space(octave **data, int size, int *cap)
{
	int ok = 0;
	// if there isn't enough space we double the capacity
	if (size == *cap) {
		*cap = *cap * 2;
		ok = 1;
	// if there is more allocated memory than necessary
	// the capacity will be split
	} else if (size < *cap / 2) {
		*cap = *cap / 2;
		ok = 1;
	}
	if (ok) {
		octave *t = (octave *)realloc(*data, *cap * sizeof(octave));
		if (!t) {
			fprintf(stderr, "realloc() failed\n");
			free_myoctave(data, size);
			exit(1);
		}
		*data = t;
	}
}

// Deep-copy in an octave variable the necessary elements
void copy_matrix(octave *dest, int **matrix, int rows, int cols, int sum)
{
	dest->rows = rows;
	dest->cols = cols;
	dest->sum = sum;
	dest->mat = matrix;
}

// Insert a matrix at the end of an octave-type array
void insert_matrix(int **mat, octave *data, int r, int c, int *size, int s)
{
	copy_matrix((data + *size), mat, r, c, s);
	*size = *size + 1;
}

// Print a matrix at a given index inside an octave-type array
void print_matrix(const octave *data, int index, int size)
{
	int n, m;
	if (check_index(index, size)) {
		print_error(1);
	} else {
		n = (data + index)->rows;
		m = (data + index)->cols;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++)
				printf("%d ", (data + index)->mat[i][j]);
			printf("\n");
		}
	}
}

// Return the sum of elements inside a matrix
int sum_elements(int n, int m, int **a)
{
	int sum = 0;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++) {
			sum += a[i][j];
			sum %= MOD;
			if (sum < 0)
				sum += MOD;
		}
	return sum;
}

// Resize a matrix at a given index inside an octave-type array
void resize_matrix(octave *data, int index, int size)
{
	int n, m;

	// Read rows that will be used
	scanf("%d", &n);
	int *rows = (int *)malloc(n * sizeof(int));
	if (!rows) {
		fprintf(stderr, "malloc() failed\n");
		free_myoctave(&data, size);
		exit(1);
	}
	for (int i = 0; i < n; i++)
		scanf("%d", &rows[i]);

	// Read columns that will be used
	scanf("%d", &m);
	int *cols = (int *)malloc(m * sizeof(int));
	if (!cols) {
		fprintf(stderr, "malloc() failed\n");
		free(rows);
		free_myoctave(&data, size);
		exit(1);
	}
	for (int i = 0; i < m; i++)
		scanf("%d", &cols[i]);

	if (check_index(index, size)) {
		print_error(1);
	} else {
		// allocate a new matrix that will replace the old one
		int **new_mat = allocate_matrix(n, m);
		if (!new_mat) {
			free(rows);
			free(cols);
			free_myoctave(&data, size);
			exit(1);
		}
		for (int i = 0; i < n; i++)
			for (int j = 0; j < m; j++)
				// new_mat[i][j] = old_mat[rows[i]][cols[j]]
				new_mat[i][j] = (data + index)->mat[rows[i]][cols[j]];
		free_matrix((data + index)->rows, (data + index)->mat);
		int sum = sum_elements(n, m, new_mat);
		copy_matrix((data + index), new_mat, n, m, sum);
	}
	free(rows);
	free(cols);
}

// Matrix multiplication in O(n^3)
int **multiply_matrix(octave *a, octave *b)
{
	int **result = allocate_matrix(a->rows, b->cols);
	if (!result)
		return NULL;
	for (int i = 0; i < a->rows; i++)
		for (int j = 0; j < b->cols; j++) {
			result[i][j] = 0;
			for (int k = 0; k < a->cols; k++) {
				int product = a->mat[i][k] * b->mat[k][j];
				product = product % MOD;
				if (product < 0)
					product += MOD;
				result[i][j] = (result[i][j] + product) % MOD;
				if (result[i][j] < 0)
					result[i][j] += MOD;
			}
		}
	return result;
}

// Swap two octave-type variables
void swap_octave(octave *a, octave *b)
{
	octave tmp;
	copy_matrix(&tmp, a->mat, a->rows, a->cols, a->sum);
	copy_matrix(a, b->mat, b->rows, b->cols, b->sum);
	copy_matrix(b, tmp.mat, tmp.rows, tmp.cols, tmp.sum);
}

// Sort an octave array in increasing order
// based on the sum of elements in each matrix
void sort_storage(octave *data, int size)
{
	for (int i = 0; i < size - 1; i++)
		for (int j = i + 1; j < size; j++)
			if ((data + i)->sum > (data + j)->sum)
				swap_octave(&data[i], &data[j]);
}

// Transpose a matrix at a given index inside an octave-type array
void transpose(octave *data, int index, int size)
{
	if (check_index(index, size)) {
		print_error(1);
	} else {
		int n = (data + index)->rows;
		int m = (data + index)->cols;
		// create a new matrix that will replace the old one
		int **tr = allocate_matrix(m, n);
		if (!tr) {
			free_myoctave(&data, size);
			exit(1);
		}
		for (int i = 0; i < m; i++)
			for (int j = 0; j < n; j++)
				tr[i][j] = (data + index)->mat[j][i];
		free_matrix(n, (data + index)->mat);
		int sum = sum_elements(m, n, tr);
		copy_matrix((data + index), tr, m, n, sum);
	}
}

// Erase a matrix at a given index inside an octave-type array
void erase_matrix(octave *data, int index, int *size)
{
	if (check_index(index, *size)) {
		print_error(1);
	} else {
		free_matrix((data + index)->rows, (data + index)->mat);
		for (int i = index; i < *size - 1; i++)
			data[i] = data[i + 1];
		*size = *size - 1;
	}
}

// addition of two square matrix. The function returns a pointer
// to the result matrix
int **add_matrix(int **a, int **b, int size)
{
	int **tmp = allocate_matrix(size, size);
	if (!tmp)
		return NULL;
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++) {
			tmp[i][j] = (a[i][j] + b[i][j]) % MOD;
			if (tmp[i][j] < 0)
				tmp[i][j] += MOD;
		}
	return tmp;
}

// subtraction of two square matrix. The function returns a pointer
// to the result matrix
int **subtract_matrix(int **a, int **b, int size)
{
	int **tmp = allocate_matrix(size, size);
	if (!tmp)
		return NULL;
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++) {
			tmp[i][j] = (a[i][j] - b[i][j]) % MOD;
			if (tmp[i][j] < 0)
				tmp[i][j] += MOD;
		}
	return tmp;
}

// Split a matrix into 4 sub-matrices that are inserted inside an octave array
void insert_cropped_matrix(octave **dest, octave *src)
{
	int mid = src->rows / 2;
	for (int i = 0; i < 4; i++) {
		(*dest + i)->mat = allocate_matrix(mid, mid);
		(*dest + i)->rows = (*dest + i)->cols = mid;
	}
	for (int i = 0; i < mid; i++)
		for (int j = 0; j < mid; j++) {
			(*dest + 0)->mat[i][j] = src->mat[i][j];
			(*dest + 1)->mat[i][j] = src->mat[i][j + mid];
			(*dest + 2)->mat[i][j] = src->mat[i + mid][j];
			(*dest + 3)->mat[i][j] = src->mat[i + mid][j + mid];
		}
}

// Function that is called every time we fail to allocate memory inside the
// Strassen algorithm. It frees all the arrays that are used inside the s()
// function.
void free_strassen_failed(octave **a, octave **b, octave **c)
{
	free_myoctave(a, 4);
	free_myoctave(b, 4);
	free_myoctave(c, 4);
	fprintf(stderr, "malloc() failed\n");
}

// We declare the Strassen function here because it is needed for the
// create_intermediate_values() function
int **s(octave *a, octave *b);

// Function that creates the 4 sub-matrices that are needed for making the
// result of the Strassen algorithm
void create_intermediate_values(octave *a, octave *b, octave **c, int size)
{
	octave atmp, btmp;
	atmp.rows = (a + 0)->rows;
	atmp.cols = (a + 0)->cols;
	btmp.rows = (b + 0)->rows;
	btmp.cols = (b + 0)->cols;
	// the tmp array is needed to make 7 temporary matrices that are needed for
	// operations
	octave *tmp = (octave *)malloc(10 * sizeof(octave));
	if (!tmp) {
		free_strassen_failed(&a, &b, c);
	} else {
		for (int i = 0; i < 8; i++)
			copy_matrix((tmp + i), NULL, (a + 0)->rows, (a + 0)->cols, 0);

		atmp.mat = add_matrix((a + 0)->mat, (a + 3)->mat, size);
		btmp.mat = add_matrix((b + 0)->mat, (b + 3)->mat, size);
		(tmp + 0)->mat = s(&atmp, &btmp);
		free_matrix(size, atmp.mat);
		free_matrix(size, btmp.mat);

		atmp.mat = add_matrix((a + 2)->mat, (a + 3)->mat, size);
		btmp.mat = (b + 0)->mat;
		(tmp + 1)->mat = s(&atmp, &btmp);
		free_matrix(size, atmp.mat);

		atmp.mat = (a + 0)->mat;
		btmp.mat = subtract_matrix((b + 1)->mat, (b + 3)->mat, size);
		(tmp + 2)->mat = s(&atmp, &btmp);
		free_matrix(size, btmp.mat);

		atmp.mat = (a + 3)->mat;
		btmp.mat = subtract_matrix((b + 2)->mat, (b + 0)->mat, size);
		(tmp + 3)->mat = s(&atmp, &btmp);
		free_matrix(size, btmp.mat);

		atmp.mat = add_matrix((a + 0)->mat, (a + 1)->mat, size);
		btmp.mat = (b + 3)->mat;
		(tmp + 4)->mat = s(&atmp, &btmp);
		free_matrix(size, atmp.mat);

		atmp.mat = subtract_matrix((a + 2)->mat, (a + 0)->mat, size);
		btmp.mat = add_matrix((b + 0)->mat, (b + 1)->mat, size);
		(tmp + 5)->mat = s(&atmp, &btmp);
		free_matrix(size, atmp.mat);
		free_matrix(size, btmp.mat);

		atmp.mat = subtract_matrix((a + 1)->mat, (a + 3)->mat, size);
		btmp.mat = add_matrix((b + 2)->mat, (b + 3)->mat, size);
		(tmp + 6)->mat = s(&atmp, &btmp);
		free_matrix(size, atmp.mat);
		free_matrix(size, btmp.mat);
		// using tmp we make 4 matrices necessary for the final result
		atmp.mat = add_matrix((tmp + 0)->mat, (tmp + 3)->mat, size);
		btmp.mat = subtract_matrix(atmp.mat, (tmp + 4)->mat, size);
		(*c + 0)->mat = add_matrix(btmp.mat, (tmp + 6)->mat, size);
		free_matrix(size, atmp.mat);
		free_matrix(size, btmp.mat);

		(*c + 1)->mat = add_matrix((tmp + 2)->mat, (tmp + 4)->mat, size);

		(*c + 2)->mat = add_matrix((tmp + 1)->mat, (tmp + 3)->mat, size);

		atmp.mat = subtract_matrix((tmp + 0)->mat, (tmp + 1)->mat, size);
		btmp.mat = add_matrix((tmp + 2)->mat, (tmp + 5)->mat, size);
		(*c + 3)->mat = add_matrix(atmp.mat, btmp.mat, size);
		free_matrix(size, atmp.mat);
		free_matrix(size, btmp.mat);

		free_myoctave(&tmp, 7);
	}
}

// Strassen algorithm O(n^2.81)
int **s(octave *a, octave *b)
{
	if (a->rows / 2 == 0) {
		int **product = multiply_matrix(a, b);
		return product;
	}
	int **c = allocate_matrix(a->rows, a->rows);
	// a_mins and b_mins will store the splitted matrices a and b
	octave *a_mins = (octave *)malloc(5 * sizeof(octave));
	octave *b_mins = (octave *)malloc(5 * sizeof(octave));
	// c_mins will store the splitted matrix of the product a * b
	octave *c_mins = (octave *)malloc(5 * sizeof(octave));
	if (!c || !a_mins || !b_mins || !c_mins) {
		free_matrix(a->rows, c);
		free_strassen_failed(&a_mins, &b_mins, &c_mins);
		return NULL;
	}

	insert_cropped_matrix(&a_mins, a);
	insert_cropped_matrix(&b_mins, b);

	create_intermediate_values(a_mins, b_mins, &c_mins, a->rows / 2);
	for (int i = 0; i < 4; i++) {
		(a_mins + i)->rows = a->rows / 2;
		(b_mins + i)->rows = a->rows / 2;
		(c_mins + i)->rows = a->rows / 2;
		if (!(b + i) || !(a + i) || !(c + i)) {
			free_matrix(a->rows, c);
			free_strassen_failed(&a_mins, &b_mins, &c_mins);
			return NULL;
		}
	}

	// create the final product
	int mid = a->rows / 2;
	for (int i = 0; i < mid; i++)
		for (int j = 0; j < mid; j++) {
			c[i][j] = (c_mins + 0)->mat[i][j];
			c[i][j + mid] = (c_mins + 1)->mat[i][j];
			c[mid + i][j] = (c_mins + 2)->mat[i][j];
			c[mid + i][mid + j] = (c_mins + 3)->mat[i][j];
		}
	free_myoctave(&c_mins, 4);
	free_myoctave(&a_mins, 4);
	free_myoctave(&b_mins, 4);
	return c;
}

// Function used for the multiplication of 2 matrices
// if id = 'M' it will multiply using the basic algorithm
// if id = 'S' it will multiply using Strassen algorithm
void multiplication(octave **data, int a, int b, char id, int *size, int *cap)
{
	if (a >= *size || b >= *size || a < 0 || b < 0) {
		print_error(1);
	} else if ((*data + a)->cols != (*data + b)->rows) {
		print_error(3);
	} else {
		int **product;
		if (id == 'M')
			product = multiply_matrix((*data + a), (*data + b));
		else if (id == 'S')
			product = s((*data + a), (*data + b));
		if (!product) {
			free_myoctave(data, *size);
			exit(1);
		}
		int n = (*data + a)->rows;
		int m = (*data + b)->cols;
		int sum = sum_elements(n, m, product);
		check_space(data, *size, cap);
		insert_matrix(product, *data, n, m, size, sum);
	}
}
