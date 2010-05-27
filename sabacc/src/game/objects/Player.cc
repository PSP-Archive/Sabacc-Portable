// Standard library headers
#include <algorithm>
#include <numeric>
#include <string>
#include <vector>

// Sabacc headers
#include "game/CardConstants.hh"
#include "game/objects/Card.hh"
#include "game/objects/Player.hh"
#include "exceptions/GamePlay.hh"

// Debug
#if defined(_DEBUGPLAYER) || defined(_DEBUGPLAYERTOTAL)
#include "utility/Log.hh"
#endif // _DEBUGPLAYER

Player::Player() : name("Player"), credits(1000), hand(), selected_card(),
        race(), occupation(), wins(0), losses(0), streak(0), total_winnings(0),
        avg_winnings(0), rank(0.0f) {}
Player::Player(std::string pl_name) : name(pl_name), credits(1000), hand(), selected_card(),
        race(), occupation(), wins(0), losses(0), streak(0), total_winnings(0),
        avg_winnings(0), rank(0.0f) {}
Player::Player(std::string pl_name, std::string pl_race, std::string pl_occu) :
        name(pl_name), credits(1000), hand(), selected_card(),
        race(pl_race), occupation(pl_occu), wins(0), losses(0), streak(0),
        total_winnings(0), avg_winnings(0), rank(0.0f) {}
Player::Player(std::string pl_name, long cred) : name(pl_name), credits(cred), hand(), selected_card(),
        race(), occupation(), wins(0), losses(0), streak(0), total_winnings(0),
        avg_winnings(0), rank(0.0f) {}
Player::Player(std::string pl_name, long cred, std::string pl_race, std::string pl_occu) :
        name(pl_name), credits(cred), hand(), selected_card(),
        race(pl_race), occupation(pl_occu), wins(0), losses(0), streak(0),
        total_winnings(0), avg_winnings(0), rank(0.0f) {}
Player::Player(const Player& p) : name(p.name), credits(p.credits), hand(p.hand),
        selected_card(p.selected_card), race(p.race), occupation(p.occupation),
        wins(p.wins), losses(p.losses), streak(p.streak), total_winnings(p.total_winnings),
        avg_winnings(p.avg_winnings), rank(p.rank) {}

Player& Player::operator=(const Player& p) {
    name = p.name;
    credits = p.credits;
    hand = p.hand;
    selected_card = p.selected_card;
    race = p.race;
    occupation = p.occupation;
    wins = p.wins;
    losses = p.losses;
    streak = p.streak;
    total_winnings = p.total_winnings;
    avg_winnings = p.avg_winnings;
    rank = p.rank;

    return(*this);
}	// operator=
bool Player::operator==(const Player& lhs) {
    // Check each attribute possible, and return if any are false
    // In multiplayer game, names mus be unique, so this should be all necessary
    // but we'll be safe here
    if (lhs.name != this->name) return(false);
    if (lhs.credits != this->credits) return(false);

    // return true if we get this far
    return(true);
}	// operator==

std::string Player::getName() {
    return(name);
}
void Player::setName(std::string name) {
    this->name = name;
}
long Player::getCredits() {
    return(credits);
}
void Player::setCredits(long credits) {
    this->credits = credits;
}
void Player::decCredits(long dec) {
    credits -= dec;
}
void Player::incCredits(long inc) {
    credits += inc;
}
std::string Player::getRace() {
    return(race);
}
void Player::setRace(std::string race) {
    this->race = race;
}
std::string Player::getOccupation() {
    return(occupation);
}
void Player::setOccupation(std::string occupation) {
    this->occupation = occupation;
}

unsigned int Player::addWin() {
    // Adds win to the stats win variable and returns the new value
    // as well as updates the streak data
    streak = (0 < streak) ? ++streak : 1;
    return(++wins);
} // addWin

unsigned int Player::addWin(long credits = 0) {
    // Adds win to the stats win variable and returns the new value
    // as well as updates the streak data
    streak = (0 < streak) ? ++streak : 1;
    total_winnings += credits;
    return(++wins);
} // addWin

unsigned int Player::getWins() {
    return(wins);
}

unsigned int Player::addLoss() {
    // Adds loss to the stats loss variable and returns the new value
    // as well as updates the streak data
    streak = (0 > streak) ? --streak : -1;
    return(++losses);
} // addLoss

unsigned int Player::addLoss(long credits = 0) {
    // Adds loss to the stats loss variable and returns the new value
    // as well as updates the streak data
    streak = (0 > streak) ? --streak : -1;
    total_winnings -= credits;
    return(++losses);
} // addLoss

