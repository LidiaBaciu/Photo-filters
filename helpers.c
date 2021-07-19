#include "helpers.h"
#include <stdio.h>
#include <math.h>
#include <stdbool.h>


// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for(int row = 0; row < height; row++)
    {
        for(int column = 0; column < width; column++)
        {
            RGBTRIPLE pixel = image[row][column];
            int average = round((pixel.rgbtRed + pixel.rgbtGreen + pixel.rgbtBlue) / 3.0);
            image[row][column].rgbtBlue = average;
            image[row][column].rgbtGreen = average;
            image[row][column].rgbtRed = average;
        }
    }
}

void swap(RGBTRIPLE* pixel1, RGBTRIPLE* pixel2)
{
    RGBTRIPLE temp = *pixel1;
    *pixel1 = *pixel2;
    *pixel2 = temp;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for(int row = 0; row < height; row++)
    {
        for (int column = 0; column < width / 2; column++)
        {
            swap(&image[row][column], &image[row][width - 1 - column]);
        }
    }
}

bool is_valid_pixel(int i, int j, int height, int width)
{
    return i >= 0 && i < height && j >= 0 && j < width;
}

RGBTRIPLE get_blurred_pixel(int i, int j, int height, int width, RGBTRIPLE image[height][width])
{
    int red_value = 0;
    int blue_value = 0;
    int green_value = 0;

    /*
    TODO: is this a better way of declaring the variables?
    int red_value, blue_value, green_value; red_value = blue_value = green_value = 0;
    */

    int numValidPixels = 0;

    for(int di = -1; di <= 1; di++)
    {
        for(int dj = -1; dj <= 1; dj++)
        {
            int new_i = i + di;
            int new_j = j + dj;
            if(is_valid_pixel(new_i, new_j, height, width))
            {
                numValidPixels++;
                red_value += image[new_i][new_j].rgbtRed;
                blue_value += image[new_i][new_j].rgbtBlue;
                green_value += image[new_i][new_j].rgbtGreen;
            }
        }
    }

    RGBTRIPLE blurred_pixel;
    blurred_pixel.rgbtRed = round((float) red_value / numValidPixels);
    blurred_pixel.rgbtGreen = round((float) green_value / numValidPixels);
    blurred_pixel.rgbtBlue = round((float) blue_value / numValidPixels);
    return blurred_pixel;
}

int cap(int value)
{
    return value < 255 ? value: 255;
}

RGBTRIPLE get_edged_pixel(int i, int j, int height, int width, RGBTRIPLE image[height][width])
{
    int Gx[3][3] = {{ -1, 0, 1 }, {-2, 0, 2}, {-1, 0, 1}};
    int red_valueX = 0;
    int blue_valueX = 0;
    int green_valueX = 0;
    int red_valueY = 0;
    int blue_valueY = 0;
    int green_valueY = 0;

    for(int di = -1; di <= 1; di++)
    {
        for(int dj = -1; dj <= 1; dj++)
        {
            int new_i = i + di;
            int new_j = j + dj;
            if(is_valid_pixel(new_i, new_j, height, width))
            {
                int weightX = Gx[di + 1][dj + 1];
                red_valueX += weightX * image[new_i][new_j].rgbtRed;
                green_valueX += weightX * image[new_i][new_j].rgbtGreen;
                blue_valueX += weightX * image[new_i][new_j].rgbtBlue;

                int weightY = Gx[dj + 1][di + 1];
                red_valueY += weightY * image[new_i][new_j].rgbtRed;
                green_valueY += weightY * image[new_i][new_j].rgbtGreen;
                blue_valueY += weightY * image[new_i][new_j].rgbtBlue;

            }
        }
    }
    RGBTRIPLE pixel;
    pixel.rgbtRed = cap(round(sqrt(red_valueX * red_valueX + red_valueY * red_valueY)));
    pixel.rgbtGreen = cap(round(sqrt(green_valueX * green_valueX + green_valueY * green_valueY)));
    pixel.rgbtBlue = cap(round(sqrt(blue_valueX * blue_valueX + blue_valueY * blue_valueY)));
    return pixel;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE new_image[height][width];
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            RGBTRIPLE pixel = get_blurred_pixel(i, j, height, width, image);
            new_image[i][j] = pixel;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = new_image[i][j];
        }
    }
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE new_image[height][width];
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            RGBTRIPLE pixel = get_edged_pixel(i, j, height, width, image);
            new_image[i][j] = pixel;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = new_image[i][j];
        }
    }
}

