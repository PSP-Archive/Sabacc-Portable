#if (_PSP_FW_VERSION > 100)// avoid compiling except for PSP

/**
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: January 15, 2009.
 *
 * File: SaveGame.cc
 *
 * A utility class for saving player data on PSP.
 *
 * To do: Add functionality for saving game data so
 * that the game in progress may be saved.
 *
 */

// Standard library headers
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <string>
using std::string;

// PSP SDK headers
#include <pspdisplay.h>
#include <pspmoduleinfo.h>
#include <pspgu.h>
#include <pspgum.h>
#include <pspkerneltypes.h>
#include <psputility.h>

// PSP SDK
#include "psputility/SaveGame.hh"

// Icons
#include "images/icon0.h"

// SMF Debugging
#if defined(_DEBUG) || defined(_DEBUGSAVE_DATA)
#include <cstdio>
#include "utility/Log.hh"
#endif

// The PSP dialogs have been moved to the PSPUtility namespace
// so that both SMF dialogs and PSP dialogs may be used together
// in the same application.
namespace PSPUtility {

  static const string game_title = "Sabacc Portable";
  static const string game_version = "0.7.2";

  static const char save_player_title[] = "Save Player Data";
  static const char load_player_title[] = "Load Player Data";  

  static const char encryption_key[] = "4fb2893ac02834de";
  
  static char data_names[][20] = {
    "0000",
    "0001",
    "0002",
    "0003",
    "0004",
    ""
  };

  static const string data_file = "SabaccPortable.savedata";

  SaveGameData::SaveGameData(char* data, size_t data_size, string game_desc) :
    save_game_data(data), save_game_data_length(data_size),
    dialog_mode(PSP_UTILITY_SAVEDATA_LISTLOAD), game_description(game_desc),
    save_data(), data_list(), is_init(false)
  {
  }
  SaveGameData::SaveGameData(const SaveGameData& src) :
    save_game_data(src.save_game_data),
    save_game_data_length(src.save_game_data_length),
    dialog_mode(src.dialog_mode),
    game_description(src.game_description),
    save_data(), data_list(), is_init(false)
  {
  }
  SaveGameData::~SaveGameData()
  {
    cleanup();
  }

  SaveGameData& SaveGameData::operator=(const SaveGameData& other)
  {
    if(&other != this) {
      save_game_data = other.save_game_data;
      save_game_data_length = other.save_game_data_length;
      dialog_mode = other.dialog_mode;
      game_description = other.game_description;

      is_init = false;
    }

    return(*this);
  }
  
  const char* SaveGameData::getData()
  {
    return(save_game_data);
  }
  size_t SaveGameData::getDataSize()
  {
    return(save_game_data_length);
  }
  void SaveGameData::setData(char* data, size_t data_size)
  {
    save_game_data = data;
    save_game_data_length = data_size;

    is_init = false;
  }
  string SaveGameData::getDescription()
  {
    return(game_description);
  }
  void SaveGameData::setDescription(string new_description)
  {
    game_description = new_description;

    is_init = false;
  }

  PspUtilitySavedataMode SaveGameData::getDialogMode()
  {
    return(dialog_mode);
  }
  void SaveGameData::setDialogMode(PspUtilitySavedataMode new_mode)
  {
    dialog_mode = new_mode;
  }
  
