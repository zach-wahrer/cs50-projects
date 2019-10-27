#include <cs50.h>
#include <stdio.h>

// Function to do maths with CC digits
int add_digits(int digit);

int main(void) {
    
    // Get user input
    long i = get_long("Number: ");
    
    // Declare array
    long cc[16];
    
    // Seperate user input into the array, last digit is in cc[0], first in cc[15]
    for (int c = 0; c < 16; c++) {
        cc[c] = i % 10;
        i /= 10;
    }
    
    // Get first two CC digits
    int first2 = 0;
    if (cc[15]) {
        first2 = cc[15] * 10 + cc[14];
    }
    else if (cc[14]) {
        first2 = cc[14] * 10 + cc[13];
    }
    else if (cc[13]) {
        first2 = cc[13] * 10 + cc[12];
    }
    else {
        first2 = cc[12] * 10 + cc[11];
    }
 
    
    // Send every other digit to be processed, starting from second to last
    int check = 0;
    for (int c = 1; c < 16; c+=2) {
        check += add_digits(cc[c]);
    }
    
    // Add in every other digit, starting from the first
    for (int c = 0; c < 16; c+=2) {
        check += cc[c];
    }
    
    // If check is modulo 10 (valid), print brand
    if (!(check %= 10)) {
        
        // Check for which brand, then print result
        if (first2 == 34 || first2 == 37) {
            printf("AMEX\n");
        }
        else if (first2 >= 51 && first2 <=55) {
            printf("MASTERCARD\n");    
        }
        else if (first2 >= 40 && first2 <= 49) {
            printf("VISA\n"); 
        }
        else {
            printf("INVALID\n");
        }
    }
    else {
        printf("INVALID\n");
    }

    
}

// Function to do maths with CC digits
int add_digits(int digit) {
    int x = digit * 2;
    int y = x % 10;
    x /= 10;
    return x + y;
}
