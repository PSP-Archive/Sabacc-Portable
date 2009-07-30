#ifndef __RULESET_HH
#define __RULESET_HH

// Standard headers

// SDL headers

// Sabacc headers
#include "utility/Singleton.hh"

enum SabaccOptions {
    SABACC_23_ONLY = 0,	// Sabacc/Pure sabacc is only +-23
    SABACC_46,			// Sabacc is +-23 or +-46
    SABACC_PURE_23		// Sabacc is +-46, Pure is +-23, Pure scores better
};

enum BustOptions {
    BUST_ZERO_ONLY = 0,	// Only hands totalling zero are a bust
    BUST_ZERO_OVER_23,	// Hands totalling zero or under -23/over 23 are busts
    BUST_ZERO_OVER_46	// Hands totalling zero or outside +-46 are busts
};

class Ruleset {
    // Chip related rules
    unsigned int		chip_size;	// Number of credits a chip represents

    // Betting rules
    unsigned int		min_bet;	// Minimum bet in number of chips
    unsigned int		max_bet;	// Maximum bet in number of chips

    // Pot rules
    unsigned long		hand_pot_max;	// The maximum amount in CREDITS allowed in the hand pot, once exceeded, see rule below.
    bool				house_keeps_hand_excess;	// The house keeps any chips above hand pot maximum. If false, hand pot maximum is invalid and pot grows infinitely.
    unsigned long		sabacc_pot_max;	// The maximum amount in CREDITS of the Sabacc pot, the house keeping remainder.

    // Hand rules
    BustOptions			bust_rule;	// Determines how a bust is ruled.
    SabaccOptions		sabacc_rule;	// Determines what exactly sabacc is.

public:
    Ruleset();
    Ruleset(unsigned int, unsigned int, unsigned int, unsigned long, bool, unsigned long, BustOptions, SabaccOptions);
    ~Ruleset();

    // Chip size of the establishment
    unsigned int getChipSize();
    void setChipSize(unsigned int);

    // Betting rules
    unsigned int getMinBet();
    void setMinBet(unsigned int);
    unsigned int getMaxBet();
    void setMaxBet(unsigned int);

    // House rules
    unsigned long getHandMax();
    void setHandMax(unsigned long);
    bool getGreedyHouse();
    void setGreedyHouse(bool);
    unsigned long getSabaccMax();
    void setSabaccMax(unsigned long);
    BustOptions getBustRule();
    void setBustRule(BustOptions);
    SabaccOptions getSabaccRule();
    void setSabaccRule(SabaccOptions);

}; // class Ruleset

Ruleset& getRules();

#endif // __RULESET_HH
