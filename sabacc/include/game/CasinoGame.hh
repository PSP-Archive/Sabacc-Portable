#ifndef __CASINOGAME_HH
#define __CASINOGAME_HH

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

#if defined(_DEBUG) || defined(_DEBUGCASINOGAME) || defined(_DEBUGSHIFTTIMER)
// Debug headers
#include "utility/Log.hh"
#endif // _DEBUG

class CasinoGame : public GameBase {

    // UI stuff
    TextLabel				dealer_caption, player_caption, selected_info,
#if !defined(__PSP__) && !defined(__PSPTEST__)
    rules_text, round_caption, round_info;
#else
    rules_text, round_caption, round_info;
#endif

    CardImageSet			dealer_cimages, player_cimages;

    StaticImage					table_image;

    SDL_TimerID shift_timer;

    // The casino game only supports two players, we'll define them here.
    static const int DEALER = 0;
    // The player may not necessarily be human :)
    static const int HUMAN = 1;

    // A game cannot be copied or assigned
    CasinoGame(const CasinoGame&);
    CasinoGame& operator=(const CasinoGame&);

    // A function to update all labels and possibly other UI elements
    void updateUI();

    // The various rounds
    virtual void startNewRound();
    virtual void deal();
    virtual void call();
    virtual void dealerCall();

  static Uint32 timedShiftCallback(Uint32 interval, void *data) {

#ifdef _DEBUGSHIFTTIMER
    logAppend("Shifting via timer.");
#endif

    static_cast<CasinoGame*>(data)->shift();

    //static_cast<CasinoGame*>(data)->setLastBet(inst->getCurrentBet());
    //static_cast<CasinoGame*>(data)->getPlayers().back().decCredits(inst->getCurrentBet());
    //static_cast<CasinoGame*>(data)->addToHandPot(inst->getCurrentBet());
    static_cast<CasinoGame*>(data)->dealer_cimages.setHideCards(true);
    
    static_cast<CasinoGame*>(data)->updateUI();

    return(interval);
  }

public:

    CasinoGame();
    CasinoGame(std::vector<Player>);	// Vector of players with which to start the game
    CasinoGame(std::vector<Player>, long);	// Vector of players, and a set amount each is to start at
    virtual ~CasinoGame();

    // Hold a card
    virtual void hold(std::vector<Card>::iterator);
    virtual void hold(std::vector<Card>::size_type);

    // Start the game, run the event loop, &c.
    virtual void start();

  // This is used for resuming a game
  virtual void game();

  // Needs to be public for SDL_Timer callback to work
  virtual void shift();

};	// CasinoBase

#endif // __CASINOGAME_HH
