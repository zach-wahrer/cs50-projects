// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Represents number of buckets in a hash table ie. 26^4
#define N 456976

// Create int for the counter
unsigned int counter = 0;

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Represents a hash table
node *hashtable[N];

// Hashes word to a number between 0 and 25, inclusive, based on its first letter
unsigned int hash(const char *word)
{
    int wlength = strlen(word);

    if (wlength >= 4 && word[3] != '\'')
    {
        // Count in base 26, sort of
        return ((tolower(word[0]) - 'a') * 17576) + ((tolower(word[1]) - 'a') * 676) + ((tolower(word[2]) - 'a') * 26) + (tolower(word[3]) - 'a');
    }

    // Check if it is 3 or larger letter word
    if (wlength == 3 && word[2] != '\'')
    {
        // Count in base 26, sort of
        return ((tolower(word[0]) - 'a') * 17576) + ((tolower(word[1]) - 'a') * 676) + ((tolower(word[2]) - 'a') * 26);
    }
    // For two letter, put into "letter, letter" + "a" list
    else if (wlength == 2 && word[1] != '\'')
    {
        return ((tolower(word[0]) - 'a') * 17576) + ((tolower(word[1]) - 'a') * 676);
    }
    // Put single letter words into the "letter" + "a" + "a" bucket
    else
    {
        return ((tolower(word[0]) - 'a') * 17576);
    }


    // Old hash function
    //return tolower(word[0]) - 'a';
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{

    // Initialize hash table
    for (int i = 0; i < N; i++)
    {
        hashtable[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into hash table
    while (fscanf(file, "%s", word) != EOF)
    {
        // Allocate memory for new node and check the return
        node *new_node = malloc(sizeof(node));
        if (!new_node)
        {
            unload();
            return false;
        }

        // Copy current word into node and increment counter
        strcpy(new_node->word, word);
        counter++;

        // Hash the current word to find which list it goes into
        unsigned int list = hash(word);

        // Insert new node into correct list
        if (hashtable[list] == NULL)
        {
            // If null, create pointer to new node (first time through)
            new_node->next = NULL;
            hashtable[list] = new_node;
        }
        else
        {
            // Point new node towards the current first node, then reset list pointer to new node
            new_node->next = hashtable[list];
            hashtable[list] = new_node;
        }
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // Value created in load function
    return counter;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Get location in linked list array
    unsigned int loc = hash(word);

    // Make sure the word is lower-case to compare
    int length = strlen(word);
    char fixedword[length];
    for (int i = 0; i <= length; i++)
    {
        if (isupper(word[i]))
        {
            fixedword[i] = tolower(word[i]);
        }
        else
        {
            fixedword[i] = word[i];
        }
    }

    // Get into the correct linked list
    node *ptr = hashtable[loc];

    // Check the word against list until match found or end
    while (ptr != NULL)
    {
        // Check for match
        if (!(strcmp(ptr->word, fixedword)))
        {
            return true;
        }
        // Go to next node
        ptr = ptr->next;
    }

    // Made it to end, no match found
    return false;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // Loop to go through each letter in the hashtable
    for (int i = 0; i <= N; i++)
    {
        // Loop to free each node
        for (node *ptr = hashtable[i], *nextptr = NULL; ptr != NULL; ptr = nextptr)
        {
            // Get address of next node before we free the current one
            nextptr = ptr->next;
            free(ptr);
        }
    }

    return true;
}
