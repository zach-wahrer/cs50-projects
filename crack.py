# A program to crack 5-character alphabetical DES hashed passwords

from sys import argv
import string
import crypt


def main():
    # Exit if user doesn't provide correct input
    if len(argv) != 2:
        print("Usage: python crack.py hash")
        quit()

    # Extract the salt
    insalt = argv[1][0] + argv[1][1]

    # Create alpha list
    alpha = list(string.ascii_letters)

    # Check each length of password
    for i in range(5):
        crack(i, insalt, alpha)

def hash_check(word, insalt):
    potential = crypt.crypt(word, salt=insalt)
    if (potential == argv[1]):
        print(word)
        quit()

def crack(length, insalt, alpha):

    for letter1 in alpha:
        word = letter1
        if length == 1:
            hash_check(word, insalt)

        if length >= 2:
            for letter2 in alpha:
                word = letter1 + letter2
                if length == 2:
                    hash_check(word, insalt)

                if length >= 3:
                    for letter3 in alpha:
                        word = letter1 + letter2 + letter3
                        if length == 3:
                            hash_check(word, insalt)

                        if length >= 4:
                            for letter4 in alpha:
                                word = letter1 + letter2 + letter3 + letter4
                                if length == 4:
                                    hash_check(word, insalt)

                                if length >= 5:
                                    for letter5 in alpha:
                                        word = letter1 + letter2 + letter3 + letter4 + letter5
                                        if length == 5:
                                            hash_check(word, insalt)

if __name__ == "__main__":
    main()