#include "helpers.h"
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    float media_cores;
    
    // Cycle through each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Substitutes for the average of colors
            media_cores = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.00);
            
            image[i][j].rgbtBlue = media_cores;
            image[i][j].rgbtGreen = media_cores;
            image[i][j].rgbtRed = media_cores;
        }
    }
    
    return;
}

int limit(int color);
void swap(RGBTRIPLE *a, RGBTRIPLE *b);

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Width to be switched
    int tmp_width = width - 1;
    
    // Exchange the pixel from the beginning to the end 
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            swap(&image[i][j], &image[i][tmp_width - j]);
        }
    }
    
    return;
}

// Function to exchange values
void swap(RGBTRIPLE *a, RGBTRIPLE *b)
{
    RGBTRIPLE tmp = *a;
    *a = *b;
    *b = tmp;
}

int limit(int color)
{
    if (color > 255)
    {
        color = 255;
    }
    
    return color;
}

int lineStart, lineEnd, columnStart, columnEnd;

void blurMatrix(int i, int j, int height, int width);

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tmp_image[height][width];
    
    // Creates an equal temporary image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            tmp_image[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sumRed = 0, sumGreen = 0, sumBlue = 0;
            double count = 0.0;
            
            // Checks whether adjacent pixels are inside the image
            blurMatrix(i, j, height, width);
            
            // Traverses the matrix of each pixel and its adjacent
            for (int h = lineStart; h <= lineEnd; h++)
            {
                for (int w = columnStart; w <= columnEnd; w++)
                {
                    sumRed += tmp_image[h][w].rgbtRed;
                    sumGreen += tmp_image[h][w].rgbtGreen;
                    sumBlue += tmp_image[h][w].rgbtBlue;
                    
                    count++;
                }
            }
        
            // Stores the average of the pixel values and their adjacent
            image[i][j].rgbtRed = limit(round(sumRed / count)); 
            image[i][j].rgbtGreen = limit(round(sumGreen / count));
            image[i][j].rgbtBlue = limit(round(sumBlue / count));
        }
    }
    
    return;
}

void blurMatrix(int i, int j, int height, int width)
{
    if (i - 1 < 0)
    {
        lineStart = i;
    }
    else
    {
        lineStart = i - 1;
    }
    
    if (i + 1 >= height)
    {
        lineEnd = i;
    } 
    else
    {
        lineEnd = i + 1;
    }
    
    if (j - 1 < 0)
    {
        columnStart = j;
    } 
    else
    {
        columnStart = j - 1;
    }          
    
    if (j + 1 >= width)
    {
        columnEnd = j;
    } 
    else
    {
        columnEnd = j + 1;
    }
}


int gx(int h, int w, int height, int width, int color_value);
int gy(int h, int w, int height, int width, int color_value);

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tmp_image[height][width];
    
    // Creates an equal temporary image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            tmp_image[i][j] = image[i][j];
        }
    }

    // Cycle through each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int gxRed = 0, gxGreen = 0, gxBlue = 0, gyRed = 0, gyGreen = 0, gyBlue = 0;
            
            // Checks whether adjacent pixels are inside the image using the blurMatrix function
            blurMatrix(i, j, height, width);
            
            // Traverses the matrix of each pixel and its adjacent
            for (int h = lineStart; h <= lineEnd; h++)
            {
                for (int w = columnStart; w <= columnEnd; w++)
                {
                    // Sum the values of each corresponding gx and gy
                    gxRed += gx(h, w, i, j, tmp_image[h][w].rgbtRed);
                    gxGreen += gx(h, w, i, j, tmp_image[h][w].rgbtGreen); 
                    gxBlue += gx(h, w, i, j, tmp_image[h][w].rgbtBlue);
                    
                    gyRed += gy(h, w, i, j, tmp_image[h][w].rgbtRed);
                    gyGreen += gy(h, w, i, j, tmp_image[h][w].rgbtGreen); 
                    gyBlue += gy(h, w, i, j, tmp_image[h][w].rgbtBlue);
                }
            }
        
            // Uses the formula to calculate and modify the image
            image[i][j].rgbtRed = limit(round(sqrt(gxRed * gxRed + gyRed * gyRed))); 
            image[i][j].rgbtGreen = limit(round(sqrt(gxGreen * gxGreen + gyGreen * gyGreen)));
            image[i][j].rgbtBlue = limit(round(sqrt(gxBlue * gxBlue + gyBlue * gyBlue)));
        }
    }
    
    return;
}

// Multiplies by the corresponding value of the gx position
int gx(int h, int w, int height, int width, int color_value)
{
    int gx_value = 0;
    
    if (h == height - 1 || h == height + 1)
    {
        if (w == width - 1)
        {
            gx_value = -1 * color_value;
        }
        
        if (w == width + 1)
        {
            gx_value = 1 * color_value;
        }
    }
    
    if (h == height)
    {
        if (w == width - 1)
        {
            gx_value = -2 * color_value;
        }
        
        if (w == width + 1)
        {
            gx_value = 2 * color_value;
        }
    }
    
    return gx_value;
}

// Multiplies by the corresponding value of the gy position
int gy(int h, int w, int height, int width, int color_value)
{
    int gy_value = 0;
    
    if (h == height - 1)
    {
        if (w == width - 1 || w == width + 1)
        {
            gy_value = -1 * color_value;
        }
        if (w == width)
        {
            gy_value = -2 * color_value;
        }
    }
    
    if (h == height + 1)
    {
        if (w == width - 1 || w == width + 1)
        {
            gy_value = 1 * color_value;
        }
        if (w == width)
        {
            gy_value = 2 * color_value;
        }
    }
    
    return gy_value;
}