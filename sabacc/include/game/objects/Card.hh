#ifndef __GAMECARD_HH
#define __GAMECARD_HH

// STL headers
#include <string>
#include <sstream>

// Sabacc headers
#include "game/CardConstants.hh"

class Card
  {
  public:

    Card();
    Card(short, unsigned short);
    Card(Card const &);

    Card &operator=(Card const &);

    // The comparison operators.
    bool operator==(Card const &);
    bool operator!=(Card const &);

    // The following comparison operators compare by value only.
    bool operator<(Card const &);
    bool operator>(Card const &);

    // Swaps the value and suite of this card with another.
    Card &Swap(Card &);

    // Value and suite accessors. The set functions return a reference to
    // this card for chaining of functions.
    short Value();
    Card &Value(short);

    unsigned short Suite();
    Card &Suite(unsigned short);

    bool Hold();
    Card &Hold(bool);

    // Get the names and values of cards.

    std::string Name();
    std::string LongName();
    std::string ValueString();
    std::string SuiteName();
    std::string DescriptiveName();

  private:

    short	value;		// Value of the card
    unsigned short	suite;		// Card suite
    bool	hold;		// Card being held by player

  }; // Card

bool operator==(Card const &, Card const &);
bool operator!=(Card const &, Card const &);
bool operator<(Card const &, Card const &);
bool operator>(Card const &, Card const &);

#endif // __GAMECARD_HH
