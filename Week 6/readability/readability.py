def main():
    while True:
        text = input("Text: ")
        break

    score = coleman_liau(text)

    if score < 1:
        print("Before Grade 1")
    elif score >= 16:
        print("Grade 16+")
    else:
        print(f"Grade {round(score)}")


def coleman_liau(text):
    letters = 0
    words = 1
    sentences = 0

    for character in text:
        if character.isalpha():
            letters += 1
        if character == " ":
            words += 1
        if character in ["?", "!", "."]:
            sentences += 1

    score = 0.0588 * letters * 100 / words - 0.296 * sentences * 100 / words - 15.8
    return score


main()