#ifndef __GAMEBASE_HH
#define __GAMEBASE_HH

// Standard library headers
#include <string>
#include <vector>

// SDL headers
#include <SDL_mixer.h>

// Sabacc headers
#include "controls/StaticImage.hh"
#include "controls/StaticTextLabel.hh"
#include "sitextensions/CardImageSet.hh"

// Sabacc forward declarations
class Card;
class Player;

class GameBase {

    std::vector<Player>		players, winners;
    std::vector<Card>		deck, waste;

    unsigned int		current_round;

    long			current_bet, last_bet, hand_pot, sabacc_pot;

    Mix_Chunk*			select_sound;
    Mix_Chunk*			hold_sound;

    // A game cannot be copied or assigned
    GameBase(const GameBase&);
    GameBase& operator=(const GameBase&);

    // The various rounds
    virtual void startNewRound() = 0;
    virtual void deal() = 0;
    virtual void shift() = 0;
    virtual void dealerCall() = 0;
    virtual void call() = 0;

public:

    GameBase();
// Vector of players with which to start the game
    GameBase(std::vector<Player>);	
// Vector of players, and a set amount each is to start at
    GameBase(std::vector<Player>, long);	
    virtual ~GameBase();

    // Deck/Card operations
    virtual void createDeck();
    virtual void shuffleDeck();

    // Player operations
    // There is no longer a clear players function... that's just end of game.
    // Access function for the derived classes to acess players
    virtual std::vector<Player>& getPlayers();
    // Get a single player
    virtual Player& getPlayer(int);

    virtual void addPlayer(Player);
    virtual void remPlayer(const std::vector<Player>::size_type);
    virtual void remPlayer(Player);
    virtual void remPlayer(std::vector<Player>::iterator);

    // Access functions for the pot
    // No takeFromXXXPot functions as they only get added to or emptied
    virtual void resetLastBet();
    virtual long getLastBet();
    virtual void setLastBet(long);
    virtual void resetCurrentBet();
    virtual const long getCurrentBet();
    virtual void raiseCurrentBet(long);
  virtual void lowerCurrentBet(long);
    virtual const long getHandPot();
    virtual void setHandPot(long);
    virtual void emptyHandPot();
    virtual void addToHandPot(long);
    virtual const long getSabaccPot();
    virtual void setSabaccPot(long);
    virtual void emptySabaccPot();
    virtual void addToSabaccPot(long);

    // Access functions for the derived classes to acess the deck and waste
    virtual std::vector<Card>& getDeck();
    virtual std::vector<Card>& getWaste();

    // Access the current round info
    virtual const unsigned int getRound();
    virtual const unsigned int firstRound();
    virtual const unsigned int nextRound();

    // Play sounds
    void playSelectSound();
    void playHoldSound();

    // Hold a card
    virtual void hold(std::vector<Card>::iterator) = 0;
    virtual void hold(std::vector<Card>::size_type) = 0;

    // Start the game, run the event loop, &c.
    virtual void start() = 0;

  // For resuming a game. Also called by start when a new game is started
  virtual void game() = 0;

};	// GameBase

#endif // __GAMEBASE_HH
