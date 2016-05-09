/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>
#include <stdio.h>

#include "helpers.h"

/**
 * Returns true if 'value' is in array of n 'values', else false.
 */
bool search(int value, int values[], int n)
{
    // Binary search of sorted array 'values' for the int 'value'
    // Return true if found, false if not.
    int left = 0, right = n - 1, middle = (n - 1) / 2;
    while (left <= right)
    {
        if (value > values[middle])
        {
            left = middle + 1;
            middle = (left + right) / 2;            
        }
        
        else if (value == values[middle])
            return true;
        
        else
        {
            right = middle - 1;
            middle = (left + right) / 2;
        }
    }
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // Bubble sort
    int swap = 1;
    int tmp;
    for (int i = 0; i < n - 1 && swap; i++)
    {
        swap = 0;
        for (int j = 0; j < n - i - 1; j++)
        {
            if (values[j] > values[j + 1])
            {
                tmp = values[j];
                values[j] = values[j + 1];
                values[j + 1] = tmp;
                swap = 1;
            }
        }
    }   
}
