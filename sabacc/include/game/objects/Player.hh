#ifndef __PLAYER_HH
#define __PLAYER_HH

// Standard Library headers
#include <std::vector>
#include <std::string>

// Sabacc headers
#include "Card.hh"

/* Class: Player
 * This is a base class for the various types of players in the game.
 * It also serves as the class representing a local human player, as
 * all that is needed for such a player is the basic functionality provided. */
class Player {

    // Required properties
    std::std::string			name;
    long				credits;

    // The player's hand
    std::std::vector<Card>	hand;
    std::std::vector<Card>::iterator	selected_card;

    // Additional Player data
    std::std::string			race;
    std::std::string			occupation;

    // Sabacc player statistics
    unsigned int		wins, losses;
    int					streak;
    long				total_winnings;
    double				avg_winnings;
    float				rank;

public:
    Player();
    Player(std::std::string);
    Player(std::std::string, std::string, std::string);
    Player(std::std::string, long);
    Player(std::std::string, long, std::string, std::string);
    Player(const Player&);
    virtual ~Player() { }

    // Assignment operator
    virtual Player& operator=(const Player&);

    // Operator== to check equality
    virtual bool operator==(const Player&);

    // Main player properties
    std::std::string getName();
    void setName(std::std::string);
    long getCredits();
    void setCredits(long);
    void incCredits(long);
    void decCredits(long);
    std::std::string getRace();
    void setRace(std::std::string);
    std::std::string getOccupation();
    void setOccupation(std::std::string);

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
    std::std::vector<Card>::iterator getSelectedCardIterator();
    void holdCard();
    int getHoldCount();

    // Player operations
    virtual void dropCard();
    virtual void dropCard(int);
    virtual void dropCard(std::std::vector<Card>::iterator);
    virtual bool takeCard(Card&);

    std::std::vector<Card>& getHand();
  virtual void emptyHand();
    short getHandTotal();

    virtual bool pureSabacc();
    bool sabacc();
    bool idiotArray();
    bool bomb();

}; // class Player

#endif // PLAYER_HH_INCLUDED
