#ifndef __PLAYER_HH
#define __PLAYER_HH

// Standard Library headers
#include <vector>
#include <string>

// Sabacc headers
#include "Card.hh"

/* Class: Player
 * This is a base class for the various types of players in the game.
 * It also serves as the class representing a local human player, as
 * all that is needed for such a player is the basic functionality provided. */
class Player
  {

  public:

    Player(std::string);
    Player(std::string, long);
    Player(const Player&);
      ~Player();

    // Assignment operator
    Player &operator=(Player const &);

    // Operator== to check equality
    bool operator==(Player const &);
    // And inequality
    bool operator!=(Player const &);

    // Main player properties
    std::string Name();
    Player &Name(std::string);
    long Credits();
    Player &Credits(long);
    Player &AddCredits(long);
    Player &SubtractCredits(long);

    unsigned long Score();
    Player &Score(unsigned long);

    std::string Race();
    Player &Race(std::string);
    std::string Occupation();
    Player &Occupation(std::string);

    // Set up streaks
    unsigned int GamesPlayed();
    Player &GamePlayed();

    unsigned int Wins();
    Player &AddWin();
    Player &AddWin(long);

    unsigned int Losses();
    Player &AddLoss();
    Player &AddLoss(long);

    int Streak();

    long TotalWinnings();

    double AvgWinnings();

    // Card operations
    void SelectionLeft();
    void SelectionRight();
    std::vector<Card>::iterator SelectedCard();

    // Card operations
    std::vector<Card> const &Hand() const;

    Player &RemoveCard();
    Player &RemoveCard(std::vector<Card>::size_type);
    Player &RemoveCard(std::vector<Card>::iterator);

    Player &PushCard(Card const &);
    Player &PopCard();

    // Swaps the currently selected card with another.
    Player &SwapCard(Card &);
    // Swaps any currently held card with another.
    Player &SwapCard(std::vector<Card>::iterator, Card &);

      std::vector<Card>::iterator HandBegin();
    std::vector<Card>::reverse_iterator HandRBegin();
    std::vector<Card>::iterator HandEnd();
    std::vector<Card>::reverse_iterator HandREnd();

    Card &HandFront();
    Card &HandBack();

    std::vector<Card>::size_type HandSize() const;

    Player &EmptyHand();

    short HandTotal();

    // If the card exists and is held, returns true, otherwise false.
    bool Held(std::vector<Card>::iterator);
    bool SelectedHeld();
    // Attempts to hold the card. If the card is found and the operation
    // is successful, return true. If the card does not exist in hand, returns
    // false.
      bool Hold(std::vector<Card>::iterator);
    bool HoldSelected();
    int HoldCount();

  private:

    // Required properties
    std::string	name;
    long credits;
    unsigned long score;

    // The player's hand
    std::vector<Card> hand;
    std::vector<Card>::iterator	selected_card;

    // Additional Player data
    std::string race;
    std::string	occupation;

    // Sabacc player statistics
    unsigned int wins, losses, complete_games_played;
    int	streak;
    long total_winnings;

  }; // class Player

#endif // PLAYER_HH_INCLUDED
