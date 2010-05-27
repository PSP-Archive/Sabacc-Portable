#ifndef __GAMECARD_HH
#define __GAMECARD_HH

// Sabacc headers
#include "game/CardConstants.hh"

struct Card {
    short	value;		// Value of the card
    short	suite;		// Card suite

    bool	selected;	// Is card selected?

    bool	hold;		// Card being held by player

    Card();
    Card(short, unsigned short);
    Card(const Card&);

    short operator+(const Card&);
    Card& operator=(const Card&);
    bool operator==(Card&);
    bool operator!=(Card&);
    bool operator<(Card&);
    bool operator>(Card&);

}; // Card

#endif // __GAMECARD_HH
