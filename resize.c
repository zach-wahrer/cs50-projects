// Resizes BMP Images

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{

    // Set factor size
    const int n = atoi(argv[1]);
    // ensure proper usage
    if (argc != 4 || n <= 0 || n > 100)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }


    // determine padding for in scanlines
    int ipadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;


    // Make an array for the ouput file headers and make changes
    BITMAPFILEHEADER bfo = bf;
    BITMAPINFOHEADER bio = bi;
    bio.biWidth *= n;
    bio.biHeight *= n;
    int opadding = (4 - (bio.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;     // determine padding for in scanlines
    bio.biSizeImage = ((sizeof(RGBTRIPLE) * bio.biWidth) + opadding) * abs(bio.biHeight);
    bfo.bfSize = bio.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bfo, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bio, sizeof(BITMAPINFOHEADER), 1, outptr);



    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // Loop to *n for height
        for (int h = 0; h < n; h++)
        {
            // iterate over pixels in scanline
            for (int j = 0; j < bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // Loop to *n for width
                for (int k = 0; k < n; k++)
                {
                    // write RGB triple to outfile
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }

            // add opadding, if needed
            for (int k = 0; k < opadding; k++)
            {
                fputc(0x00, outptr);
            }

            // skip over ipadding, if any
            fseek(inptr, ipadding, SEEK_CUR);

            // Return to beginning of current input line
            fseek(inptr, -(bi.biWidth * sizeof(RGBTRIPLE) + ipadding), SEEK_CUR);
        }
        // Advance to next row
        fseek(inptr, (bi.biWidth * sizeof(RGBTRIPLE) + ipadding), SEEK_CUR);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
