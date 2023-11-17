#include <memory.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <windows.h> 
#ifndef _O_U16TEXT
#define _O_U16TEXT (0x20000)
#endif
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004

enum { //suits
  SUIT_HEART,
  SUIT_SPADE,
  SUIT_CLUB,
  SUIT_DIAMOND,
  SUIT_COUNT 
}; 
enum { // ranks
  RANK_A,
  RANK_2,
  RANK_3,
  RANK_4,
  RANK_5,
  RANK_6,
  RANK_7,
  RANK_8,
  RANK_9,
  RANK_10,
  RANK_J,
  RANK_Q,
  RANK_K,
  RANK_COUNT
};
typedef struct card { // card def 
  int suit;
  int rank;
  int revealed;
} card;
//suit to unicode symbols 
#ifdef UNICODE
const char *suit_to_charptr(int suit) {
  switch (suit) {
  case SUIT_HEART:
    return "\u2665"; // 2665  BLACK HEART SUIT
  case SUIT_SPADE:
    return "\u2660"; // 2660  BLACK SPADE SUIT
  case SUIT_CLUB:
    return "\u2663"; // 2663  BLACK CLUB SUIT
  case SUIT_DIAMOND:
    return "\u2666"; // 2666  BLACK DIAMOND SUIT
  default:
    return "?";
  }
}

#else
const char *suit_to_charptr(int suit) {
  switch (suit) {
  case SUIT_HEART:
    return "H";
  case SUIT_SPADE:
    return "S";
  case SUIT_CLUB:
    return "C";
  case SUIT_DIAMOND:
    return "D";
  default:
    return "?";
  }
}
#endif
// rank to char 
const char *rank_to_charptr(int rank) {
  switch (rank) {
  case RANK_A:
    return "A";
  case RANK_2:
    return "2";
  case RANK_3:
    return "3";
  case RANK_4:
    return "4";
  case RANK_5:
    return "5";
  case RANK_6:
    return "6";
  case RANK_7:
    return "7";
  case RANK_8:
    return "8";
  case RANK_9:
    return "9";
  case RANK_10:
    return "10";
  case RANK_J:
    return "J";
  case RANK_Q:
    return "Q";
  case RANK_K:
    return "K";
  default:
    return "?";
  }
}
void print_card(card *c) {// print card 
  printf("|%s%s|", rank_to_charptr(c->rank), suit_to_charptr(c->suit));
}
// making the card pointer 
card *make_card_ptr(int suit, int rank) {
  card *c = malloc(sizeof(card));
  c->suit = suit;
  c->rank = rank;
  c->revealed =0;
  return c;
}

// checking card color 
int is_black(card c) { return c.suit == SUIT_CLUB || c.suit == SUIT_SPADE; }
int is_alternate_color(card first, card second) {return is_black(first) != is_black(second);}

// ~~other conditions 
int is_in_sequence(card lower, card higher) { return higher.rank == lower.rank + 1;} // for foundation n tableau
// for tableau
int can_be_placed_bottom(card parent, card child) {
  return is_alternate_color(parent, child) && is_in_sequence(child, parent);
}  
// for foundation 
int is_same_suit(card first, card second) { return first.suit == second.suit; }
int can_be_placed_on_foundation(card parent, card child) {
  return is_same_suit(parent, child) && is_in_sequence(parent, child);
} 

#define CARD_COUNT 52
#define PILE_LIST
#ifdef PILE_LIST
typedef struct card_node { // making a card node (for sll)
  card *value;
  struct card_node *next;
} card_node;
typedef struct pile { // pile 
  card_node *head;
  int num_cards;
  char type;
} pile;
// finding the last card in a pile, returns 0 if null
card_node *find_tail(pile *pile) {
  card_node *tail = pile->head;
  if (tail == 0) return 0;
  while (tail->next != 0) {tail = tail->next;}
  return tail;
}
// function to make the sll of card nodes 
card_node *make_node(card *card) {
   card_node *node = (card_node *)calloc(1, sizeof(card_node));
  node->value = card;
  node->next = 0;
  return node;
}

