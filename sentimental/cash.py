# A small program to calculate how few coins change can be given in

from cs50 import get_float


def main():
    # Get input
    change = get_float("Change owed: ")

    # Make sure input is positive
    while (change <= 0):
        change = get_float("Change owed: ")

    # Set up coin list
    coinlist = [.25, .10, .05, .01]

    # Iterate through coin list, doing math
    for i in coinlist:
        # Slightly different for first item
        if i == .25:
            counter = change // i
            remainder = round(change % i, 2)
        # For the subsequent passes
        else:
            counter += remainder // i
            remainder = round(remainder % i, 2)
    # Print answer without the float
    print(int(counter))


if __name__ == "__main__":
    main()