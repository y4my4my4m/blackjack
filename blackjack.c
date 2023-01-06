#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

#define MAX_CARDS 52
#define MAX_SUIT 4
#define MAX_FACE 13
#define BLACK "\033[1;30m"
#define RED   "\033[1;31m"
#define GREEN "\033[1;32m"
#define WHITE "\033[1;37m"

const char *suits[MAX_SUIT] = {"♠", "♥", "♣", "♦"};
const char *faces[MAX_FACE] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};

struct card {
  int face;
  int suit;
  int value;
};

struct card deck[MAX_CARDS];
struct card player[MAX_CARDS];
struct card dealer[MAX_CARDS];

void build_deck(struct card *deck) {
  int i = 0;
  for (int suit = 0; suit < MAX_SUIT; suit++) {
    for (int face = 0; face < MAX_FACE; face++) {
      deck[i].face = face;
      deck[i].suit = suit;
      if (face < 9) {
        // Face cards 2-9 have a value equal to their face value
        deck[i].value = face + 2;
      } else if (face < 12) {
        // Face cards J, Q, K have a value of 10
        deck[i].value = 10;
      } else {
        // Ace has a value of 11
        deck[i].value = 11;
      }
      i++;
    }
  }
}

void shuffle_deck(struct card *deck) {
  for (int i = 0; i < MAX_CARDS; i++) {
    int j = rand() % MAX_CARDS;
    struct card temp = deck[i];
    deck[i] = deck[j];
    deck[j] = temp;
  }
}

void check_color(int value) {
  if (value % 2 == 1) {
    printf("%s",RED);
  }
  else {
    printf("%s",BLACK);
  }
}

void print_card(struct card *cards, int num_cards) {
  for (int i = 0; i < num_cards; i++) {
    check_color(cards[i].suit);
    printf("┌─────┐ ");
  }
  printf("\n");
  for (int i = 0; i < num_cards; i++) {
    check_color(cards[i].suit);
    printf("│%2s   │ ", faces[cards[i].face]);
  }
  printf("\n");
  for (int i = 0; i < num_cards; i++) {
    // ASCII art for the suits
    check_color(cards[i].suit);
    printf("│  %s  │ ", suits[cards[i].suit]);
  }
  printf("\n");
  for (int i = 0; i < num_cards; i++) {
    check_color(cards[i].suit);
    printf("│   %2s│ ", faces[cards[i].face]);
  }
  printf("\n");
  for (int i = 0; i < num_cards; i++) {
    check_color(cards[i].suit);
    printf("└─────┘ ");
  }
  printf("\n");
  printf("%s",WHITE);
}


void draw_card(struct card *deck, struct card *hand, int *num_cards, int *deck_pos, int hand_pos) {
  if (hand_pos >= MAX_CARDS || *deck_pos >= MAX_CARDS) {
    printf("Shuffling the remaining cards in the deck...\n");
    shuffle_deck(deck);
    *num_cards = 0;
    *deck_pos = 0;
  }
  hand[hand_pos] = deck[*deck_pos];
  (*num_cards)++;
  (*deck_pos)++;
}

int get_hand_total(struct card *hand, int num_cards) {
  int total = 0;
  int num_aces = 0;
  for (int i = 0; i < num_cards; i++) {
    total += hand[i].value;
    if (hand[i].value == 11) {
      num_aces++;
    }
  }
  while (total > 21 && num_aces > 0) {
    total -= 10;
    num_aces--;
  }
  return total;
}

