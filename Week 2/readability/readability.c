#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

// Returns the Coleman-Liau reading level index
float cl(string s)
{
    int n = strlen(s);
    int num_letters = n + 1;
    int num_words = 1;
    int num_sentences = 0;

    for (int i = 0; i <= n; i++)
    {
        if (s[i] < 'A' || s[i] > 'z' || (s[i] > 'Z' && s[i] < 'a'))
        {
            num_letters -= 1;
        }
        if (s[i] == ' ')
        {
            num_words += 1;
        }
        if (s[i] == '?' || (s[i] == '!' || s[i] == '.'))
        {
            num_sentences += 1;
        }
    }

    float coleman_liau = 0.0588 * (num_letters * 100.0 / num_words) - 0.296 *
        (num_sentences * 100.0 / num_words) - 15.8;

    return coleman_liau;
}

int main(void)
{
    string s = get_string("Text: ");

    float coleman_liau = cl(s);

    if (coleman_liau < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (coleman_liau >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %li\n", lroundf(coleman_liau)); // lroundf: round a float & convert to a li
    }
}