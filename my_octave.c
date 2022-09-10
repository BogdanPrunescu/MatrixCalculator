// Prunescu Bogdan-Andrei 311CA
#include <stdio.h>
#include <stdlib.h>
#include "my_commands.h"

int main(void)
{
	// create our array of matrices using a struct
	int size = 0, capacity = 1;
	octave *data = (octave *)malloc(capacity * sizeof(octave));

	int index, m, n, a, b, sum;
	char command;
	// Read commands until 'Q' command pops up
	scanf(" %c", &command);
	while (command != 'Q') {
		switch (command) {
		case 'L':
			scanf("%d%d", &n, &m);
			int **new_mat = allocate_matrix(n, m);
			if (!new_mat) {
				free_myoctave(&data, size);
				return -1;
			}
			read_matrix(n, m, new_mat);
			check_space(&data, size, &capacity);
			sum = sum_elements(n, m, new_mat);
			insert_matrix(new_mat, data, n, m, &size, sum);
			break;

		case 'D':
			scanf("%d", &index);
			print_size(data, index, size);
			break;

		case 'P':
			scanf("%d", &index);
			print_matrix(data, index, size);
			break;

		case 'C':
			scanf("%d", &index);
			resize_matrix(data, index, size);
			break;

		case 'M':
			scanf("%d%d", &a, &b);
			multiplication(&data, a, b, 'M', &size, &capacity);
			break;

		case 'O':
			sort_storage(data, size);
			break;

		case 'T':
			scanf("%d", &index);
			transpose(data, index, size);
			break;

		case 'F':
			scanf("%d", &index);
			erase_matrix(data, index, &size);
			check_space(&data, size, &capacity);
			break;

		case 'S':
			scanf("%d%d", &a, &b);
			multiplication(&data, a, b, 'S', &size, &capacity);
			break;

		default:
			print_error(2);
		}
		scanf(" %c", &command);
	}
	free_myoctave(&data, size);
	return 0;
}
