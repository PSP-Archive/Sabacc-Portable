/** Note: Todo: for options menu: Use copy ctor to copy from main menu
    Menu* options_menu = new Menu(main_menu);
    to quickly copy positioning data. */

// Standard library headers
#include <string>
#include <sstream>
using std::string;
using std::stringstream;

// SDL
#if defined(__APPLE__)
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#else
#include <SDL.h>
#include <SDL_mixer.h>
#endif

// Sabacc headers
#include "game/CasinoGame.hh"
#include "game/GameBase.hh"
#include "game/GameBoot.hh"
#include "game/objects/Player.hh"

// Sabacc Interface headers
#include "controls/StaticMenu.hh"
#include "controls/StaticTextLabel.hh"
#include "dialog/SingleButtonDialog.hh"
#include "dialog/OKCancelDialog.hh"
#include "exceptions/GameImpl.hh"
#include "system/EventManager.hh"
#include "system/SystemManager.hh"
#include "system/VideoManager.hh"
#include "system/RenderManager.hh"
#include "geometry/Rect.hh"

#if (_PSP_FW_VERSION >= 100)
// PSP essentials
#include <pspkerneltypes.h>

// PSP dialogs
#include "psputility/SingleButtonDialog.hh"
#include "psputility/OKCancelDialog.hh"

// PSP OSK
#include "psputility/Keyboard.hh"

// PSP game saving and loading
#include "psputility/SaveGame.hh"
#endif

// Sabacc Media Framework constants
#include "constants/EventConstants.hh"

// Resources
#include "images/LogoData.h"
#include "images/SabaccMenuBitmap.h"
#ifdef __USE_GAMEBOOT_MUSIC
#include "sounds/ThemeSound.h"
#endif

// Debug
#if defined(_DEBUG) || defined(_DEBUGGAMEBOOT)
#include "utility/Log.hh"
#endif

GameBoot::GameBoot() : 
  bg(sabacc_menu_bitmap, size_sabacc_menu_bitmap, Rect(0, 0, 480, 272), "autosize: true; pixelformat: display"),
  logo(main_logo_image, size_main_logo_image, Rect(0, 0, 480, 272), "autosize: true; pixelformat: displayalpha"),
  copyright("2008, goumba", Rect(380, 252), "autosize: true; anchor: bottom right"),
  version("version 0.8.0", Rect(5, 252), "autosize: true; anchor: bottom left"),
  main_menu("Main Menu", default_rect, "justify: center; autosize: false;"),
  allow_continue(false), menu_music(0), player_name("Player")
{

    main_menu.Position(
	Rect(15, 120,
	     getSystemManager().getVideo().getResolution().Width() - 15,
	     getSystemManager().getVideo().getResolution().Height() -
	     copyright.Height() - 135));

    main_menu.add("Four Cards", "Starts a new game of Sabacc at the\nMos Eisley Cantina, with casual four card play.");
    main_menu.add("Options", "Personalize the game and change rules to play by.");
    main_menu.add("Quit", "Leave the Cantina\n(You know you don't really want to do this).");
    main_menu.setTextColor(default_menu_forecolor);

    // The PSP library can not use Mix_LoadMUS_RW yet, so don't load music
    // PlayMusic will just fail, printing cannot play null chunk to stderr
    // no big deal

#ifdef __USE_GAMEBOOT_MUSIC
    SDL_RWops* music_data = SDL_RWFromMem(theme_data, size_theme_data);
    menu_music = Mix_LoadWAV_RW(music_data, true);
#endif

  eventLoop();

}	// GameBoot

GameBoot::~GameBoot() {

  // destroyControls();

  if (menu_music) {
    Mix_FreeChunk(menu_music);
    menu_music = 0;
  }

}

