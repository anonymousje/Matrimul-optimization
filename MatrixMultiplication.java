import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.Scanner;

public class MatrixMultiplication {

    public static void main(String[] args) throws FileNotFoundException {
        if (args.length != 1) {
            System.out.println("Usage: java MatrixMultiplication <input_file>");
            System.exit(1);
        }

        String filePath = args[0];

        // Measure total execution time
        long startTime = System.nanoTime();

        // Read matrices from file
        int[][] matrixA = null;
        int[][] matrixB = null;
        try (Scanner scanner = new Scanner(new File(filePath))) {
            // Skip Opcode and data type
            scanner.nextInt();
            scanner.nextInt();

            // Read dimensions of first matrix
            matrixA = readMatrix(scanner);

            // Read dimensions of second matrix
            matrixB = readMatrix(scanner);

            // Validate dimensions for multiplication
            if (matrixA[0].length != matrixB.length) {
                System.err.println("Error: Incompatible matrix dimensions for multiplication.");
                System.exit(1);
            }
        }

        // Perform matrix multiplication
        long startTimeMultiply = System.nanoTime();
        int[][] resultMatrix = multiplyMatrices(matrixA, matrixB);
        long endTimeMultiply = System.nanoTime();

        // Write result matrix to file
        long startTimeWrite = System.nanoTime();
        writeMatrix(resultMatrix, "output.txt"); // Change to desired output file name
        long endTimeWrite = System.nanoTime();

        // Calculate and print execution times
        long totalTime = System.nanoTime() - startTime;
        double totalTimeSeconds = (double) totalTime / 1_000_000_000;

        long multiplyTime = endTimeMultiply - startTimeMultiply;
        double multiplyTimeSeconds = (double) multiplyTime / 1_000_000_000;

        long writeTime = endTimeWrite - startTimeWrite;
        double writeTimeSeconds = (double) writeTime / 1_000_000_000;


        System.out.println("Total Execution Time (s): " + totalTimeSeconds);
        System.out.println("Matrix Multiplication Time (s): " + multiplyTimeSeconds);
        System.out.println("Output Write Time (s): " + writeTimeSeconds);
    }

    private static int[][] readMatrix(Scanner scanner) {
        // Read dimensions
        int rows = scanner.nextInt();
        int cols = scanner.nextInt();

        // Allocate matrix based on read dimensions
        int[][] matrix = new int[rows][cols];

        // Read matrix data
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                matrix[i][j] = scanner.nextInt();
            }
        }

        return matrix;
    }

     private static int[][] multiplyMatrices(int[][] matrixA, int[][] matrixB) {
        int[][] result = new int[matrixA.length][matrixB[0].length];

        for (int i = 0; i < matrixA.length; i++) {
            for (int j = 0; j < matrixB[0].length; j++) {
                for (int k = 0; k < matrixA[0].length; k++) {
                    result[i][j] += matrixA[i][k] * matrixB[k][j];
                }
            }
        }

        return result;
    }

    private static void writeMatrix(int[][] matrix, String filePath) throws FileNotFoundException 
    {
    PrintWriter writer = new PrintWriter(filePath);

    // Write the first line with dimensions for clarity
    writer.println(matrix.length + " " + matrix[0].length); // Write actual rows and columns

    for (int i = 0; i < matrix.length; i++) {
        for (int j = 0; j < matrix[0].length; j++) {
            writer.print(matrix[i][j] + " ");
        }
        writer.println(); // Newline after each row
    }

    writer.close();
    
    }
}
