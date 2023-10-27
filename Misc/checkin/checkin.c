#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    setvbuf(stdin, 0LL, 2, 0LL);
    setvbuf(stdout, 0LL, 2, 0LL);
    setvbuf(stderr, 0LL, 2, 0LL);

    printf(
        "Welcome to WHUCTF 2023!\n"
        "Let's play a mini game to find the checkin flag!\n"
        "Can you guess the number(0 <= number < 1024) I want?\n"
        );
    
    srand(time(NULL));
    const int value = rand() % 1024;

    int chance_remain = 10;
    while (chance_remain)
    {
        printf("[%d CHANCE REMAIN] Please input: ", chance_remain);
        int input;
        if (scanf("%d", &input) == 1)
        {
            if (input == value)
            {
                printf("Bingo! Here is your flag: NOCTF{WelC0m3_4nd_tRy_70_ch4l14ng3_y0us3lF}\n");
                return 0;
            }
            else if (input < value)
                printf("Seems your input is too SMALL.\n");
            else
                printf("Seems your input is too LARGE.\n");
        }
        else
            printf("Invalid input!\n");
        --chance_remain;
    }

    printf("You have used up your chance this time :<\n");
    return 0;
}