#ifndef __CARDUTIL_HH
#define __CARDUTIL_HH

// Standard headers
#include <string>

class Card;

std::string getCardName(short);
std::string getCardNameLong(short);
std::string getCardValue(short);
std::string getCardSuite(short);
std::string getFullCardName(Card&);

#endif // __CARDUTIL_HH
