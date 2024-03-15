import time
import sys


def read_matrix(filename):
    """
    Reads a matrix from a file, ignoring Opcode and data type information.

    Args:
        filename: The path to the file containing the matrix.

    Returns:
        A list of lists representing the matrix.
    """
    with open(filename, 'r') as f:
        # Skip Opcode and data type lines
        f.readline()
        f.readline()

        # Read dimensions
        rows1, cols1 = map(int, f.readline().strip().split())

        matrix = []
        for _ in range(rows1):
            row = [float(x) for x in f.readline().strip().split()]
            matrix.append(row)

        # Read dimensions of the second matrix (assuming it's in the same file)
        rows2, cols2 = map(int, f.readline().strip().split())

        # Read data for the second matrix
        matrix2 = []
        for _ in range(rows2):
            row = [float(x) for x in f.readline().strip().split()]
            matrix2.append(row)

    return matrix, matrix2


def multiply_matrices(matrix1, matrix2):
    """
    Multiplies two matrices.

    Args:
        matrix1: The first matrix.
        matrix2: The second matrix.

    Returns:
        The product of the two matrices.
    """
    rows1, cols1 = len(matrix1), len(matrix1[0])
    rows2, cols2 = len(matrix2), len(matrix2[0])
    if cols1 != rows2:
        raise ValueError("Incompatible matrix dimensions")
    result = [[0 for _ in range(cols2)] for _ in range(rows1)]
    for i in range(rows1):
        for j in range(cols2):
            for k in range(cols1):
                result[i][j] += matrix1[i][k] * matrix2[k][j]
    return result


def write_matrix(filename, matrix):
    """
    Writes a matrix to a file.

    Args:
        filename: The path to the output file.
        matrix: The matrix to write.
    """
    with open(filename, 'w') as f:
        f.write(f"{len(matrix)} {len(matrix[0])}\n")
        for row in matrix:
            f.write(" ".join([str(x) for x in row]) + "\n")


def main():
    """
    The main function that reads input files from arguments, performs matrix multiplication,
    measures time, and writes output to a file.
    """
    if len(sys.argv) != 2:
        print("Usage: python matrix_multiply.py <input_file>")
        sys.exit(1)

    # Get input filename from arguments
    input_file = sys.argv[1]

    start_time = time.time()

    # Read input matrices from the same file
    matrix1, matrix2 = read_matrix(input_file)

    # Perform matrix multiplication
    result = multiply_matrices(matrix1, matrix2)

    # Calculate time excluding IO
    execution_time = time.time() - start_time

    # Write output to file (replace with your desired output path)
    output_file = "output.txt"  # You can change this to a desired filename
    write_matrix(output_file, result)

    print(f"Execution time (excluding IO): {execution_time:.6f} seconds")
    print(f"Output written to: {output_file}")


if __name__ == "__main__":
    main()
