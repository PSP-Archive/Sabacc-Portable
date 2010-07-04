// STL headers
#include <cstdio>
#include <string>
#include <sstream>

// Sabacc headers
#include "game/objects/Card.hh"
#include "game/CardConstants.hh"

#if _DEBUG || _DEBUGCARD
#include "utility/Log.hh"
#endif

Card::Card() :
    value(0), suite(0), hold(false)
{ }
Card::Card(short val, unsigned short ste) :
    value(val), suite(ste), hold(false)
{ }
Card::Card(Card const &other) :
    value(other.value), suite(other.suite), hold(false)
{ }

Card
&Card::operator=(Card const &other)
{
  if (&other != this)
    {
      value = other.value;
      suite = other.suite;
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
  return *this;
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
  if (1 > value)
    {
      return face_names_neg[value + face_neg_offset];
    }
  else if (11 < value)
    {
      return face_names_pos[value + face_pos_offset];
    }

  // Finally just return the value as a string.
  char card_value[2];
  sprintf(card_value, "%d", value);
  return card_value;
}	// Name

std::string
Card::LongName()
// Returns a more descriptive name.
{

  if (1 > value)
    {
      return face_names_neg_full[value + face_neg_offset];
    }
  else if (11 < value)
    {
      return face_names_pos_full[value + face_pos_offset];
    }

  // Finally just return the value as a string.
  char card_value[6];
  sprintf(card_value, "The %d", value);
  return card_value;
}// LongName

std::string
Card::ValueString()
{
  char card_value[2];
  sprintf(card_value, "%d", value);
  return card_value;
}	// ValueString

std::string
Card::SuiteName()
{
  return suite_names[suite];
}	// getCardSuite

std::string
Card::DescriptiveName()
// Returns a descriptive name for the card such as "The 2 of Staves."
{
  std::string full_name = LongName();
  if ((0 < suite) && (5 > suite)) full_name += " of " + SuiteName();
  if (hold) full_name += " (card is being held)";

  return full_name;
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
