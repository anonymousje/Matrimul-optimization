#include <stdio.h>
#include <stdlib.h>
#include <limits.h>  // For INT_MAX
#include <time.h>

#define MAX_SIZE 4096 // Maximum matrix size

typedef enum {
  INT32 = 1,
  FLOAT32 = 2,
  INT64 = 3,
  DOUBLE64 = 4
} data_type_t;

int read_matrix_dimensions(FILE *in_file, int *rows, int *cols) {
  if (fscanf(in_file, "%d %d", rows, cols) != 2) {
    fprintf(stderr, "Error: Failed to read matrix dimensions\n");
    return -1;
  }
  return 0;
}

void *allocate_matrix(data_type_t data_type, int rows, int cols) 
{
  if (rows <= 0 || cols <= 0) {
    fprintf(stderr, "Error: Invalid matrix dimensions (%d x %d)\n", rows, cols);
    return NULL;
  }

  size_t element_size;
  switch (data_type) {
    case INT32:
      element_size = sizeof(int);
      break;
    case FLOAT32:
      element_size = sizeof(float);
      break;
    case INT64:
      element_size = sizeof(long long);
      break;
    case DOUBLE64:
      element_size = sizeof(double);
      break;
    default:
      fprintf(stderr, "Error: Unsupported data type (%d)\n", data_type);
      return NULL;
  }

  size_t total_size = element_size * rows * cols;
  if (total_size > INT_MAX) {
    fprintf(stderr, "Error: Memory allocation overflow\n");
    return NULL;
  }

  void *matrix = malloc(total_size);
  if (!matrix) {
    fprintf(stderr, "Error: Memory allocation failed\n");
  }
  return matrix;
}

int read_matrix_data(FILE *in_file, data_type_t data_type, void *matrix, int rows, int cols) 
{
  switch (data_type) {
    case INT32:
      for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
          if (fscanf(in_file, "%d", ((int *)matrix + i * cols + j)) != 1) {
            fprintf(stderr, "Error: Failed to read matrix data\n");
            return -1;
          }
        }
      }
      break;
    case FLOAT32:
      for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
          if (fscanf(in_file, "%f", ((float *)matrix + i * cols + j)) != 1) {
            fprintf(stderr, "Error: Failed to read matrix data\n");
            return -1;
          }
        }
      }
      break;
    case INT64:
      for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
          if (fscanf(in_file, "%lld", ((long long *)matrix + i * cols + j)) != 1) {
            fprintf(stderr, "Error: Failed to read matrix data\n");
            return -1;
          }
        }
      }
      break;
    case DOUBLE64:
      for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
          if (fscanf(in_file, "%lf", ((double *)matrix + i * cols + j)) != 1) {
            fprintf(stderr, "Error: Failed to read matrix data\n");
            return -1;
          }
        }
      }
      break;
  }
  return 0;
}

void free_matrix(void *matrix) {
  if (matrix) {
    free(matrix);
  }
}

