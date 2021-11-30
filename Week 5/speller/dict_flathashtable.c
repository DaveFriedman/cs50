// Implements a dictionary's functionality
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include "dictionary.h"
#include <cs50.h>

const unsigned int BUCKETS = 10000000;
char hashTable[BUCKETS][LENGTH + 1];
int numWords = 0;
uint32_t fnv1a(unsigned char oneByte, uint32_t hash);
unsigned int hash(const char *word);


// FNV1a hash, via https://create.stephan-brumme.com/fnv-hash/
const uint32_t prime = 0x01000193; //   16777619
const uint32_t seed  = 0x811C9DC5; // 2166136261
// "inline" for compiler optimizations,
// https://en.cppreference.com/w/c/language/inline
uint32_t fnv1a(unsigned char oneByte, uint32_t hash)
{
    return (oneByte ^ hash) * prime;
}

// Hashes word to a number
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
    return hash;
}

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    /*char newWord[LENGTH+1];
    strcpy(newWord, word);
    //printf("word: %s {} newWord: %s\n", word, newWord);

    int i = 0;
    while(newWord[i] != '\0')
    {
        if(islower(newWord[i])==false || newWord[i]=='\'')
        {
        newWord[i] = tolower(newWord[i]);
        }
        i++;
    }*/

    int index = hash(word) % BUCKETS;
    //printf("word: %s, hashtable[%i]: %s\n", newWord, index, hashTable[index]);
    if (strcmp(&hashTable[index][0], word) == 0)
    {
        //printf("dict word %s == txt word %s\n", &hashTable[index][0], word);
        return true;
    }
    else
    {
        printf("txt word [%s] has hash %i. dict word at that hash is [%s] ", word, index, &hashTable[index][0]);
        printf("txt word %s != dict word %s\n", word, &hashTable[index][0]);

    }
    return false;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf("FILE == NULL\n");
        return false;
    }

    char buffer[LENGTH + 1]; // 1 correctly-spelled word, via dictionary file
    int i = 0;

    while (fscanf(file, "%s", buffer) != EOF)
    {

        int index = hash(buffer) % BUCKETS;

        //printf("The word %s has index %i\n", buffer, index);
        strcpy(&hashTable[index][0], buffer);

        numWords++;

        /*
        printf("num words: %i\n", numWords);
        printf("index: %i\n", index);
        for(int a = 0; hashTable[index][a] != '\0'; a++)
        {
            printf("%c", hashTable[index][a]);
        }
        printf("\n");
        */

    }
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return numWords;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    return true;
}
