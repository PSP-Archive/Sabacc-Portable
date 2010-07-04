// Standard library headers
#include <vector>
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <iterator>
using namespace std;

// SDL headers
#ifdef __APPLE__
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#else
#include <SDL.h>
#include <SDL_image.h>
#endif

// Sabacc headers
#include "controls/StaticTextLabel.hh"
#include "dialog/DialogBase.hh"
#include "exceptions/base/ExceptionBase.hh"
#include "exceptions/GamePlay.hh"
#include "game/GameBase.hh"
#include "game/Ruleset.hh"
#include "game/objects/Card.hh"
#include "game/objects/Player.hh"
#include "sitextensions/CardImage.hh"
#include "sitextensions/CardImageSet.hh"
#include "system/SystemManager.hh"
#include "utility/Random.hh"

// Sounds
#include "sounds/ChirpSound.h"
#include "sounds/HoldSound.h"

#ifdef _DEBUGGAMEBASE
// Debug headers
#include "utility/Log.hh"
#endif // _DEBUG

GameBase::GameBase() :
    players(), winners(), deck(), waste(), current_round(1),
    current_bet(0), last_bet(0),
    hand_pot(0), sabacc_pot(0), select_sound(NULL), hold_sound(NULL)
{
  InitGameBase();
}	// GameBase
GameBase::GameBase(std::vector<Player> pls) :
    players(pls), winners(), deck(),
    waste(), current_round(1), current_bet(0), last_bet(0),
    hand_pot(0), sabacc_pot(0), select_sound(NULL), hold_sound(NULL)
{
  InitGameBase();
}	// GameBase
GameBase::GameBase(std::vector<Player> pls, long starting_creds) :
    players(pls),
    winners(), deck(), waste(), current_round(1), current_bet(0), last_bet(0),
    hand_pot(0), sabacc_pot(0), select_sound(NULL), hold_sound(NULL)
{
  // Give everyone their starting credits
  for (std::vector<Player>::iterator it = players.begin();
       players.end() > it;
       ++it)
    {
      (*it).Credits(starting_creds);
    }	// for(it)

  InitGameBase();
}	// GameBase(p, int)

GameBase::~GameBase()
{
  DestroyGameBase();
}

void
GameBase::CreateDeck()
{
  deck.clear();
  // Add standard value cards and those with face values up to 15

  for (int suite = 1; suite < 5; ++suite)
    {
      for (int value = 1; value < 16; ++value)
        {
          deck.push_back(Card(value, suite));
        }
    }

  // Add other face cards, two of each
  for (int c = 0; 2 > c; ++c)
    {
      deck.push_back(Card(FACE_QUEEN_AIR_DARK, SUITE_NONE));
      deck.push_back(Card(FACE_ENDURANCE, SUITE_NONE));
      deck.push_back(Card(FACE_BALANCE, SUITE_NONE));
      deck.push_back(Card(FACE_DEMISE, SUITE_NONE));
      deck.push_back(Card(FACE_MODERATION, SUITE_NONE));
      deck.push_back(Card(FACE_EVIL_ONE, SUITE_NONE));
      deck.push_back(Card(FACE_STAR, SUITE_NONE));
      deck.push_back(Card(FACE_IDIOT, SUITE_NONE));
    }	// for(c)
}	// createDeck()

void
GameBase::ShuffleDeck()
{
  Random rnum;
  std::random_shuffle(deck.begin(), deck.end(), rnum);
} // ShuffleDeck

std::vector<Player>
&GameBase::Players()
{
  return players;
}

std::vector<Player>::size_type
GameBase::PlayerCount() const
  {
    return players.size();
  }

Player
&GameBase::PlayerByIndex(typename
                         std::vector<Player>::size_type player_number)
{
  // Throw this class on error.
  class InvalidPlayer { };

  if (players.size() < player_number)
    throw InvalidPlayer();

  return players[player_number];
}

std::vector<Player>::iterator
GameBase::PlayersBegin()
{
  return players.begin();
}

std::vector<Player>::iterator
GameBase::PlayersEnd()
{
  return players.end();
}

Player
&GameBase::PlayersFront()
{
  return players.front();
}
// Get the last of the players
Player
&GameBase::PlayersBack()
{
  return players.back();
}

GameBase
&GameBase::AddPlayer(Player p)
{
  players.push_back(p);
  return *this;
}	// addPlayer

GameBase
&GameBase::RemovePlayer(const std::vector<Player>::size_type p)
{
  // Throw this class on error.
  class InvalidPlayer { };

  if (players.size() < p)
      throw InvalidPlayer();

  players.erase(players.begin() + p);

  return *this;
}	// remPlayer

