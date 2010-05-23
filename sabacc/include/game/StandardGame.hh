#ifndef __STANDARDGAME_HH
#define __STANDARDGAME_HH

// Standard library headers
#include <string>
#include <vector>

// SDL headers
#include <SDL_mixer.h>

// Sabacc headers
#include "controls/StaticImage.hh"
#include "controls/StaticTextLabel.hh"
#include "game/GameBase.hh"
#include "sitextensions/CardImageSet.hh"

class StandardGame : public GameBase {

    // UI stuff
    TextLabel				dealer_caption, player_caption, selected_info,
#if !defined(__PSP__) && !defined(__PSPTEST__)
    rules_text, help_text, round_caption, round_info;
#else
    rules_text, round_caption, round_info;
    StaticImage					help_text;
#endif

    CardImageSet			dealer_cimages, player_cimages;

    StaticImage					table_image;

    // A game cannot be copied or assigned
    StandardGame(const StandardGame&);
    StandardGame& operator=(const StandardGame&);

    // A function to update all labels and possibly other UI elements
    void updateUI();

    // The various rounds
    virtual void startNewRound();
    virtual void deal();
    virtual void shift();
    virtual void dealerCall();
    virtual void call();

public:

    StandardGame();
    StandardGame(std::vector<Player>);	// Vector of players with which to start the game
    StandardGame(std::vector<Player>, long);	// Vector of players, and a set amount each is to start at
    virtual ~StandardGame();

    // Hold a card
    virtual void hold(std::vector<Card>::iterator);
    virtual void hold(std::vector<Card>::size_type);

    // Start the game, run the event loop, &c.
    virtual void start();

};	// CasinoBase

#endif // __STANDARDGAME_HH
