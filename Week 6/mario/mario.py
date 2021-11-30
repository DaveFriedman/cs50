while True:
    h = input("Height: ")
    if h.isdigit():
        h = int(h)
        if 0 < h < 9:
            break
        else:
            print("Height between 1 and 8!")
    else:
        print("Height must be a digit!")

# row method:
for i in range(1, h+1):
    print(" "*(h-i) + "#"*i + "  " + "#"*i)

"""
# loop method:
 for y in range(h):
    for x in range(h):
        if (h - x > y + 1):
            print(" ", end="")
        else:
            print("#", end="")

    print("  ", end="")

    for x in range(y+1):
        print("#", end="")
    print("")
"""