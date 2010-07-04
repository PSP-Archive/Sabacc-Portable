// Standard library headers
#include <algorithm>
#include <cstdio>
#include <functional>
#include <string>
#include <vector>

// Standard library headers
#include <iostream>
#include <cstdlib>
#include <string>

// SDL headers
#ifdef __APPLE__
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#else
#include <SDL.h>
#include <SDL_image.h>
#endif

// PSP SDK headers
#if (_PSP_FW_VERSION >= 100)
#include <pspkerneltypes.h>
#endif

// Sabacc headers
#include "constants/PSPControlConstants.hh"
#include "controls/StaticTextLabel.hh"
#include "dialog/SingleButtonDialog.hh"
#include "exceptions/base/ExceptionBase.hh"
#include "exceptions/GamePlay.hh"
#include "game/CasinoGame.hh"
#include "game/Ruleset.hh"
#include "game/objects/Card.hh"
#include "game/objects/Player.hh"
#include "sitextensions/CardImage.hh"
#include "sitextensions/CardImageSet.hh"
#include "system/EventManager.hh"
#include "system/RenderManager.hh"
#include "system/SystemManager.hh"
#include "utility/Random.hh"

// Dialogs
#if (_PSP_FW_VERSION >= 100)
#include "psputility/SingleButtonDialog.hh"
#endif

// Images
#include "images/TableData.h"

#if defined(_DEBUG) || defined(_DEBUGCASINOGAME) || defined(_DEBUGDEALERHAND)
// Debug headers
#include "utility/Log.hh"
#include <sstream>
using std::stringstream;
#endif // _DEBUG

CasinoGame::CasinoGame() :
    GameBase(),
    dealer_caption("Dealer", Rect(10, 0), "autosize: true"),
    player_caption("Player", Rect(10, 252),
                   "autosize: true; anchor: bottom left"),
    selected_info("No card selected", Rect(32, 128),
                  "autosize: true; anchor: left center"),
    rules_text("", Rect(350, 50),
               "autosize: true; anchor: top right; justify: top left"),
    round_caption("\\bRound 1\\b", Rect(340, 56),
                  "autosize: true; anchor: bottom center"),
    round_info("Hand pot: 0cr\nSabacc Pot: 0cr", Rect(340, 178),
               "autosize: true; anchor: right center"),
    dealer_cimages(),
    player_cimages(),
    table_image(table_image_data, size_table_image_data),
    shift_timer()
{
  InitCasinoGame();
}	// CasinoGame
CasinoGame::CasinoGame(vector<Player> players) :
    GameBase(players),
    dealer_caption("Dealer", Rect(), "autosize: true"),
    player_caption("Player", Rect(), "autosize: true; anchor: bottom left"),
    selected_info("No card selected", Rect(),
                  "autosize: true; anchor: left center"),
    rules_text("", Rect(),
               "autosize: true; anchor: top right; justify: top left"),
    round_caption("\\bRound 1\\b", Rect(),
                  "autosize: true; anchor: bottom center"),
    round_info("Hand pot: 0cr\nSabacc Pot: 0cr",
               Rect(400, 200, 79, 100), "anchor: left center"),
    dealer_cimages(),
    player_cimages(),
    table_image("table_top.png"),
    shift_timer()
{
  InitCasinoGame();
}	// CasinoGame(players)

CasinoGame::CasinoGame(vector<Player> players, long starting_credits) :
    GameBase(players, starting_credits),
    dealer_caption("Dealer", Rect(), "autosize: true"),
    player_caption("Player", Rect(), "autosize: true; anchor: bottom left"),
    selected_info("No card selected", Rect(),
                  "autosize: true; anchor: left center"),
    rules_text("", Rect(),
               "autosize: true; anchor: top right; justify: top left"),
    round_caption("\\bRound 1\\b", Rect(),
                  "autosize: true; anchor: bottom center"),
    round_info("Hand pot: 0cr\nSabacc Pot: 0cr",
               Rect(400, 200, 79, 100), "anchor: left center"),
    dealer_cimages(),
    player_cimages(),
    table_image("table_top.png"),
    shift_timer()
{
  InitCasinoGame();
}	// CasinoGame(players, credits)

CasinoGame::~CasinoGame()
{
  DestroyCasinoGame();
}