// check if pile is empty 
int is_empty(pile *pile) { return pile->num_cards == 0; }
// remove a card from a pile, relinking the list
void delete (pile *pile, card *card) {
  if (is_empty(pile)) {return;}
  card_node *prev = NULL;  // no previous node for the first item
  card_node *current;
  for (current = pile->head; current != NULL; prev = current, current = current->next) {
    if (current->value == card) {
      if (prev == NULL) { // special case if the first item was found
        pile->head = current->next;
      } 
      else { // skip the current item in the list
        prev->next = current->next;
      }
      pile->num_cards--;
      free(current);
      return;
    }
  }
}
//append to end of list 
void push(pile *pile, card *card) {
  card_node *tail = find_tail(pile);
  if (tail == NULL) {
    pile->head = make_node(card);
  } else {
    tail->next = make_node(card);
  }
  pile->num_cards++;
}
// remove a card from the beginning of the list
card *shift(pile *pile) {
  pile->num_cards--;
  card_node *old_head = pile->head;
  pile->head = old_head->next;
  card *card = old_head->value;
  free(old_head);
  return card;
}
// see card at given index in pile 
card *peek_card_at(pile *pile, int index) {
  card_node *head = pile->head;
  for (int i = 0; i < index; i++)
    head = head->next;
  return head->value;
}
// see card at head or see if pile is empty
card *peek(pile *pile) {
  if (pile->head == NULL) {
    return NULL;
  }
  return pile->head->value;
}
// see last card calls peek_at_card func w last index 
card *peek_last(pile *pile) {
  if (pile->head == NULL) { return NULL;}
  if (pile->num_cards == 0){ return NULL;}
  return peek_card_at(pile, pile->num_cards - 1);
}
card *peek_lastf(pile *pile) {
  if (pile->head == NULL) { return NULL; }
  if (pile->num_cards == 0) { return NULL; }
  return peek_card_at(pile, pile->num_cards - 1);
}
// making a new pile 
pile *make_pile() {
 pile *pile_ptr = calloc(1, sizeof(pile));
  pile_ptr->num_cards = 0;
  return pile_ptr;
}
// creating all 52 cards in a deck
void fill_deck(pile *pile) {
  for (int rank = 0; rank < RANK_COUNT; rank++) {
    for (int suit = 0; suit < SUIT_COUNT; suit++) {
      push(pile, make_card_ptr(suit, rank));
    }
  }
}
#endif 

#define TABLEAU_COUNT 7
#define FOUNDATION_COUNT 4
// deck , waste pile , 4 foundations , 7 tableaus 
enum {
  PILE_DECK,
  PILE_WASTE,
  PILE_FOUNDATION1,
  PILE_FOUNDATION2,
  PILE_FOUNDATION3,
  PILE_FOUNDATION4,
  PILE_TABLEAU1,
  PILE_TABLEAU2,
  PILE_TABLEAU3,
  PILE_TABLEAU4,
  PILE_TABLEAU5,
  PILE_TABLEAU6,
  PILE_TABLEAU7,
  PILE_COUNT
};
char pile_types[] = "dwffffttttttt";

