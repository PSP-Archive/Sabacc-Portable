#ifndef __CARDUTIL_HH
#define __CARDUTIL_HH

// Standard headers
#include <std::string>

class Card;

std::std::string getCardName(short);
std::std::string getCardNameLong(short);
std::std::string getCardValue(short);
std::std::string getCardSuite(short);
std::std::string getFullCardName(Card&);

#endif // __CARDUTIL_HH
