// Standard headers
#include <sstream>
#include <string>

// Sabacc headers
#include "game/CardConstants.hh"
#include "game/CardUtil.hh"
#include "game/objects/Card.hh"

#if defined(_DEBUGCARDUTIL)
#include <cstdio>
#include "utility/log.hh"
#endif

std::string getCardName(short value) {

    if (1 > value) {
        return(face_names_neg[value + face_neg_offset]);
    } else if (11 < value) {
        return(face_names_pos[value + face_pos_offset]);
    }

    std::stringstream card_value;
    card_value << value;

    return(card_value.str());

}	// getCardName
std::string getCardNameLong(short value) {

    if (1 > value) {
        return(face_names_neg_full[value + face_neg_offset]);
    } else if (11 < value) {
        return(face_names_pos_full[value + face_pos_offset]);
    }

    std::stringstream card_value;
    card_value << "The ";
    card_value << value;

    return(card_value.str());

}	// getCardName
std::string getCardValue(short value) {

    std::stringstream card_value;
    card_value << value;

    return(card_value.str());

}	// getCardName
std::string getCardSuite(short suite) {
    return(suite_names[suite]);
}	// getCardSuite
std::string getFullCardName(Card& c) {

    std::string full_name = getCardNameLong(c.value);

    if ((0 < c.suite) && (5 > c.suite)) {
        full_name += " of " + getCardSuite(c.suite);
    }

    full_name += " (" + getCardValue(c.value) + ")";

    return(full_name);

}	// getFullcardName
