#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // Print the winner
    if (score1 > score2) 
    {
        printf("Player 1 wins!\n");
    } 
    else if (score2 > score1)
    {
        printf("Player 2 wins!\n");
    }
    else 
    {
        printf("Tie!\n");
    }
    
}

// Compute and return score for string
int compute_score(string word)
{
    int score = 0;
    // Scrolls all over the word
    for (int i = 0, n = strlen(word); i < n; i++) 
    {
        // Decrease 97 to a = 0, b = 1 ..
        int c = tolower(word[i]) - 97;
        if (c >= 0 && c < 26)
        {
            score += POINTS[c];
        }
    }
    
    return score;
}