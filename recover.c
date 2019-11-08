// Program to recover lost JPGs from RAW files

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Check for correct input
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover infile\n");
        return 1;
    }

    // Get file name
    char *infile = argv[1];

    // Open input file and exit if unsuccessful
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Input file unavailable\n");
        return 2;
    }

    // Set up read buffer
    unsigned char buffer[512];

    // Read the first block from infile to buffer
    int eof = fread(&buffer, 512, 1, inptr);

    // Exit if it isn't a full block
    if (eof == 0)
    {
        fprintf(stderr, "File too small to contain JPG\n");
        return 2;
    }

    // Set initial variables
    char filename[] = "999.jpg";
    int write = 0;
    int firstmatch = 0;
    int i = 0;

    // Read file until EOF
    for (eof = 1; eof != 0; i++)
    {
        // Create file name
        if (write == 1)
        {
            sprintf(filename, "%03i.jpg", i);
        }
        // Open recovery file
        FILE *outptr = fopen(filename, "w");
        // Exit if it cannot be created
        if (outptr == NULL)
        {
            fprintf(stderr, "Cannot create output file\n");
            return 2;
        }

        // Write buffer to file if flag is set and then advance for next loop
        if (write == 1)
        {

            fwrite(&buffer, 512, 1, outptr);
            write = 0;
            eof = fread(&buffer, 512, 1, inptr);
        }

        // Run until we need a new file
        while (eof != 0)
        {
            // Check the signature area for a JPG header
            if (buffer[0] == 0xff &&
                buffer[1] == 0xd8 &&
                buffer[2] == 0xff &&
                (buffer[3] & 0xf0) == 0xe0)
            {
                // Close current file, break out, and write data
                if  (firstmatch == 1) // if this isn't the first time, close file
                {
                    fclose(outptr);
                }
                write = 1;
                firstmatch = 1;
                break;
            }
            // Run if not a header
            else
            {
                // Check to see if we've found a header in the past
                if (firstmatch == 1)
                {
                    // Write buffer to new file
                    fwrite(&buffer, 512, 1, outptr);
                    // Advance the read
                    eof = fread(&buffer, 512, 1, inptr);
                }
                // If none found yet, skip ahead
                else
                {
                    // Advance the read
                    eof = fread(&buffer, 512, 1, inptr);
                }
            }

        }

        // Close final output file when EOF reached
        if (eof == 0)
        {
            fclose(outptr);
            break;
        }

    }

    // Close input file
    fclose(inptr);

    // Erase initial garbage file
    remove("./999.jpg");

    // Successful exit
    return 0;
}