void CasinoGame::Start()
{

  if (2 != Players().size())
    throw Exceptions::GamePlay::TooManyPlayers(__FILE__);

  dealer_cimages.setPlayer(&PlayersFront());
  player_cimages.setPlayer(&PlayersBack());
  player_cimages.use_selection = true;

  dealer_cimages.setHideCards(true);

  // Do initial deal
  NewRound();

  StartGame();

  return;
}//start

void CasinoGame::StartGame()
{
  bool		done = false;
  SDL_Event	event;

  if (!SDL_WasInit(SDL_INIT_TIMER)) SDL_Init(SDL_INIT_TIMER);

  // loop - draw graphics and await input
  while (!done)
    {

      // Draw everything
      if (getSystemManager().getRenderer().empty())
        {
          getSystemManager().getRenderer().push(&table_image);
          getSystemManager().getRenderer().push(&dealer_caption);
          getSystemManager().getRenderer().push(&player_caption);
          getSystemManager().getRenderer().push(&selected_info);
          getSystemManager().getRenderer().push(&rules_text);
          getSystemManager().getRenderer().push(&dealer_cimages);
          getSystemManager().getRenderer().push(&player_cimages);
          getSystemManager().getRenderer().push(&round_caption);
          getSystemManager().getRenderer().push(&round_info);
        }	// if(renderer empty)

      getSystemManager().getRenderer().draw();

      if (SDL_PollEvent(&event))
        {

          if (((SDL_JOYBUTTONDOWN == event.type) &&
               (psp_triangle == event.jbutton.button)) ||
              ((SDL_KEYDOWN == event.type) &&
               (SDLK_h == event.key.keysym.sym)))
            {

#if _DEBUG || _DEBUGCASINOGAME
              char holdc[128];
              sprintf(holdc, "Player holds %d cards",
                      PlayersBack().HoldCount());
              logAppend(holdc);
#endif

              if (PlayersBack().SelectedHeld())
                {
                  PlayersBack().HoldSelected();
                }
              else if (!PlayersBack().SelectedHeld()
                       && (2 > PlayersBack().HoldCount()))
                {
                  PlayHoldSound();
                  PlayersBack().HoldSelected();
                }
              else
                {
                  // Display error dialog
                  SingleButtonDialog
                  toomany("Unable to hold another card.",
                          "You can hold a maximum of two cards.");

                  getSystemManager().getRenderer().push(&toomany);
                  getSystemManager().getEventManager().push(&toomany);

                  while (event_dialog_dismiss
                         != getSystemManager().getEventManager().doEvents())
                    {
                      getSystemManager().getRenderer().draw();
                    }

                  getSystemManager().getEventManager().pop();
                  getSystemManager().getRenderer().pop();

                }// try...
            }// if(PSP Triangle || Keyb "h")

          if (((SDL_JOYBUTTONDOWN == event.type) &&
               (psp_dpad_up == event.jbutton.button)) ||
              ((SDL_KEYDOWN == event.type) &&
               (SDLK_UP == event.key.keysym.sym)))
            {
              long bet_amount =
                getRules().getMinBet() * getRules().getChipSize();
              long max_bet =
                (getRules().getMaxBet() * getRules().getChipSize());

              if (max_bet >= (CurrentBet() + bet_amount))
                {
                  RaiseCurrentBet(bet_amount);
                  PlayersBack().SubtractCredits(bet_amount);
                }

              UpdateUI();

            }//if(PSP D-Pad up || Keyb CurUp)


          if (((SDL_JOYBUTTONDOWN == event.type) &&
               (psp_dpad_down == event.jbutton.button)) ||
              ((SDL_KEYDOWN == event.type) &&
               (SDLK_DOWN == event.key.keysym.sym)))
            {

              long bet_amount = getRules().getMinBet() * getRules().getChipSize();
              if (0 <= (CurrentBet() - bet_amount))
                {
                  LowerCurrentBet(bet_amount);
                  PlayersBack().AddCredits(bet_amount);
                }

              UpdateUI();

            }//if(PSP D-Pad down || Keyb CurDn)

          if (((SDL_JOYBUTTONDOWN == event.type) &&
               (psp_cross == event.jbutton.button)) ||
              ((SDL_KEYDOWN == event.type) &&
               (SDLK_RETURN == event.key.keysym.sym)))
            {

              if (0 < CurrentBet())
                {

                  if (LastBet() > CurrentBet())
                    {

                      char bet_too_low[256];
                      sprintf(bet_too_low,
                              "Your bet must be at least as much as the last highest bet,\nwhich was %ld credits.",
                              LastBet());
                      SingleButtonDialog bet_error(bet_too_low);
                      getSystemManager().getRenderer().push(&bet_error);
                      getSystemManager().getEventManager().push(&bet_error);

                      while (event_dialog_dismiss
                             != getSystemManager().getEventManager().doEvents())
                        {
                          getSystemManager().getRenderer().draw();
                        }

                      getSystemManager().getEventManager().pop();
                      getSystemManager().getRenderer().pop();

                    }
                  else
                    {


                      // Fixed: The dealer should call before moving on
                      // to the next round.
                      DealerCall();

                      // Uncomment to follow fourth round shift rule
                      // if(4 <= ++getRound()) shift();
                      NextRound();
                      Shift();

                      LastBet(CurrentBet());
                      PlayersBack().SubtractCredits(CurrentBet());
                      AddToHandPot(CurrentBet());

                      // To Reset the current bet to zero when continuing
                      // to the next round, uncomment the following
                      // function call, otherwise the bet will not be
                      // Reset, which makes sense when the next bet can
                      // not be lower than this one.
                      // ResetCurrentBet();

                      // Hack alert: rehide dealer cards
                      dealer_cimages.setHideCards(true);

                      UpdateUI();
                    }

                }
              else
                {

                  SingleButtonDialog bet_error("You must place a bet before continuing.", "Try Again");
                  getSystemManager().getRenderer().push(&bet_error);
                  getSystemManager().getEventManager().push(&bet_error);

                  while (event_dialog_dismiss != getSystemManager().getEventManager().doEvents())
                    {
                      getSystemManager().getRenderer().draw();
                    }

                  getSystemManager().getEventManager().pop();
                  getSystemManager().getRenderer().pop();

                }

            }//if(PSP Cross || Keyb Enter)

          if (((SDL_JOYBUTTONDOWN == event.type) &&
               (psp_square == event.jbutton.button)) ||
              ((SDL_KEYDOWN == event.type) &&
               (SDLK_c == event.key.keysym.sym)))
            {

              // Call can only happen after the third round
              if (4 > Round())
                {

                  SingleButtonDialog
                  call_error("You may not call before the fourth round.");
                  getSystemManager().getRenderer().push(&call_error);
                  getSystemManager().getEventManager().push(&call_error);

                  while (event_dialog_dismiss
                         != getSystemManager().getEventManager().doEvents())
                    {
                      getSystemManager().getRenderer().draw();
                    }

                  getSystemManager().getEventManager().pop();
                  getSystemManager().getRenderer().pop();

                }
              else
                {
                  Call();
                }

            }//if(PSP Square || Keyb "C")

          if (((SDL_JOYBUTTONDOWN == event.type) &&
               (psp_dpad_left == event.jbutton.button)) ||
              ((SDL_KEYDOWN == event.type) &&
               (SDLK_LEFT == event.key.keysym.sym)))
            {
              // Move card iterator left
              PlayersBack().SelectionLeft();

              UpdateUI();

              PlaySelectSound();

            }//if(PSP D-Pad Left || Keyb CurLt)

          if (((SDL_JOYBUTTONDOWN == event.type) &&
               (psp_dpad_right == event.jbutton.button)) ||
              ((SDL_KEYDOWN == event.type) &&
               (SDLK_RIGHT == event.key.keysym.sym)))
            {

              // Move card iterator right
              PlayersBack().SelectionRight();

              UpdateUI();

              PlaySelectSound();
            }//if(PSP D-Pad Right || Keyb CurRt)

          if (((SDL_JOYBUTTONDOWN == event.type) &&
               (psp_select == event.jbutton.button)) ||
              ((SDL_KEYDOWN == event.type) &&
               (SDLK_ESCAPE == event.key.keysym.sym)))
            {

              done = true;

            }// if(PSP Select || Keyb Escape)

          if (SDL_QUIT == event.type) done = true;

        } // if(SDL_WaitEvent)

      if (0 > PlayersBack().Credits())
        {

          SingleButtonDialog out_of_luck("No credits, no play.", "Get out of here!");

          getSystemManager().getRenderer().push(&out_of_luck);
          getSystemManager().getEventManager().push(&out_of_luck);

          while (event_dialog_dismiss != getSystemManager().getEventManager().doEvents())
            {
              getSystemManager().getRenderer().draw();
            }

          getSystemManager().getEventManager().pop();
          getSystemManager().getRenderer().pop();

          done = true;

        }//if(player out of credits)

    } // while(!done)

  // SDL_RemoveTimer(shift_timer);

  getSystemManager().getRenderer().clear();
  return;
}

