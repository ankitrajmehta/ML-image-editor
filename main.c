#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ml.h"

void apply(int height, int width, RGBTRIPLE image[height][width],float w_blue, float b_blue,float w_red, float b_red,float w_green, float b_green)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = image[i][j].rgbtBlue* w_blue + b_blue;
            image[i][j].rgbtRed = image[i][j].rgbtRed * w_red + b_red;
            image[i][j].rgbtGreen =  image[i][j].rgbtGreen * w_green + b_green;
            image[i][j].rgbtBlue = fmin(image[i][j].rgbtBlue, 255);
            image[i][j].rgbtBlue = fmax(image[i][j].rgbtBlue, 0);
            image[i][j].rgbtRed = fmax(image[i][j].rgbtRed,0);
            image[i][j].rgbtRed = fmin(image[i][j].rgbtRed,255);
            image[i][j].rgbtGreen =fmax(image[i][j].rgbtGreen,0);
            image[i][j].rgbtGreen =fmin(image[i][j].rgbtGreen,255);

        }
    }


}

int main()
{
    
    // Remember filenames
    char *infile = "aaa.bmp";
    char *outfile = "omaxmin.bmp";
    char *label_file="reded.bmp";

    // Open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 4;
    }    
    FILE *label = fopen(label_file, "r");
    if (label == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 4;
    }

    // Open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        printf("Could not create %s.\n", outfile);
        return 5;
    }

    // Read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // Read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // Ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        printf("Unsupported file format.\n");
        return 6;
    }

    // Get image's dimensions
    int height = abs(bi.biHeight);
    int width = bi.biWidth;

    // Allocate memory for image
    RGBTRIPLE(*image)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (image == NULL)
    {
        printf("Not enough memory to store image.\n");
        fclose(outptr);
        fclose(label);
        fclose(inptr);
        return 7;
    }

    // Determine padding for scanlines
    int padding = (4 - (width * sizeof(RGBTRIPLE)) % 4) % 4;

    // Iterate over infile's scanlines
    for (int i = 0; i < height; i++)
    {
        // Read row into pixel array
        fread(image[i], sizeof(RGBTRIPLE), width, inptr);

        // Skip over padding
        fseek(inptr, padding, SEEK_CUR);
    }
    // Allocate memory for edited image
    RGBTRIPLE(*edited_image)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (edited_image == NULL)
    {
        printf("Not enough memory to store image.\n");
        fclose(outptr);
        fclose(label);
        fclose(inptr);
        return 7;
    }
    // Iterate over infile's scanlines
    for (int i = 0; i < height; i++)
    {
        //Read row into pixel array
        //fread(edited_image[i], sizeof(RGBTRIPLE), width, label);

       // Skip over padding
        //fseek(label, padding, SEEK_CUR);
        for (int j =0; j<width;j++)
         {
               int b =ceil(image[i][j].rgbtBlue*.3);
               int c = ceil(image[i][j].rgbtRed*5);
               int d =ceil(image[i][j].rgbtGreen*.5);


            b= fmin(b, 255);
            b = fmax(b, 0);
            c = fmax(c,0);
            c = fmin(c,255);
            d =fmax(d,0);
            d =fmin(d,255);

               edited_image[i][j].rgbtBlue = b;
               edited_image[i][j].rgbtRed = c;
               edited_image[i][j].rgbtGreen= d;
               
         }
    }
    // Filter image
    //for blue
    int train[height*width][2];
    int c = 0;
    for (int i = 0; i <height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            train[c][0]=fmin(fmax(image[i][j].rgbtBlue,0),255);
            train[c][1]=fmin(ceil(image[i][j].rgbtBlue*.3),255);
            c++;

        }
    }

    float* wb = linear_regression_model(c, train);
    float w_blue = wb[0];
    float b_blue=wb[1];
    free(wb);

    //for red
    c = 0;
    for (int i = 0; i <height; i++)
    {
        for (int j = 0; j < width; j++)
        {            
            train[c][0]=fmin(fmax(image[i][j].rgbtRed,0),255);
            train[c][1]=fmin(fmax(edited_image[i][j].rgbtRed,0),255);
            c++;
        }
    }

    float* wr = linear_regression_model(c, train);
    float w_red = wr[0];
    float b_red=wr[1];
    free(wr);

    //for green
    c = 0;
    for (int i = 0; i <height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            train[c][0]=fmin(fmax(image[i][j].rgbtGreen,0),255);
            train[c][1]=fmin(fmax(ceil(image[i][j].rgbtGreen*.5),0),255);
            c++;
        }
    }


    float* wg = linear_regression_model( c, train);
    float w_green = wg[0];
    float b_green=wg[1];
    free(wg);
    printf("w_blue: %f b_blue: %f, w_r: %f b_r: %f, w_g: %f b_g: %f", w_blue, b_blue, w_red, b_red, w_green, b_green);
   
    apply(height, width, image, w_blue, b_blue, w_red, b_red, w_green, b_green);

    // Write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // Write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // Write new pixels to outfile
    for (int i = 0; i < height; i++)
    {
        // Write row to outfile
        fwrite(image[i], sizeof(RGBTRIPLE), width, outptr);

        // Write padding at end of row
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
    }

    // Free memory for image
    free(image);

    // Close files
    fclose(inptr);
    fclose(label);
    fclose(outptr);
    return 0;
}
