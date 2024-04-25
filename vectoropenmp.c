#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <immintrin.h> // Include AVX intrinsics
#include "omp.h"
#define MAX_SIZE 4096
#define ALIGN 32  // Alignment for AVX instructions (multiple of cache line size)

typedef int __attribute__((aligned(ALIGN))) aligned_int; // Aligned integer type

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_file> \n", argv[0]);
        return 1;
    }

    // Open input and output files
    FILE *in_file = fopen(argv[1], "r");
    if (!in_file) {
        printf("Error opening input file: %s\n", argv[1]);
        return 1;
    }
    
    int opcode, data_type;
    fscanf(in_file, "%d %d", &opcode, &data_type);

    // Read dimensions of Matrix 1
    int rows_a, cols_a;
    fscanf(in_file, "%d %d", &rows_a, &cols_a);

    // Read elements of Matrix 1
    int **a = (int **)malloc(rows_a * sizeof(int *));
    for (int i = 0; i < rows_a; i++) {
        a[i] = (int *)malloc(cols_a * sizeof(int));
        for (int j = 0; j < cols_a; j++) {
            fscanf(in_file, "%d", &a[i][j]);
        }
    }

    // Read dimensions of Matrix 2
    int rows_b, cols_b;
    fscanf(in_file, "%d %d", &rows_b, &cols_b);

    // Read elements of Matrix 2
    /*int **b = (int **)malloc(rows_b * sizeof(int *));
    for (int i = 0; i < rows_b; i++) {
        b[i] = (int *)malloc(cols_b * sizeof(int));
        for (int j = 0; j < cols_b; j++) {
            fscanf(in_file, "%d", &b[i][j]);
        }
    }*/
    
    aligned_int **b = (aligned_int **)aligned_alloc(ALIGN, rows_b * sizeof(aligned_int *));
    for (int i = 0; i < rows_b; i++) {
    b[i] = (aligned_int *)aligned_alloc(ALIGN, cols_a * sizeof(aligned_int));
    for (int j = 0; j < cols_b; j++) {
            fscanf(in_file, "%d", &b[i][j]);
        }
}

    // Validate matrix dimensions for multiplication
    if (cols_a != rows_b) {
        printf("Error: Incompatible matrix dimensions (%dx%d) and (%dx%d)\n",
               rows_a, cols_a, rows_b, cols_b);
        fclose(in_file);
        for (int i = 0; i < rows_a; i++) {
            free(a[i]);
        }
        free(a);
        for (int i = 0; i < rows_b; i++) {
            free(b[i]);
        }
        free(b);
        return 1;
    }
   aligned_int **result = (aligned_int **)aligned_alloc(ALIGN, rows_a * sizeof(aligned_int *));
    for (int i = 0; i < rows_a; i++) {
        result[i] = (aligned_int *)aligned_alloc(ALIGN, cols_b * sizeof(aligned_int));
    }
    
    int j = 0;
    int k = 0;
    int start_time = time(NULL);
 #pragma omp parallel for private(j,k) num_threads(12)

    for (int i = 0; i < rows_a; i++) {
        for (int j = 0; j < cols_b; j += 24) {  // Process blocks of 8 columns
            __m256i sum = _mm256_setzero_si256();
            for (int k = 0; k < cols_a; k++) {
                __m256i vec_a = _mm256_set1_epi32(a[i][k]);
                __m256i vec_b = _mm256_load_si256((__m256i*)&b[k][j]);
                __m256i mul = _mm256_mullo_epi32(vec_a, vec_b);
                sum = _mm256_add_epi32(sum, mul);
            }
            // Store partial results back to result matrix
            _mm256_storeu_si256((__m256i*)&result[i][j], sum);
        }
    }
    //int _time = time(NULL) - _time;
    // Measure time after computation
   int end_time = time(NULL);
    
    int _time = end_time - start_time;

    FILE *out_file = fopen("vectorrrr.txt", "w");
  if (!out_file) {
    fprintf(stderr, "Error: Could not open file: %s\n", argv[1]);
    return 1;
  }
    // Write result matrix to output file
    fprintf(out_file, "%d %d\n", rows_a, cols_b);
    for (int i = 0; i < rows_a; i++) {
        for (int j = 0; j < cols_b; j++) {
            fprintf(out_file, "%d ", result[i][j]);
            //printf("%d ", result[i][j]);
        }
        fprintf(out_file, "\n");
    }

    // Calculate and print execution time (excluding I/O)
    //double cpu_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("CPU time %d", _time);
  

  fclose(in_file);
  fclose(out_file);
    return 0;

}
