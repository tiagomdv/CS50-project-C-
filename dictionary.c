// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 10000;

// Number of words in dictionary
unsigned int size_w = 0;

// Hash table
node *table[N - 1]; // Array of pointers to node structure

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    int index = hash(word);

    node *cursor;
    cursor = table[index];

    if (cursor == NULL)
    {
        return false;
    }

    do
    {
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        else
        {
            cursor = cursor->next;
        }
    }
    while (cursor != NULL);

    free(cursor);
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word) // Hash function idea from youtube channel Jacob Sorber!
{
    int length = strlen(word);
    int lowercase = 0;
    unsigned int hash_value = 0;

    for (int i = 0; i < length; i++)
    {
        if (word[i] > 64 && word[i] < 91)
        {
            lowercase = word[i] + 32;
        }
        else
        {
            lowercase = word[i];
        }
        hash_value += lowercase;
        hash_value = (hash_value * lowercase) % N;
    }
    return hash_value;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Open dictionary file to read
    FILE *dict = fopen(dictionary, "r");

    if (dict == NULL)
    {
        printf("Could not open dictionary");
        return false;
    }


    for (; ;)
    {
        node *n = malloc(sizeof(node)); // Allocates memory to temporary pointer to node structure

        // Fills the temporary node
        int eeof = fscanf(dict, "%s", n->word); // Gets the word from the dictionary file and puts it into node
        if (eeof != 1)
        {
            free(n);
            break;
        }

        n->next = NULL;

        int index = hash(n->word);

        if (table[index] == NULL)
        {
            table[index] = n; // Fills the Hash Table with created temporary nodes
            size_w++;
        }
        else
        {
            node *cursor; // Temporary node to go to end of linked list in an index
            cursor = table[index]; // Define the starting point of certain index

            // Goes to the end of an index linked list
            while (cursor->next != NULL)
            {
                cursor = cursor->next;
            }

            cursor->next = n;
            size_w++;
        }
    }

    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return size_w;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        if (table[i] == NULL)
        {
            continue;
        }

        if (table[i] != NULL)
        {
            node *cursor;
            node *tmp;

            cursor = table[i];

            while (cursor != NULL)
            {
                tmp = cursor;
                cursor = cursor->next;
                free(tmp);
            }

            free(cursor);
        }
    }
    return true;
}
