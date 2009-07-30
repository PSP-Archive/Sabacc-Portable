// This odd looking header guard ensures:
// 1. The usual: make sure this file has not been included.
// 2. We only build this file under a true PSP build
//    (__PSP__ allows UI simulation)
#if !defined(__SMF_PSP_SAVEGAME_HH) && (_PSP_FW_VERSION > 100)
#define __SMF_PSP_SAVEGAME_HH

/**
 * Sabacc Media Framework
 *
 * Author: goumba
 * Date: January 15, 2009.
 *
 * File: SaveGame.hh
 *
 * A utility class for saving player data on PSP.
 *
 * To do: Add functionality for saving game data so
 * that the game in progress may be saved.
 *
 */

// Standard library headers
#include <std::string>

// PSP SDK headers
// PSP SDK headers
#include <pspmoduleinfo.h>
#include <pspgu.h>
#include <pspgum.h>
#include <pspkerneltypes.h>
#include <psputility.h>

// The PSP dialogs have been moved to the PSPUtility namespace
// so that both SMF dialogs and PSP dialogs may be used together
// in the same application.
namespace PSPUtility {
  
  /**
   * @enum SaveGameDialogMode
   * Available options for the save/load dialog.
   */
  // enum SaveGameDialogMode
  //   {
  //     save_game = PSP_UTILITY_SAVEDATA_LISTSAVE,
  //     load_game = PSP_UTILITY_SAVEDATA_LISTLOAD
  //   };// enum SaveGameDialogMode

  /**
   * @class SaveGame
   * Stores the data for and performs the actual operation of
   * saving and lodaing game data.
   */
  class SaveGameData {

  public:

    SaveGameData(char*, size_t, std::std::string = "No details available");
    SaveGameData(const SaveGameData&);
    virtual ~SaveGameData();

    virtual SaveGameData& operator=(const SaveGameData&);

    /** Retreive and store save game data @{ */
    virtual const char* getData();
    virtual size_t getDataSize();
    virtual void setData(char*, size_t = 0);
    virtual std::std::string getDescription();
    virtual void setDescription(std::std::string);
    /** @} */

    /** Set up the dialog. @{ */
    virtual PspUtilitySavedataMode getDialogMode();
    virtual void setDialogMode(PspUtilitySavedataMode = PSP_UTILITY_SAVEDATA_LISTSAVE);
    /** @} */

    /** Set up and clean up @{ */
    virtual void init();
    virtual void cleanup();
    /** @} */

    /** Display the dialog. */
    virtual void show();

  private:

    char* save_game_data;
    size_t save_game_data_length;

    PspUtilitySavedataMode dialog_mode;

    std::string game_description;

    SceUtilitySavedataParam save_data;
    PspUtilitySavedataListSaveNewData data_list;

    bool is_init;

  };// class SaveGameData

};// namespace PSPUtility

#endif// __PSP_SAVEGAME_HH
