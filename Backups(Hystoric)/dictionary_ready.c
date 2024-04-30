// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>


#include "dictionary.h"

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
int hash_letters(char letter);


// TODO: Choose number of buckets in hash table
const unsigned int N = 27;

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
        if (strcasecmp(word, cursor->word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}

unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    // this function returns different indexes for different words
    // by using the following method.
    // in general, each index consists of four "sub" indexes
    // based on four letters of the given word
    // the first letter index is doubled by 27 powered by 0 (1)
    // the second is doubled by 27 powered by 1 (27)
    // the third one by 27 of power 2 and the 4th one by 27 of 3
    // you will find that each letter has a "range" for itself (aka ab != ba)
    // the letters are picked by a constant skipping value based on
    // the length of the word - 1 dividing by 3 (reminder removed by using int)
    // for instance: word of length 5: skipping value is (5 - 1) / 3 = 1
    // the 4 letters are: 0th, 1th, 2th, 3th
    // more one: word of length 13: skipping value is (13 - 1) / 3 = 4
    // the 4 letters to hash are: 0th, 4th, 8th, 12th
    // the index of a word less than 4 letters will consist of only
    // the number of the letters
    // for instance a word of 2 letters will have only 2 "sub" indexes

    int index = 0, len = strlen(word), skip;

    // skipping value will be more than 1 only if the length is 7 or more
    skip = len < 7 ? 1 : (len - 1) / 3;

    // update len to maximum 4 letters to hash (if longer than 4)
    len = (len < 4) ? len : 4;

    for(int i = 0, j = 0, n = 1; i < len; i++, j += skip, n *= N)
    {
        // ignore
        if (word[j] != '\'')
        {
            index += hash_letters(word[j]) * n;
        }
    }
    return index;
}

// return the index of a letter according to its alphabetical location
int hash_letters(char letter)
{
    return tolower(letter) - 'a' + 1;
}

// Load dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    while (fscanf(file, "%s", buffer) == 1)
    {
        if(!add_words(buffer))
        {
            fclose(file);
            return false;
        }
        count_words_dictionary++;
    }
    fclose(file);
    // printf ("there're %i linked lists\n", sum_lists());
    // printf ("longest list is %i\n", longest());
    return true;
}

bool add_words(char *word)
{
    // get the index of the word
// printf("%s\n", word);
    int index = hash(word);

    // lists[index]++;

    // allocate memory for a new node and copy the word into
    node *new = malloc(sizeof(node));
    if (new == NULL)
    {
        return false;
    }
    new->next = NULL;
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

// Return number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return count_words_dictionary;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < npow; i++)
    {
        // printf ("start iterating");
        unload_node(table[i]);
    }
    return true;
}

void unload_node(node *node_to_unload)
{
    if (node_to_unload == NULL)
    {
        return;
    }
    if (node_to_unload->next != NULL)
    {
        unload_node(node_to_unload->next);
    }
    free(node_to_unload);
}





// int sum_lists()
// {
//     int x = 0;
//     for (int i = 0; i < N * N * N * N; i++)
//     {
//         if (lists[i] != 0)
//         {
//             x++;
//         }
//     }
//     return x;
// }

// int longest()
// {
//     int length = 0;
//     for (int i = 0; i < npow; i++)
//     {
//         if (length < lists[i])
//         {
//             length = lists[i];
//         }
//     }
//     return length;
// }