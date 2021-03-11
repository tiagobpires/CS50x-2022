from cs50 import get_string


def main():
    text = get_string("Text: ")
    
    # Stores the number of letters, words and sentences
    letters = count_letters(text)
    words = count_words(text)
    sentences = count_sentences(text)

    # L = average number of letters per 100 words
    L = (100 / words) * letters
    
    # S = avarage number of sentences per 100 words
    S = (100 / words) * sentences

    index = round(0.0588 * L - 0.296 * S - 15.8)
    
    if index < 1:
        print("Before Grade 1\n")
    elif index >= 16:
        print("Grade 16+")
    else:
        print(f"Grade {index}")
    
    
def count_letters(text):
    letters = 0
    
    for i in range(0, len(text), 1):
        if text[i].isalpha():
            letters += 1
        
    return letters


def count_words(text):
    # First word has no space before
    words = 1

    for i in range(0, len(text), 1):
        if text[i].isspace():
            words += 1
    
    return words


def count_sentences(text):
    sentences = 0
    
    # Consider sentence the phrase that ends with. ! or?
    for i in range(0, len(text), 1):
        if text[i] == "." or text[i] == "!" or text[i] == "?":
            sentences += 1

    return sentences


if __name__ == "__main__":
    main()
