#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define MAX_CARDS 52
#define MAX_SUIT 4
#define MAX_FACE 13

const char *suits[MAX_SUIT] = {"Spades", "Hearts", "Diamonds", "Clubs"};
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
      deck[i].value = face + 2;
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

void print_card(struct card cards[], int num_cards) {
  for (int i = 0; i < num_cards; i++) {
    printf("┌─────┐ ");
  }
  printf("\n");
  for (int i = 0; i < num_cards; i++) {
    printf("│%2s   │ ", faces[cards[i].face]);
  }
  printf("\n");
  for (int i = 0; i < num_cards; i++) {
    // ASCII art for the suits
    char *suit_art;
    if (cards[i].suit == 0) {
      suit_art = "♠";
    } else if (cards[i].suit == 1) {
      suit_art = "♥";
    } else if (cards[i].suit == 2) {
      suit_art = "♦";
    } else {
      suit_art = "♣";
    }
    printf("│  %s  │ ", suit_art);
  }
  printf("\n");
  for (int i = 0; i < num_cards; i++) {
    printf("│   %2s│ ", faces[cards[i].face]);
  }
  printf("\n");
  for (int i = 0; i < num_cards; i++) {
    printf("└─────┘ ");
  }
  printf("\n");
}


void draw_card(struct card *deck, struct card *hand, int *num_cards) {
  hand[*num_cards] = deck[*num_cards];
  (*num_cards)++;
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

  int player_total = 0;
  int dealer_total = 0;
  int num_player_cards = 0;
  int num_dealer_cards = 0;
  int balance = 100;

  while (balance > 0) {
    printf("Your balance: $%d\n", balance);
    printf("Enter bet: $");
    int bet;
    scanf("%d", &bet);
    if (bet > balance) {
      printf("You don't have that much money!\n");
      continue;
    }
    draw_card(deck, player, &num_player_cards);
    draw_card(deck, dealer, &num_dealer_cards);
    draw_card(deck, player, &num_player_cards);
    draw_card(deck, dealer, &num_dealer_cards);

    printf("Dealer's hand:\n");
    print_card(dealer, 1);
    printf("\nand an unknown card\n");
    printf("Your hand:\n");
    print_card(player, 2);
    // printf(" and\n");
    // print_card(player[1]);
    printf("\n");

    player_total = get_hand_total(player, num_player_cards);
    dealer_total = get_hand_total(dealer, num_dealer_cards);

    char response;
    while (player_total < 21) {
      printf("Hit or stand (h/s)? ");
      scanf(" %c", &response);
      if (response == 'h') {
        draw_card(deck, player, &num_player_cards);
        player_total = get_hand_total(player, num_player_cards);
        printf("Your hand:\n");
        for (int i = 0; i < num_player_cards; i++) {
          print_card(player,i);
        //   printf(" ");
        }
        printf("\n");
      } else if (response == 's') {
        break;
      }
    }

    if (player_total > 21) {
      printf("You bust!\n");
      balance -= bet;
    } else {
      printf("Dealer's hand:\n");
      for (int i = 0; i < num_dealer_cards; i++) {
        print_card(dealer, i);
        printf(" ");
      }
      printf("\n");

      while (dealer_total < 17) {
        draw_card(deck, dealer, &num_dealer_cards);
        dealer_total = get_hand_total(dealer, num_dealer_cards);
      }

      if (dealer_total > 21) {
        printf("Dealer busts! You win!\n");
        balance += bet;
      } else if (dealer_total > player_total) {
        printf("Dealer wins!\n");
        balance -= bet;
      } else if (dealer_total < player_total) {
        printf("You win!\n");
        balance += bet;
      } else {
        printf("Push\n");
      }
    }

    printf("\n");
  }

  printf("You are out of money!\n");

  return 0;
}