int main(void) {
  srand(time(NULL));
  build_deck(deck);
  shuffle_deck(deck);

  int deck_pos = 0;
  int player_total = 0;
  int dealer_total = 0;
  int num_player_cards = 0;
  int num_dealer_cards = 0;
  int balance = 100;

  while (balance > 0) {
    printf("%s",WHITE);
    printf("Your balance: $%d\n", balance);
    printf("Enter bet: $%s", GREEN);
    int bet;
    scanf("%d", &bet);
    if (bet > balance) {
      printf("You don't have that much money!\n");
      continue;
    }
    system("clear");
    draw_card(deck, player, &num_player_cards, &deck_pos, num_player_cards);
    draw_card(deck, dealer, &num_dealer_cards, &deck_pos, num_dealer_cards);
    draw_card(deck, player, &num_player_cards, &deck_pos, num_player_cards);
    draw_card(deck, dealer, &num_dealer_cards, &deck_pos, num_dealer_cards);

    printf("%s",WHITE);
    printf("Dealer's hand:\n");
    print_card(dealer, 1);
    printf(" and a blank card\n");
    printf("Your hand:\n");
    print_card(player, num_player_cards);
    player_total = get_hand_total(player, num_player_cards);
    printf("Balance: $%d | Bet: $%d\n", balance, bet);
    printf("Total: %d\n", player_total);

    char response;
    do {
      printf("Hit or stand (h/s)? ");
      scanf(" %c", &response);
      if (response == 'h') {
        system("clear");
        printf("Dealer's hand:\n");
        print_card(dealer, num_dealer_cards);
        draw_card(deck, player, &num_player_cards, &deck_pos, num_player_cards);
        printf("Dealer's: %d\n", dealer_total);
        printf("Your hand:\n");
        print_card(player, num_player_cards);
        player_total = get_hand_total(player, num_player_cards);
        printf("Balance: $%d | Beting: $%d\n", balance, bet);
        printf("Yours: %d\n", player_total);
      }
    } while (response == 'h' && player_total < 21);

    if (player_total > 21) {
      system("clear");
      printf("Dealer's hand:\n");
      print_card(dealer, num_dealer_cards);
      dealer_total = get_hand_total(dealer, num_dealer_cards);
      printf("Dealer's: %d\n", dealer_total);
      printf("Your hand:\n");
      print_card(player, num_player_cards);
      printf("Bust! %sYou lose.\n", RED);
      player_total = get_hand_total(player, num_player_cards);
      printf("Yours: %d\n", player_total);
      balance -= bet;
      num_player_cards = 0;
      num_dealer_cards = 0;
      memset(player, 0, sizeof(player));
      memset(dealer, 0, sizeof(dealer));
      continue;
    }

    dealer_total = get_hand_total(dealer, num_dealer_cards);
    printf("Dealer's turn.\n");
    print_card(dealer, num_dealer_cards);
    while (dealer_total < 17) {
      draw_card(deck, dealer, &num_dealer_cards, &deck_pos, num_dealer_cards);
      print_card(dealer, num_dealer_cards);
      dealer_total = get_hand_total(dealer, num_dealer_cards);
      printf("Total: %d\n", dealer_total);
    }

system("clear");
printf("Dealer's hand:\n");
print_card(dealer, num_dealer_cards);
dealer_total = get_hand_total(dealer, num_dealer_cards);
printf("Total: %d\n", dealer_total);
printf("Your hand:\n");
print_card(player, num_player_cards);
player_total = get_hand_total(player, num_player_cards);
printf("Yours: %d\n", player_total);
if (dealer_total > 21) {
  printf("Dealer busts! %sYou win.\n", GREEN);
  balance += bet;
} else if (dealer_total > player_total) {
  printf("%sDealer wins.\n", RED);
  balance -= bet;
} else if (dealer_total < player_total) {
  printf("%sYou win!\n", GREEN);
  balance += bet;
} else {
  printf("Push.\n");
}
num_player_cards = 0;
num_dealer_cards = 0;
memset(player, 0, sizeof(player));
memset(dealer, 0, sizeof(dealer));
dealer_total = get_hand_total(dealer, num_dealer_cards);
player_total = get_hand_total(player, num_player_cards);
}

printf("Game over. You lost all your money.\n");

return 0;
}
