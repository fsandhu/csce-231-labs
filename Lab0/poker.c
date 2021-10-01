/* HOMEWORK 0
 * STUDENT: Fateh Sandhu 
*/

/* This simple program should populate a standard 52-card deck and then
 * randomly select five cards from the deck.  It will then print the
 * poker hand and report the best way it can be characterized (flush, full
 * house, one pair, high card, etc.)
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "card.h"


card deck[52];          // a "deck" of 52 cards


void populateDeck()
{
        /* ADD CODE HERE TO PLACE THE 52 STANDARD CARDS IN DECK[] */
        int j = 4;
        for (int i = 1 ; i < 14 ; i++) {
            card* clubsCard = malloc(sizeof(card));
            createCard(i, CLUBS, clubsCard);
            card* diamondsCard = malloc(sizeof(card));
            createCard(i, DIAMONDS, diamondsCard);
            card* heartsCard = malloc(sizeof(card));
            createCard(i, HEARTS, heartsCard);
            card* spadesCard = malloc(sizeof(card));
            createCard(i, SPADES, spadesCard);
            if (i == 1) {
                deck[0] = *clubsCard;
                deck[1] = *diamondsCard;
                deck[2] = *heartsCard;
                deck[3] = *spadesCard;
            } else {
                deck[j] = *clubsCard;
                deck[j+1] = *diamondsCard;
                deck[j+2] = *heartsCard;
                deck[j+3] = *spadesCard;
                j=j+4;
            }
        }
}


card* sort(card* subdeck, int size)
{
    card temp;
    for (int i = 0; i < size-1; i++)
        for (int j = i+1; j < size; j++)
            if (subdeck[i].value > subdeck[j].value)
            {
                temp = subdeck[i];
                subdeck[i] = subdeck[j];
                subdeck[j] = temp;
            }
    return subdeck;
}


card* getHand(card* emptyHand, int sizeOfHand)
/* Randomly selects "sizeOfHand" cards and returns them.  The emptyHand argument must
 * have space for at least "sizeOfHand" cards.
 */{
    int selection;
    for (int i = 0; i < sizeOfHand; i++)
    {
        selection = rand() % 52;
        emptyHand[i] = deck[selection];     // if this were more robust, we'd check for duplicates
    }
    return sort(emptyHand,sizeOfHand);
}


int isPair(card* hand, int sizeOfHand)
/* Returns 1 if two of the cards in "hand" have the same value; returns 0 otherwise.
 * "hand" must be sorted from least value to greatest value.
 */{
        /* STUDY THIS CODE.  WHY DOES IT PRODUCE THE INTENDED RESULT? */
    int pair = 0;
    for (int i = 0; i < sizeOfHand-1; i++)
        pair = pair || (hand[i].value == hand[i+1].value);   // because hand is sorted, a pair must be two adjacent cards
    return pair;
}


int isTwoPair(card* hand, int sizeOfHand)
/* Returns 1 if two of the cards in "hand" are a pair and two other are also a pair; returns 0 otherwise.
 * "hand" must be sorted from least value to greatest value.
 */{
    int numberOfPairs = 0;
    card* partialHand;
    int i=0;
    while (i < sizeOfHand)
    {                               /* RECALL THAT ARRAYS ARE POINTERS */
        partialHand = hand + i;     /* THIS IS CHANGING THE ADDRESS IN THE "PARTIALHAND" POINTER TO A DIFFERENT PART OF THE ARRAY */
        if (isPair(partialHand,2))
        {
            numberOfPairs++;
            i += 2;
        }
        else
        {
            i++;
        }
    }
    return (numberOfPairs == 2);
}


int isThreeOfKind(card* hand, int sizeOfHand)
/* Returns 1 if three of the cards in "hand" have the same value; returns 0 otherwise.
 * "hand" must be sorted from least value to greatest value.
 */{
        /* WRITE THIS FUNCTION */
    int threeOfKind = 0;
    for (int i = 0; i < sizeOfHand-2; i++)
        threeOfKind = threeOfKind || ((hand[i].value == hand[i+1].value) && (hand[i+1].value == hand[i+2].value));
    return threeOfKind;
}


