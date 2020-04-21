#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // Get name and say it back
    string n = get_string("What is your name?\n");
    printf("hello, %s\n", n);
}
