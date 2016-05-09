/**
* resize.c
* Charles Williams
* lose311@aol.com
*
* Program that resizes a 24-bit uncompressed bitmap by a factor of n.
* n must be between 1 and 100, inclusive. If n=1 the bitmap is simply copied.
*
* Usage:
* ./resize n infile outfile
*
* where 'n' is resize factor, 'infile' is file to be transformed
* and 'outfile' is output file name. 
*/
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./resize n infile outfile\n");
        return 1;
    }

    // remember filenames
    int n = atoi(argv[1]);
    char* infile = argv[2];
    char* outfile = argv[3];

    // check for proper n value
    if (n < 1 || n > 100)
    {
        printf("Usage: n must be between 1 and 100\n");
        return 1;
    }

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER oldbf, newbf;
    fread(&oldbf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER oldbi, newbi;
    fread(&oldbi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (oldbf.bfType != 0x4d42 || oldbf.bfOffBits != 54 || oldbi.biSize != 40 || 
        oldbi.biBitCount != 24 || oldbi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // determine padding and header info for new bitmap
    newbi = oldbi;
    newbf = oldbf;
    newbi.biWidth = oldbi.biWidth * n;
    newbi.biHeight = oldbi.biHeight * n;
    int oldpadding = (4 - (oldbi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int newpadding = (4 - (newbi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    newbi.biSizeImage = ((newbi.biWidth * sizeof(RGBTRIPLE) + newpadding)) * abs(newbi.biHeight);
    newbf.bfSize = 54 + newbi.biSizeImage;

    // write outfile's BITMAPFILEHEADER
    fwrite(&newbf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&newbi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over outfile's scanlines
    for (int h = 0, biHeight = abs(oldbi.biHeight); h < biHeight; h++)
    {
        // repeat each scanline n times
        for (int i = 0; i < n; i++)
        {
            // iterate over pixels in scanline
            for (int j = 0; j < oldbi.biWidth; j++)
            {
                RGBTRIPLE triple;
    
                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
    
                // write RGB triple to outfile n times
                for (int k = 0; k < n; k++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }
            
            // insert new padding at end of each scanline
            for (int l = 0; l < newpadding; l++)
                {
                    fputc(0x00, outptr);
                } 
                
            // seek to beginning of line if repeating    
            if (i < n - 1)
                fseek(inptr, -(sizeof(RGBTRIPLE)*oldbi.biWidth), SEEK_CUR);  
        }
        // skip padding and skip to next line
        fseek(inptr, oldpadding, SEEK_CUR);  
    }         

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
