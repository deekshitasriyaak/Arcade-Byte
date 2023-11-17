#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

void welcome_display()
{
    printf("\t##   ##  ### ###  ####      ## ##    ## ##   ##   ##  ### ###           #### ##   ## ##     \n");
    printf("\t##   ##   ##  ##   ##      ##   ##  ##   ##   ## ##    ##  ##           # ## ##  ##   ## \n");
    printf("\t##   ##   ##       ##      ##       ##   ##  # ### #   ##                 ##     ##   ##      \n");
    printf("\t## # ##   ## ##    ##      ##       ##   ##  ## # ##   ## ##              ##     ##   ##\n");
    printf("\t# ### #   ##       ##      ##       ##   ##  ##   ##   ##                 ##     ##   ##    \n");
    printf("\t ## ##    ##  ##   ##  ##  ##   ##  ##   ##  ##   ##   ##  ##             ##     ##   ##  \n");
    printf("\t##   ##  ### ###  ### ###   ## ##    ## ##   ##   ##  ### ###            ####     ## ##      \n");
    printf("\n");
    printf("\t\t###  ##      ##   ###   ##   ## ##   ##   ##      ##   ###   ## \n");
    printf("\t\t ##  ##      ##    ###  ##  ##   ##   ## ##       ##    ###  ##  \n");
    printf("\t\t ##  ##    ## ##   # ##  #  ##       # ### #    ## ##   # ##  #\n");
    printf("\t\t ######   ##  ##   ## ## #  ##  ###  ## # ##   ##  ##   ## ## #\n");
    printf("\t\t ##  ##   ######   ##   ##  ##   ##  ##   ##   ######   ##   ##  \n");
    printf("\t\t ##  ##   ##  ##   ##   ##  ##   ##  ##   ##   ##  ##   ##   ##  \n");
    printf("\t\t###  ##  ###  ##  ###   ##   ## ##   ##   ##  ###  ##  ###   ##  \n");

}
void win_display()
{
    printf("\n"); printf("\n"); printf("\n"); 
    printf("\t\t##  ##    ## ##   ##  ###           ##   ##    ####   ###  ##   \n");
    printf("\t\t##  ##   ##   ##  ##   ##           ##   ##     ##      ## ##   \n");
    printf("\t\t##  ##   ##   ##  ##   ##           ##   ##     ##     # ## #   \n");
    printf("\t\t ## ##   ##   ##  ##   ##           ## # ##     ##     ## ##    \n");
    printf("\t\t  ##     ##   ##  ##   ##           # ### #     ##     ##  ##   \n");
    printf("\t\t  ##     ##   ##  ##   ##            ## ##      ##     ##  ##   \n");
    printf("\t\t  ##      ## ##    ## ##            ##   ##    ####   ###  ##   \n");
}
void loss_display()
{
    printf("\n"); printf("\n"); printf("\n"); 
    printf("\t\t ## ##     ##     ##   ##  ### ###            ## ##   ###  ### ### ###  ### ##    \n");
    printf("\t\t##   ##     ##     ## ##    ##  ##           ##   ##   ##  ##   ##  ##   ##  ##   \n");
    printf("\t\t##        ## ##   # ### #   ##               ##   ##   ##  ##   ##       ##  ##  \n");
    printf("\t\t##  ###   ##  ##  ## # ##   ## ##            ##   ##   ##  ##   ## ##    ## ##    \n");
    printf("\t\t##   ##   ## ###  ##   ##   ##               ##   ##   ### ##   ##       ## ##    \n");
    printf("\t\t##   ##   ##  ##  ##   ##   ##  ##           ##   ##    ###     ##  ##   ##  ##   \n");
    printf("\t\t ## ##   ###  ##  ##   ##  ### ###            ## ##      ##    ### ###  ####  ##   \n");
}

// Define a struct to hold a single character
struct letter {
    char letter;
    char revealed;
    struct letter* next;
};

// Define a struct for a word
struct word {
    int length;
    char* letters; // Head of the linked list
};

struct wordlist {
    int count;
    struct word* list[1400];
};

struct word*  createWordlist() {
    char temp[10];
    char* path = "common7_2.txt";
    struct wordlist* words = (struct wordlist*)malloc(sizeof(struct wordlist));
    if (words == NULL) {
        printf("Failed to allocate memory for the wordlist.\n");
        exit(1);
    }

    int index = 0;
    FILE* pFileWords;
    pFileWords = fopen(path, "r");

    if (pFileWords == NULL) {
        printf("\nNo wordlist found at %s\n", path);
        exit(1);
    }

