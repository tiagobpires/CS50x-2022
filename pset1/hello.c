#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //get the user name
    string name = get_string("What's your name? \n");
    
    printf("hello, %s\n", name);
}