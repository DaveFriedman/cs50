import csv
import sys


def main():

    if len(sys.argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        sys.exit(1)

    # csv database to dictionary
    csvfile = open(sys.argv[1], "r")
    db = csv.DictReader(csvfile)

    strs = {}  # "[S]hort [T]andem [R]epeat[S]", a short pattern of DNA bases
    for pattern in db.fieldnames:  # k:v is DNA pattern : # consecutive repeats
        strs[pattern] = 0  # except: (name:0) is the match's name, used later

    with open(sys.argv[2], "r") as txtfile:  # a sequence of someone's DNA bases
        sequence = txtfile.read()

    for pattern in strs.keys():  # for each DNA pattern...
        sequence_start = pattern_location = consecutives = 0  # initialize

        # Search sequence for pattern, then search again but start at the char
        # to the right of the found pattern. If the patterns are consecutive,
        # increment, otherwise, reset the count to 1
        # "name" is a pattern but will never be found in a dna sequence
        while pattern_location != -1:
            pattern_location = sequence[sequence_start:].find(pattern)

            if (sequence_start == sequence_start+pattern_location):
                consecutives += 1
            else:
                if pattern_location != -1:
                    consecutives = 1

            if consecutives > strs[pattern]:
                strs[pattern] = consecutives

            sequence_start += pattern_location + len(pattern)

    # If all the consecutive pattern counts match a DB entry, get the entry name
    for row in db:
        match = False

        for pattern, count in row.items():
            if pattern != "name":
                if strs[pattern] != int(count):
                    match = False
                    break
                else:
                    match = True

        if match == True:
            strs["name"] = row["name"]

    if strs["name"] != 0:
        print(strs["name"])
    else:
        print("No Match")

    csvfile.close()


main()