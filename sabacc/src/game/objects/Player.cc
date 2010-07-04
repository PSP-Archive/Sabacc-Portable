// Standard Library headers
#include <algorithm>
#include <functional>
#include <vector>
#include <string>

// Sabacc headers
#include "game/objects/Card.hh"
#include "game/objects/Player.hh"

/* Class: Player
 * This is a base class for the various types of players in the game.
 * It also serves as the class representing a local human player, as
 * all that is needed for such a player is the basic functionality provided. */
Player::Player(std::string player_name) :
    name(player_name), credits(1000), score(0),
    hand(), selected_card(),
    race(), occupation(), wins(0), losses(0), complete_games_played(0),
    streak(0), total_winnings(0)
{}

Player::Player(std::string player_name, long starting_credits) :
    name(player_name), credits(starting_credits), score(0),
    hand(), selected_card(),
    race(), occupation(), wins(0), losses(0), complete_games_played(0),
    streak(0), total_winnings(0)
{}

Player::Player(Player const &other) :
    name(other.name), credits(other.credits), score(other.score),
    hand(other.hand), selected_card(),
    race(other.race), occupation(other.occupation), wins(other.wins),
    losses(other.losses), complete_games_played(other.complete_games_played),
    streak(other.streak), total_winnings(other.total_winnings)
{}

Player::~Player()
{ }

Player
&Player::operator=(Player const &other)
{
  if (&other != this)
    {
      name = other.name;
      credits = other.credits;
      score = other.score;
      hand.assign(other.hand.begin(), other.hand.end());
      selected_card = hand.begin();
      race = other.race;
      occupation = other.occupation;
      wins = other.wins;
      losses = other.losses;
      complete_games_played = other.complete_games_played;
      streak = other.streak;
      total_winnings = other.total_winnings;
    }
  return *this;
}

bool
Player::operator==(const Player &other)
{
  // Comparison by name and credits should be sufficient in most
  // cases as names must be unique.
  return (name == other.name) && (credits == other.credits);
}

bool
Player::operator!=(const Player &other)
{
  return (name != other.name) || (credits != other.credits);
}

std::string
Player::Name()
{
  return name;
}

Player
&Player::Name(std::string new_name)
{
  name = new_name;
  return *this;
}

long
Player::Credits()
{
    return credits;
}

Player
&Player::Credits(long new_credits)
{
  credits = new_credits;
  return *this;
}

Player
&Player::AddCredits(long val)
{
  credits += val;
  return *this;
}

Player
&Player::SubtractCredits(long val)
{
  credits -= val;
  return *this;
}

unsigned long
Player::Score()
{
  return score;
}

Player
&Player::Score(unsigned long new_score)
{
  score = new_score;
  return *this;
}

std::string
Player::Race()
{
  return race;
}

Player
&Player::Race(std::string new_race)
{
  race = new_race;
  return *this;
}

std::string
Player::Occupation()
{
  return occupation;
}

Player
&Player::Occupation(std::string new_occupation)
{
  occupation = new_occupation;
  return *this;
}

unsigned int
Player::GamesPlayed()
{
  return complete_games_played;
}

Player
&Player::GamePlayed()
{
  ++complete_games_played;
  return *this;
}

unsigned int
Player::Wins()
{
  return wins;
}

Player
&Player::AddWin()
{
  if (0 <= streak)
    {
      ++streak;
    }
  else
    {
      streak = 1;
    }

  ++wins;
  return *this;
}

Player
&Player::AddWin(long credits_won)
{
  credits += credits_won;

  if (0 <= streak)
    {
      ++streak;
    }
  else
    {
      streak = 1;
    }

  ++wins;
  return *this;
}


unsigned int
Player::Losses()
{
  return losses;
}

Player
&Player::AddLoss()
{
  if (0 >= streak)
    {
      --streak;
    }
  else
    {
      streak = -1;
    }

  ++losses;
  return *this;
}