GameBase
&GameBase::RemovePlayer(const std::vector<Player>::iterator p)
{
  players.erase(p);
  return *this;
}	// remPlayer

GameBase
&GameBase::RemovePlayer(Player plr)
{
  for (std::vector<Player>::iterator pl_it = players.begin();
       players.end() > pl_it;
       ++pl_it)
    {
      if (plr == *pl_it) players.erase(pl_it);
    } // for(pl_it)

  return *this;
} // remPlayer(Player)

GameBase
&GameBase::ResetLastBet()
{
  last_bet = 0;
  return *this;
}

long
GameBase::LastBet() const
  {
    return last_bet;
  }

GameBase
&GameBase::LastBet(long bet)
{
  last_bet = bet;
  return *this;
}

GameBase
&GameBase::ResetCurrentBet()
{
  current_bet = 0;
  return *this;
}

long
GameBase::CurrentBet() const
  {
    return current_bet;
  }

GameBase
&GameBase::RaiseCurrentBet(long bet)
{
  current_bet += bet;
  return *this;
}
GameBase
&GameBase::LowerCurrentBet(long bet)
{
  current_bet -= bet;
  return *this;
}

long
GameBase::HandPot() const
  {
    return hand_pot;
  }

GameBase
&GameBase::HandPot(long credits)
{
  hand_pot = credits;
  return *this;
}
GameBase
&GameBase::EmptyHandPot()
{
  hand_pot = 0;
  return *this;
}

GameBase
&GameBase::AddToHandPot(long credits)
{
  hand_pot += credits;
  return *this;
}

long
GameBase::SabaccPot() const
  {
    return sabacc_pot;
  }

GameBase
&GameBase::SabaccPot(long credits)
{
  sabacc_pot = credits;
  return *this;
}
GameBase
&GameBase::EmptySabaccPot()
{
  sabacc_pot = 0;
  return *this;
}

GameBase
&GameBase::AddToSabaccPot(long credits)
{
  sabacc_pot += credits;
  return *this;
}

std::vector<Card>
&GameBase::Deck()
{
  return deck;
}

std::vector<Card>::iterator
GameBase::DeckBegin()
{
  return deck.begin();
}

std::vector<Card>::iterator
GameBase::DeckEnd()
{
  return deck.end();
}

Card
GameBase::DeckLast()
{
  Card last_card(deck.back());
  deck.pop_back();
  return last_card;
}

GameBase
&GameBase::DeckClear()
{
  deck.clear();
  return *this;
}

std::vector<Card>
&GameBase::Waste()
{
  return waste;
}

std::vector<Card>::iterator
GameBase::WasteBegin()
{
  return waste.begin();
}

std::vector<Card>::iterator
GameBase::WasteEnd()
{
  return waste.end();
}

GameBase
&GameBase::WasteClear()
{
  waste.clear();
  return *this;
}

unsigned int
GameBase::Round() const
  {
    return current_round;
  }

GameBase
&GameBase::Round(unsigned int new_round)
{
  current_round = new_round;
  return *this;
}

GameBase
&GameBase::FirstRound()
{
  current_round = 1;
  return *this;
}

GameBase
&GameBase::NextRound()
{
  ++current_round;
  return *this;
}

void
GameBase::PlaySelectSound()
{
  if (-1 == Mix_PlayChannel(-1, select_sound, 0))
    {

#if defined(_DEBUGGAMEBASE) || defined(_DEBUGSOUND)
      logAppend("Could not play card select sound file.");
      logAppend(SDL_GetError());
#endif

      // throw(Exceptions::Sound::PlayFailed(__FILE__, SDL_GetError()));
    }	// if(!play)
}	// playChirpSound

void
GameBase::PlayHoldSound()
{
  if (-1 == Mix_PlayChannel(-1, hold_sound, 0))
    {

#if defined(_DEBUGGAMEBASE) || defined(_DEBUGSOUND)
      logAppend("Could not play hold sound file.");
      logAppend(SDL_GetError());
#endif

      // throw(Exceptions::Sound::PlayFailed(__FILE__, SDL_GetError()));
    }	// if(!play)
}	// playHoldSound

void
GameBase::InitGameBase()
{
  SDL_RWops* select_stream =
    SDL_RWFromMem(chirp_sound_data, size_chirp_sound_data);
  SDL_RWops* hold_stream =
    SDL_RWFromMem(hold_sound_data, size_hold_sound_data);

  select_sound = Mix_LoadWAV_RW(select_stream, true);
  hold_sound = Mix_LoadWAV_RW(hold_stream, true);

  CreateDeck();
  ShuffleDeck();
}

void
GameBase::DestroyGameBase()
{
  if (hold_sound)
    Mix_FreeChunk(hold_sound);

  if (select_sound)
    Mix_FreeChunk(select_sound);
}
