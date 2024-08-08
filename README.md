README:
Running the Program on Linux

Introduction: 

This program is designed to execute different operations based on an opcode specified in an
input file. It leverages external scripts like `matrix.py`, `MatrixMultiplication.java`, `matrix`,
`vector`, `thread`, and `vectorthread` to perform these tasks.

Prerequisites:

Linux System: Ensure you're running on a Linux distribution.
C++ Compiler: You'll need a C++ compiler like GCC or Clang installed.
External Scripts: The program assumes the presence of the following external scripts in your
system:
1. matrix.py (Python script)
2. MatrixMultiplication.java (Java program)
3. matrix, vector, thread, vectorthread (executable programs)
   
Installation (if necessary):


Downloading source code and compiling manually: 

.Compiling the Program

1. Open a terminal window.
2. Navigate to the directory containing the program's source code (`main.cpp`).
3. Compile the code using a C++ compiler:
   
bash
g++ main.cpp -o program_name # Replace "program_name" with your desired
executable name

Running the Program:

1. Create an input file named `input.txt` (or any name you prefer).2. In the `input.txt` file, place the opcode on the first line, representing the operation you want to
execute. Valid opcodes are:
1: Executes python matrix.py input_file
2: Executes java MatrixMultiplication input_file
3: Executes ./matrix input_file
4: Executes ./vector input_file
5: Executes ./thread input_file
6: Executes ./vectorthread input_file
3. In the terminal, navigate to the directory containing the compiled program and the input file.
4. Execute the program with the input file as an argument.
   
   bash
./program_name input.txt

(Replace `program_name` with the actual name you used when compiling.)
The program will read the opcode from the input file, launch the appropriate external script with
the input file as a parameter, and return the execution result.

(PS: You can also execute populate.cpp in the package to run the script to fill txt file with dummy
data.)

