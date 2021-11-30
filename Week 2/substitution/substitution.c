#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void substitution(string key, string plaintext)
{
    char abc[] = "abcdefghijklmnopqrstuvwxyz";
    int cipher[26];
    int n = strlen(plaintext);
    char ciphertext[n];
    ciphertext[n] = '\0'; // String must end with Null

    for (int i = 0; i < 26; i++)
    {
        cipher[i] = tolower(key[i]) - abc[i];
    }

    for (int i = 0; i < n; i++)
    {
        if (isalpha(plaintext[i]) == 0) // Don't change non-letter chars
        {
            ciphertext[i] = plaintext[i];
        }
        else
        {
            for (int j = 0; j < 26; j++)
            {
                if (tolower(plaintext[i]) == abc[j])
                {
                    ciphertext[i] = plaintext[i] + cipher[j];
                }
            }
        }
    }

    printf("ciphertext: %s\n", ciphertext);
}

int main(int argc, string argv[])
{
    string key = argv[1];

    if (argc != 2 || strlen(key) != 26) // check for key & key's correct length
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    for (int i = 0; i != 26; i++) // check key for every letter, occurring once
    {
        if (isalpha(key[i]) == 0)
        {
            printf("Key must be alphabetic.\n");
            return 1;
        }

        int duplicates = 0;
        for (int j = 0; j < 26; j++)
        {
            if (tolower(key[i]) == tolower(key[j]))
            {
                duplicates += 1;
                if (duplicates > 1)
                {
                    printf("Key must not contain repeated letters.\n");
                    return 1;
                }
            }
        }
    }

    string plaintext = get_string("plaintext: ");
    substitution(key, plaintext);

    return 0;
}