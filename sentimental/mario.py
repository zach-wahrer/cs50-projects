# A program to print a mario'esk block setup

from cs50 import get_int


def main():
    # Get height from user and check it
    while True:
        h = get_int("Height: ")
        if h <= 8 and h >= 1:
            break

    # Iterate for height
    for curHeight in range(h):
        block = ""
        lSpace = ""
        # Create left space
        for j in range(h - curHeight - 1):
            lSpace += " "
        # Create blocks
        for k in range(curHeight + 1):
            block += "#"
        # Build row
        print(lSpace + block + "  " + block)


if __name__ == "__main__":
    main()