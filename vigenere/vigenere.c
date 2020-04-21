// Program to encrypt messages using Vigenere's cipher. Usage: ./vingenere keyword

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int cypher(int plain, int key);

int main(int argc, string argv[])
{
    
    // Check to make sure the keyword is entered
    if (argc == 2)
    {
        // Check to make sure there are no numbers in the keyword
        int wrong = 0;
        int keylength = strlen(argv[1]);

        for (int i = 0, j = 0; i < keylength; i++)
        {
            j = isdigit(argv[1][i]);
            if (j)
            {
                wrong = 1;
                // Exit if numbers in keyword
                printf("Usage: ./vigenere keyword\n");
                return 1;
            }
        }
        // Continue if keyword valid
        if (wrong == 0) 
        {
            
            // Ask user for plaintext
            string plaintext = get_string("plaintext: ");
            
            // Start Output
            printf("ciphertext: ");
            
            // Do cypher shift loop for each letter
            int plainlength = strlen(plaintext);
            for (int i = 0, j = 0; i < plainlength; i++)
            {
                // Cypher the letter if lower case
                if (isalpha(plaintext[i]) && islower(plaintext[i]))
                {
                    int k = cypher(plaintext[i], argv[1][j]);
                    printf("%c", k);
                    j++;
                }
                // Cypher the letter if upper case
                else if (isalpha(plaintext[i]) && isupper(plaintext[i]))
                {
                    int k = cypher(plaintext[i], argv[1][j]);                    
                    printf("%c", k);                        
                    j++;
                }
                // Pass the character through if it is not a letter
                else
                {
                    printf("%c", plaintext[i]);    
                }
                
                // Reset key counter when you reach the end of the key
                if (j == keylength) 
                {
                    j = 0;
                }
            }
            // Print final newline after cyphered text
            printf("\n");
        }
    }
    // Exit if the user fails to enter a properly formated keyword
    else
    {
        printf("Usage: ./vigenere keyword\n");
        return 1;
    }   
}

int cypher(int plain, int key)
{
    int k;
    // Check to see if key is lower or upper to perform appropriate math
    if (islower(key))
    {
        k = key - 97 + plain;
    }
    else
    {
        k = key - 65 + plain;
    }
    // Check to see if the letter need to wrap backwards if lowercase
    if (islower(key))
    {
        if (k > 122)
        {
            k -= 26;
        } 
    }
    // Check if need to wrap if uppercase
    else
    {
        if (k > 90)
        {
            k -= 26;  
        }  
    }
    return k;
}
