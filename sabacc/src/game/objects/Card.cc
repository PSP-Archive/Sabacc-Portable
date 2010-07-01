// STL headers
#include <string>
#include <sstream>

// Sabacc headers
#include "game/objects/Card.hh"
#include "game/CardConstants.hh"

Card::Card() :
    value(0), suite(0), selected(false), hold(false) { }
Card::Card(short val, unsigned short ste) :
    value(val), suite(ste), selected(false), hold(false) { }
Card::Card(Card const &other) :
    value(other.value), suite(other.suite), selected(false),
    hold(false) { }

Card
&Card::operator=(Card const &other)
{
    if (&other != this)
    {
	value = other.value;
	suite = other.suite;
	selected = false;
	hold = false;
    }
	    
    return *this;
}

// The comparison operators.

bool
Card::operator==(Card const &other)
{
    return (value = other.value) && (suite == other.suite);
}
    
bool
Card::operator!=(Card const &other)
{
    return !operator==(other);
}

// The following comparison operators compare by value only.
    
bool
Card::operator<(Card const &other)
{
    return value < other.value;
}
		
bool
Card::operator>(Card const &other)
{
    return value > other.value;
}

Card
&Card::Swap(Card &other)
{
    Card swapper(other);
    other.value = value;
    other.suite = suite;
    *this = swapper;
}
// Value and suite accessors. The set functions return a reference to
// this card for chaining of functions.

short
Card::Value()
{
    return value;
}
    
Card
&Card::Value(short v)
{
    value = v;
    return *this;
}

unsigned short
Card::Suite()
{
    return suite;
}
    
Card
&Card::Suite(unsigned short s)
{
    suite = s;
    return *this;
}

bool
Card::Selected()
{
    return selected;
}

Card
&Card::Selected(bool s)
{
    selected = s;
    return *this;
}
    
bool
Card::Hold()
{
    return hold;
}
    
Card
&Card::Hold(bool h)
{
    hold = h;
    return *this;
}


// Member functions that aren't required to be inline.
std::string
Card::Name()
{
    if (1 > value) {
	return(face_names_neg[value + face_neg_offset]);
    } else if (11 < value) {
	return(face_names_pos[value + face_pos_offset]);
    }
    
    // Finally just return the value as a string.
    std::stringstream card_value;
    card_value << value;
    
    return(card_value.str());
    
}	// Name

std::string
Card::LongName()
// Returns a more descriptive name.
{

    if (1 > value) {
	return(face_names_neg_full[value + face_neg_offset]);
    } else if (11 < value) {
	return(face_names_pos_full[value + face_pos_offset]);
    }

    std::stringstream card_value;
    card_value << "The ";
    card_value << value;

    return(card_value.str());

}// LongName

std::string
Card::ValueString()
{
    std::stringstream card_value;
    card_value << value;
	    
    return(card_value.str());
}	// ValueString

std::string
Card::SuiteName()
{
    return(suite_names[suite]);
}	// getCardSuite

std::string
Card::DescriptiveName()
// Returns a descriptive name for the card such as "The 2 of Staves."
{
    std::string full_name = LongName();
    if ((0 < suite) && (5 > suite)) full_name += " of " + SuiteName();
    if (hold) full_name += " (card is being held)";

    return(full_name);
}	// getFullcardName

// Non member comparison operators

bool
operator==(Card const &first, Card const &second)
{
    return first == second;
}
    
bool
operator!=(Card const &first, Card const &second)
{
    return first != second;
}
    
bool
operator<(Card const &first, Card const &second)
{
    return first < second;
}
		
bool
operator>(Card const &first, Card const &second)
{
    return first > second;
}