    while (fscanf(pFileWords, "%s", temp) != EOF) {
        int length = strlen(temp);

        words->list[index] = (struct word*)malloc(sizeof(struct word));

        if (words->list[index] == NULL) {
            printf("Failed to allocate memory for a word.\n");
            exit(1);
        }
        words->list[index]->letters = (char *)malloc(7 * sizeof(char) + 1);
        strcpy(words->list[index]->letters, temp);
        words->list[index]->length = length;
        index++;

    }
    fclose(pFileWords);
    srand(time(NULL));
    int randomNum = (rand() % 1371);
    struct word* chosen=words->list[randomNum];

}


struct letter* prepare_game(struct word* chosenword)
{
    struct letter* head = NULL;
    struct letter* tail = NULL;
    // Iterate through each character in the word and create a letter node
    for (int i = 0; i < chosenword->length; i++) {
        struct letter* newLetter = (struct letter*)malloc(sizeof(struct letter));
        if (newLetter == NULL) { // Handle memory allocation failure
            printf("Failed to allocate memory for a letter.\n");
            return NULL;
        }

        newLetter->letter = chosenword->letters[i];
        newLetter->revealed = 0;
        newLetter->next = NULL;
        if (head == NULL) {
            head = newLetter;
            tail = newLetter;
        } else {
            tail->next = newLetter;
            tail = newLetter;
        }
    }
    return head;


}
void printHolderList(struct letter* word) {
    struct letter *dummy=word;
    while (dummy != NULL) {
        printf("%c", dummy->letter);
        dummy = dummy->next;
    }
}

void printGuess(struct letter*word)
{
    struct letter *dummy=word;
    while (dummy != NULL) {
        if (dummy->revealed==1) printf("%c", dummy->letter);
        else printf("_");
        dummy = dummy->next;
    }
    printf("\n");
}

int isValidLowerCaseAlpha(char guessletter) {
    if (islower(guessletter)) return 1; 
    else return 0;
}
int isPresent(char alreadyguessed[], char chr) {
    int present = 0;
    for (int i = 0; alreadyguessed[i] != '\0'; i++) {
        if (alreadyguessed[i] == chr) {
            present = 1;
            return present;
        }
    }
    return 0;
}
int updateRevealed(struct letter *word, char chr1) {
    int count = 0;
    struct letter *dummy=word;
    while(dummy!=NULL) {
        if (dummy->letter == chr1) {
            dummy->revealed = 1;
            count++;
        }
        dummy=dummy->next;
    }
    return count;
}
int play(struct letter *word,char alreadyguessed[])
{
    char chr;
    printf("enter guess :");
    scanf("%c",&chr);
    getchar();
    if (!isValidLowerCaseAlpha(chr)) {
        return -2;}
    int c = 0;
    if (isPresent(alreadyguessed, chr))
        return -1;
    else {
        alreadyguessed[strlen(alreadyguessed)] = chr;
        alreadyguessed[strlen(alreadyguessed) + 1] = '\0';
        c = updateRevealed(word, chr);
        return c;
    }
    
}
int main() {
    //printf("Hello!\n");
    struct word* loadedWord;
    loadedWord = createWordlist();  //printf("created\n");
    struct letter* guess_word=prepare_game(loadedWord);     //printf("prepared");
   // printf("Randomly chosen word: ");  //printHolderList(guess_word);
    printf("\n");
    char alreadyguessed[26] = { 0 };
    int guess=0;
	int chances = 12;
	welcome_display();
    getchar();
    system("cls");
    
    printf("Welcome to Hangman!\n");
    printf("GAME PLAY\n");
    printf("1. All letters are in lowercase\n2. You are given seven lives  \n3. All words are seven letters \n4. In each turn you can enter one guess letter\n");
    printf("\n press enter to begin");getchar();
    system("cls");
    printf("Welcome to Hangman!\n");
    printf("Guess the word: ");
    printGuess(guess_word);
    printf("total chances: %d length: %d\n", chances,7);
    int flag_counter = 0;
    int this = 0;
    while (guess < chances && flag_counter != 7) 
    {
        this=play(guess_word,alreadyguessed);
        if (this== -2){ printf("invalid: enter lowercase alphabet character\n");}
        else if (this == -1) {
            printf("invalid : already guessed this letter\n");
        } else if (this == 0) {
            printf("incorrect guess : one life deducted \n");
            guess++;
        } else {
            flag_counter += this;
            printf("valid guess\n");
        }
        printf("\n");
        printf("Guess the word: ");
        printGuess(guess_word);
        printf("remaining chances: %d\n", (chances-guess));
    }
    if (flag_counter == 7)
       { //printf("\n\t\t~~CONGRATULATIONS YOU WIN~~");
        system("cls");
        win_display(); 
        getchar();
       }
    else
       { //printf("\n\t\tsorry you lose :(");
        system("cls");
        loss_display();
        printf("\n\t\t\t\t\t\tguess word was :"); 
         printHolderList(guess_word);
        getchar();
       }
    free(guess_word);    
    return 0;
}
