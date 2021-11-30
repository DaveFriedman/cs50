#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // TODO: Prompt for start size
    int start = 0;
    while (start < 9)
    {
        start = get_int("Population start size: ");
        if (start < 9)
        {
            printf("Sorry, population too small!\n");
        }
    }

    // TODO: Prompt for end size
    int end;
    bool valid = false;
    while (valid == false)
    {
        end = get_int("Population end size: ");
        if (end > 2147483647) // greater than max INT
        {
            printf("Sorry, population too large!\n");
            valid = false;
        }
        else if (end < start)
        {
            printf("Sorry, population can't decrease!\n");
            valid = false;
        }
        else
        {
            valid = true;
        }
    }


    // TODO: Calculate number of years until we reach threshold
    float pop = (float) start;
    int generation = 0;
    while (end > (int) pop)
    {
        float born = pop / 3.0;
        float died = pop / 4.0;
        pop = pop + (int) born - (int) died;
        generation += 1;
        // printf("generation: %d, population %d.\n", generation, (int) pop); // Test
    };


    // TODO: Print number of years
    // printf("The final population was %d.\n", (int) pop);
    // printf("It took %d years to hit our population end size.\n", generation);
    printf("Years: %d\n", generation);
}