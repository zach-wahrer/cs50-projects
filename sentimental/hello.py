# A program to greet the user

from cs50 import get_string


def main():
    # Get user's name
    name = get_string("What is your name?\n")
    # Print it
    print(f"hello, {name}")


if __name__ == "__main__":
    main()