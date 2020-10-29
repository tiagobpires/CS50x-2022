#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
    bool key = true;
    
    //checks if all characters in the second argument are digits
    if (argc >= 2)
    {
        for (int i = 0, n = strlen(argv[1]); i < n; i++)
        {
            if (!isdigit(argv[1][i]))
            {
                key = false;
            }
        }
    }
    
    //if there are two main arguments, and a key for just digits
    if (argc == 2 && key == true)
    {
        //if k is > 26, store the division remainder instead
        int keyc = atoi(argv[1]) % 26;
        
        string plaintext = get_string("plaintext:  ");
        
        printf("ciphertext: ");

        for (int i = 0, n = strlen(plaintext); i < n; i++)
        {
            //stores the integer equivalent to the sum
            int cipheri = (plaintext[i] + keyc);
            
            char cipher = plaintext[i] + keyc;
            
            //if it exceeds the last letter, it decreases 26 to be the corresponding
            if (islower(plaintext[i]))
            {
                if (cipheri > 'z')
                {
                    printf("%c", cipher - 26);
                } 
                else 
                {
                    printf("%c", cipher);
                }              
            }
            else if (isupper(plaintext[i]))
            {
                if (cipheri > 'Z')
                {
                    printf("%c", cipher - 26);
                } 
                else 
                {
                    printf("%c", cipher);
                }
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
    else 
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
}
