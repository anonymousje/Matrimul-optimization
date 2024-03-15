#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<pthread.h>
#include <immintrin.h> // Include AVX intrinsics
#define MAX_SIZE 4096
#define ALIGN 32  // Alignment for AVX instructions (multiple of cache line size)

typedef int __attribute__((aligned(ALIGN))) aligned_int; // Aligned integer type

// Structure to store thread arguments
typedef struct {
    int **a;
    aligned_int **b;
    aligned_int **result;
    int rows_a;
    int cols_a;
    int start_col;
    int end_col;
} thread_args_t;

// Thread function to perform matrix multiplication for a sub-block
void *matrix_multiply_thread(void *arg) {
    thread_args_t *args = (thread_args_t *)arg;

    for (int i = 0; i < args->rows_a; i++) {
        for (int j = args->start_col; j < args->end_col; j += 8) {
            __m256i sum = _mm256_setzero_si256();
            for (int k = 0; k < args->cols_a; k++) {
                __m256i vec_a = _mm256_set1_epi32(args->a[i][k]);
                __m256i vec_b = _mm256_load_si256((__m256i*)&args->b[k][j]);
                __m256i mul = _mm256_mullo_epi32(vec_a, vec_b);
                sum = _mm256_add_epi32(sum, mul);
            }
            _mm256_storeu_si256((__m256i*)&args->result[i][j], sum);
        }
    }

    return NULL;
}

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

// Determine number of threads (adjust as needed)
int num_threads = 12;  // Example: Use 4 threads

// Allocate result matrix
aligned_int **result = (aligned_int **)aligned_alloc(ALIGN, rows_a * sizeof(aligned_int *));
for (int i = 0; i < rows_a; i++) {
    result[i] = (aligned_int *)aligned_alloc(ALIGN, cols_b * sizeof(aligned_int));
}

int _time = time(NULL);

// Thread creation and management
pthread_t threads[num_threads];
thread_args_t thread_args[num_threads];

// Divide work among threads (assuming equal workload distribution)
int workload_per_thread = cols_b / num_threads;
int remaining_cols = cols_b % num_threads;

for (int i = 0; i < num_threads; i++) {
    int start_col = i * workload_per_thread;
    int end_col = start_col + workload_per_thread;

    // Handle remaining columns for the last thread
    if (i == num_threads - 1) {
        end_col += remaining_cols;
    }

    thread_args[i].a = a;
    thread_args[i].b = b;
    thread_args[i].result = result;
    thread_args[i].rows_a = rows_a;
    thread_args[i].cols_a = cols_a;
    thread_args[i].start_col = start_col;
    thread_args[i].end_col = end_col;

    pthread_create(&threads[i], NULL, matrix_multiply_thread, &thread_args[i]);
}

// Wait for all threads to finish
for (int i = 0; i < num_threads; i++) {
    pthread_join(threads[i], NULL);
}

// Measure time after computation
_time = time(NULL) - _time;


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
            printf("%d ", result[i][j]);
        }
        fprintf(out_file, "\n");
    }

fclose(in_file);
fclose(out_file);
 printf("CPU time %d", _time);
  

 

// Free memory allocated for threads and matrices
for (int i = 0; i < rows_a; i++) {
    free(a[i]);
    free(result[i]);
}
free(a);
for (int i = 0; i < rows_b; i++) {
    free(b[i]);
}
free(b);

return 0;
}
