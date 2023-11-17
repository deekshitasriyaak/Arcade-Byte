#include <stdio.h>
#include<windows.h>
#include "hanoi.h"
#include "rps.h"
#include "snake.h"
#include "solitaire.h"
#include "ttt.h"
#include <ctype.h>
#include "musicalchair.h"

#include "newhangmain.h"
// Define a structure for a game
typedef struct {
    char name[50];
    char description[100];
    void (*gameFunction)();
} Game;

// Define functions for individual games
void TowerOfHanoi() {
    // Initialize the three stacks
    struct Stack pegs[3];
    for (int i = 0; i < 3; i++) {
        initializeStack(&pegs[i]);
    }

    int numDisks;
    printf("Enter the number of disks: ");
    scanf("%d", &numDisks);

    // Push disks onto source stack in decreasing order
    for (int i = numDisks; i >= 1; i--) {
        PUSH(&pegs[0], i);
    }

    run_game(pegs, numDisks);
}


void RockPaperScissors() {
    rps();
}
void TicTacToe(){
    int board[9] = {0,0,0,0,0,0,0,0,0};
    //computer squares are 1, player squares are -1.
    printf("Computer: O, You: X\nPlay (1)st or (2)nd? ");
    int player=0;
    scanf("%d",&player);
    printf("\n");
    unsigned turn;
    for(turn = 0; turn < 9 && win(board) == 0; ++turn) {
        if((turn+player) % 2 == 0)
            computerMove(board);
        else {
            draw(board);
            playerMove(board);
        }
    }
    switch(win(board)) {
        case 0:
            printf("A draw. How droll.\n");
            break;
        case 1:
            draw(board);
            printf("You lose.\n");
            break;
        case -1:
            printf("You win. Inconceivable!\n");
            break;
    }
}


