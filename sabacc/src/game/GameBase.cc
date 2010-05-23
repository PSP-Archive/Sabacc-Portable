// Standard library headers
#include <vector>
#include <iostream>
#include <cstdio>
#include <algorithm>

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
#include "game/CardUtil.hh"
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

GameBase::GameBase() : players(), winners(), deck(), waste(), current_round(1),
        current_bet(0), last_bet(0),
        hand_pot(0), sabacc_pot(0), select_sound(NULL), hold_sound(NULL) {

  SDL_RWops* select_stream = SDL_RWFromMem(chirp_sound_data, size_chirp_sound_data);
  SDL_RWops* hold_stream = SDL_RWFromMem(hold_sound_data, size_hold_sound_data);
  select_sound = Mix_LoadWAV_RW(select_stream, true);
  hold_sound = Mix_LoadWAV_RW(hold_stream, true);

    createDeck();
    shuffleDeck();
}	// GameBase
GameBase::GameBase(std::vector<Player> pls) : players(pls), winners(), deck(),
        waste(), current_round(1), current_bet(0), last_bet(0),
        hand_pot(0), sabacc_pot(0), select_sound(NULL), hold_sound(NULL) {

  SDL_RWops* select_stream = SDL_RWFromMem(chirp_sound_data, size_chirp_sound_data);
  SDL_RWops* hold_stream = SDL_RWFromMem(hold_sound_data, size_hold_sound_data);
  select_sound = Mix_LoadWAV_RW(select_stream, true);
  hold_sound = Mix_LoadWAV_RW(hold_stream, true);

    createDeck();
    shuffleDeck();
}	// GameBase
GameBase::GameBase(std::vector<Player> pls, long starting_creds) : players(pls),
        winners(), deck(), waste(), current_round(1), current_bet(0), last_bet(0),
        hand_pot(0), sabacc_pot(0), select_sound(NULL), hold_sound(NULL) {

    // Give everyone their starting credits
    for (std::vector<Player>::iterator it = players.begin(); players.end() > it; ++it) {
        it->setCredits(starting_creds);
    }	// for(it)

   SDL_RWops* select_stream = SDL_RWFromMem(chirp_sound_data, size_chirp_sound_data);
  SDL_RWops* hold_stream = SDL_RWFromMem(hold_sound_data, size_hold_sound_data);
  select_sound = Mix_LoadWAV_RW(select_stream, true);
  hold_sound = Mix_LoadWAV_RW(hold_stream, true);

    createDeck();
    shuffleDeck();
}	// GameBase(p, int)
GameBase::~GameBase() {

    if (hold_sound) Mix_FreeChunk(hold_sound);
    if (select_sound) Mix_FreeChunk(select_sound);

}

void GameBase::createDeck() {
    deck.clear();
    // Add standard value cards and those with face values up to 15
    for (int suite = 1; suite < 5; ++suite)
        for (int value = 1; value < 16; ++value) {
            deck.push_back(Card(value, suite));
        }

    // Add other face cards, two of each
    for (int c = 0; 2 > c; ++c) {
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

void GameBase::shuffleDeck() {
    Random rnum;
    std::random_shuffle(deck.begin(), deck.end(), rnum);
} // ShuffleDeck

std::vector<Player>& GameBase::getPlayers() {
    return(players);
}
Player &GameBase::getPlayer(int player_number)
{
    // Throw this class on error.
    class InvalidPlayer { };

    if (players.size() < player_number) throw InvalidPlayer();

    return players[player_number];
}
void GameBase::addPlayer(Player p) {
    players.push_back(p);
}	// addPlayer
void GameBase::remPlayer(const std::vector<Player>::size_type p) {

    std::vector<Player>::size_type player_number = 0;
    for (std::vector<Player>::iterator it = players.begin();
            players.end() > it; ++it, ++player_number) {

        if (p == player_number) players.erase(it);
        return;

    }	// for(it)

    // If we got here the player was not found, prob because the func was passed to high of a number
    // throw(Exceptions::Game::InvalidPlayerIndex(__FILE__));

    // players.erase(p);
}	// remPlayer
void GameBase::remPlayer(Player plr) {
    for (std::vector<Player>::iterator pl_it = players.begin(); players.end() > pl_it; ++pl_it) {
        if (plr == *pl_it) players.erase(pl_it);
    } // for(pl_it)
} // remPlayer(Player)
void GameBase::remPlayer(std::vector<Player>::iterator p) {
    players.erase(p);
}	// remPlayer

void GameBase::resetLastBet() {
    last_bet = 0;
}
long GameBase::getLastBet() {
    return(last_bet);
}
void GameBase::setLastBet(long bet) {
    last_bet = bet;
}
void GameBase::resetCurrentBet() {
    current_bet = 0;
}
const long GameBase::getCurrentBet() {
    return(current_bet);
}
void GameBase::raiseCurrentBet(long bet) {
    current_bet += bet;
}
void GameBase::lowerCurrentBet(long bet) {
  current_bet -= bet;
}
const long GameBase::getHandPot() {
    return(hand_pot);
}
void GameBase::setHandPot(long credits) {
    hand_pot = credits;
}
void GameBase::emptyHandPot() {
    hand_pot = 0;
}
void GameBase::addToHandPot(long credits) {
    hand_pot += credits;
}
const long GameBase::getSabaccPot() {
    return(sabacc_pot);
}
void GameBase::setSabaccPot(long credits) {
    sabacc_pot = credits;
}
void GameBase::emptySabaccPot() {
    sabacc_pot = 0;
}
void GameBase::addToSabaccPot(long credits) {
    sabacc_pot += credits;
}

std::vector<Card>& GameBase::getDeck() {
    return(deck);
}
std::vector<Card>& GameBase::getWaste() {
    return(waste);
}

const unsigned int GameBase::getRound() {
    return(current_round);
}
const unsigned int GameBase::firstRound() {
    return(current_round = 1);
}
const unsigned int GameBase::nextRound() {
    return(++current_round);
}

void GameBase::playSelectSound() {
    if (-1 == Mix_PlayChannel(-1, select_sound, 0)) {

#if defined(_DEBUGGAMEBASE) || defined(_DEBUGSOUND)
        logAppend("Could not play card select sound file.");
        logAppend(SDL_GetError());
#endif

        // throw(Exceptions::Sound::PlayFailed(__FILE__, SDL_GetError()));
    }	// if(!play)
}	// playChirpSound
void GameBase::playHoldSound() {
    if (-1 == Mix_PlayChannel(-1, hold_sound, 0)) {

#if defined(_DEBUGGAMEBASE) || defined(_DEBUGSOUND)
        logAppend("Could not play hold sound file.");
        logAppend(SDL_GetError());
#endif

        // throw(Exceptions::Sound::PlayFailed(__FILE__, SDL_GetError()));
    }	// if(!play)
}	// playHoldSound
