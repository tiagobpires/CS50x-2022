#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    float cash;
    
    //checks if cash is positive
    do
    {
        cash = get_float("Change owed: ");
    }
    while (cash < 0);
    
    //convert and rounds to an integer number
    int cents = round(cash * 100);
    
    int coins = 0;
    
    //check if it is possible to decrease
    while (cents >= 25)
    {
        cents = cents - 25;
        
        //count the number of change coins
        coins++;
    }
    
    while (cents >= 10)
    {
        cents = cents - 10;
        coins++;
    }
    
    while (cents >= 5)
    {
        cents = cents - 5;
        coins++;
    }
    
    while (cents >= 1)
    {
        cents = cents - 1;
        coins++;
    }
    
    printf("%i\n", coins);
}