// game state
typedef struct game_state {
  pile **piles;
  int pile_count;
  int score;
} game_state;
// creates all piles 
game_state *make_game_state() {
  game_state *state =calloc(1, sizeof(game_state)); 
  state->piles = calloc(PILE_COUNT, sizeof(pile *));
  for (int pile_idx = 0; pile_idx < PILE_COUNT; pile_idx++) {
    state->piles[pile_idx] = make_pile();
    state->piles[pile_idx]->type = pile_types[pile_idx];
  }
  return state;
}
//insert card at specific position 
void insert(pile *pile, card *card, int idx) {
  card_node *pre_tail = pile->head;
  for (int i = 0; i < idx; i++)
    pre_tail = pre_tail->next;
  card_node *card_node = make_node(card);
  card_node->next = pre_tail->next;
  pre_tail->next = card_node;
  pile->num_cards++;
}
// to shuffle deck before game starts 
void shuffle_pile(pile *pile) {
  int shuffle_times = pile->num_cards * 10;
  for (int i = 0; i < shuffle_times; i++) { // unshift a card and insert to random place
    int idx = rand() % pile->num_cards - 1;
    card *card = shift(pile);
    insert(pile, card, idx);
  }
}

//accessing piles 
pile *stock(game_state *state) { return state->piles[PILE_DECK]; }
pile *waste(game_state *state) { return state->piles[PILE_WASTE]; }
pile *tableau(game_state *state, int index) { return state->piles[PILE_TABLEAU1 + index- 1]; }
pile *foundation(game_state *state, int index) { return state->piles[PILE_FOUNDATION1 + index - 1];}

// reveal card 
int reveal(card *card) {
  if (card == NULL) return 0;
  card->revealed = 1;
  return 1;
}
// hide card 
void hide(card *card) {
  if (card == NULL) return;
  card->revealed = 0;
}
// moves 1 card from stock to waste
void turn(game_state *state) {
  card *revealed_card = shift(stock(state));
  reveal(revealed_card);
  push(state->piles[PILE_WASTE], revealed_card);
}
void deal(game_state *state) { // dealing cards
  pile *deck = state->piles[PILE_DECK];
  for (int i = 0; i < TABLEAU_COUNT; i++) {
    int column_idx = i + 1;
    pile *tableau = state->piles[PILE_TABLEAU1 + i];
    for (int card_num = 0; card_num < column_idx; card_num++) {
      card *card = shift(deck);
      push(tableau, card);
      if (card_num == column_idx - 1) { reveal(card); }
    }
  }
  turn(state);
}
// initialize console api?
void init_console(int* rows, int* cols) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConsoleMode;

    // Initialize the console
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) { return; // Handle error
    } // Get console dimensions
    *rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    *cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    // Set console mode for cursor positioning
    if (!GetConsoleMode(hConsole, &dwConsoleMode)) return;  // Handle error
    dwConsoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING; // Enable VT sequences
    if (!SetConsoleMode(hConsole, dwConsoleMode)) return;  // Handle error
}
// display card 
void printf_card(card *c) {
  if (c == NULL) {
    
    printf("[     ]");
    return;
  }
  if (c->revealed) {
    printf("%s%s", rank_to_charptr(c->rank), suit_to_charptr(c->suit));
  } else printf("[ ]");
}
void printf_fcard(card *c) {
  if (c == NULL) {
    printf("[     ]");
    return;
  }
  if (c->revealed) {
    printf("%s%s", rank_to_charptr(c->rank), suit_to_charptr(c->suit));
  } else printf("[ ]");
}
// prints pile size 
void printf_pile_size(pile *pile) {
    printf("(%d cards)", pile->num_cards);
}
//move cursor 
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void print_all_console(game_state *state) { // 2 rows, 7 columns each -> top row has a fixed height of 1 card + bottom row can have n cards
  gotoxy(0, 1);
  printf("\n");
  char *first_row_headers[] = {"Stock","Waste pile","score ","Foundation 1", "Foundation 2", "Foundation 3","Foundation 4"};
  char *second_row_headers[] = {"Tableau 1", "Tableau 2", "Tableau 3", "Tableau 4", "Tableau 5", "Tableau 6", "Tableau 7"};
  int column_size = 14;
  card *d;

  for (int i = 0; i < 7; i++) {  // row 1 headers 
    gotoxy(column_size * i, 3); 
    printf("%s", first_row_headers[i]);
  }
  pile *stock_pile = stock(state);
  pile *waste_pile = waste(state);
  // first row content
    gotoxy(0, 4);
    printf_card(peek(stock_pile));
    gotoxy(0, 5);
    printf_pile_size(stock_pile);
    gotoxy(column_size, 4);
    printf_card(peek_last(waste_pile));
    gotoxy(column_size, 5);
    printf_pile_size(waste_pile);
    gotoxy(column_size*2, 4);
    printf("%d",state->score);
  // foundations
  for (int f = 0; f < FOUNDATION_COUNT; f++) {
        int foundation_1_column = 3;
        pile *found=foundation(state,f+1);
        int k = found->num_cards;
        gotoxy((foundation_1_column + f) * column_size, 4);
        {
          d = peek_lastf(found);
          printf_fcard(d);
        }
        gotoxy((foundation_1_column + f) * column_size, 5);
        printf_pile_size(found);
    } 
    // second row header
  for (int i = 0; i < TABLEAU_COUNT; i++) {
        gotoxy(column_size * i, 6);
        printf("%s", second_row_headers[i]);
        gotoxy(column_size * i, 7);
        printf_pile_size(tableau(state, i + 1));
    }
  for (int i = 0; i < TABLEAU_COUNT; i++) {
        pile *col = tableau(state, i + 1);
        int base_row = 8;
        for (int c = 0; c < col->num_cards; c++) {
            gotoxy(column_size * i, base_row + c);
            printf_card(peek_card_at(col, c));
        }
    }
  // status bar for the commands
  HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    if (GetConsoleScreenBufferInfo(hConsoleOutput, &consoleInfo)) {
        int numRows = consoleInfo.dwSize.Y; // Number of rows in the console.
        int numColumns = consoleInfo.dwSize.X; // Number of columns in the console.
    }
  gotoxy(0, consoleInfo.dwSize.Y - 1);
  printf("\n");
  printf("\n");
  printf("solitaire-cli > ");
}
// fill deck , shuffle n deal
void prepare_game(game_state *state) {
  pile *stock_pile = stock(state);
  fill_deck(stock_pile);
  shuffle_pile(stock_pile);
  deal(state);
  state->score = 0;
}

