#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
    bool key = true;
    
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    
    //check if you have two arguments and 26 letters in the key
    else if (argc == 2 && strlen(argv[1]) == 26)
    {
        //checks if it contains only letters of the alphabet
        for (int i = 0, n = strlen(argv[1]); i < n; i++)
        {
            if (!isalpha(argv[1][i]))
            {
                key = false;
            }
            
            //checks if the letter is doubled
            for (int j = 0; j < i; j++)
            {
                if (argv[1][i] == argv[1][j])
                {
                    key = false;
                }
            }
        }
        
        if (key == false)
        {
            printf("Key must contain 26 different characters.\n");
            return 1;
        }
        
        if (key == true)
        {
            string plaintext = get_string("plaintext: ");
            
            printf("ciphertext: ");
            for (int i = 0, n = strlen(plaintext); i < n; i++)
            {
                //decreases the number that starts the alphabet to correspond to the same position of the string
                if (islower(plaintext[i]))
                {
                    int cipheri = plaintext[i] - 97;
                    //prints the character of the corresponding key
                    printf("%c", tolower(argv[1][cipheri])); 
                    
                }
                else if (isupper(plaintext[i]))
                {
                    int cipheri = plaintext[i] - 65;                    
                    printf("%c", toupper(argv[1][cipheri])); 
                    
                }
                else
                {
                    //if it is not alphabetic, print the way it is
                    printf("%c", plaintext[i]);
                    
                }
            }
            
            printf("\n");
            return 0;
        }
        
    }
    else
    {
        printf("Key must contain 26 different characters.\n");
        return 1;
    }
}
