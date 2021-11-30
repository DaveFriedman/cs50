#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            int grey = round((image[h][w].rgbtRed +
                              image[h][w].rgbtGreen +
                              image[h][w].rgbtBlue) / 3.0);
            image[h][w].rgbtRed = image[h][w].rgbtGreen = image[h][w].rgbtBlue = grey;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE swap;

    for (int h = 0; h < height; h++)
    {
        // For each row of the image, up to (but not including) the median,
        // swap the RGBTRIPLEs (pixels) that are equidistant from the median
        for (int w = 0; w < floor(width / 2.0); w++)
        {
            swap = image[h][w];
            image[h][w] = image[h][(width-w-1)];
            image[h][(width-w-1)] = swap;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE blurred[height][width];
    int adjacentRed, adjacentGreen, adjacentBlue;
    float denominator;

    // For every pixel h by w ...
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            adjacentRed = adjacentGreen = adjacentBlue = denominator = 0;

            // then for every pixel adjacent to [h][w]...
            for (int x = -1; x <= 1; x++)
            {
                for (int y = -1; y <= 1; y++)
                {
                    // if [h][w] is a edge pixel, and the adjacent pixel
                    // is out of range...
                    if (h+x < 0 || h+x >= height || w+y < 0 || w+y >= width)
                    {
                        // then do nothing
                        continue;
                    }
                    else
                    {
                        // otherwise, get the average of each color
                        adjacentRed   += image[h+x][w+y].rgbtRed;
                        adjacentGreen += image[h+x][w+y].rgbtGreen;
                        adjacentBlue  += image[h+x][w+y].rgbtBlue;
                        denominator += 1;
                    }
                }
            }
            blurred[h][w].rgbtRed   = round(adjacentRed   / denominator);
            blurred[h][w].rgbtGreen = round(adjacentGreen / denominator);
            blurred[h][w].rgbtBlue  = round(adjacentBlue  / denominator);
        }
    }

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image[h][w] = blurred[h][w];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE edged[height][width];
    int edgedRedGx, edgedGreenGx, edgedBlueGx,
        edgedRedGy, edgedGreenGy, edgedBlueGy,
        dRed, dBlue, dGreen; // distance, red blue green

    int gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // For every pixel h by w...
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            edgedRedGx = edgedGreenGx = edgedBlueGx = 0;
            edgedRedGy = edgedGreenGy = edgedBlueGy = 0;

            // then, for every pixel adjacent to [h][w]...
            for (int x = -1; x <= 1; x++)
            {
                for (int y = -1; y <= 1; y++)
                {
                    // if [h][w] is a edge pixel, and the surrounding pixel
                    // is out of range...
                    if (h+x < 0 || h+x >= height || w+y < 0 || w+y >= width)
                    {
                        // then assume a black border; the colors are (0,0,0)
                        continue;
                    }
                    else
                    {
                        // otherwise, find the color distance by operating over
                        // the convolutional matrices, gx and gy
                        edgedRedGx   += (image[h+x][w+y].rgbtRed   * gx[x+1][y+1]);
                        edgedGreenGx += (image[h+x][w+y].rgbtGreen * gx[x+1][y+1]);
                        edgedBlueGx  += (image[h+x][w+y].rgbtBlue  * gx[x+1][y+1]);

                        edgedRedGy   += (image[h+x][w+y].rgbtRed   * gy[x+1][y+1]);
                        edgedGreenGy += (image[h+x][w+y].rgbtGreen * gy[x+1][y+1]);
                        edgedBlueGy  += (image[h+x][w+y].rgbtBlue  * gy[x+1][y+1]);
                    }
                }
            }

            // Compute the color distance between [h][w] and its adjacents.
            // The larger the distance, the higher the color values, making
            // edge pixels brighter, therefore highlighting the edges.
            dRed   = round(sqrt(edgedRedGx*edgedRedGx     + edgedRedGy*edgedRedGy));
            dGreen = round(sqrt(edgedGreenGx*edgedGreenGx + edgedGreenGy*edgedGreenGy));
            dBlue  = round(sqrt(edgedBlueGx*edgedBlueGx   + edgedBlueGy*edgedBlueGy));

            // No color value may be above 255
            edged[h][w].rgbtRed   = (dRed   > 255) ? 255 : dRed;
            edged[h][w].rgbtGreen = (dGreen > 255) ? 255 : dGreen;
            edged[h][w].rgbtBlue  = (dBlue  > 255) ? 255 : dBlue;
        }
    }

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image[h][w] = edged[h][w];
        }
    }
    return;
}