#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
     // Get card number
    long card = 0;
    while (card < 1)
    {
        card = get_long("Number: ");
    }
    int length = floor( log10(card) ) + 1;

    // Lhun's algorithm
    int luhn = 0;
    for (int i = 0; i < length; i++)
    {
        long mod = card % 10;
        //printf("card: %li, mod: %li\n", card, mod);
        card /= 10;

        if (i%2 == 0) // 1s, 100s, 10000s ...
        {
            printf("odd digit, mod: %li\n", mod);
            luhn += mod;
        }
        else // 10s, 1000s, 10000s ...
        {
            printf("even digit, mod*2: %li\n", (mod*2));
            if (mod*2 > 9)
            {
                luhn += mod*2 %10 + 1;
                printf("double digit, sum is: %li\n", (mod%10));
            }
            else
            {
                luhn += mod*2;
            }
        }
    }
    if (luhn%10 == 0)
    {
        printf("VALID\n");
    }
    else
    {
        printf("INVALID\n");
    }
    printf("luhn %i, check: %i, length: %i\n", luhn, luhn%10, length);

}