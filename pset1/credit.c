#include <cs50.h>
#include <stdio.h>

int main(void)
{
    long card;
    
    int sum_of_evens = 0, sum_of_odd = 0, digits = 0, number = 0, number2 = 0;
    
    do 
    {
        card = get_long("Number: ");
    }
    while (card < 0);
    
    while (card > 0)
    {
        //number of digits
        digits ++;
        
        //store the second number at the end
        number2 = number;
        
        //store the digit number of the card
        number = card % 10;
        
        
        if (digits % 2 == 0)
        {
            int multiply = number * 2;
            
            //sum of the two digits
            sum_of_evens += (multiply / 10) + (multiply % 10);
            
        }
        
        if (digits % 2 != 0)
        {
            sum_of_odd += number;
        }
        
        //decrease one digit of the card
        card /= 10;
        
        
    }
    
    int firstn = number * 10 + number2;
    
    //check if the card is valid
    if ((sum_of_evens + sum_of_odd) % 10 == 0)
    {
        if ((digits == 13 || digits == 16) && number == 4)
        {
            printf("VISA\n");
        } 
        else if (digits == 15 && (firstn == 34 || firstn == 37))
        {
            printf("AMEX\n");
        } 
        else if (digits == 16 && firstn >= 51 && firstn <= 55)
        {
            printf("MASTERCARD\n");
        } 
        else
        {
            printf("INVALID\n");
        }
    }
    else 
    {
        printf("INVALID\n");
    }
    
}