void GameBoot::eventLoop() {

    std::string menu_result = "";
    GameBase* sabacc_game = 0;

    if (-1 == Mix_PlayChannel(-1, menu_music, 0)) {

#if defined(_DEBUG) || defined(_DEBUGGAMEBOOT)
        logAppend(Mix_GetError());
#endif
    }

    RenderManager& renderer = getSystemManager().getRenderer();
    EventManager& events = getSystemManager().getEventManager();

    bool done = false;
    while (!done) {
        // Draw everything

        try {
            if (renderer.empty()) {

	      // createControls();

#if defined(_DEBUG) || defined(_DEBUGGAMEBOOT)
                logAppend("GameBoot: Recreate objects.");
#endif

#if defined(_DEBUG) || defined(_DEBUGGAMEBOOT)
                logAppend("GameBoot: Created.");
#endif

                renderer.push(&bg);
		renderer.push(&logo);
                renderer.push(&copyright);
                renderer.push(&version);
                renderer.push(&main_menu);
            }	// if(queue empty)
            if (events.empty()) {
                events.push(&main_menu);
            }

            renderer.draw();

        } catch (Exceptions::ExceptionBase& e) {
#if defined(_DEBUG) || defined(_DEBUGGAMEBOOT)
            logAppend("An error occurred while drawing the render queue.");
            logAppend(e.getFile());
            logAppend(e.getMessage());
            logAppend(e.getDetails());
#endif

            exit(1);
        }	// try...catch

        smf_event_t event_code = events.doEvents();
        if (event_menu_item_selected == event_code) menu_result = main_menu.getSelectedItemText();

        if ("Four Cards" == menu_result) {

#if defined(_DEBUG) || defined(_DEBUGGAMEBOOT)
            logAppend("Starting new game...");
#endif

            Mix_HaltMusic();

	    renderer.clear();
	    events.clear();

	    // destroyControls();

	    if(allow_continue) {

	      smf_event_t dialog_result = event_none;

#if (_PSP_FW_VERSION >= 200)
	      PSPUtility::
#endif
	      OKCancelDialog confirm("Do you wish to continue? All winnings will be lost.", "A game is already in progress.");

	      renderer.push(&confirm);
	      events.push(&confirm);

	      while (event_none == dialog_result) {
		
		renderer.draw();
		dialog_result = events.doEvents();

	      }

#if defined(_DEBUG) || defined(_DEBUGGAMEBOOT)
	      char debug_string[64];
	      sprintf(debug_string, "Dlg ret val: 0x%x", dialog_result);
	      logAppend(debug_string);
#endif

	      events.pop();
	      renderer.pop();

	      if(event_dialog_ok == dialog_result) {

#if defined(_DEBUG) || defined(_DEBUGGAMEBOOT)
		logAppend("Deleting old game...");
#endif
		
		if(sabacc_game) delete(sabacc_game);

		sabacc_game = new CasinoGame;

		if (sabacc_game) {

		  sabacc_game->addPlayer(Player("Dealer"));
		  sabacc_game->addPlayer(Player(player_name));

		  sabacc_game->start();

		} else {

		  throw(Exceptions::Game::CouldNotCreateGameObject(__FILE__, "Game pointer is invalid."));
		
		}	// if(game object created)

	      } // event_dialog_ok == dialog_result

	    } else { // !allow_continue

#if defined(_DEBUG) || defined(_DEBUGGAMEBOOT)
	      logAppend("No game to continue from.");
#endif

	      // There was no previous game so start a new one,
	      // if sabacc_game exists here, there is a problem
	      if(sabacc_game) {
		throw(Exceptions::Game::CouldNotCreateGameObject(__FILE__, "Game object already exists!"));

#if defined(_DEBUG) || defined(_DEBUGGAMEBOOT)
		logAppend("A game object already exists, please check code!");
#endif
	      }

	      sabacc_game = new CasinoGame;

	      if (sabacc_game) {
		sabacc_game->addPlayer(Player("Dealer"));
		sabacc_game->addPlayer(Player("Player"));

		sabacc_game->start();

		main_menu.insert("Four Cards", "Continue", "Resume a game \\ialready in progress\\i.");
	      } else {

		throw(Exceptions::Game::CouldNotCreateGameObject(__FILE__, "Game pointer is invalid."));
		
	      }	// if(game object created)


	    } // if(allow_continue)

	    allow_continue = true;

#if defined(_DEBUG) || defined(_DEBUGGAMEBOOT)
		logAppend("Returned to Main Menu from \"Four Cards\" game.");
#endif

		if (-1 == Mix_PlayChannel(-1, menu_music, 0)) {

#if defined(_DEBUG) || defined(_DEBUGGAMEBOOT)
                logAppend(Mix_GetError());
#endif
            }

        } else if ("Continue" == menu_result) {

	  Mix_HaltMusic();

	  renderer.clear();
	  events.clear();

	  // destroyControls();

	  if(sabacc_game) sabacc_game->game();

	  if (-1 == Mix_PlayChannel(-1, menu_music, 0)) {

#if defined(_DEBUG) || defined(_DEBUGGAMEBOOT)
                logAppend(Mix_GetError());
#endif
	    }

        } else if("Options" == menu_result) {
	  
	  renderer.pop();
	  events.pop();
	  
	  // Copy menu from main menu to conveniently copy positioning data
	  Menu options_menu(main_menu);
	  options_menu.clear();

	  options_menu.add("Change Name", "Change the name used by the current player.");
	  options_menu.add("Save", "Save current player information.\n(This does not save game data.)");
	  options_menu.add("Load", "Load previosuly saved player data.");
	  options_menu.add("Back", "Go back to the main menu.");
	  
	  renderer.push(&options_menu);
	  events.push(&options_menu);

	  while (event_menu_item_selected != events.doEvents()) {
	    renderer.draw();
	  }
	  
	  string option_selected = options_menu.getSelectedItemText();
	  
	  if("Change Name" == option_selected)
	    {

	      // Display dialog or PSP OSK

#if (_PSP_FW_VERSION >= 200)
	      PSPKeyboard keyb(player_name, "Gimme ya name:");
	      player_name = keyb.show();
#else
	      SingleButtonDialog not_implemented("This feature is not yet implemented.", "We don't care about your name here.");
	      
	      renderer.push(&not_implemented);
	      events.push(&not_implemented);
	      
	      while (event_dialog_dismiss != events.doEvents()) {
		renderer.draw();
	      }

	      events.pop();
	      renderer.pop();
#endif

	    }
	  else if("Save" == option_selected)
	    {
#if (_PSP_FW_VERSION >= 100)
	      if(!sabacc_game)
		{
		  PSPUtility::SingleButtonDialog no_data("There is no data to save. You must start a game first.");
	      
		  renderer.push(&no_data);
		  events.push(&no_data);
		  
		  while (event_dialog_dismiss != events.doEvents()) {
		    renderer.draw();
		  }
		  
		  events.pop();
		  renderer.pop();

		}
	      else
		{

		  Player* save_player = new Player(sabacc_game->getPlayers().back());

		  if(!save_player) throw(Exceptions::ExceptionBase("Could not create data necessary to save player data.","","", __FILE__, __LINE__));
		  
		  PSPUtility::SaveGameData saver(reinterpret_cast<char*>(save_player), sizeof(Player));
		  
		  saver.setDialogMode(PSP_UTILITY_SAVEDATA_LISTSAVE);
		  saver.show();

		  delete(save_player);
		  
		}// if(!sabacc_game)
	      
	      
#else
	      SingleButtonDialog not_implemented("This feature is not yet implemented on platforms\nother than PlayStation Portable.", "You shouldn't see me.");
	      
	      renderer.push(&not_implemented);
	      events.push(&not_implemented);
	      
	      while (event_dialog_dismiss != events.doEvents()) {
		renderer.draw();
	      }

	      events.pop();
	      renderer.pop();
#endif		  
	    }
	  else if("Load" == option_selected)
	    {

#if (_PSP_FW_VERSION >= 100)
	      if(!sabacc_game)
		{

		  sabacc_game = new CasinoGame;

		  if (sabacc_game) 
		    {
		      sabacc_game->addPlayer(Player("Dealer"));
		      sabacc_game->addPlayer(Player("Player"));
		      
		      sabacc_game->start();
		      
		      main_menu.insert("Four Cards", "Continue", "Resume a game\\ialready in progress\\i.");
		      
		    }// if(sabacc_game successful)
		  
		}// if(game did not already exist)

	      PSPUtility::SaveGameData saver(reinterpret_cast<char*>(&sabacc_game->getPlayers().back()), sizeof(Player));
	      
	      saver.show();

#else
	      SingleButtonDialog not_implemented("This feature is not yet implemented on platforms\nother than PlayStation Portable.", "You shouldn't see me.");
	      
	      renderer.push(&not_implemented);
	      events.push(&not_implemented);
	      
	      while (event_dialog_dismiss != events.doEvents()) {
		renderer.draw();
	      }

	      events.pop();
	      renderer.pop();
#endif		  

	    }
	  else if("Back" == option_selected)
	    {

	      // When saving is implemented, offer to
	      // save settings
	    }
	  
	  events.pop();
	  renderer.pop();

	  renderer.push(&main_menu);
	  events.push(&main_menu);
     
	}else if ("Quit" == menu_result) {
	  if(sabacc_game) delete(sabacc_game);
          done = true;
        }	// if(menu_result)

        menu_result = "";
        // done = true;

    } // end main loop
}	// event loop