// move details 
typedef struct parsed_input {
  char source;
  char destination;
  int source_index;
  int destination_index;
  int source_amount;
  int success;
} parsed_input;
// reading input 
parsed_input parse_input(char *command) {
  parsed_input parsed;
  parsed.success = 1; parsed.source_amount = 1;
  // input patterns 
  char *pattern_multi_move = "%d t%d t%d";
  char *pattern_single_move_tf = "t%d f%d";
  char *pattern_single_move2 = "%d %d";
  char *pattern_waste_move_wt = "w%d t%d";
  char *pattern_waste_move_wf = "w%d f%d";
  char *pattern_multi_stock = "%ds";
  int one = 0;
  int k =0;
  if (sscanf(command, pattern_multi_move, &parsed.source_amount,
                    &parsed.source_index, &parsed.destination_index) == 3) {
    parsed.source = 't';
    parsed.destination = 't';
  } else if (sscanf(command, pattern_single_move_tf, &parsed.source_index,
                    &parsed.destination_index) == 2) {
    parsed.source = 't';
    parsed.destination = 'f';
    } else if (sscanf(command, pattern_single_move2, &parsed.source_index,
                    &parsed.destination_index) == 2) {
    parsed.source = 't';
    parsed.destination = 't';
  } else if (sscanf(command,  pattern_waste_move_wt, &one, &parsed.destination_index) == 2) {
    parsed.source = 'w';
    parsed.destination = 't';
    } else if (sscanf(command, pattern_waste_move_wf, &one, &parsed.destination_index) == 2) {
    parsed.source = 'w';
    parsed.destination = 'f';
  } else if (sscanf(command, pattern_multi_stock, &parsed.source_amount) == 1) {
    parsed.source = 's';
    parsed.destination = 's';
   k = sscanf(command, pattern_waste_move_wf, &one, &parsed.destination_index);
  } else if (strcmp(command, "quit\n") == 0) {
    parsed.success=-1;
  } else {
    parsed.success = 0;
  } return parsed;
}