void Hangman() {
    //printf("Hello!\n");
    struct word* loadedWord;
    loadedWord = createWordlist();  //printf("created\n");
    struct letter* guess_word=Prepare_game(loadedWord);     //printf("prepared");
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
}
void SnakeGame(){
	welcomeArt();
	
	do
	{	
		switch(mainMenu())
		{
			case 0:
				loadGame();
				break;
			case 1:
				displayHighScores();
				break;	
			case 2:
				controls();
				break;		
			case 3:
				exitYN(); 
				break;			
		}		
	} while(1);	
}
void MusicalChair(){
    int numPlayers, round = 1;
    char names[MAX_PLAYERS][MAX_NAME_LENGTH];

    printf("\t\t\t\tWelcome to the Musical Chairs Game!\n");

    // Add your ASCII art here
    printf(R"EOF(
                               
                                                       ......::::::::----==.              
                                                       .....::---:::::-=====.             
                                                        ....:.         .---:              
                                   ....                 ....:.                            
                                  ....::::::..           ..:::                            
                                  ......:::::--.         ..:::.                           
                                  .::       :-==.         .::::.                          
                                 .:::        :--           :::::                          
                                 :::.                      .::::.                         
                                .:::                        :::::.                        
                        ...::::.::::                      ..::::::                        
                     .....:::::::::.                  ..:::::::---.                       
                    ...:::::::-----                 ...:::::::-----                       
                   ..:::::---------               ....::::::------=-                      
                    ::--------===:               ....::::::-----===-                      
                     .:--=====-.                .....:::::-----====.                      
                                                ....::::-----=====.                       
                                                ...::::-----====-.                        
                                                 .:::-----====-.                          
                                                  .::----=-:.                             
                                                                                          
                                                                                          
                                                                      ....                
               .......................................................::-==:              
              .......................................................::--==+              
              ::::::::::::::::::::::::::::::::::::::::::::::::::::::::--==++.             
              -------------------------------------------------------====+++              
               =+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++.              
                 .:++++++++++++-:::::::-++********=::::::::+***++++++++:..                
                  :========+++:        .===+++++++:        .+++++======-                  
                 .------=====:         .----======:         .======-----:                 
                 :::::-----=:           :::----===:          .=------::::.                
                ...:::::---:            ..:::----=.           .----::::...                
               ....::::---:             ...:::---=.            :---::::....               
              ....::::---:              ...:::---=.             :---::::....              
             ....::::---:               ...:::---=.              :---::::....             
            ....::::----                ...:::---=.               :--:::::....            
           ....::::----                 ...:::---=.                :--:::::....           
          ....::::----                  ...:::---=.                 :--:::::....          
         ....::::----                   ...:::---=.                  ---:::::....         
         ...::::----                    ...:::---=.                   ---:::::....        
        ...::::----                     ...:::---=.                    ---:::::...        
         .::::----.                     ...:::---=.                     ---:::::..        
           .::--:                         .:::--:.                       ::-::.       
    )EOF");
    waitForInput();
    system("cls");

    
    printf("Enter the number of players: ");
    scanf("%d", &numPlayers);

    Queue players;
    initialize(&players);

    takeParticipantNames(&players, names, numPlayers);

    // Seed the random number generator
    srand(time(NULL));

    // Simulating the musical chairs-style game
    system("cls");
    printf("\n\n\n\n\n\n\n\n\n\n\t\t\t\tThe game begins with %d players.\n", numPlayers);

    while (numPlayers > 1) {
        int passes = rand() % (numPlayers - 1) + 1;  // Generate a random number of passes
        waitForInput();  // Wait for user input to start each round

        // Play the song for a random amount of time (e.g., 2-5 seconds)
        int songDuration = 5 + rand() % 2;
        playSong(songDuration);

        for (int i = 1; i < passes; i++) {
            int currentPlayer = dequeue(&players);
            enqueue(&players, currentPlayer);
        }
        int eliminatedPlayerID = dequeue(&players);
        printf("\n\t\t\t\tRound %d: %s is eliminated after the song ends.\n", round, names[eliminatedPlayerID - 1]);
        numPlayers--;
        round++;
    }
    system("cls");
    int winnerID = dequeue(&players);
    printf("\n\n\t\t\t\tGame Over!\n");
    printf("\n\t\t\t\tRound %d: %s wins the game!\n", round, names[winnerID - 1]);
    
    printf("\n\t\t\t\tLeaderboard:\n");
    for (int i = 0; i < numPlayers; i++) {
        printf("\t\t\t\tRound %d: %s was eliminated.\n", i + 1, names[i]);
    }
}
void Solitaire(){
    srand(time(NULL)); //  srand(3);
    setlocale(LC_ALL, "");
    int rows, cols;
    init_console(&rows, &cols);
    welcome_rules();
    system("cls");
    // prepare the game state
    game_state *state = make_game_state();
    prepare_game(state);
    char buffer[80];
    print_all_console(state);
    int winning;
    // game loop
    while (1) {
      fgets(buffer, sizeof(buffer), stdin);
      system("cls");
      int result = attempt_move(state, buffer);
      if (result==8) { 
          break;}
      printf("%d",result);
      mvprintf(rows - 2, 0, "Move status: %s", move_results[result]);  
      // snew state
      print_all_console(state);
      winning=win(state);
      if (winning==1){
        break;
      }
    }
    if (winning==1){
      system("cls");
      //printf("you win ");
      win_display();

      printf("\n\t\t\t\t\tSCORE : %d ",state->score);
    } 
    else {
    system("cls");
    //printf("you lose ");
    loss_display();
    } 
    getchar();
}

