#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>  // For system()

using namespace std;

int main(int argc, char* argv[]) {
  // Check for valid arguments
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " <input_file>" << endl;
    return 1;
  }

  // Get input file name from argument
  string input_file = argv[1];

  // Open input file
  ifstream infile(input_file);
  if (!infile.is_open()) {
    cerr << "Error opening input file: " << input_file << std::endl;
    return 1;
  }

    int opcode;

    infile >> opcode;

    infile.close();

    string command;

    switch (opcode) {
        case 1:
            command = "python matrix.py " + input_file;
            break;
        case 2:
            command = "java MatrixMultiplication " + input_file;
            break;
        case 3:
            command = "./matrix " + input_file;
            break;
        case 4:
            command = "./vector " + input_file;
            break;
        case 5:
            command = "./thread " + input_file;
            break;
        case 6:
            command = "./vectorthread " + input_file;
            break;
        default:
            std::cerr << "Invalid opcode" << std::endl;
            return 1;
    }

    // Execute the command with the input file as argument
    int result = system(command.c_str());
    return result;
}
