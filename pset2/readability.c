#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    string text = get_string("Text: ");

    //stores the number of letters, words and sentences
    int letters = count_letters(text), words = count_words(text), sentences = count_sentences(text);
    
    //L = average number of letters per 100 words
    float L = (100 / (float)words) * letters;
    
    //S = avarage number of sentences per 100 words
    float S = (100 / (float)words) * sentences;
    
    //índice Coleman-Liau
    int index = round(0.0588 * L - 0.296 * S - 15.8);

    if (index < 1)
    {
        printf("Before Grade 1\n");
    } 
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
    
}

int count_letters(string text)
{
    int letters = 0;

    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (isalpha(text[i]))
        {
            letters++;
        }
    }
    return letters;
}

int count_words(string text)
{
    //first word has no space before
    int words = 1;
    
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (isspace(text[i]))
        {
            words ++;
        }
    }
    return words;
}

int count_sentences(string text)
{
    int sentences = 0;
    
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        //consider sentence the phrase that ends with. ! or?
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentences ++;
        }
    }
    return sentences;
}