// which pile 
pile *get_pile(game_state *state, char pile_prefix, int pile_index_one_based) {
  switch (pile_prefix) {
  case 's':
    return stock(state);
  case 'w':
    return waste(state);
  case 'f':
    return foundation(state, pile_index_one_based);
  case 't':
    return tableau(state, pile_index_one_based);
  default:
    return NULL;
  }
}
// maintaining score 
void add_score(game_state *state, int score) {
  state->score += score;
  if (state->score < 0) {state->score = 0;}
}

// move type enum 
enum { MOVE_OK, MOVE_INVALID_COMMAND, MOVE_SOURCE_EMPTY,
  MOVE_INVALID_MOVE, MOVE_TOO_MANY_CARDS, MOVE_CANNOT_REDEAL,
  MOVE_INVALID_DESTINATION,  MOVE_INVALID_SOURCE,QUIT_GAME };
// print move type 
char *move_results[] = {"OK", "Invalid command","Source pile empty",
                        "Invalid move","Too many cards to move!", "Cannot redeal, stock pile empty",
                        "Invalid destination", "Invalid source"};
// what happens when move is attempted 
void move_card(game_state *state, card *card, pile *source_pile,pile *destination_pile) {
  delete (source_pile, card);
  if (reveal(peek_last(source_pile))) { add_score(state, 5); }// turn over column card
  push(destination_pile, card);
  if (destination_pile->type == 'f') 
  {
    reveal(peek_last(destination_pile));
   add_score(state, 10); // add score for the moves
  }
  if (source_pile->type == 'w' && destination_pile->type == 't') add_score(state, 5);
}

// waste->stock  if waste not empty -> only call if stock empty 
void redeal(game_state *state) {
  while (!is_empty(waste(state))) {
    card *card = shift(waste(state));
    hide(card);
    push(stock(state), card);
  }
  add_score(state, -50);
}
// attempts to move 
int attempt_move(game_state *state, char *command) {  
  parsed_input parsed = parse_input(command);
  if (parsed.success != 1) 
  {
    if (parsed.success==-1) return QUIT_GAME;
    return MOVE_INVALID_COMMAND;
  }
  if((parsed.destination == 't' && ((parsed.destination_index > TABLEAU_COUNT) || (parsed.destination_index < 1)))
      || (parsed.destination == 'f' && ((parsed.destination_index > FOUNDATION_COUNT) || (parsed.destination_index < 1))))
      {
        return MOVE_INVALID_DESTINATION;
      }
  // source_index can also be broken
  if(parsed.source == 't' && ((parsed.source_index > TABLEAU_COUNT) || (parsed.source_index < 1)))
  {
 return MOVE_INVALID_SOURCE;
  }
  // figure out destination
  if (parsed.source == 's') {
    for (int i = 0; i < parsed.source_amount; i++) {
      if (is_empty(stock(state))) {  // try to redeal
        if (is_empty(waste(state))) return MOVE_CANNOT_REDEAL;
        redeal(state);
      }
      turn(state);
    }
    return MOVE_OK;
  }
  pile *source_pile = get_pile(state, parsed.source, parsed.source_index);
  pile *destination_pile = get_pile(state, parsed.destination, parsed.destination_index);
  // check if valid 
  if (is_empty(source_pile)) return MOVE_SOURCE_EMPTY;   
  if (source_pile->num_cards < parsed.source_amount) return MOVE_TOO_MANY_CARDS;
  int first_card_index = source_pile->num_cards - parsed.source_amount;
  if (parsed.source_amount >= 1) {  // multi-card 
    card *c = peek_card_at(source_pile, first_card_index); // check if all cards have been revealed
    if (c->revealed == 0) return MOVE_TOO_MANY_CARDS;
  }
  for (int card_index = 0; card_index < parsed.source_amount; card_index++) {
    // card index doesn't move - the card is always at the same index
    card *source_card = peek_card_at(source_pile, first_card_index);

    // check if the move is valid based on the destination type
    if (parsed.destination == 'f') {
      if (is_empty(destination_pile)) {
        if (source_card->rank == RANK_A) { // only ace goes if the foundation is empty
          move_card(state, source_card, source_pile, destination_pile);
        } else return MOVE_INVALID_MOVE;
      } else {// non-empty foundation, pick up the first card
        card *top_foundation_card = peek_last(destination_pile);
        if (can_be_placed_on_foundation(*top_foundation_card, *source_card)) {
          move_card(state, source_card, source_pile, destination_pile);
        } else {
          return MOVE_INVALID_MOVE;
        }
      }
    } 
    else if (parsed.destination == 't') {
      if (is_empty(destination_pile)) { // only king can go in an empty column
        if (source_card->rank == RANK_K) {
          move_card(state, source_card, source_pile, destination_pile);
        } else return MOVE_INVALID_MOVE;
      } else { 
        card *bottom_column_card = peek_last(destination_pile);
        if (can_be_placed_bottom(*bottom_column_card, *source_card)) {
          move_card(state, source_card, source_pile, destination_pile);
        } else return MOVE_INVALID_MOVE;
      }
    } 
    else
    {
        return MOVE_INVALID_DESTINATION;
    } 
} return MOVE_OK;
}

