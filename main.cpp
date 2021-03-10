// Matthew Flynn and Andrew Suh
// CSF Assignment #3

#include <iostream>
#include <string>

#include "main.h"

using std::cerr;
using std::cout;
using std::endl;
using std::stoi;
using std::string;

/* 
 * Check if a character string represents a number. 
 * 
 * Returns true if the character string contains only numeric digits. Returns
 * false otherwise.
 */
bool is_numeric(const char *str) {
    while (48 <= *str && *str <= 57) {
        str++;
    }
    return *str == '\0';
}

bool is_pow_of_two(int a) {
    int pows = 1;
    while (pows < a) {
        pows *= 2;
    }
    return pows == a;
}

// TODO this function is too long already, probably break up
int main(int argc, char *argv[]) {
    // check to make sure we have enough arguments
    if (argc < 7) {
        cerr << "Not enough arguments" << endl;
        return 1;
    }

    // first argument - number of sets
    int sets = 0;
    if (!is_numeric(argv[1])) {
        cerr << "Invalid argument #1" << endl;
        return 1;
    } else {
        sets = stoi(argv[1]);
        if (!is_pow_of_two(sets)) {
            cerr << "Invalid argument #1" << endl;
            return 1;
        }
    }
    // TODO handle first argument 

    // second argument - number of blocks in set
    int blocks = 0;
    if (!is_numeric(argv[2])) {
        cerr << "Invalid argument #2" << endl;
        return 1;
    } else {
        blocks = stoi(argv[2]);
        if (!is_pow_of_two(blocks)) {
            cerr << "Invalid argument #2" << endl;
            return 1;
        }
    }
    // TODO handle second argument

    // third argument - number of bytes in a block
    int bytes = 0;
    if (!is_numeric(argv[3])) {
        cerr << "Invalid argument #3" << endl;
        return 1;
    } else {
        bytes = stoi(argv[3]);
        if (bytes < 4 || !is_pow_of_two(bytes)) {
            cerr << "Invalid argument #3" << endl;
            return 1;
        }
    }
    // TODO handle third argument

    // fourth argument - write-allocate or no-write-allocate
    string write_load(argv[4]);
    if (write_load != "write-allocate" && write_load != "no-write-allocate") {
        cerr << "Invalid argument #4" << endl;
        return 1;
    }
    // TODO handle fourth argument

    // fifth argument - write-back or write-through
    string write_loc(argv[5]);
    if (write_loc != "write-back" && write_loc != "write-through") {
        cerr << "Invalid argument #5" << endl;
        return 1;
    }
    // TODO handle fifth argument

    // sixth argument - lru or fifo
    string evic(argv[6]);
    if (evic != "lru" && evic != "fifo") {
        cerr << "Invalid argument #6" << endl;
        return 1;
    }
    // TODO handle sixth argument


    return 0;
}