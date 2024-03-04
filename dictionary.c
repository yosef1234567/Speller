// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>


#include "dictionary.h"

// a macro to get an index for a single letter
#define hash_letters(x) (tolower(x) - 'a')

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// prototypes
bool add_words(char *word);
void unload_node(node *node_to_unload);

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// a buffer to get a new word of dictionary
char buffer[LENGTH + 1];

// count the number of words in the dictionary
unsigned int count_words_dictionary;

// Hash table
const unsigned int npow = N * N * N * N;
node *table[npow];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    // get the index of the word
    unsigned int index = hash(word);

    // the cursor iterates through the list
    // until finding the word or getting the end of the list
    node *cursor = table[index];
    while (cursor != NULL)
    {
        // check if the word is in the current node
        if (strcasecmp(word, cursor->word) == 0)
        {
            return true;
        }
        // if not move to the next node in the list
        cursor = cursor->next;
    }
    return false;
}

// return an index for each given word
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    int index = 0, len = strlen(word);

    // the index is based in a N (here 26) base system (like decimal or hexdecimal)
    // the first letter index represents N to the 0th power, the second - N to the 1th power etc.
    // the naximum is N to the 4th power for the 4th letter
    if (len < 5)
    {
        for (int i = 0, n = 1; i < len; i++, n *= N)
        {
            if (word[i] != '\'')
            {
                index += hash_letters(word[i]) * n;
            }
        }
    }

    // if there're more than 4 letters - in order to prevent duplication - use the reflection method
    // each letter of the first four is added to the reflecting one from the end (for instance thfirst to the last
    // the second to the one before the end), then the rmainder of modulu N is the index
    else
    {
        char holder[len];
        strcpy(holder, word);

        for (int i = 0, n = 1; i < 4; i++, n *= N)
        {
            // find the reflecting locatoin in the word
            int rflct = len - i - 1;
            if (holder[i] != '\'' && holder[rflct] != '\'')
            {
                // add to the index the letter index doubled by the current power of N
                index += ((hash_letters(holder[i]) + hash_letters(holder[rflct])) % N) * n;
            }
        }
    }
    return index;
}


// Load dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    // open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    // read a word to the buffer and add it to the indexed location in the table
    while (fscanf(file, "%s", buffer) == 1)
    {
        if (!add_words(buffer))
        {
            fclose(file);
            return false;
        }
        // update the number of words of dictionary
        count_words_dictionary++;
    }
    fclose(file);

    return true;
}

// get a word and add it to the table
bool add_words(char *word)
{
    // get the index of the word
    int index = hash(word);

    // allocate memory for a new node and copy the word into
    node *new = malloc(sizeof(node));
    if (new == NULL)
    {
        return false;
    }
    new->next = NULL;

    // copy the word into the new node
    strcpy(new->word, word);

    // create a node pointer called cursor to point to the linked list while adding words
    node *cursor;

    // the cursor points to the head of the list
    cursor = table[index];

    // the indexed root points to the new node
    table[index] = new;

    // the new node points to the cursor and essentially to the linked list
    new->next = cursor;

    return true;
}

// Return number of words in dictionary if loaded, else 0 if not loaded yet
unsigned int size(void)
{
    // TODO
    return count_words_dictionary;
}

// Unload dictionary from memory, return true
bool unload(void)
{
    // TODO
    // iterate over the table and unload each linked list
    for (int i = 0; i < npow; i++)
    {
        unload_node(table[i]);
    }
    return true;
}

// get a pointer to a linked list and unload the list recursively
void unload_node(node *node_to_unload)
{
    // if reached the end - stop (recursive function base case)
    if (node_to_unload == NULL)
    {
        return;
    }

    // unload all the list recursively
    if (node_to_unload->next != NULL)
    {
        unload_node(node_to_unload->next);
    }
    free(node_to_unload);
}