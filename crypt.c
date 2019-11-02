// Basic Encrypter, usage ./crypt salt(2) key

#include <cs50.h>
#include <stdio.h>
#include <crypt.h>

int main(int argc, string argv[])
{
    
    // Pull the first two hash characters for the salt
    const char salt[2] = {argv[1][0], argv[1][1]};
    char *key = argv[2];
    char *hash = crypt(key, salt);
    printf("%s\n", hash);
}