void CasinoGame::UpdateUI()
{

  char ui_output[128];

#if defined(_DEBUG) || defined(_DEBUGDEALERHAND)
  sprintf(ui_output, "\\b\\i%s\\i\\b     %d total in hand", PlayersFront().Name().c_str(), PlayersFront().HandTotal());
  dealer_caption.setText(ui_output);
#endif// debug dealer

  sprintf(ui_output, "\\b\\i%s\\i\\b     %ld credits, %d total in hand", PlayersBack().Name().c_str(), PlayersBack().Credits(), PlayersBack().HandTotal());
  player_caption.setText(ui_output);

  sprintf(ui_output,
          "\\bCurrent bet:\\b %ldcr\n\\bHand pot:\\b %ldcr\n\\bSabacc pot:\\b %ldcr",
          CurrentBet(),
          HandPot(),
          SabaccPot());
  round_info.setText(ui_output);

  selected_info.setText(PlayersBack().SelectedCard()->DescriptiveName());

  sprintf(ui_output, "\\bRound %d\\b", Round());
  round_caption.setText(ui_output);

  dealer_cimages.setHideCards(true);

}	// UpdateUI

void
CasinoGame::NewRound()
{
  for (vector<Player>::iterator it = Players().begin();
       Players().end() > it;
       ++it)
    {
      (*it).EmptyHand();
    }

  /* Once upon a time (pre 0.41), a round ended but the deck
   * continued to be used.
   * This meant that when cards ran out, bad things happened
   * (usually a crash).
   * Also, the waste deck wasn't emptied, so the deck continued to grow
   * with each round...
   * (WTF? The deck has 100 cards? Now 130!!).
   *
   * This comment is only here to serve as a reminder. */
  WasteClear();
  CreateDeck();

  // v0.41 forgot to shuffle oops hahaha.
  ShuffleDeck();

  Deal();

  dealer_cimages.init();
  player_cimages.init();

  dealer_cimages.setHideCards(true);

  FirstRound();

  dealer_cimages.setHideCards(true);

  UpdateUI();
}	// startNewRound