int main() {
    // Create an array of Game objects
    Game games[] = {
        {"Game 1", "Tower of Hanoi", TowerOfHanoi},
        {"Game 2", "Rock Paper Scissors", RockPaperScissors},
        {"Game 3", "Hangman", Hangman},
        {"Game 4", "Snake Game", SnakeGame},
        {"Game 5", "Musical Chair", MusicalChair},
        {"Game 6", "Solitaire", Solitaire},
        {"Game 7", "Tic Tac Toe", TicTacToe}
    };

    int numGames = sizeof(games) / sizeof(Game);

    int choice;
    int playAgain = 1;  // Initialize playAgain flag to true

    
        // Welcome screen
        printf("\t\t\t\t\t\t\t\tWelcome to Arcade Byte - A Collection of Mini Games\n");
        Sleep(5000);
        printf(R"EOF(                                                                -@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*                        
                                                                    -@=             :=============-              @*                        
                                                                    -@=           =@#=#===========*@+            @*                        
                                                                    -@=          .@#-#@*-       .#--@.           @*                        
                                                                    -@=           @@-+@=:=====--+  +@            @*                        
                                                                    -@=           .+###+====#####*:              @*                        
                                                                    -@+-----------------------------------------:@*                        
                                                                    -@@#+++++++++++++++++++++++++++++++++++++++*@@*                        
                                                                    -@#@=                                     .##@*                        
                                                                    -@=-@=                                   .@# @*                        
                                                                    -@= -@=                                 .##  @*                        
                                                                    -@=  =@+                               .##   @*                        
                                                                    -@=   +@=                             .@#.   @*                        
                                                                    -@=    -@*****************************##.    @*                        
                                                                    -@=    .@+:::-:::-::::::::::::::::::::@+     @*                        
                                                                    -@=    +@  :## .#@-                   *@     @*                        
                                                                    -@=    ##:##-:#@*.                    =@-    @*                        
                                                                    -@=   :@= ::*@+.                      .@#    @*                        
                                                                    -@=   *@::#@+.                         #@.   @*                        
                                                                    -@=   @@ .-.                           =@-   @*                        
                                                                    -@=  -@+               :                @*   @*                        
                                                                    -@=  +@.             :@*@*              *@   @*                        
                                                                    -@= .#@############* :@#@* =#############@-  @*                        
                                                                    -@++@*::::-----::::.  .@=  .::::------:::=@#:@*                        
                                                                    -@#+.    +@#**@+     :@*@*      @***@@     =*@*                        
                                                                    -@########@###@#######@#@@######@###@@#######@*                        
                                                                    -@=                                          @*                        
                                                                    -@*==========================================@*                        
                                                                    -@*==========================================@*                        
                                                                    -@=                                          @*                        
                                                                    -@=      .+++++++++++++++++++++++++++=       @*                        
                                                                    -@=      +@*========================@@       @*                        
                                                                    -@=      +@- .::           .+#@@#+. @@       @*                        
                                                                    -@=      +@--@+#          +@:*@.@# @@        @*                        
                                                                    -@=      +@-.#@@+          =@+#@-@# @@       @*                        
                                                                    -@=      +@- .#-            -+##*-  @@       @*                        
                                                                    -@=      +@#************************@@       @*                        
                                                                    -@=       :::::::::::::::::::::::::::.       @*                        
                                                                    -@=                                          @*                        
                                                                    -@=                                          @*                        
                                                                    -@=                                          @*                        
                                                                    -@=                                          @*                        
                                                                    -@=                                          @*                        
                                                                    -@=                                          @*                        
                                                                    -@=.=======================================: @*                        
                                                                    -@=.=======================================: @*                        
                                                                    -@###########################################@*                        
    )EOF");
    
        Sleep(5000);
        system("cls");
    while (playAgain) {
        // Display the menu of games
        printf("Choose a game to play:\n");
        for (int i = 0; i < numGames; i++) {
            printf("%d. %s - %s\n", i + 1, games[i].name, games[i].description);
        }
        printf("8. Quit\n");

        printf("Enter the number of the game you want to play (or 8 to quit): ");
        scanf("%d", &choice);

        // Check if the choice is to quit
        if (choice == 8) {
            playAgain = 0;  // Set playAgain flag to false to exit the loop
            printf("Goodbye!\n");
            continue;
        }

        // Check if the choice is valid
        if (choice >= 1 && choice <= numGames) {
            // Call the selected game's function
            games[choice - 1].gameFunction();
        } else {
            printf("Invalid choice. Please select a valid game or 8 to quit.\n");
        }

        // Ask the user if they want to play again
        printf("\nDo you want to play another game? (1 for Yes, 0 for No): ");
        scanf("%d", &playAgain);

        // Clear the input buffer
        while ((getchar()) != '\n');
        
        system("cls");
    }

    return 0;
}