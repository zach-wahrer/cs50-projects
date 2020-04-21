#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Ask user for height of blocks
    int height = get_int("Height:");
    
    // Check input
    while (height < 1 || height > 8)
    {
        height = get_int("Height: ");
    }
    
    // Set up some vars
    string block = "#";
    string space = " ";
    int blockcount = 1;
    int rowcount = 1;
    int spacecount = 1;
    
    // This 'while' does each row
    while (rowcount <= height)
    {
        // Print intial spaces to make everything align
        int spaces = height - rowcount;
        while (spaces >= spacecount)
        {
            printf("%s", space);
            spacecount++;
        }
        
        // This 'while' sets up the proper blocks beginning for each row
        while (blockcount <= rowcount)
        {
            printf("%s", block);
            blockcount++;
        }
        
        // Print middle spaces, reset the counter
        printf("  ");
        blockcount = 1;
        
        // This 'while' sets up the proper blocks for end of each row
        while (blockcount <= rowcount)
        {
            printf("%s", block);
            blockcount++;
        }

        // Print ending new line
        printf("\n");
        
        // Reset some vars and increment the counter for the next loop
        blockcount = 1;
        spacecount = 1;
        rowcount++;
    }
    
}
