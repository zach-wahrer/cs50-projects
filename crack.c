// A program to brute force crack 1-5 digit alphabetic (lower and uppercase) passwords, when given a hash

#include <cs50.h>
#include <stdio.h>
#include <crypt.h>
#include <string.h>

int main(int argc, string argv[])
{
    // Exit if user doesn't enter hash
    if (argc != 2)
    {
        printf("Usage: ./crack hash\n");
        return 1;
    }
    
    // Initialize key variables
    char* hash = argv[1];
    char testkey[6];
    char* alpha = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int length = strlen(alpha);
    
    // Pull the first two hash characters for the salt
    const char salt[2] = {argv[1][0], argv[1][1]};


    // CHECK FOR ONE LENGTH
    // Check first place
    for (int first = 0; first < length; first++)
    {
        // Create test key
        testkey[0] = alpha[first];
        //printf("testkey: %s\n", testkey);
        // Test the key
        if (strcmp(crypt(testkey, salt), hash) == 0)
        {
            printf("%s\n", testkey);
            return 0;
        }
    }

    // CHECK FOR TWO LENGTH
    // Check first place
    for (int first = 0; first < length; first++)
    {
        // Create test key
        testkey[0] = alpha[first];
        //printf("testkey: %s\n", testkey);
        // Test the key
        if (strcmp(crypt(testkey, salt), hash) == 0)
        {
            printf("%s\n", testkey);
            return 0;
        }
        
        // Check second place
        for (int second = 0; second < length; second++)
        {
            testkey[1] = alpha[second];
            //printf("testkey: %s\n", testkey);
            if (strcmp(crypt(testkey, salt), hash) == 0)
            {
                printf ("%s\n", testkey);
                return 0;
            }
        }
    }   

    // CHECK FOR THREE LENGTH
    // Check first place
    for (int first = 0; first < length; first++)
    {
        // Create test key
        testkey[0] = alpha[first];
        //printf("testkey: %s\n", testkey);
        // Test the key
        if (strcmp(crypt(testkey, salt), hash) == 0)
        {
            printf("%s\n", testkey);
            return 0;
        }
        
        // Check second place
        for (int second = 0; second < length; second++)
        {
            testkey[1] = alpha[second];
            //printf("testkey: %s\n", testkey);
            if (strcmp(crypt(testkey, salt), hash) == 0)
            {
                printf ("%s\n", testkey);
                return 0;
            }
                
            // Check third place
            for (int third = 0; third < length; third++)
            {
                testkey[2] = alpha[third];
                //printf("testkey: %s\n", testkey);
                if (strcmp(crypt(testkey, salt), hash) == 0)
                {
                    printf ("%s\n", testkey);
                    return 0;
                }
            }
        }
    }

    // CHECK FOR FOUR LENGTH
    // Check first place
    for (int first = 0; first < length; first++)
    {
        // Create test key
        testkey[0] = alpha[first];
        //printf("testkey: %s\n", testkey);
        // Test the key
        if (strcmp(crypt(testkey, salt), hash) == 0)
        {
            printf("%s\n", testkey);
            return 0;
        }
        
        // Check second place
        for (int second = 0; second < length; second++)
        {
            testkey[1] = alpha[second];
            //printf("testkey: %s\n", testkey);
            if (strcmp(crypt(testkey, salt), hash) == 0)
            {
                printf ("%s\n", testkey);
                return 0;
            }
                
            // Check third place
            for (int third = 0; third < length; third++)
            {
                testkey[2] = alpha[third];
                //printf("testkey: %s\n", testkey);
                if (strcmp(crypt(testkey, salt), hash) == 0)
                {
                    printf ("%s\n", testkey);
                    return 0;
                }
                
                // Check fourth place
                for (int fourth = 0; fourth < length; fourth++)
                {
                    testkey[3] = alpha[fourth];
                    //printf("testkey: %s\n", testkey);
                    if (strcmp(crypt(testkey, salt), hash) == 0)
                    {
                        printf ("%s\n", testkey);
                        return 0;
                    }
                }
            }
        }
    }

    // CHECK FOR FIVE LENGTH
    // Check first place
    for (int first = 0; first < length; first++)
    {
        // Create test key
        testkey[0] = alpha[first];
        //printf("testkey: %s\n", testkey);
        // Test the key
        if (strcmp(crypt(testkey, salt), hash) == 0)
        {
            printf("%s\n", testkey);
            return 0;
        }
        
        // Check second place
        for (int second = 0; second < length; second++)
        {
            testkey[1] = alpha[second];
            //printf("testkey: %s\n", testkey);
            if (strcmp(crypt(testkey, salt), hash) == 0)
            {
                printf ("%s\n", testkey);
                return 0;
            }
                
            // Check third place
            for (int third = 0; third < length; third++)
            {
                testkey[2] = alpha[third];
               // printf("testkey: %s\n", testkey);
                if (strcmp(crypt(testkey, salt), hash) == 0)
                {
                    printf ("%s\n", testkey);
                    return 0;
                }
                
                // Check fourth place
                for (int fourth = 0; fourth < length; fourth++)
                {
                    testkey[3] = alpha[fourth];
                    //printf("testkey: %s\n", testkey);
                    if (strcmp(crypt(testkey, salt), hash) == 0)
                    {
                        printf ("%s\n", testkey);
                        return 0;
                    }
                    
                    // Check fifth place
                    for (int fifth = 0; fifth < length; fifth++)
                    {
                        testkey[4] = alpha[fifth];
                        //printf("testkey: %s\n", testkey);
                        if (strcmp(crypt(testkey, salt), hash) == 0)
                        {
                            printf ("%s\n", testkey);
                            return 0;
                        }
                    } // Close of fifth
                } // Close of forth                
            } // Close of third              
        } // Close of second      
    } // Close of first  
}
