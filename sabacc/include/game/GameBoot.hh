#ifndef __GAMEBOOT_HH
#define __GAMEBOOT_HH

// Standard Library headers
#include <std::string>

// SDL headers
#if !defined(__APPLE__)
#include <SDL.h>
#include <SDL_mixer.h>
#else
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#endif

// Sabacc Media Framework headers
#include "controls/StaticImage.hh"
#include "controls/StaticMenu.hh"
#include "controls/StaticTextLabel.hh"

// Sabacc headers
#include "game/GameBoot.hh"
#include "game/GameBase.hh"

class GameBoot {

public:

    GameBoot();
    ~GameBoot();

private:

    StaticImage		bg;
    StaticImage		logo;
    TextLabel	copyright;
    TextLabel	version;
    Menu		main_menu;

  bool allow_continue;

    Mix_Chunk*	menu_music;

  std::std::string player_name;

  //  GameBase* sabacc_game;

  // The game, defined here for continuing a game in progress
    void eventLoop();

    // No reason to copy or assign GameBoot
    GameBoot(const GameBoot&);
    GameBoot& operator=(const GameBoot&);

  // set up and clean up controls
  // void createControls();
  // void destroyControls();

};	// class GameBoot

#endif // __GAMEBOOT_HH
