from cs50 import get_string
from sys import argv


def main():
    # Exit if user doesn't provide correct input
    if len(argv) != 2:
        print("Usage: python bleep.py dictionary")
        exit(1)

    # Open file
    try:
        file = open(argv[1], "r")
    except OSError:
        print("File not found or not readable")
        exit(1)

    # Load words into a set and close file
    dictionary = {}
    for line in file:
        if not dictionary:
            dictionary = {line.rstrip('\n')}
        else:
            dictionary.add(line.rstrip('\n'))
    file.close()

    # Clean the dict
    for i in dictionary:
        i.rstrip('\n')

    # Get input to censor and split into individual words
    censor = get_string("What message would you like to censor?\n").split(' ')

    outstring = ""

    # Loop to check words
    for i in censor:
        match = False
        bleep = ""
        # Loop to compare each word to dict
        for j in dictionary:
            # Do the actual check
            if i.lower() == j:
                # If Matched, make bleep
                match = True
                for k in i:
                    bleep += "*"
                # Add a single trailing space
                bleep += " "

        # Add word or bleep to the out string
        if match:
            outstring += bleep
        else:
            outstring += i + " "

    # Print back message with words banned
    print(outstring)


if __name__ == "__main__":
    main()
