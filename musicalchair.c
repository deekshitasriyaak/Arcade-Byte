#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>  // For Beep function
#include <unistd.h>    // For sleep function

#define MAX_NAME_LENGTH 50
#define MAX_PLAYERS 100

// Circular Queue structure
typedef struct {
    int front, rear;
    int items[MAX_PLAYERS];
} Queue;

void initialize(Queue* q) {
    q->front = q->rear = -1;
}

int isEmpty(Queue* q) {
    return q->front == -1;
}

void enqueue(Queue* q, int item) {
    if ((q->rear + 1) % MAX_PLAYERS == q->front) {
        printf("Queue is full. Can't add more players.\n");
    } else {
        if (q->front == -1) {
            q->front = q->rear = 0;
        } else {
            q->rear = (q->rear + 1) % MAX_PLAYERS;
        }
        q->items[q->rear] = item;
    }
}

int dequeue(Queue* q) {
    int item;
    if (isEmpty(q)) {
        printf("Queue is empty.\n");
        return -1;
    } else {
        item = q->items[q->front];
        if (q->front == q->rear) {
            q->front = q->rear = -1;
        } else {
            q->front = (q->front + 1) % MAX_PLAYERS;
        }
        return item;
    }
}

void takeParticipantNames(Queue* q, char names[][MAX_NAME_LENGTH], int numPlayers) {
    for (int i = 1; i <= numPlayers; i++) {
        printf("\nENTER THE NAME OF PLAYER %d: ", i);
        scanf("%s", names[i - 1]);
        enqueue(q, i);
        printf("\n\t\t\t\t%s has been added to the game.\n", names[i - 1]);
    }
}

void waitForInput() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);  // Consume any remaining characters in the input buffer
    printf("Press Enter...");
    while (getchar() != '\n');
}

void playSong(int duration) {
    printf("\nPlaying the song for %d seconds...\n", duration);

    for (int x = 0; x < duration; x++) {
        Beep(1000, 150);
        Beep(800, 200);
        Beep(760, 150);
        Beep(1000, 220);
        Beep(500, 220);
    }

    sleep(duration);
}

int main() {
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

    return 0;
}
