/* HOMEWORK 0
 * STUDENT: Fateh Sandhu
*/

/* This source code, along with its header, defines a card and provides a
 * couple of operations for cards
*/

#include <stdio.h>
#include <stdlib.h>
#include "card.h"


card* createCard( int value, suits suit, card* blankCard )
/* Assigns the provided values to a blank card, making it no longer blank.
 * Returns the no-longer-blank card.
 */{
        /* ADD CODE HERE TO ASSIGN VALUE AND SUIT TO BLANKCARD'S FIELDS */
        blankCard->value = value;
        blankCard->suit = suit;

    return blankCard;
}


char* displayCard( card* theCard, char *displayString )
/* Places the printable representation of theCard into displayString and returns the string.
 * The argument displayString must have at least 21 bytes allocated.
 */{
        /* REPLACE WITH AN ASSIGNMENT TO THECARD'S VALUE */
    int value = theCard->value;
        /* REPLACE WITH AN ASSIGNMENT TO THECARD'S SUIT */
    suits suit = theCard->suit;
    char *valueString;
    char *suitString;
    switch (suit)
    {
        case CLUBS:
            suitString = "CLUBS";
            break;
        case DIAMONDS:
            suitString = "DIAMONDS";
            break;
        case HEARTS:
            suitString = "HEARTS";
            break;
        case SPADES:
            suitString = "SPADES";
            break;
        default:
            suitString = "UNKNOWN";
    }
    if ((value<1)||(value>13))              // Illegal values
    {
        valueString = "UNKNOWN";
    }
    else if ((value>1)&&(value<11))         // Number card
    {
        valueString = malloc(3);
            /* PLACE THE CONTROL STRING IN THE SECOND ARGUMENT THAT YOU WOULD USE TO PRINT AN INTEGER */
        sprintf(valueString, "%d", value);
    }
    else                                    // Ace or face card
    {
        switch (value)
        {
            case 1:
                valueString = "ACE";
                break;
            case 11:
                valueString = "JACK";
                break;
            case 12:
                valueString = "QUEEN";
                break;
            case 13:
                valueString = "KING";
                break;
            default:
                valueString = "DEADCODE";   // This line is unreachable
        }
    }
    sprintf(displayString, "%s of %s", valueString, suitString);
    if ((value>1)&&(value<11)) free(valueString);
    return displayString;
}


/*
int main()
{
    card *c = malloc(sizeof(card));
    c = createCard(3,HEARTS,c);
    char *s = malloc(21);
    printf("%s\n",displayCard(c,s));
}
*/
