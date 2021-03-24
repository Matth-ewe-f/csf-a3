// Matthew Flynn (mflynn24) and Andrew Suh (wsuh1)
// Main function and command line argument validation for Cache Simulator
// CSF Assignment #3

#ifndef CACHE_SIM
#define CACHE_SIM

bool is_numeric(const char *str);
bool is_pow_of_two(int a);
bool validate_numeric_args(char *argv[]);
bool validate_textual_args(char *argv[]);

#endif