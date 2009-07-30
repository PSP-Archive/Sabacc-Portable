// Sabacc headers
#include "game/Ruleset.hh"

Ruleset::Ruleset() :
        chip_size(5), min_bet(1), max_bet(10), hand_pot_max(500), house_keeps_hand_excess(false),
        sabacc_pot_max(1000000), bust_rule(BUST_ZERO_ONLY), sabacc_rule(SABACC_23_ONLY) { }
Ruleset::Ruleset(unsigned int ch_size, unsigned int n_bet, unsigned int x_bet, unsigned long h_max,
                 bool greedy_house, unsigned long s_max, BustOptions b_rule, SabaccOptions s_rule) :
        chip_size(ch_size), min_bet(n_bet), max_bet(x_bet), hand_pot_max(h_max), house_keeps_hand_excess(greedy_house),
        sabacc_pot_max(s_max), bust_rule(b_rule), sabacc_rule(s_rule) { }
Ruleset::~Ruleset() { }

unsigned int Ruleset::getChipSize() {
    return(chip_size);
}
void Ruleset::setChipSize(unsigned int chip_size) {
    this->chip_size = chip_size;
}

unsigned int Ruleset::getMinBet() {
    return(min_bet);
}
void Ruleset::setMinBet(unsigned int bet) {
    this->min_bet = bet;
}
unsigned int Ruleset::getMaxBet() {
    return(max_bet);
}
void Ruleset::setMaxBet(unsigned int bet) {
    this->max_bet = bet;
}

unsigned long Ruleset::getHandMax() {
    return(hand_pot_max);
}
void Ruleset::setHandMax(unsigned long max) {
    this->hand_pot_max = max;
}

bool Ruleset::getGreedyHouse() {
    return(house_keeps_hand_excess);
}
void Ruleset::setGreedyHouse(bool greedy) {
    this->house_keeps_hand_excess = greedy;
}

unsigned long Ruleset::getSabaccMax() {
    return(sabacc_pot_max);
}
void Ruleset::setSabaccMax(unsigned long max) {
    this->sabacc_pot_max = max;
}

BustOptions Ruleset::getBustRule() {
    return(bust_rule);
}
void Ruleset::setBustRule(BustOptions rule) {
    this->bust_rule = rule;
}

SabaccOptions Ruleset::getSabaccRule() {
    return(sabacc_rule);
}
void Ruleset::setSabaccRule(SabaccOptions rule) {
    this->sabacc_rule = rule;
}

Ruleset& getRules() {
    static Ruleset rules;
    return(rules);
}	// getRules
