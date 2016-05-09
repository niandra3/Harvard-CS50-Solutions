/**
* dictionary.c
* Charles Williams
* lose311@aol.com
*
* Reads words from a dictionary list to create a dictionary
* hash table, then checks the words in the input file against
* the dictionary to determine how many words are misspelled.
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

#define HASHTABLE_SIZE 500000

// Counts # of words in dictionary
unsigned int dictsize = 0;

typedef struct node
{
    char word[LENGTH + 1];
    struct node* next;
}
node;

node* hashtable[HASHTABLE_SIZE];

// Below is a slightly modified SDBM hash function 
// from http://www.cse.yorku.ca/~oz/hash.html
unsigned hash(char* word)
{
    unsigned hash = 0;
    int c;
    while ((c = *word++))
        hash = c + (hash << 6) + (hash << 16) - hash;
    return hash % HASHTABLE_SIZE;
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    char check_word[strlen(word) + 1];
    strncpy(check_word, word, strlen(word) + 1);
    // Convert words to lowercase (dictionary is all lower)
    for(int i = 0; i < strlen(check_word); i++)
    {
        check_word[i] = tolower(check_word[i]);
    }
    unsigned spot = hash(check_word);
    node* crawler = hashtable[spot];
    // Crawls through hashtable looking for word match
    while (crawler != NULL)
    {
        if (strcmp(check_word, crawler->word) == 0)
            return true;
        crawler = crawler->next;
    }
    // No match >> word must be misspelled
    return false;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    // Open input file or return error
    FILE* inptr = fopen(dictionary, "r");
    if (inptr == NULL)
    {
        return false;
    }
    
    // Max word length
    char word[LENGTH + 1];
    
    // Read words until EOF
    while(fscanf(inptr, "%46s", word) != EOF)
    {
        dictsize++;
        node* new_node = malloc(sizeof(node));
        strncpy(new_node->word, word, LENGTH + 1);
        new_node->next = NULL;
        unsigned spot = hash(word);
        new_node->next = hashtable[spot];
        hashtable[spot] = new_node;
    }
    // Successful load
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return dictsize;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    node* temp = NULL;
    node* next = NULL;
    for (int i = 0; i < HASHTABLE_SIZE; i++)
    {
        temp = hashtable[i];
        while (temp != NULL)
        {
            next = temp->next;
            free(temp);
            temp = next;
        }
    }
    // Unload successful
    return true;
}