// Card operations
void
CasinoGame::Deal()
{

#ifdef _DEBUGCASINOGAME
  logAppend("Deal in CasinoGame.");
#endif

  if (2 != PlayerCount())
    throw Exceptions::GamePlay::TooManyPlayers(__FILE__);

  for (int card_cnt = 0; 4 > card_cnt; ++card_cnt)
    {
      PlayersFront().PushCard(DeckLast());
      PlayersBack().PushCard(DeckLast());
    }	// for(int card)

  dealer_cimages.init();
  player_cimages.init();

  dealer_cimages.setHideCards(true);

}	// deal_cards

// Shift works like this:
// - A die is rolled, and the outcome determines if shift is in effect
// - From the cards not held, cards are selected at random to be shifted
//   This may be all held cards, or may be none.
// - Selected cards are replaced.
//
// Uncomment lines for random shift, otherwise cards not held are always shifted,
// as in Casino Sabacc
void
CasinoGame::Shift()
{

#ifdef _DEBUGCASINOGAME
  char debug_string[64];
  sprintf(debug_string, "Shift is in effect for round %d, in CasinoGame.",
          Round());
  logAppend(debug_string);
#endif

  // Once upon a time, the deck would be checked on each card shifted
  // for empty, and the waste emptied. This was done for every card in
  // play. Now, just check for a sufficient number of cards before starting
  // shift, hoping to minimize performance lost.
  // Eight seems reasonable as at most only eight cards are in play at any
  // given time in a "four cards" game.
  if (8 > Deck().size())
    {

#if _DEBUGCASINOGAME
      logAppend("Replacing empty deck");
      sprintf(debug_string, "Waste size %u", Waste().size());
      logAppend(debug_string);
#endif

      copy(WasteBegin(), WasteEnd(),
           back_inserter(Deck()));

      ShuffleDeck();
      WasteClear();
    }	// if(1 > Deck().size())


  // Each player's cards get shifted in turn
  for (vector<Player>::iterator pl_it = PlayersBegin();
       PlayersEnd() != pl_it;
       ++pl_it)
    {
      // Go through cards, determine which cards get shifted
      for (vector<Card>::iterator cd_it = (*pl_it).HandBegin();
           (*pl_it).HandEnd() > cd_it;
           ++cd_it)
        {
          /* Early versions (pre 0.41) never moved waste cards
           * back to the deck, so at some
           * point when the deck no longer had any cards to deal,
           * a bad pointer would be passed to
           * the CardImage widget, and well, things were bad.
           * Ooops! Led to hours of fruitless debugging.
           *
           * This is only here as a reminder. */

          if (!((*cd_it).Hold()))
            {
              Card new_card = DeckLast();
              (*cd_it).Swap(new_card);
              Waste().push_back(new_card);
            }	// cd_it

        } // for(cd_it)

    } // for(pl_it)

  dealer_cimages.init();
  player_cimages.init();

  dealer_cimages.setHideCards(true);

} // shift

