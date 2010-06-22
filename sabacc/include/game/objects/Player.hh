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
class Player {

    // Required properties
    std::string			name;
    long				credits;

    // The player's hand
    std::vector<Card>	hand;
    std::vector<Card>::iterator	selected_card;

    // Additional Player data
    std::string			race;
    std::string			occupation;

    // Sabacc player statistics
    unsigned int		wins, losses;
    int					streak;
    long				total_winnings;
    double				avg_winnings;
    float				rank;

public:
    Player();
    Player(std::string);
    Player(std::string, std::string, std::string);
    Player(std::string, long);
    Player(std::string, long, std::string, std::string);
    Player(const Player&);
    virtual ~Player() { }

    // Assignment operator
    virtual Player& operator=(const Player&);

    // Operator== to check equality
    virtual bool operator==(const Player&);

    // Main player properties
    std::string getName();
    Player &setName(std::string);
    long getCredits();
    Player &setCredits(long);
    void incCredits(long);
    void decCredits(long);
    std::string getRace();
    void setRace(std::string);
    std::string getOccupation();
    void setOccupation(std::string);

    // Set up streaks
    unsigned int addWin();
    unsigned int addWin(long);
    unsigned int getWins();
    unsigned int addLoss();
    unsigned int addLoss(long);
    unsigned int getLosses();
    int getStreak();
    long getToalWinnings();
    double getAvgWinnings();
    float getRank();

    // Card operations
    void moveSelectedLeft();
    void moveSelectedRight();
    Card& getSelectedCard();
    std::vector<Card>::iterator getSelectedCardIterator();
    void holdCard();
    int getHoldCount();

    // Player operations
    virtual void dropCard();
    virtual void dropCard(int);
    virtual void dropCard(std::vector<Card>::iterator);
    virtual bool takeCard(Card&);

    std::vector<Card>& getHand();
  virtual void emptyHand();
    short getHandTotal();

    virtual bool pureSabacc();
    bool sabacc();
    bool idiotArray();
    bool bomb();

}; // class Player

#endif // PLAYER_HH_INCLUDED
