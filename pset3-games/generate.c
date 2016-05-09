/***************************************************************************
 * generate.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Generates pseudorandom numbers in [0,LIMIT), one per line.
 *
 * Usage: generate n [s]
 *
 * where n is number of pseudorandom numbers to print
 * and s is an optional seed
 ***************************************************************************/
 
// standard libraries
#define _XOPEN_SOURCE
#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// constant
#define LIMIT 65536

int main(int argc, string argv[])
{
    // Confirm one or two command arguments provided by user. If zero
    // or more than two arguments are found, return error and tell user.
    if (argc != 2 && argc != 3)
    {
        printf("Usage: generate n [s]\n");
        return 1;
    }

    // 'n' is the number of psuedorandom numbers requested by user as 
    // command argument.
    int n = atoi(argv[1]);

    // If user specified a seed value, send it to srand48 to seed and 
    // initialize drand48.
    // If not, use the current time (in seconds) as a seed value.
    if (argc == 3)
    {
        srand48((long int) atoi(argv[2]));
    }
    else
    {
        srand48((long int) time(NULL));
    }

    // For each number requested by user, call drand48 function
    // to generate a psuedorandom double-precision float between 0 and 1. 
    // Multiply this number by the limit (65536) and cast it to int
    // to print a whole number between 0 and 65536.
    for (int i = 0; i < n; i++)
    {
        printf("%i\n", (int) (drand48() * LIMIT));
    }

    // success
    return 0;
}