void mvprintf(int y, int x, const char *format, ...) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position = {x, y};
    va_list args;
    va_start(args, format);
    SetConsoleCursorPosition(hConsole, position);
    vprintf(format, args);
    va_end(args);
}

int win(game_state *state){
  int i=2;
  int count=0;
  game_state *dummy=state;
  for (i;i<=5;i++){
    if (dummy->piles[i]->num_cards==13){ count+=1;}
  }
  if (count ==4) return 1;
  else return 0;
}


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
    printf("\t ## ##    ## ##   ####       ####   #### ##     ##     ####   ### ##   ### ###   \n");
    printf("\t##   ##  ##   ##   ##         ##    # ## ##     ##      ##     ##  ##   ##  ##   \n");
    printf("\t####     ##   ##   ##         ##      ##      ## ##     ##     ##  ##   ##       \n");
    printf("\t #####   ##   ##   ##         ##      ##     ##  ##     ##     ## ##    ## ##    \n");
    printf("\t    ###  ##   ##   ##         ##      ##     ######     ##     ## ##    ##       \n");
    printf("\t##   ##  ##   ##   ##  ##     ##      ##    ##   ##     ##     ##  ##   ##  ##   \n");
    printf("\t ## ##    ## ##   ### ####  ######    ##   ###   ##    ####   ####  ### ### ###  \n");

}

void welcome_rules(){
    welcome_display();
    printf("\n\t\t\t\t\t(press enter to proceed) ");
    getchar();
    system("cls");
    printf("\t\t\tHOW TO MOVE CARDS AND SCORING SYSTEM \n\n");
    printf("1. for tableau to tableau move(+10) - <num of cards> t<number> t<number>\nexample :3 t2 t5\n\n");
    printf("2. for tableau to foundation move (+15)- t<number> f<number>\nexample :t1 f4\n\n");
    printf("3. for waste to tableau move(+5) - w0 t<number>\nexample :w0 t7\n\n");
    printf("4. for waste to foundation move(+10) - w0 f<number>\nexample :w0 f1\n\n");
    printf("5. to draw from stock - <number of cards> s\nexample :1s when redealed (-50)\n");
    printf("\n\t\t\t(press enter to proceed to game) ");
    getchar();
   
   
    
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







