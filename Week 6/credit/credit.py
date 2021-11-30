def main():
    while True:
        card = input("Number: ")
        if card.isdigit():
            break

    # Convert the cc number into a list, each element is a digit
    card = list(map(int, card))

    valid = luhn(card)
    if valid == True:
        network = whichNetwork(card)
        print(network)
    else:
        print("INVALID")


def luhn(card):
    even = odd = 0
    card.reverse()  # python lists index left to right, we need R->L

    for digit in card[1::2]:  # for every other element, starting at Tens digit...
        if digit*2 >= 10:     # if digit*2 >=10, aka between 10 and 18...
            even = even + 1 + (digit*2) % 10  # split into digits, then add digits
        else:
            even = even + digit*2  # otherwise just add digit*2

    for digit in card[::2]:  # for every other element, starting at Ones digit...
        odd = odd + digit    # sum them

    card.reverse()  # flip back

    if (even + odd) % 10 == 0:
        return True
    else:
        return False


def whichNetwork(digits):
    if len(digits) == 15 and digits[0] == 3 and digits[1] in [4, 7]:
        network = "AMEX"
    elif len(digits) == 16 and digits[0] == 5 and digits[1] in [1, 2, 3, 4, 5]:
        network = "MASTERCARD"
    elif len(digits) == 16 or len(digits) == 13 and digits[0] == 4:
        network = "VISA"
    else:
        network = "INVALID"
    return network


main()