  void SaveGameData::init()
  {

    size_t save_data_size = sizeof(SceUtilitySavedataParam);
    
    memset(&save_data, 0, save_data_size);
    memset(&data_list, 0, sizeof(data_list));

    save_data.base.size = save_data_size;

    save_data.base.language = PSP_SYSTEMPARAM_LANGUAGE_ENGLISH;
    save_data.base.buttonSwap = PSP_UTILITY_ACCEPT_CROSS;
    save_data.base.graphicsThread = 0x11;
    save_data.base.accessThread = 0x13;
    save_data.base.fontThread = 0x12;
    save_data.base.soundThread = 0x10;
    
    save_data.mode = dialog_mode;
    save_data.overwrite = 1;
    save_data.focus = PSP_UTILITY_SAVEDATA_FOCUS_LATEST;

#if (_PSP_FW_VERSION >= 200)
    // Set encryption key
    strncpy(save_data.key, encryption_key, strlen(encryption_key));
#endif

    // gameName is the name of the game,
    // saveName is the numeric identifier of the saved game
    strncpy(save_data.gameName, game_title.c_str(), game_title.length());
    strncpy(save_data.saveName, "0000", 4);

    // Available saved games
    save_data.saveNameList = data_names;

    // The name of the internal data file
    strncpy(save_data.fileName, data_file.c_str(), data_file.length());

    // Allocate buffers used to store various parts of the save data
    save_data.dataBuf = save_game_data;
    save_data.dataBufSize = save_game_data_length;
    save_data.dataSize = save_game_data_length;

    if (PSP_UTILITY_SAVEDATA_LISTSAVE == dialog_mode)
      {
	//memset(save_data.dataBuf, 0, save_game_data_length);
	//memcpy(save_data.dataBuf, save_game_data, save_game_data_length);

	strncpy(save_data.sfoParam.title, game_title.c_str(),
		game_title.length());
	strncpy(save_data.sfoParam.savedataTitle, game_version.c_str(),
		game_version.length());
	strncpy(save_data.sfoParam.detail, game_description.c_str(),
		game_description.length());
	save_data.sfoParam.parentalLevel = 1;
	
	// No icons. In PNG format stored in a variable
	save_data.icon1FileData.buf = 0;
	save_data.icon1FileData.bufSize = 0;
	save_data.icon1FileData.size = 0;

	save_data.pic1FileData.buf = 0;
	save_data.pic1FileData.bufSize = 0;
	save_data.pic1FileData.size = 0;

	save_data.icon0FileData.buf = &icon0;
	save_data.icon0FileData.bufSize = size_icon0;
	save_data.icon0FileData.size = size_icon0;
	
	save_data.snd0FileData.buf = 0;
	save_data.snd0FileData.bufSize = 0;
	save_data.snd0FileData.size = 0;

	data_list.icon0.buf = &icon0;
	data_list.icon0.bufSize = size_icon0;
	data_list.icon0.size = size_icon0;	

	// Set title
	if(PSP_UTILITY_SAVEDATA_LISTSAVE == dialog_mode)
	  {
	    data_list.title = const_cast<char*>(save_player_title);
	  } 
	else 
	  {
	    data_list.title = const_cast<char*>(load_player_title);
	  }
	
	// The list for display
	save_data.newData = &data_list;
	
	save_data.focus = PSP_UTILITY_SAVEDATA_FOCUS_FIRSTEMPTY;

      }//if(save_game)

    is_init = true;

  }
  void SaveGameData::cleanup()
  {
  }

  void SaveGameData::show()
  {

    if(!is_init) init();
	
    if(0 != sceUtilitySavedataInitStart(&save_data))
      {
#if defined(_DEBUG) || defined(_DEBUGSAVE_DATA)
	logAppend("sceUtilitySavedataInitStart(...) failed.");
#endif
      }

    int status = 0;

    for(;;) {

      status = sceUtilitySavedataGetStatus();

#if defined(_DEBUG) || defined(_DEBUGSAVE_DATA)
      char debug_string[128];
      sprintf(debug_string, "Status: %i", status);
      logAppend(debug_string);
#endif

      if(PSP_UTILITY_DIALOG_VISIBLE == status)
	{
#if defined(_DEBUG) || defined(_DEBUGSAVE_DATA)
	  logAppend("Drawing save dialog");
#endif
	  sceUtilitySavedataUpdate(1);
	}
      else if(PSP_UTILITY_DIALOG_QUIT == status)
	{
#if defined(_DEBUG) || defined(_DEBUGSAVE_DATA)
	  logAppend("Save dialog received quit signal.");
#endif
	  sceUtilitySavedataShutdownStart();
	}
      else if(PSP_UTILITY_DIALOG_FINISHED == status)
	{
	  if(dialog_mode == PSP_UTILITY_SAVEDATA_LISTSAVE)
	    {
#if defined(_DEBUG) || defined(_DEBUGSAVE_DATA)
	      logAppend("Game saved.");
#endif
	    }
	  else
	    {
	      save_game_data = reinterpret_cast<char*>(save_data.dataBuf);
	      save_game_data_length = save_data.dataBufSize;

#if defined(_DEBUG) || defined(_DEBUGSAVE_DATA)
	      char save_debug[128];
	      sprintf(save_debug, "Loaded game data, size: %d/%d", save_data.dataBufSize,
		      save_data.dataSize);
	      logAppend(save_debug);
#endif
	    }

	  return;

	}
      else if(PSP_UTILITY_DIALOG_NONE == status)
	{
#if defined(_DEBUG) || defined(_DEBUGSAVE_DATA)
	  logAppend("Dialog dismissed.");
#endif
	  return;
	}

      sceDisplayWaitVblankStart();
      sceGuSwapBuffers();

    }// while(true)    

  }// show()

};// namespace PSPUtility

#endif//_PSP_FW_VERSION
