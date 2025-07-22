/*
 * Noah Wiley
 * CSCI 114
 * Programming Assignment 1
 *
 * The Program Assignment 1 was written, tested and complied using Linux on macOs
 *
 * To execute the program:
 * By opening the Terminal using in the same directory, I use cd ~/CLionProjects/CSci114_P1
 *
 *Compile the file by executing the file:
 *g++ CSCI114_P1.cpp
 *
 *sampleFile.pdf needs to be the same name and file extension of the file you are testing.
 *Copy the sampleFile... whether that be .pdf, .docx, .mp3, etc. to data.in by executing mv sampleFile.pdf data.in
 *
 *This program may be different to run on different platforms.
 *On Linux and macOS execute ./a.out
 *
 *On Windows platform using Cygwin execute ./a.exe
 *
 *To make sure it works, in terminal use ls and view the files in the directory to look for 'a.out' or 'a.exe'
 *
 *After the coding program completes, execute mv data.out outFile.pdf... open the brand new test file to ensure the binary copy works
*/

#include <iostream>
#include <fstream>

using namespace std;


int main () {
    ifstream input("data.in", ios::binary);

    if (!input) {
        cerr << "Error: Cannot open input file" << endl;
        return 1;
    }

    ofstream output("data.out", ios::binary);
    if (!output) {
        cerr << "Error: Cannot open output file" << endl;
        return 1;
    }

    char buffer[1024 * 1024]; // 1MB Buffer
    while (input) {
        input.read(buffer, sizeof(buffer));
        output.write(buffer, sizeof(buffer));
    }

    cout << "File copied successfully." << endl;
    return 0;
}