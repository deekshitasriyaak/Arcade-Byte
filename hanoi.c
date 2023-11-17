#include <stdio.h>
// Define the structure for a stack
#define MAX_SIZE 100
struct Stack {
    int arr[MAX_SIZE];
    int top;
};
// Function to initialize a stack
void initializeStack(struct Stack* stack) {
    stack->top = -1;
}
// Function to push an element onto the stack
void push(struct Stack* stack, int element) {
    if (stack->top == MAX_SIZE - 1) {
        printf("Stack Overflow\n");
        return;
    }
    stack->arr[++(stack->top)] = element;
}

// Function to pop an element from the stack
int pop(struct Stack* stack) {
    if (stack->top == -1) {
        printf("Stack Underflow\n");
        return -1;
    }
    return stack->arr[(stack->top)--];
}

// Function to display current peg state
void displayPegs(struct Stack* pegs, int numPegs) {
    for (int i = 0; i < numPegs; i++) {
        printf("Peg %c: ", 'A' + i);
        for (int j = pegs[i].top; j >= 0; j--) {
            printf("%d ", pegs[i].arr[j]);
        }
        printf("\n");
    }
}
//Function to display the solution
void solution(int n,char src,char tmp,char dst)
{
    if(n==1)
    {
     printf("\nMove disk %d from %c to %c",n,src,dst);
     return;
    }
    solution(n-1,src,dst,tmp);
    printf("\nMove disk %d form %c to %c",n,src,dst);
    solution(n-1,tmp,src,dst);
    return;   
} 

void run_game(struct Stack* pegs, int numDisks) {
    struct Stack moveStack;
    initializeStack(&moveStack);

    while (1) {
        displayPegs(pegs, 3);

        int sourcePeg, destinationPeg;
        printf("1 stands for the smallest disk and n is largest disk\n");
        printf("A=1, B=2, C=3\n");
        printf("Enter source peg (1, 2, or 3) and destination peg (1, 2, or 3), or (-1, -1) to quit, or (0, 0) for solution ");
        scanf("%d %d", &sourcePeg, &destinationPeg);

        if (sourcePeg == -1 || destinationPeg == -1) {
            break;
        }

        if (sourcePeg == 0 || destinationPeg == 0) {
            printf("The solution is\n");
            solution(numDisks, 'A', 'B', 'C');
            break;
        }

        sourcePeg--;
        destinationPeg--;

        if (sourcePeg < 0 || sourcePeg > 2 || destinationPeg < 0 || destinationPeg > 2) {
            printf("Invalid peg number. Please try again.\n");
            continue;
        }

        if (pegs[sourcePeg].top == -1) {
            printf("Source peg is empty. Please try again.\n");
            continue;
        }

        int sourceDisk = pop(&pegs[sourcePeg]);
        if (pegs[destinationPeg].top != -1 && pegs[destinationPeg].arr[pegs[destinationPeg].top] < sourceDisk) {
            printf("Invalid move. Cannot place larger disk on smaller disk. Please try again.\n");
            push(&pegs[sourcePeg], sourceDisk);
            continue;
        }

        push(&pegs[destinationPeg], sourceDisk);

        // Check for win condition
        if (pegs[2].top == numDisks - 1) {
            printf("Congratulations! You've won!\n");
            break;
        }

    }
}

int main() {
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
        push(&pegs[0], i);
    }
    run_game(pegs,numDisks);
    

    return 0;
}