unsigned int Player::getLosses() {
    return(losses);
}
int Player::getStreak() {
    return(streak);
}
long Player::getToalWinnings() {
    return(total_winnings);
}
double Player::getAvgWinnings() {
    return(avg_winnings);
}
float Player::getRank() {
    return((wins-losses) / wins);
}

void Player::moveSelectedLeft() {

    if (hand.begin() > --selected_card) selected_card = hand.begin();

#ifdef _DEBUGPLAYER
    logAppend("Selecting previous card.");
#endif // _DEBUGPLAYER

}	// moveLeft

void Player::moveSelectedRight() {

    if (hand.end() <= ++selected_card) {
        selected_card = hand.end() - 1;
    }

#ifdef _DEBUGPLAYER
    logAppend("Selecting next card.");
#endif // _DEBUGPLAYER

}	// moveRight

Card& Player::getSelectedCard() {
    return(*selected_card);
}	// getSelectedIter

std::vector<Card>::iterator Player::getSelectedCardIterator() {
    return(selected_card);
}
void Player::holdCard() {
    if (!selected_card->hold) {
        // Check before allowing card to be held
        if (2 > getHoldCount()) {
            selected_card->hold = true;
        } else {
            throw(Exceptions::GamePlay::CannotHoldMore(__FILE__, "Only two cards may be held."));
        }
    } else {
        selected_card->hold = false;
    }
}	// holdCard()
int Player::getHoldCount() {
    int hold_counter = 0;

    for (std::vector<Card>::iterator it = hand.begin(); hand.end() > it; ++it) {
        if (it->hold) ++hold_counter;
    }

    return(hold_counter);
}

void Player::dropCard() {	// Drops the selected card
    hand.erase(selected_card);
}	// dropCard
void Player::dropCard(int number) {
    // if(hand.size() < number) hand.erase(number);
}
void Player::dropCard(std::vector<Card>::iterator card_iterator) {
    hand.erase(card_iterator);
}
bool Player::takeCard(Card& c) {
    /* A player can hold a maximum of four cards in Basics.
     * If this is met, check to see if any cards are blank, as these are placeholders
     * for cards possibly removed from the hand. */
    if (4 > hand.size()) {
        hand.push_back(c);
    } else {
        return(false);
    }	// if(hand < 4 cards)

    // if(hand.size() > 0)
    selected_card = hand.begin();
    return(true);
}

std::vector<Card>& Player::getHand() {
    return(hand);
}

void Player::emptyHand() { hand.clear(); }

short Player::getHandTotal() {

#ifdef _DEBUGPLAYERTOTAL
    char player_value[64];
    logAppend("Summing player cards.");
#endif

    short sum = 0;

    // A lambda for the hell of it, for testing GCC 4.5.0.
    for_each (hand.begin(), hand.end(),
	      [&sum](Card &c) { sum += c.value; });

    // for (std::vector<Card>::iterator i = hand.begin();
    // 	 hand.end() != i; ++i) {
    // 	sum += i->value;
    // }
    
    return(sum);
} // getHandTotal

bool Player::pureSabacc() {
    return((23 == getHandTotal()) || (-23 == getHandTotal()));
} // pureSabacc

bool Player::sabacc() {
    /* This version is dependant on the rules being used, as in some variations
     * +-46 is also considered Sabacc. */

    return((23 == getHandTotal()) || (-23 == getHandTotal()));
} // sabacc

bool Player::idiotArray() {
    // Player has an idiot, and cards valued at 2 and 3.

    bool has_zero = false;
    bool has_two = false;
    bool has_three = false;

    for (std::vector<Card>::iterator cd_it = hand.begin(); hand.end() > cd_it; ++cd_it) {
      if ((0 == cd_it->value) && (SUITE_BLANK != cd_it->suite)) {
	has_zero = true;
      } else if (2 == cd_it->value) {
	has_two = true;
      } else if (3 == cd_it->value) {
	has_three = true;
      }// if(0, 2, 3)
    }	// for (card)

    return(has_zero && has_two && has_three);
} // idiotArray

bool Player::bomb() {
    /* A bomb  is dependent on the rules being used to play. Zero is always a
     * bomb, however in some variants, outside of the Sabacc range is as well. */

  return((0 == getHandTotal()) 
	 || (-23 > getHandTotal())
	 || (23 < getHandTotal()));
} // bomb
