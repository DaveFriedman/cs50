#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int h = 0;
    while (h < 1 || h > 8)
    {
        h = get_int("Height: ");
        // printf("Height is: %i\n", h); // Test
    }

    for (int y = 0; y < h; y++)
    {
        // Left-aligned triangle
        for (int x = 0; x < h; x++)
        {
            if (h - x > y + 1)
            {
                printf(" ");
            }
            else
            {
                printf("#");
            }
        }

        printf("  ");

        // Right-aligned triangle
        for (int x = 0; x <= y; x++)
        {
            printf("#");
        }
        printf("\n");
    }
}