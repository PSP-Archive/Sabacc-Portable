// Sabacc headers
#include "game/CardConstants.hh"
#include "game/objects/Card.hh"

Card::Card() : value(0), suite(SUITE_BLANK), selected(false), hold(false) { }
Card::Card(short val, unsigned short st) : value(val), suite(st), selected(false), hold(false) { }
Card::Card(const Card& source) : value(source.value), suite(source.suite), selected(false), hold(source.hold) { }

Card& Card::operator=(const Card& source) {
    this->value = source.value;
    this->suite = source.suite;
    this->hold = source.hold;

    this->selected = false;

    return(*this);
} // operator=
bool Card::operator==(Card& lhs) {
    return((lhs.value == this->value) && (lhs.suite == this->suite));
}	// operator==
bool Card::operator!=(Card& lhs) {
    return(!operator==(lhs));
}	// operator!=
bool Card::operator<(Card& rhs) {
    return(rhs.value > this->value);
}	// operator<
bool Card::operator>(Card& rhs) {
    return(rhs.value < this->value);
}	// operator>
