#include <stdio.h>
#include <stdlib.h>

int main() {
	int i, j;
	FILE *f = NULL;
	float new_float;
	
	srand(1);
	f = fopen("matrix.txt", "w");
	
	for (i = 0; i < 1000; ++i) {
		for (j = 0; j < 1000; ++j) {
			new_float = (rand() % 10000) / 100.0;
			fprintf(f, "%2.2f ", new_float);
		}
		fprintf(f, "\n");
	}
	
	fclose(f);

	return 0;
}
