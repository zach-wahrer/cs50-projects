from nltk.tokenize import sent_tokenize


def lines(a, b):
    """Return lines in both a and b"""

    # Split each string into lines
    asplit = splitter(a, "l", 0)
    bsplit = splitter(b, "l", 0)

    # Find matches
    matches = matcher(asplit, bsplit)

    return matches


def sentences(a, b):
    """Return sentences in both a and b"""

    # Turn the inputs into sentences
    asplit = splitter(a, "se", 0)
    bsplit = splitter(b, "se", 0)

    # Find matches
    matches = matcher(asplit, bsplit)

    return matches


def substrings(a, b, n):
    """Return substrings of length n in both a and b"""

    # Turn the inputs into substings
    asplit = splitter(a, "su", n)
    bsplit = splitter(b, "su", n)

    # Find matches
    matches = matcher(asplit, bsplit)

    return matches


def splitter(s, t, n):
    """Split a string in the requested way"""

    # Line split
    if t == "l":
        split = s.split('\n')

    # Sentence split
    if t == "se":
        split = sent_tokenize(s)

    # Substring in range of n into split
    if t == "su":
        split = set()
        c = 0
        for i in s:
            if not i == "" and not n+c > len(s):
                split.add(s[c:n+c])
            c += 1

    return split


def matcher(a, b):
    """Compare input strings and match"""

    # Use set data type to remove duplicates
    matches = set()

    # Compare them
    for i in a:
        # Add if matched
        if i in b:
            matches.add(i)

    # Return the list
    return list(matches)