// Hand operations
void CasinoGame::DealerCall()
{

#ifdef _DEBUGCASINOGAME
  char dealer_call[64];
  logAppend("Dealer thinking about calling in CasinoGame.");

  sprintf(dealer_call, "Player total %d, dealer %d.", PlayersBack().HandTotal(), PlayersFront().HandTotal());
  logAppend(dealer_call);
#endif

  // Calculates the probability that the dealer should call.
  float call_likeliness = 0.0f;

  Random call_indicision_gen;

  // Idiot array or sabacc is a definite time to call
  if (IdiotArray(PlayersFront())
      || PureSabacc(PlayersFront())
      || Sabacc(PlayersFront()))
  {
    Call();
  }
  
  if ((23 > PlayersFront().HandTotal())
      && (21 < PlayersFront().HandTotal()))
    {

#ifdef _DEBUGCASINOGAME
      logAppend("Between 22 and 23");
#endif

      // No brainer.
      Call();

    }
  else if ((22 > PlayersFront().HandTotal())
           && (18 < PlayersFront().HandTotal()))
    {
#ifdef _DEBUGCASINOGAME
      logAppend("Between 19 and 23");
#endif
      // I need a four or less, "should I or shouldn't I?"
      if (1 == call_indicision_gen(2)) Call();	//	"I should"
    }
  else if ((19 > PlayersFront().HandTotal())
           && (15 < PlayersFront().HandTotal()))
    {
#ifdef _DEBUGCASINOGAME
      logAppend("Between 16 and 18");
#endif
      // I need between five and seven
      int balls_factor = call_indicision_gen(23);
      float likelihood_5_7 = call_indicision_gen(23) / 12; // 1.94 greatest

      call_likeliness = (balls_factor / likelihood_5_7) / 23; // .51 greatest

      // Adjust to scale of 0 to 1 or close to if
      call_likeliness *= 1.9;

#ifdef _DEBUGCASINOGAME
      sprintf(dealer_call, "Dealer (prob: %f)", call_likeliness);
      dealer_caption.setText(dealer_call);
#endif

      if (0.85f < call_likeliness)
	  Call();	// All that fancy math for one function call hahahaha
    }
  else // Probably need more cards
    {
      return;
    }	// if(need card)

  /* Notes:
   * - Never call on hand with total greater than 23 if it is not 46.
   * This is a bomb.
   * - Improve call likelood calculation with balls factor.
   */

}	// dealerCall

