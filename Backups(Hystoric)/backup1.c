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

bool add_words(char *word);
void unload_node(node *node_to_unload);
int hash_letters(char letter);


unsigned int words_count_dictionary = 0;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

char buffer[LENGTH + 1];
unsigned int count_words_dictionary;

// Hash table
node *table[N * N * N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    // get the index of the word
    unsigned int index = hash(word);

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

// Loads dictionary into memory, returning true if successful, else false
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
    return true;
}

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
    for (int i = 0; i < N * N; i++)
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


unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    // check whether the word consists of more than one letter or not
    int i;
    for (i = 0; word[i] != '\0' || i < 3; i++)
    {}

    // if it consists of more than one letter en sure that the second isn't a quote
    // and set its index as follows:
    // first letter index * number of letters (26), plus second letter index
    // for instance "hi" will get "h" index (7) * 26 + "i" index (8) - 7*26+8 = 180
    if (i == 2 && word[1] != '\'' && word[2] != '\'')
    {
        return hash_letters(word[0]) * N * N + hash_letters(word[1]) * N + hash_letters(word[2]);
    }
    // if the word either consists of one letter or the second letter is a quote
    // set its index to the first letter index
    else
    {
        return hash_letters(word[0]);
    }
}

// return the index of a letter according to its alphabetical location
int hash_letters(char letter)
{
    return tolower(letter) - 'a';
}