int isStraight(card* hand, int sizeOfHand)
/* Returns 1 if the cards have contiguous values; returns 0 otherwise.
 * "hand" must be sorted from least value to greatest value.
 */{
        /* STUDY THIS CODE.  WHY DOES IT PRODUCE THE INTENDED RESULT? */
    int notStraight = 0;
    for (int i = 0; i < sizeOfHand-1; i++)
        notStraight = notStraight + (hand[i+1].value - hand[i].value - 1);
    return !notStraight;
}


int isFlush(card* hand, int sizeOfHand)
/* Returns 1 if the cards all have the same suit; returns 0 otherwise.
 */{
        /* STUDY THIS CODE.  WHY DOES IT PRODUCE THE INTENDED RESULT? */
    int flush = 1;
    int suit = hand[0].suit;
    for (int i = 1; i < sizeOfHand; i++)
        flush = flush && (hand[i].suit == suit);
    return flush;
}


int isFullHouse(card* hand, int sizeOfHand)
/* Returns 1 if three of the  cards in "hand" are three of a kind and another two are a pair; returns 0 otherwise.
 * "hand" must be sorted from least value to greatest value.
 */{
        /* WRITE THIS FUNCTION */
        int fullHouse = 0;
        for (int i = 0; i < sizeOfHand-4; i++) {
            fullHouse = fullHouse || 
                        ((hand[i].value == hand[i+1].value) && (hand[i+1].value == hand[i+2].value) && (hand[i+3].value == hand[i+4].value)) ||
                        ((hand[i].value == hand[i+1].value) && (hand[i+2].value == hand[i+3].value) && (hand[i+3].value == hand[i+4].value));
        }
    return fullHouse;
}


int isFourOfKind(card* hand, int sizeOfHand)
/* Returns 1 if four of the cards in "hand" have the same value; returns 0 otherwise.
 * "hand" must be sorted from least value to greatest value.
 */{
        /* WRITE THIS FUNCTION */
    int fourOfKind = 0;
    for (int i = 0; i < sizeOfHand-3; i++)
        fourOfKind = fourOfKind || ((hand[i].value == hand[i+1].value) && (hand[i+1].value == hand[i+2].value) && (hand[i+2].value == hand[i+3].value));
    return fourOfKind;
}


int isStraightFlush(card* hand, int sizeOfHand)
/* Returns 1 if the cards in "hand" are both a straight and a flush; returns 0 otherwise.
 * "hand" must be sorted from least value to greatest value.
 */{
    return isStraight(hand,sizeOfHand) && isFlush(hand,sizeOfHand);
}


int main(int argc, char const *argv[])
{
    srand(time(NULL));
    populateDeck();
    int sizeOfHand = 5;
    card* hand = malloc(sizeof(card)*sizeOfHand);
    getHand(hand, sizeOfHand);
    printf("Your hand is : \n\n");

    for (int i = 0 ; i < sizeOfHand ; i++) {
        char *s = malloc(21);
        printf("%s\n",displayCard(&hand[i],s));
    }

    printf("\n");

    if (isStraightFlush(hand, sizeOfHand)) {
        printf("The hand is a STRAIGHT FLUSH\n\n");
    } else if (isFourOfKind(hand, sizeOfHand)) {
        printf("The hand is a FOUR OF KIND\n\n");
    } else if (isFullHouse(hand, sizeOfHand)) {
        printf("The hand is a FULL HOUSE\n\n");
    } else if (isFlush(hand, sizeOfHand)) {
        printf("The hand is a FLUSH\n\n");
    } else if (isStraight(hand, sizeOfHand)) {
        printf("The hand is a STRAIGHT\n\n");
    } else if (isThreeOfKind(hand, sizeOfHand)) {
        printf("The hand is a THREE OF KIND\n\n");
    } else if (isPair(hand, sizeOfHand)) {
        printf("The hand is a PAIR\n\n");
    } else {
        printf("UH OH! No luck! \n\n");
    }

    return 0;
}
