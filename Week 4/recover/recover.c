#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *img = NULL;
    BYTE buffer[512];
    char filename[8];
    int i = 0;

    while (fread(buffer, sizeof(buffer), 1, input))
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 &&
            buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (i > 0)
            {
                fclose(img);
            }

            sprintf(filename, "%03d.jpg", i);
            img = fopen(filename, "w");
            fwrite(&buffer, sizeof(buffer), 1, img);
            i++;
        }
        else if (img != NULL)
        {
            fwrite(&buffer, sizeof(buffer), 1, img);
        }
    }

    fclose(input);
    fclose(img);
    return 0;
}