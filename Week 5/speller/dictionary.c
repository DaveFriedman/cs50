// Implements a dictionary's functionality

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of entries in dictionary
int dictEntries = 0;

// Number of buckets in hash table
const unsigned int N = 40000; // play with me

// Hash table, using chains of linked lists ("chaining") to avoid collisions
node *table[N];

// FNV1a hash, via https://create.stephan-brumme.com/fnv-hash/
const uint32_t prime = 0x01000193; //   16777619
const uint32_t seed  = 0x811C9DC5; // 2166136261

static inline uint32_t fnv1a(unsigned char oneByte, uint32_t hash)
{
    return (oneByte ^ hash) * prime;
}

unsigned int hash(const char *word)
{
    int hash = 0;
    while (*word)
    {
        if (hash == 0)
        {
            hash = seed;
        }
        hash = fnv1a((unsigned char) * word++, hash);
    }
    return hash % N;
}


// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // While words from the dictionary are gauranteed to be lowercase, words
    // from texts are not. So, we lowercase the word before hashing it.
    char loweredWord[LENGTH + 1]; // create a non-const space for the word
    strcpy(loweredWord, word);    // copy the const word to the non-const space

    for (int i = 0; loweredWord[i] != '\0'; i++) // lowercase all letters
    {
        // tolower() returns an int, must be cast to a char
        loweredWord[i] = (char) tolower(loweredWord[i]);
    }

    int index = hash(loweredWord); // find the word's index

    // create a cursor, traversing from the head of the hash's LL to the end
    for (node *cursor = table[index]; cursor != NULL; cursor = cursor->next)
    {
        if (strcmp(cursor->word, loweredWord) == 0)
        {
            return true; // return true if the word exists in our dictionary
        }
    }
    return false;
}


// Loads dictionary into memory.  Returns true if successful else false.
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r"); // load the dictionary file
    if (file == NULL)
    {
        printf("FILE == NULL\n");
        return false;
    }

    char buffer[LENGTH + 1]; // 1 correctly-spelled word, via dictionary file

    while (fscanf(file, "%s", buffer) != EOF)   // Scan through the dict file
    {

        node *dictEntry = malloc(sizeof(node)); // make a node for a dict entry
        if (dictEntry == NULL)
        {
            printf("MALLOC FAIL\n");
            fclose(file);
            return 1;
        }

        strcpy(dictEntry->word, buffer);   // put the dict word in the node
        int index = hash(dictEntry->word); // get the index by hashing the word

        dictEntry->next = table[index];    // save the LL chain in the new node
        table[index] = dictEntry;          // insert the node into the LL

        dictEntries++;                     // count the words in the dictionary
    }
    fclose(file);
    return true;
}


// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return dictEntries; // int dictEntries is global, incremented in load()
}


// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int index = 0; index < N; index++) // for each bucket's LL...
    {
        node *cursor = table[index]; // make cursor to traverse the LLs
        while (cursor != NULL)       // while not at the end of the LL...
        {
            node *temp = cursor;     // get your LL node's location
            cursor = cursor->next;   // save the rest of the chain
            free(temp);              // free your node
        }
    }
    return true;
}