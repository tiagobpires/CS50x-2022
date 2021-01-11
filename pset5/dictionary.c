// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
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
const unsigned int N = 26;

// Hash table
node *table[N];

// Number of words and index of the hashtable
unsigned int countWords = 0, indexNum = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Creates a cursor that will cycle through the hashtable
    for (node *cursor = table[hash(word)]; cursor != NULL; cursor = cursor->next)
    {
        // Compares the word with that stored in the hashtable in the case insensitive
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    indexNum = (int) tolower(word[0]);
    // Decreases 97 ('a'), to stay from 0 to 25 (index numbers)
    indexNum -= 97;

    return indexNum;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Open the dictionary and verify if it's valid
    FILE *file = fopen(dictionary, "r");
    if (dictionary == NULL)
    {
        return false;
    }
    
    // String that will store the dictionary word
    char *dictionaryWord = malloc(LENGTH);
    if (dictionaryWord == NULL)
    {
        return false;
    }

    // Reads file until the end
    while (fscanf(file, "%s", dictionaryWord) != EOF)
    {
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }

        // Copy word to node in the free space n-> word
        strcpy(n->word, dictionaryWord);
        // Update number of words
        countWords++;

        // Calculates the index of hashtable
        hash(dictionaryWord);
        // Set next to point at beginning of hashtable
        n->next = table[indexNum];

        // Set array to point at n which becomes new beginning of the hashtable
        // Words are added at the beginning of the hashtable
        table[indexNum] = n;
    }

    // Free used memory
    free(dictionaryWord);
    fclose(file);
    
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return countWords;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    node *tmp, *cursor;

    // Cycles through each hashtable index
    for (int i = 0; i < N; i++)
    {
        // Go to the prox i if there is nothing in the table [i]
        if (table[i] == NULL)
        {
            continue;
        }
        
        cursor = table[i];
        tmp = cursor;

        // Free the hastable
        while (cursor->next != NULL)
        {
            // point to the next node first so as not to lose the adress
            cursor = cursor->next;
            free(tmp);
            // point to the node actual node to free
            tmp = cursor;
        }
        free(cursor);
    }

    return true;
}
