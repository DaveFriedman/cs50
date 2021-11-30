#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <cs50.h>

const unsigned int BUCKETS = 50000;
char hashTable[BUCKETS][6];
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
        hash = fnv1a((unsigned char)*word++, hash);
    }
    return hash;
}

int main()
{
    char* word1 = "hello";
    char* word2 = "hello";

    int hash1 = hash(word1);
    int hash2 = hash(word2);

    printf("word1: %s, hash: %i\n", word1, hash1);
    printf("word2: %s, hash: %i\n", word2, hash2);

    return 0;
}