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