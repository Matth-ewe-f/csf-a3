// Matthew Flynn and Andrew Suh
// Main function and command line argument validation for cache simulator
// CSF Assignment #3

#include <iostream>
#include <string>

#include "main.h"
#include "Cache.h"

using std::cerr;
using std::cout;
using std::endl;
using std::stoi;
using std::string;
using std::cin;

int numSets;
int numBlocksInSet;
int numBytesInBlock;
bool writeAllocate;
bool writeThrough;
bool lru;

/* 
 * Check if a character string contains only numeric digit characters
 */
bool is_numeric(const char *str) {
    while (48 <= *str && *str <= 57) {
        str++;
    }
    return *str == '\0';
}

/*
 * Checks if a number is a power of two
 */
bool is_pow_of_two(int a) {
    int pows = 1;
    while (pows < a) {
        pows *= 2;
    }
    return pows == a;
}

/*
 * Checks if the first three command line arguments are valid. If an argument
 * is invalid, prints an error message to cerr.
 */
bool validate_numeric_args(char *argv[]) {
    // first arg - number of sets. Must be a number
    if (!is_numeric(argv[1])) {
        cerr << "Invalid argument #1" << endl;
        return false;
    } else {
        int sets = stoi(argv[1]);
        if (!is_pow_of_two(sets)) {
            cerr << "Invalid argument #1" << endl;
            return false;
        }
    }
    // second arg - number of blocks in set. Must be a number
    int blocks = 0;
    if (!is_numeric(argv[2])) {
        cerr << "Invalid argument #2" << endl;
        return false;
    } else {
        blocks = stoi(argv[2]);
        if (!is_pow_of_two(blocks)) {
            cerr << "Invalid argument #2" << endl;
            return false;
        }
    }
    // third arg - number of bytes in a block
    // Must be a number greater than 4, and power of 2
    int bytes = 0;
    if (!is_numeric(argv[3])) {
        cerr << "Invalid argument #3" << endl;
        return false;
    } else {
        bytes = stoi(argv[3]);
        if (bytes < 4 || !is_pow_of_two(bytes)) {
            cerr << "Invalid argument #3" << endl;
            return false;
        }
    }
    return true;
}

/*
 * Checks if the last three command line arguments are valid. The first
 * pointer in argv should point to the fourth argument. If any arguments are
 * not valid, prints an error message to cerr.
 */
bool validate_textual_args(char *argv[]) {
    // fourth argument - write-allocate or no-write-allocate
    string write_load(argv[0]);
    if (write_load == "write-allocate") {
        writeAllocate = true;
    } else if (write_load == "no-write-allocate") {
        writeAllocate = false;
    } else {
        cerr << "Invalid argument #4" << endl;
        return false;
    }
    // fifth argument - write-back or write-through
    // can only be write-through if 4th arg was no-write-allocate
    string write_loc(argv[1]);
    if (write_loc != "write-back" && write_loc != "write-through") {
        cerr << "Invalid argument #5" << endl;
        return false;
    } else if (write_loc=="write-back" && write_load=="no-write-allocate") {
        cerr << "Invalid combination of arguments #4 and #5" << endl;
        return false;
    } else if (write_loc=="write-through") {
        writeThrough = true;
    } else {
        writeThrough = false;
    }
    // sixth argument - lru or fifo
    string evic(argv[2]);
    if (evic != "lru" && evic != "fifo") {
        cerr << "Invalid argument #6" << endl;
        return false;
    } else if (evic == "lru" ) {
        lru = true;
    } else {
        lru = false;
    }
    return true;
}

int main(int argc, char *argv[]) {
    // check to make sure valid arguments were given
    if (argc < 7) {
        cerr << "Not enough arguments" << endl;
        return 1;
    }
    if (!validate_numeric_args(argv)) {
        return 1;
    }
    if (!validate_textual_args(argv + 4)) {
        return 1;
    }

    // initialize cache with the given command line arguments
    Cache cache (atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), writeAllocate, writeThrough, lru);

    // variables to pass onto cache
    char action;
    long address;
    int third;

    unsigned long index = 1;
    // loop that continues with every line in given trace
    while (cin) {
        cin >> action >> std::hex >> address >> third;
        // here, cin read an empty line and need to break
        if (action == 'a') {
            break;
        }

        // perform either load or store action
        if (action == 'l') {
            cache.performLoad(address);
        } else if (action == 's') {
            cache.performStore(address);
        } else {
            cerr << "instruction on line " << index 
                    << " not load or store: " << action << endl;
            return 1;
        }
        
        // set action to 'a' to check later if cin read empty line
        action = 'a';
        index++;
    }

    cache.printResults();

    return 0;
}