/*
#include <stdio.h>

int main(void)
{
    printf("Hello, World!\n");
}
*/

#import <stdio.h>
#import <cs50.h>

int main(void)
{
    // Get a user's name, then welcome them!
    string name = get_string("What is your name?\n");
    printf("Hello, %s!\n", name);
}