// Calls the hand, ends play for this hand
void CasinoGame::Call()
{

#ifdef _DEBUGCASINOGAME
  char debug_string[64];
  logAppend("Call in CasinoGame.");
#endif

  // Call can only happen after the third round
  if (4 > Round()) return;

#ifdef _DEBUGCASINOGAME
  sprintf(debug_string, "Player total %d, dealer %d.",
          PlayersBack().HandTotal(),
          PlayersFront().HandTotal());
  logAppend(debug_string);
#endif

  // Update the UI. Sometimes the displayed total and the final don't match
  // maybe we're not updating UI before call.
  UpdateUI();

  SingleButtonDialog call_dialog("", "Call", "Continue", Rect(30, 85 , 420, 102));

  if ((IdiotArray(PlayersBack())
       && !IdiotArray(PlayersFront()))
      || (PureSabacc(PlayersBack())
	  && !IdiotArray(PlayersFront())
	  && !PureSabacc(PlayersFront()))
      || (Sabacc(PlayersBack())
	  && !IdiotArray(PlayersFront())
	  && !PureSabacc(PlayersFront())
	  && !Sabacc(PlayersFront())))
    {

#ifdef _DEBUGCASINOGAME
      logAppend("Player wins: Idiot or Sabacc");
#endif

      PlayersBack().AddCredits(HandPot() + SabaccPot());
      EmptySabaccPot();

      if (IdiotArray(PlayersBack()))
        {
          call_dialog.setDialogText(PlayersBack().Name()
                                    + " is holding an Idiot array.");
        }
      else
        {
          call_dialog.setDialogText(PlayersBack().Name()
                                    + " is holding Sabacc.");
        }

      call_dialog.setDialogTitle(PlayersBack().Name() + " wins.");

    }
  else if ((IdiotArray(PlayersFront())
	    && !IdiotArray(PlayersBack()))
	   || (PureSabacc(PlayersFront())
	       && !IdiotArray(PlayersBack())
	       && !PureSabacc(PlayersBack()))
	   || (Sabacc(PlayersFront())
	       && !IdiotArray(PlayersBack())
	       && !PureSabacc(PlayersBack())
	       && !Sabacc(PlayersBack())))
    {

#ifdef _DEBUGCASINOGAME
      logAppend("Dealer wins: Idiot or Sabacc");
#endif

      EmptySabaccPot();

      if (IdiotArray(PlayersFront()))
        {
#if defined(_DEBUG) || defined(_DEBUGDEALERHAND)
          stringstream total;
          total << "Dealer's total: " << PlayersFront().HandTotal();
          call_dialog.setDialogText(PlayersFront().Name() + " is holding an Idiot array.\n" + total.str());
#else
          call_dialog.setDialogText(PlayersFront().Name() + " is holding an Idiot array.");
#endif
        }
      else
        {
#if defined(_DEBUG) || defined(_DEBUGDEALERHAND)
          stringstream total;
          total << "Dealer's total: " << PlayersFront().HandTotal();
          call_dialog.setDialogText(PlayersFront().Name() + " is holding Sabacc.\n" + total.str());
#else
          call_dialog.setDialogText(PlayersFront().Name() + " is holding Sabacc.");
#endif
        }

      call_dialog.setDialogTitle(PlayersFront().Name() + " wins.");

    }
  else if (Bomb(PlayersBack()) && Bomb(PlayersFront()))
    {

#ifdef _DEBUGCASINOGAME
      logAppend("Both sides bomb.");
#endif

      PlayersBack().SubtractCredits(HandPot());
// Player pays bomb penalty
      AddToSabaccPot(HandPot());

      call_dialog.setDialogText(PlayersBack().Name() + " and Dealer bomb,\nand " + PlayersBack().Name() + "pays a penalty to the Sabacc pot\nequal to the hand pot.");
      call_dialog.setDialogTitle("All players lose.");

    }
  else if (!Bomb(PlayersFront()) && Bomb(PlayersBack()))
    {

#ifdef _DEBUGCASINOGAME
      logAppend("Dealer wins: Player bombs");
#endif

      PlayersBack().SubtractCredits(HandPot());
      AddToSabaccPot(HandPot());

      call_dialog.setDialogText(PlayersBack().Name() + " bombed out,\nand pays a penalty to the Sabacc pot\nequal to the hand pot.");
      call_dialog.setDialogTitle(PlayersFront().Name() + " wins.");

    }
  else if (Bomb(PlayersFront()) && !Bomb(PlayersBack()))
    {

#ifdef _DEBUGCASINOGAME
      logAppend("Player wins: Dealer bombs");
#endif

      AddToSabaccPot(HandPot() / 2);

      call_dialog.setDialogText(PlayersBack().Name() + " wins,\n" + PlayersFront().Name() + " bombed out.");
      call_dialog.setDialogTitle(PlayersBack().Name() + " wins.");

    }
  else if ((IdiotArray(PlayersFront()) && IdiotArray(PlayersBack())) ||
           (PlayersFront().HandTotal() == PlayersBack().HandTotal()))
    {

#ifdef _DEBUGCASINOGAME
      logAppend("Draw");
#endif

      PlayersBack().AddCredits(HandPot() / 2);

      call_dialog.setDialogText("Hand is a draw.\n" + PlayersBack().Name() + " wins half of the hand pot.");
      call_dialog.setDialogTitle("Draw.");

    }
  else if ((PlayersFront().HandTotal() < PlayersBack().HandTotal()) &&
           (24 > PlayersBack().HandTotal()) && (-24 < PlayersBack().HandTotal()))
    {

#ifdef _DEBUGCASINOGAME
      logAppend("Player wins: Higher hand");
#endif

      PlayersBack().AddCredits(HandPot());
      AddToSabaccPot(HandPot() / 2);

      call_dialog.setDialogText(PlayersBack().Name()
                                + " holds the hand with the\nhighest total.");
      call_dialog.setDialogTitle(PlayersBack().Name()
                                 + " wins.");

    }
  else
    {

#ifdef _DEBUGCASINOGAME
      logAppend("Dealer wins");
#endif

      PlayersBack().SubtractCredits(HandPot() / 2);
      AddToSabaccPot(HandPot() / 2);

      call_dialog.setDialogText(PlayersFront().Name()
                                + " holds the hand with the\nhighest total.");
      call_dialog.setDialogTitle(PlayersFront().Name()
                                 + " wins.");

    }

  EmptyHandPot();
  ResetCurrentBet();
  ResetLastBet();

  dealer_cimages.setHideCards(false);

  getSystemManager().getRenderer().push(&call_dialog);
  getSystemManager().getEventManager().push(&call_dialog);

  while (event_dialog_dismiss != getSystemManager().getEventManager().doEvents())
    {
      getSystemManager().getRenderer().draw();
    }

  getSystemManager().getEventManager().pop();
  getSystemManager().getRenderer().pop();

  NewRound();

} // call

