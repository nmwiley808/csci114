/*
* Noah Wiley
 * CSCI 114
 * Programming Assignment 2
 *
 * The Program Assignment 2 was written, tested and complied using Linux on macOs
 *
 * To execute the program:
 * By opening the Terminal using in the same directory, I use cd ~/CLionProjects/CSci114_P2
 *
 *Compile the file by executing the file:
 *g++ CSCI114_P2.cpp
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
 *When the program successfully works, "This File was successfully copied using UNIX pipe" should pop out in the terminal
 *
 *After the coding program completes, execute mv data.out outFile.pdf... open the brand new test file to ensure the binary copy works
*/

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>
#define BUFFER_SIZE 4096 // Buffer size for reading and writing


using namespace std;

int main() {
    int pipefd[2]; // File descriptions for pipe
    pid_t pid;

    if (pipe(pipefd) == -1) {
        perror("Pipe failed");
        return 1;
    }

    pid = fork(); // Creates the child process

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }

    if (pid > 0) { // Process A
        close(pipefd[0]);

        ifstream infile("data.in", ios::binary); // Open file in binary mode
        if (!infile) {
            perror("Failed to open data.in");
            return 1;
        }

        vector<char> buffer(BUFFER_SIZE);
        while (infile.read(buffer.data(), BUFFER_SIZE) || infile.gcount() > 0) {
            write(pipefd[1], buffer.data(), infile.gcount());
        }

        infile.close();
        close(pipefd[1]);
        wait(nullptr);
    } else {
        close(pipefd[1]);

        ofstream outfile("data.out", ios::binary);
        if (!outfile) {
            perror("Failed to open data.out");
            return 1;
        }

        vector<char> buffer(BUFFER_SIZE);
        ssize_t bytes_read;

        while ((bytes_read = read(pipefd[0], buffer.data(), BUFFER_SIZE)) > 0) {
            outfile.write(buffer.data(), bytes_read);
        }

        outfile.close();
        close(pipefd[0]); // Close read end of the pipe

        cout << "This File was successfully copied using UNIX pipe\n";
    }

    return 0;
}

