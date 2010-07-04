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
#include "game/objects/Player.hh"

// Sabacc forward declarations
class Card;

class GameBase
  {

  public:

    GameBase();
// Vector of players with which to start the game
    GameBase(std::vector<Player>);
// Vector of players, and a set amount each is to start at
    GameBase(std::vector<Player>, long);
    virtual ~GameBase();

    // Deck/Card operations
    void CreateDeck();
    void ShuffleDeck();

    // Player operations
    // There is no longer a clear players function... that's just end of game.
    // Access function for the derived classes to acess players
    std::vector<Player>& Players();
    // The number of players in the game
    std::vector<Player>::size_type PlayerCount() const;
    // Get a single player
    Player &PlayerByIndex(typename std::vector<Player>::size_type);
    // Get the beginning of the players container for iteration
    std::vector<Player>::iterator PlayersBegin();
    // Get the end of the players container for iteration
    std::vector<Player>::iterator PlayersEnd();
    // Get the first player
    Player &PlayersFront();
    // Get the last of the players
    Player &PlayersBack();

    GameBase &AddPlayer(Player);
    GameBase &RemovePlayer(std::vector<Player>::size_type);
    GameBase &RemovePlayer(std::vector<Player>::iterator);
    GameBase &RemovePlayer(Player);

    // Access functions for the pot
    // No takeFromXXXPot functions as they only get added to or emptied
    virtual GameBase &ResetLastBet();
    virtual long LastBet() const;
    virtual GameBase &LastBet(long);
    virtual GameBase &ResetCurrentBet();
    virtual long CurrentBet() const;
    virtual GameBase &RaiseCurrentBet(long);
    virtual GameBase &LowerCurrentBet(long);
    virtual long HandPot() const;
    virtual GameBase &HandPot(long);
    virtual GameBase &EmptyHandPot();
    virtual GameBase &AddToHandPot(long);
    virtual long SabaccPot() const;
    virtual GameBase &SabaccPot(long);
    virtual GameBase &EmptySabaccPot();
    virtual GameBase &AddToSabaccPot(long);

    // Access functions for the derived classes to acess the deck and waste
    std::vector<Card> &Deck();
    // First card in deck for iteration
    std::vector<Card>::iterator DeckBegin();
    // Last card in deck for iteration
    std::vector<Card>::iterator DeckEnd();
    // Retrieves the value of the last card in the deck, and pops it.
    Card DeckLast();
    // Clears the deck
    GameBase &DeckClear();

    std::vector<Card> &Waste();
    // First card in waste deck for iteration
    std::vector<Card>::iterator WasteBegin();
    // Last card in waste deck for iteration
    std::vector<Card>::iterator WasteEnd();
    // Clears the waste deck
    GameBase &WasteClear();

    // Access the current round info
    unsigned int Round() const;
    GameBase &Round(unsigned int);
    GameBase &FirstRound();
    GameBase &NextRound();

    // Play sounds
    void PlaySelectSound();
    void PlayHoldSound();

    // Holds a card
    virtual GameBase &HoldCard(std::vector<Card>::iterator) = 0;
    virtual GameBase &HoldCard(std::vector<Card>::size_type) = 0;

    // Start the game, run the event loop, &c.
    virtual void Start() = 0;

    // For resuming a game. Also called by start when a new game is started
    virtual void StartGame() = 0;

  private:

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
    virtual void NewRound() = 0;
    virtual void Deal() = 0;
    virtual void Shift() = 0;
    virtual void DealerCall() = 0;
    virtual void Call() = 0;

    // Common initialization function
    void InitGameBase();
    // Common destruction/cleanup function
    void DestroyGameBase();

  };	// GameBase

#endif // __GAMEBASE_HH
