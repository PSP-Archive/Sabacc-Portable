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

class CasinoGame :
    public GameBase
{
    
public:

    CasinoGame();
    CasinoGame(std::vector<Player>);	// Vector of players with which to start the game
    CasinoGame(std::vector<Player>, long);	// Vector of players, and a set amount each is to start at
    virtual ~CasinoGame();

    // Hold a card
    virtual GameBase &HoldCard(std::vector<Card>::iterator);
    virtual GameBase &HoldCard(std::vector<Card>::size_type);

    // Start the game, run the event loop, &c.
    virtual void Start();

    // This is used for resuming a game
    virtual void StartGame();
    
    // Needs to be public for SDL_Timer callback to work

private:

    // UI stuff
    TextLabel dealer_caption,
	player_caption,
	selected_info,
	rules_text,
	round_caption,
	round_info;

    CardImageSet dealer_cimages, player_cimages;

    StaticImage	table_image;

    SDL_TimerID shift_timer;

    // The casino game only supports two players, we'll define them here.
    static const int DEALER = 0;
    // The player may not necessarily be human :)
    static const int HUMAN = 1;

    // A game cannot be copied or assigned
    CasinoGame(CasinoGame const &);
    CasinoGame &operator=(CasinoGame const &);

    // A function to update all labels and possibly other UI elements
    void UpdateUI();

    // The various rounds
    virtual void NewRound();
    virtual void Deal();
    virtual void Call();
    virtual void DealerCall();
    virtual void Shift();

    // Common Initialization and Destruction
    void InitCasinoGame();
    void DestroyCasinoGame();

    // SDL timer callback
    static Uint32 TimedShiftCallback(Uint32, void *);
};	// CasinoBase

#endif // __CASINOGAME_HH