// Hold offers to hold, then places hold
// void CasinoGame::hold(Card&);	// Card
GameBase
&CasinoGame::HoldCard(vector<Card>::iterator card_iter)
{	// Card iterator
  (*card_iter).Hold(true);
  return *this;
}

GameBase
&CasinoGame::HoldCard(vector<Card>::size_type card_number)
{	// Card #
  /* if(pl.Hand().size() < card_number) throw(Exceptions::InvalidHold("Fatal Error: Card not found."));

  pl.Hand()[card_number].hold = true; */
  return *this;
} // hold(Player, int)

void
CasinoGame::InitCasinoGame()
{
  round_caption.setTextStyle(font_style_bold | font_style_underline);
  round_caption.setTextColor(0xFF, 0xD0, 0x40, 0xFF);

#if defined(__PSP__) || defined(__PSPTEST__)
  dealer_cimages.Position(Rect(20, 20, 288, 96));
  player_cimages.Position(Rect(20, player_caption.Top() - 96, 448, 96));
#else
  dealer_cimages.Position(Rect(20, 20, 0, 0));
  player_cimages.Position(Rect(20, player_caption.Top() - 96, 0, 0));
#endif

}

void
CasinoGame::DestroyCasinoGame()
{
  getSystemManager().getRenderer().clear();
}

bool
CasinoGame::PureSabacc(Player &pl)
{
    return (23 == pl.HandTotal()) || (-23 == pl.HandTotal());
}

bool
CasinoGame::Sabacc(Player &pl)
{
    return (23 == pl.HandTotal()) || (-23 == pl.HandTotal());
}

bool
CasinoGame::IdiotArray(Player &pl)
{
    // Player has an idiot, and cards valued at 2 and 3.
    bool has_zero = false;
    bool has_two = false;
    bool has_three = false;

    for (std::vector<Card>::iterator cd_it = pl.HandBegin();
	 pl.HandEnd() > cd_it;
	 ++cd_it)
    {
	if ((0 == (*cd_it).Value()) && (SUITE_BLANK != (*cd_it).Suite()))
	{
	    has_zero = true;
	} 
	else if (2 == (*cd_it).Value())
	{
	    has_two = true;
	}
	else if (3 == (*cd_it).Value())
	{
	    has_three = true;
	}// if(0, 2, 3)
    }	// for (card)

    return(has_zero && has_two && has_three);
}

bool
CasinoGame::Bomb(Player &pl)
{
    return ((0 == pl.HandTotal()) || (23 < pl.HandTotal()));
}
