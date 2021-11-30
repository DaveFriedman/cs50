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
    int length = floor(log10(card)) + 1;

    // Lhun's algorithm
    int luhn = 0;
    long c = card;
    for (int i = 0; i < length; i++)
    {
        long mod = c % 10;
        c /= 10;
        if (i % 2 == 0) // 1s, 100s, 10000s ...
        {
            luhn += mod;
        }
        else // 10s, 1000s, 10000s ...
        {
            if (mod * 2 > 9)
            {
                luhn += mod * 2 % 10 + 1;
            }
            else
            {
                luhn += mod * 2;
            }
        }
    }

    // MASTERCARD: 16 digits, starts with 51, 52, 53, 54, or 55
    // VISA: 16 digits, starts with 4
    if (luhn % 10 == 0 && length == 16)
    {
        if (floor((float) card / pow(10, 14)) >= 51 && floor((float) card / pow(10, 14)) <= 55)
        {
            printf("MASTERCARD\n");
        }
        else if (floor((float) card / pow(10, 15)) == 4)
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }

    // AMEX: 15 digits, starts with 34 or 37
    else if (luhn % 10 == 0 && length == 15)
    {
        if (floor((float) card / pow(10, 13)) == 34 || floor((float) card / pow(10, 13)) == 37)
        {
            printf("AMEX\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    // VISA: 13 or 16 digits, starts with 4
    else if (luhn % 10 == 0 && length == 13)
    {
        if (floor((float) card / pow(10, 12)) == 4)
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}