// The creation functions commented out for test with the PSP
// void GameBoot::createControls() {
//
//  Rect& screen_size = const_cast<Rect&>(getSystemManager().getVideo().getResolution());
//
//  if(!bg) bg = new StaticImage("sabaccmenubg.jpg", Rect(0, 0, 480, 272), "autosize: true; pixelformat: display");
//
//  if(!logo) logo = new StaticImage("sabacclogo.png", Rect(0, 0, 480, 272), "autosize: true; pixelformat: displayalpha");
//
//  if(!copyright) copyright = new TextLabel("2008, goumba", Rect(380, 252), "autosize: true; anchor: bottom right");
//
//  if(!version) version = new TextLabel("version 0.5.9", Rect(5, 252), "autosize: true; anchor: bottom left");
//
//  if(!main_menu) main_menu = new Menu("Main Menu", Rect(15, 120,
//    screen_size.getWidth() - 15, screen_size.getHeight() - 
//    copyright->getHeight() - 135), "justify: center; autosize: false;");
//
//  if(main_menu) {
//
//    main_menu->clear();
//
//    main_menu->add("Four Cards", "Starts a new game of Sabacc at the\nMos Eisley Cantina, using casual four card play.");
//    main_menu->add("Options", "Personalize the game and change rules to play by.");
//    main_menu->add("Quit", "Leave the Cantina\n(You know you don't \\breally\\b want to do this).");
//
//    if(allow_continue) main_menu->insert("Four Cards", "Continue", 
//					 "Resume a game\\ialready in progress\\i.");
//
//    main_menu->setTextColor(default_menu_forecolor);
//
//  } // if(main_menu)
//}// createControls

// void GameBoot::destroyControls() { 
//  if(bg) {
//    delete(bg);
//    bg = 0;
//  }
//  if(logo) {
//    delete(logo);
//    logo = 0;
//  }
//  if(copyright) {
//    delete(copyright);
//    copyright = 0;
//  }
//  if(version) {
//    delete(version);
//    version = 0;
//  }
//  if(main_menu) {
//    main_menu->clear();
//    delete(main_menu);
//
//    main_menu = 0;
//  }
//
// }//destroyControls
