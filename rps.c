// how to play

#include <stdio.h>
#include <stdlib.h>
#include<windows.h>
#include <time.h>
#define max 5

int usrScore = 0, compScore = 0;
int user[max];
int sys[max];
int top = -1;
int underflow()
{
    if (top == -1)
        return 1;
    else
        return 0;
}
int overflow()
{
    if (top == max - 1)
        return 1;
    else
        return 0;
}
void peek() // peek operation of stack
{
    if (underflow())
        printf("No records yet!\n");
    else
    {
        printf("--------------------\n");
        printf("|  Comp   |   User  |\n");
        printf("--------------------\n");
        if (sys[top - 1] == 1 && user[top - 1] == 0)
            printf("|   Won   |   Lost  |\n");
        else if (user[top-1] == 1 && sys[top-1] == 0)
            printf("|  Lost   |    Won  |\n");
        else
            printf("|        TIE        |\n");
        printf("--------------------\n");
    }
}

void viewscore() // deletion operation of stack
{
    int temp = top;
    if (!overflow())
        temp = top - 1;
    if (underflow())
        printf("No records yet!\n");
    else
    {
        printf("--------------------\n");
        printf("|       RECORD      |\n");
        printf("--------------------\n");
        printf("|  Comp   |   User  |\n");
        printf("--------------------\n");
        while (temp != -1)
        {
            if (sys[temp] == 1 && user[temp] == 0)
                printf("|   Won   |   Lost  |\n");
            else if (user[temp] == 1 && sys[temp] == 0)
                printf("|  Lost   |    Won  |\n");
            else
                printf("|        TIE        |\n");
            temp--;
        }
    }
}

void scoreboard()
{
    viewscore();
    printf("--------------------\n");
    if (usrScore > compScore)
        printf("|     You Won :)    |\n");
    else if (usrScore < compScore)
        printf("|    You Lose :(    |\n");
    else
        printf("|     Its a tie     |\n");
    printf("|    Final Score    |\n");
    printf("| You : %d           |\n", usrScore);
    printf("| Computer: %d       |\n", compScore);
    printf(" --------------------\n");
    printf("\a");
}

void checkOptions(int usr, int comp) // insertion operation of stack
{
    // 1 = scissor 2= paper 3=rock
    if (underflow())
    {
        top = 0;
    }
    if (usr == comp)
    {
        printf(" --------------------\n");
        printf("|    Its a tie       |\n");
        printf(" --------------------\n");
        sys[top] = 2;
        user[top] = 2;
    }
    else if (usr == 1)
    {
        if (comp == 2)
        {
            printf(" --------------------\n");
            printf("| You : Scissor      |\n");
            printf("| Computer : Paper   |\n");
            printf(" --------------------\n");

            sys[top] = 0;
            user[top] = 1;
            usrScore++;
        }
        else
        {
            printf(" --------------------\n");
            printf("| You : Scissor      |\n");
            printf("| Computer : Rock    |\n");
            printf(" --------------------\n");

            sys[top] = 1;
            user[top] = 0;
            compScore++;
        }
    }
    else if (usr == 2)
    {
        if (comp == 1)
        {
            printf(" --------------------\n");
            printf("| You : Paper        |\n");
            printf("| Computer : Scissor |\n");
            printf(" --------------------\n");
            sys[top] = 1;
            user[top] = 0;
            compScore++;
        }
        else
        {
            printf(" --------------------\n");
            printf("| You : Paper        |\n");
            printf("| Computer : Rock    |\n");
            printf(" --------------------\n");
            sys[top] = 0;
            user[top] = 1;
            usrScore++;
        }
    }
    else if (usr == 3)
    {
        if (comp == 1)
        {
            printf(" --------------------\n");
            printf("| You : Rock        |\n");
            printf("| Computer : Scissor |\n");
            printf(" --------------------\n");
            sys[top] = 0;
            user[top] = 1;
            usrScore++;
        }
        else
        {
            printf(" --------------------\n");
            printf("| You : Rock         |\n");
            printf("| Computer : Paper    |\n");
            printf(" --------------------\n");

            sys[top] = 1;
            user[top] = 0;
            compScore++;
        }
    }
    else
    {
        // Beep(750,500);
        printf("\n Invalid Option");
    }
    if (overflow())
    {
        scoreboard(); // scoreboard block
    }
}
void rps()
{
    int option, userChoice, compChoice;
    srand(time(0));
    printf("\t\t\t\t=====================================Welcome to Rock, Paper & Scissor Game=====================================\n");
    while (top != max)
    {
        printf("Please select the option:\n1.Play\t2.View current score and record\t  3.Peek previous record\n");
        scanf("%d", &option);
        if (option == 2)
        {
            viewscore();
            if (!underflow())
                printf("--------------------\n");
        }
        else if (option == 1)
        {
            printf("\n1.Scissor\t2.paper\t\t3.Rock\n");
            scanf("%d", &userChoice);
            compChoice = (rand() % 3) + 1;
            checkOptions(userChoice, compChoice);
            top++;
        }
        else if (option == 3)
        {
            peek();
        }
        else
        {
            Beep(750,500);
            printf("\n Invalid Option");
        }
    }
}

int main()
{
    rps();
    return 0;
}