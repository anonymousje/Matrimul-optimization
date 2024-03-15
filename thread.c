#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define MAX_SIZE 4096

struct thread_data {
  int **a;
  int **b;
  int **result;
  int row_start;
  int row_end;
  int cols_a;
  int cols_b;
};

void *multiply_rows(void *arg) {
  struct thread_data *data = (struct thread_data *)arg;

  for (int i = data->row_start; i < data->row_end; i++) {
    for (int j = 0; j < data->cols_b; j++) {
      data->result[i][j] = 0;
      for (int k = 0; k < data->cols_a; k++) {
        data->result[i][j] += data->a[i][k] * data->b[k][j];
      }
    }
  }

  return NULL;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <input_file>\n", argv[0]);
    return 1;
  }

  // Open input file
  FILE *in_file = fopen(argv[1], "r");
  if (!in_file) {
    printf("Error opening input file: %s\n", argv[1]);
    return 1;
  }

  // Discard opcode and data type (if present)
  int opcode, data_type;
  fscanf(in_file, "%d %d", &opcode, &data_type);

  // Read dimensions of the first matrix
  int rows_a,cols_a;
  fscanf(in_file, "%d %d", &rows_a, &cols_a);
  if (rows_a <= 0 || cols_a <= 0 || rows_a > MAX_SIZE || cols_a > MAX_SIZE) {
    printf("Error: Invalid matrix dimensions (%dx%d)\n", rows_a, cols_a);
    fclose(in_file);
    return 1;
  }

  // Allocate memory for the first matrix
  int **a = (int **)malloc(rows_a * sizeof(int *));
  for (int i = 0; i < rows_a; i++) {
    a[i] = (int *)malloc(cols_a * sizeof(int));
  }

  // Read elements of the first matrix
  for (int i = 0; i < rows_a; i++) {
    for (int j = 0; j < cols_a; j++) {
      fscanf(in_file, "%d", &a[i][j]);
    }
  }

  // Read dimensions of the second matrix
    int rows_b,cols_b;

  fscanf(in_file, "%d %d", &rows_b, &cols_b);
  if (rows_b <= 0 || cols_b <= 0 || rows_b > MAX_SIZE || cols_b > MAX_SIZE) {
    printf("Error: Invalid matrix dimensions (%dx%d)\n", rows_b, cols_b);
    fclose(in_file);
    for (int i = 0; i < rows_a; i++) {
      free(a[i]);
    }
    free(a);
    return 1;
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
    return 1;
  }

  // Allocate memory for the second matrix
  int **b = (int **)malloc(rows_b * sizeof(int *));
  for (int i = 0; i < rows_b; i++) {
    b[i] = (int *)malloc(cols_b * sizeof(int));
  }

  // Read elements of the second matrix
  for (int i = 0; i < rows_b; i++) {
    for (int j = 0; j < cols_b; j++) {
      fscanf(in_file, "%d", &b[i][j]);
    }
  }

  // Measure time before reading input
  time_t _time = time(NULL);

  // Define the number of threads (adjust based on your system)
  int num_threads = 8;

  // Create threads
  pthread_t threads[num_threads];
  struct thread_data data[num_threads];

  int rows_per_thread = rows_a / num_threads;
  int remaining_rows = rows_a;

  // Allocate memory for result matrices (outside the loop now)
  int **result = (int **)malloc(rows_a * sizeof(int *));
  for (int i = 0; i < rows_a; i++) {
    result[i] = (int *)malloc(cols_b * sizeof(int));
  }

  // Loop to distribute rows among threads
  for (int i = 0; i < num_threads; i++) {
    int row_start = i * rows_per_thread;
    int row_end = (i == num_threads - 1) ? rows_a : row_start + rows_per_thread;

    data[i].a = a;
    data[i].b = b;
    data[i].result = result; // All threads point to the same final result matrix
    data[i].row_start = row_start;
    data[i].row_end = row_end;
    data[i].cols_a = cols_a;
    data[i].cols_b = cols_b;

    // Create threads
    if (pthread_create(&threads[i], NULL, multiply_rows, &data[i]) != 0) {
      perror("pthread_create");
      for (int j = 0; j < i; j++) {
        pthread_detach(threads[j]); // Detach created threads to avoid resource leaks if creation fails
      }
      for (int j = 0; j < rows_a; j++) {
        free(result[j]);
      }
      free(result);
      for (int j = 0; j < rows_a; j++) {
        free(a[j]);
      }
      free(a);
      for (int j = 0; j < rows_b; j++) {
        free(b[j]);
      }
      free(b);
      fclose(in_file);
      return 1;
    }
  }

  // Wait for all threads to finish
  for (int i = 0; i < num_threads; i++) {
    if (pthread_join(threads[i], NULL) != 0) {
      perror("pthread_join");
      for (int j = 0; j < rows_a; j++) {
        free(result[j]);
      }
      free(result);
      for (int j = 0; j < rows_a; j++) {
        free(a[j]);
      }
      free(a);
      for (int j = 0; j < rows_b; j++) {
        free(b[j]);
      }
      free(b);
      fclose(in_file);
      return 1;
    }
  }

  // Measure time after computation
  _time = time(NULL) - _time;

  // Write result matrix to output file (you can choose the output file name)
  printf("Write the output file name: ");
  char output_file[100];
  scanf("%s", output_file);
  FILE *out_file = fopen(output_file, "w");
  if (!out_file) {
    printf("Error opening output file: %s\n", output_file);
    for (int i = 0; i < rows_a; i++) {
      free(result[i]);
    }
    free(result);
    for (int i = 0; i < rows_a; i++) {
      free(a[i]);
    }
    free(a);
    for (int i = 0; i < rows_b; i++) {
      free(b[i]);
    }
    
    free(b);
    fclose(in_file);
    return 1;
  }

  fprintf(out_file, "%d %d\n", rows_a, cols_b);
  for (int i = 0; i < rows_a; i++) {
    for (int j = 0; j < cols_b; j++) {
      fprintf(out_file, "%d ", result[i][j]);
    }
    fprintf(out_file, "\n");
  }

  // Free allocated memory
  for (int i = 0; i < rows_a; i++) {
    free(result[i]);
  }
  free(result);
  for (int i = 0; i < rows_a; i++) {
    free(a[i]);
  }
  free(a);
  for (int i = 0; i < rows_b; i++) {
    free(b[i]);
  }
  free(b);

  fclose(out_file);

  printf("CPU time (excluding I/O): %d seconds\n", _time);

  return 0;

}