Player
&Player::AddLoss(long credits_lost)
{
  credits -= credits_lost;

  if (0 >= streak)
    {
      --streak;
    }
  else
    {
      streak = -1;
    }

  ++losses;
  return *this;
}

int
Player::Streak()
{
  return streak;
}

long
Player::TotalWinnings()
{
  return total_winnings;
}

double
Player::AvgWinnings()
{
  return (0 == complete_games_played)
         ? 0
         : total_winnings / complete_games_played;
}

void
Player::SelectionLeft()
{
  if (hand.begin() > --selected_card)
    {
      selected_card = hand.begin();
    }
}

void
Player::SelectionRight()
{
  if (hand.end() == ++selected_card)
    {
      selected_card = hand.end() - 1;
    }
}

std::vector<Card>::iterator
Player::SelectedCard()
{
  return selected_card;
}

std::vector<Card> const
&Player::Hand() const
  {
    return hand;
  }

Player
&Player::RemoveCard()
{
  hand.erase(selected_card);
  return *this;
}

Player
&Player::RemoveCard(std::vector<Card>::size_type card_number)
{
  if (hand.size() > card_number)
    {
      hand.erase(hand.begin() + card_number);
    }
  return *this;
}

Player
&Player::RemoveCard(std::vector<Card>::iterator card)
{
  hand.erase(card);
  return *this;
}

Player
&Player::PushCard(Card const &new_card)
{
  hand.push_back(new_card);
  selected_card = hand.begin();
  return *this;
}

Player
&Player::PopCard()
{
  hand.pop_back();
  selected_card = hand.begin();
  return *this;
}

Player
&Player::SwapCard(Card &second)
{
  (*selected_card).Swap(second);
  return *this;
}

Player
&Player::SwapCard(std::vector<Card>::iterator first, Card &second)
{
  (*first).Swap(second);
  return *this;
}

std::vector<Card>::iterator
Player::HandBegin()
{
    return hand.begin();
}

std::vector<Card>::reverse_iterator
Player::HandRBegin()
{
    return hand.rbegin();
}

std::vector<Card>::iterator
Player::HandEnd()
  {
      return hand.end();
  }

std::vector<Card>::reverse_iterator
Player::HandREnd()
  {
    return hand.rend();
  }

Card
&Player::HandFront()
{
  return hand.front();
}

Card
&Player::HandBack()
{
  return hand.back();
}

std::vector<Card>::size_type
Player::HandSize() const
{
  return hand.size();
}

Player
&Player::EmptyHand()
{
  hand.clear();
  return *this;
}

short
Player::HandTotal()
{
  short sum = 0;
  std::for_each(hand.begin(), hand.end(),
		[&sum](Card &c){ sum += c.Value(); });
  return(sum);
}

bool
Player::Held(std::vector<Card>::iterator find_me)
  {
      for (std::vector<Card>::iterator fnd = hand.begin();
	   hand.end() != fnd;
	   ++fnd)
      {
	  if (fnd == find_me)
	  return (*find_me).Hold();
      }

    // If we got here the card was not found, just return false.
    return false;
  }

bool
Player::SelectedHeld()
{
  return Held(selected_card);
}

bool
Player::Hold(std::vector<Card>::iterator card_to_hold)
{
      for (std::vector<Card>::iterator fnd = hand.begin();
	   hand.end() != fnd;
	   ++fnd)
      {
	  if (card_to_hold == fnd)
	  {
	      if ((*card_to_hold).Hold())
	      {
		  (*card_to_hold).Hold(false);
	      }
	      else
	      {
		  (*card_to_hold).Hold(true);
	      }
	      return true;
	  }
      }
    
    return false;
}

bool
Player::HoldSelected()
{
  return Hold(selected_card);
}	// holdCard()

int
Player::HoldCount()
{
  int hold_count = 0;
  for (std::vector<Card>::iterator it = hand.begin();
       hand.end() > it;
       ++it)
    {
      if ((*it).Hold()) ++hold_count;
    }
  return(hold_count);
}
