#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {
    // Check if the file name is provided
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename.txt>" << std::endl;
        return 1;
    }

    // Open the file using a file pointer
    std::ifstream file(argv[1]);

    // Check if the file opened successfully
    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file: " << argv[1] << std::endl;
        return 1;
    }

    // Read the file character by character and display the content
    char ch;
    while (file.get(ch)) {
        std::cout << ch;
    }

    // Close the file
    file.close();

    return 0;
}