// Function to perform matrix multiplication
void matrix_multiply(void *matrix_a, int rows_a, int cols_a,
                      void *matrix_b, int rows_b, int cols_b,
                      void *result, data_type_t data_type) {

  if (cols_a != rows_b) {
    fprintf(stderr, "Error: Incompatible matrix dimensions for multiplication\n");
    return;
  }

  int i, j, k;
  size_t element_size;
  switch (data_type) {
    case INT32:
      element_size = sizeof(int);
      for (i = 0; i < rows_a; i++) {
        for (j = 0; j < cols_b; j++) {
          int sum = 0;
          for (k = 0; k < cols_a; k++) {
            sum += ((int *)matrix_a)[i * cols_a + k] * ((int *)matrix_b)[k * cols_b + j];
          }
          ((int *)result)[i * cols_b + j] = sum;
        }
      }
      break;
    case FLOAT32:
      element_size = sizeof(float);
      for (i = 0; i < rows_a; i++) {
        for (j = 0; j < cols_b; j++) {
          float sum = 0.0f;
          for (k = 0; k < cols_a; k++) {
            sum += ((float *)matrix_a)[i * cols_a + k] * ((float *)matrix_b)[k * cols_b + j];
          }
          ((float *)result)[i * cols_b + j] = sum;
        }
      }
      break;
    case INT64:
      element_size = sizeof(long long);
      for (i = 0; i < rows_a; i++) {
        for (j = 0; j < cols_b; j++) {
          long long sum = 0;
          for (k = 0; k < cols_a; k++) {
            sum += ((long long *)matrix_a)[i * cols_a + k] * ((long long *)matrix_b)[k * cols_b + j];
          }
          ((long long *)result)[i * cols_b + j] = sum;
        }
      }
      break;
    case DOUBLE64:
      element_size = sizeof(double);
      for (i = 0; i < rows_a; i++) {
        for (j = 0; j < cols_b; j++) {
          double sum = 0.0;
          for (k = 0; k < cols_a; k++) {
            sum += ((double *)matrix_a)[i * cols_a + k] * ((double *)matrix_b)[k * cols_b + j];
          }
          ((double *)result)[i * cols_b + j] = sum;
        }
      }
      break;
  }
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
    return 1;
  }

  // Open input file
  FILE *in_file = fopen(argv[1], "r");
  if (!in_file) {
    fprintf(stderr, "Error: Could not open file: %s\n", argv[1]);
    return 1;
  }

  // Read opcode (ignored)
  int opcode;
  fscanf(in_file, "%d", &opcode);

  // Read data type
  int data_type_int;
  if (fscanf(in_file, "%d", &data_type_int) != 1) {
    fprintf(stderr, "Error: Failed to read data type\n");
    fclose(in_file);
    return 1;
  }
  data_type_t data_type;
  switch (data_type_int) {
    case INT32:
      data_type = INT32;
      break;
    case FLOAT32:
      data_type = FLOAT32;
      break;
    case INT64:
      data_type = INT64;
      break;
    case DOUBLE64:
      data_type = DOUBLE64;
      break;
    default:
      fprintf(stderr, "Error: Unsupported data type: %d\n", data_type_int);
      fclose(in_file);
      return 1;
  }

  // Read dimensions of first matrix
  int rows_a, cols_a;
  if (read_matrix_dimensions(in_file, &rows_a, &cols_a) != 0) 
  {
    fclose(in_file);
    return 1;
  }

  // Allocate memory for first matrix
  void *matrix_a = allocate_matrix(data_type, rows_a, cols_a);
  if (!matrix_a) {
    fclose(in_file);
    
    return 1;
  }

  // Read first matrix data
  if (read_matrix_data(in_file, data_type, matrix_a, rows_a, cols_a) != 0) {
    free_matrix(matrix_a);
    fclose(in_file);
    return 1;
  }

  // Read dimensions of second matrix
  int rows_b, cols_b;
  if (read_matrix_dimensions(in_file, &rows_b, &cols_b) != 0) {
    free_matrix(matrix_a);
    fclose(in_file);
    return 1;
  }

  // Validate matrix dimensions for multiplication
  if (cols_a != rows_b) {
    fprintf(stderr, "Error: Incompatible matrix dimensions (%dx%d) and (%dx%d)\n",
            rows_a, cols_a, rows_b, cols_b);
    free_matrix(matrix_a);
    fclose(in_file);
    return 1;
  }

  // Allocate memory for second matrix
  void *matrix_b = allocate_matrix(data_type, rows_b, cols_b);
  if (!matrix_b) {
    free_matrix(matrix_a);
    fclose(in_file);
    return 1;
  }

  // Read second matrix data
  if (read_matrix_data(in_file, data_type, matrix_b, rows_b, cols_b) != 0) {
    free_matrix(matrix_a);
    free_matrix(matrix_b);
    fclose(in_file);
    return 1;
  }

  // Allocate memory for the result matrix
  void *result = allocate_matrix(data_type, rows_a, cols_b);
  if (!result) {
    free_matrix(matrix_a);
    free_matrix(matrix_b);
    fclose(in_file);
    return 1;
  }
    time_t _time = time(NULL);

  // Perform matrix multiplication
  matrix_multiply(matrix_a, rows_a, cols_a, matrix_b, rows_b, cols_b, result, data_type);
    _time = time(NULL) - _time;
    printf("CPU time (excluding I/O): %d seconds\n", _time);

  // Print the result matrix (add your preferred printing logic here)
  /*printf("Resultant Matrix:\n");
  for (int i = 0; i < rows_a; i++) {
    for (int j = 0; j < cols_b; j++) {
      switch (data_type) {
        case INT32:
          printf("%d ", ((int *)result)[i * cols_b + j]);
          break;
        case FLOAT32:
          printf("%f ", ((float *)result)[i * cols_b + j]);
          break;
        case INT64:
          printf("%lld ", ((long long *)result)[i * cols_b + j]);
          break;
        case DOUBLE64:
          printf("%lf ", ((double *)result)[i * cols_b + j]);
          break;
      }
    }
    printf("\n");
  }*/
FILE *out_file = fopen("outputfinal.txt", "w");
  if (!in_file) {
    fprintf(stderr, "Error: Could not open file: %s\n", argv[1]);
    return 1;
  }
  fprintf(out_file, "%d %d\n", rows_a, cols_b);
  for (int i = 0; i < rows_a; i++) {
    switch (data_type) {
      case INT32:
        for (int j = 0; j < cols_b; j++) {
          fprintf(out_file, "%d ", ((int *)result)[i * cols_b + j]);
        }
        break;
      case FLOAT32:
        for (int j = 0; j < cols_b; j++) {
          fprintf(out_file, "%f ", ((float *)result)[i * cols_b + j]);
        }
        break;
      case INT64:
        for (int j = 0; j < cols_b; j++) {
          fprintf(out_file, "%lld ", ((long long *)result)[i * cols_b + j]);
        }
        break;
      case DOUBLE64:
        for (int j = 0; j < cols_b; j++) {
          fprintf(out_file, "%lf ", ((double *)result)[i * cols_b + j]);
        }
        break;
    }
    fprintf(out_file, "\n");
  }

  // Free allocated memory
  free_matrix(matrix_a);
  free_matrix(matrix_b);
  free_matrix(result);

  fclose(in_file);
  fclose(out_file);
  return 0;
}
