// Standard library headers
#include <algorithm>
#include <cstdio>
#include <functional>
#include <string>
#include <vector>
using std::for_each;
using std::mem_fun;
using std::string;
using std::vector;

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
#include "game/CardUtil.hh"
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
    player_caption("Player", Rect(10, 252), "autosize: true; anchor: bottom left"),
    selected_info("No card selected", Rect(32, 128), "autosize: true; anchor: left center"),
    rules_text("", Rect(350, 50), "autosize: true; anchor: top right; justify: top left"),
    round_caption("\\bRound 1\\b", Rect(340, 56), "autosize: true; anchor: bottom center"),
    round_info("Hand pot: 0cr\nSabacc Pot: 0cr", Rect(340, 178), "autosize: true; anchor: right center"),
#if defined(__PSP__) || defined(__PSPTEST__)
    dealer_cimages(Rect(20, 20, 288, 96)),
    player_cimages(Rect(20, player_caption.getTop() - 96, 448, 96)),
#else
    dealer_cimages(Rect(20, 20, 0, 0)),
    player_cimages(Rect(20, player_caption.getTop() - 96, 0, 0)),
#endif
    table_image(table_image_data, size_table_image_data) 
{
    
    round_caption.setTextStyle(font_style_bold | font_style_underline);
    round_caption.setTextColor(0xFF, 0xD0, 0x40, 0xFF);

    createDeck();
    shuffleDeck();
}	// CasinoGame
CasinoGame::CasinoGame(vector<Player> players) : 
    GameBase(players),
    dealer_caption("Dealer", Rect(), "autosize: true"),
    player_caption("Player", Rect(), "autosize: true; anchor: bottom left"),
    selected_info("No card selected", Rect(), "autosize: true; anchor: left center"),
    rules_text("", Rect(), "autosize: true; anchor: top right; justify: top left"),
    round_caption("\\bRound 1\\b", Rect(), "autosize: true; anchor: bottom center"),
    round_info("Hand pot: 0cr\nSabacc Pot: 0cr", Rect(400, 200, 79, 100), "anchor: left center"),
    dealer_cimages(Rect(10, 20, 0, 0)),
#if defined(__PSP__) || defined(__PSPTEST__)
    player_cimages(Rect(10, player_caption.getTop() - 96, 380, 96)),
#else
    player_cimages(Rect(10, player_caption.getTop() - 128, 0, 0)),
#endif
    table_image("table_top.png") {
    
    round_caption.setTextStyle(font_style_bold | font_style_underline);
    round_caption.setTextColor(0xFF, 0xD0, 0x40, 0xFF);

    createDeck();
    shuffleDeck();
}	// CasinoGame(players)
CasinoGame::CasinoGame(vector<Player> players, long starting_creds) :
        GameBase(players, starting_creds), dealer_caption("Dealer", Rect(), "autosize: true"),
        player_caption("Player", Rect(), "autosize: true; anchor: bottom left"),
        selected_info("No card selected", Rect(), "autosize: true; anchor: left center"),
        rules_text("<U>Rules<U>\n- Player pays penalty\nequivalent to\nhand pot on bomb.", Rect(), "autosize: true; anchor: top right; justify: top left"),
        round_caption("Round 1", Rect(), "autosize: true; anchor: bottom center"),
        round_info("Hand pot: 0cr\nSabacc Pot: 0cr", Rect(), "autosize: true; anchor: right center"),
        dealer_cimages(Rect(10, 20, 0, 0)),
#if defined(__PSP__) || defined(__PSPTEST__)
        player_cimages(Rect(10, player_caption.getTop() - 96, 380, 96)),
#else
        player_cimages(Rect(10, player_caption.getTop() - 128, 0, 0)),
#endif
        table_image("table_top.png") {

    round_caption.setTextStyle(font_style_bold | font_style_underline);
    round_caption.setTextColor(0xFF, 0xD0, 0x40, 0xFF);

    createDeck();
    shuffleDeck();
}	// CasinoGame(players, credits)
CasinoGame::~CasinoGame() {

    getSystemManager().getRenderer().clear();

}

void CasinoGame::start() {

#ifdef _DEBUGCASINOGAME
    char debug_string[64];
#endif

    if (2 != getPlayers().size()) throw(Exceptions::GamePlay::TooManyPlayers(__FILE__));

    dealer_cimages.setPlayer(&getPlayer(DEALER));
    player_cimages.setPlayer(&getPlayer(HUMAN));
    player_cimages.use_selection = true;

    dealer_cimages.setHideCards(true);

    // Do initial deal
    startNewRound();

    game();

    return;
}//start
void CasinoGame::game() {
    bool		done = false;
    SDL_Event	event;

    if(!SDL_WasInit(SDL_INIT_TIMER)) SDL_Init(SDL_INIT_TIMER);

    // loop - draw graphics and await input
    while (!done) {

        // Draw everything
        if (getSystemManager().getRenderer().empty()) {
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

        if (SDL_PollEvent(&event)) {

	  if(((SDL_JOYBUTTONDOWN == event.type) &&
	      (psp_triangle == event.jbutton.button)) ||
	     ((SDL_KEYDOWN == event.type) && 
	      (SDLK_h == event.key.keysym.sym))) {
	    
                    playHoldSound();

                    try {
			getPlayer(HUMAN).holdCard();
                    } catch (Exceptions::GamePlay::CannotHoldMore) {

                        // Display error dialog
                        SingleButtonDialog toomany("Unable to hold another card.", "You can hold a maximum of two cards.");
                        getSystemManager().getRenderer().push(&toomany);
                        getSystemManager().getEventManager().push(&toomany);

                        while (event_dialog_dismiss != getSystemManager().getEventManager().doEvents()) {
                            getSystemManager().getRenderer().draw();
                        }

                            getSystemManager().getEventManager().pop();
                            getSystemManager().getRenderer().pop();


		    }// try...
	  }// if(PSP Triangle || Keyb "h")

	  if(((SDL_JOYBUTTONDOWN == event.type) &&
	      (psp_dpad_up == event.jbutton.button)) ||
	     ((SDL_KEYDOWN == event.type) && 
	      (SDLK_UP == event.key.keysym.sym))) {

		  long bet_amount = getRules().getMinBet() * getRules().getChipSize();
		  long max_bet = (getRules().getMaxBet() * getRules().getChipSize());
		  if(max_bet >= (getCurrentBet() + bet_amount)) {
                    raiseCurrentBet(bet_amount);
                    getPlayer(HUMAN).decCredits(bet_amount);
		  }

                    updateUI();

	  }//if(PSP D-Pad up || Keyb CurUp)


	  if(((SDL_JOYBUTTONDOWN == event.type) &&
	      (psp_dpad_down == event.jbutton.button)) ||
	     ((SDL_KEYDOWN == event.type) && 
	      (SDLK_DOWN == event.key.keysym.sym))) {

		  long bet_amount = getRules().getMinBet() * getRules().getChipSize();
		  if(0 <= (getCurrentBet() - bet_amount)) {
                    lowerCurrentBet(bet_amount);
		    getPlayer(HUMAN).incCredits(bet_amount);
		  }

                    updateUI();

	  }//if(PSP D-Pad down || Keyb CurDn)

	  if(((SDL_JOYBUTTONDOWN == event.type) &&
	      (psp_cross == event.jbutton.button)) ||
	     ((SDL_KEYDOWN == event.type) &&
	      (SDLK_RETURN == event.key.keysym.sym))) {

                    if (0 < getCurrentBet()) {

                        if (getLastBet() > getCurrentBet()) {

                            char bet_too_low[256];
                            sprintf(bet_too_low, "Your bet must be at least as much as the last highest bet,\nwhich was %d credits.", getLastBet());
                            SingleButtonDialog bet_error(bet_too_low);
                            getSystemManager().getRenderer().push(&bet_error);
                            getSystemManager().getEventManager().push(&bet_error);

                            while (event_dialog_dismiss != getSystemManager().getEventManager().doEvents()) {
                                getSystemManager().getRenderer().draw();
                            }

                            getSystemManager().getEventManager().pop();
                            getSystemManager().getRenderer().pop();

                        } else {


			  // Fixed: The dealer should call before moving on
			  // to the next round.
			  dealerCall();

                            // Uncomment to follow fourth round shift rule
                            // if(4 <= ++getRound()) shift();
                            nextRound();
                            shift();

                            setLastBet(getCurrentBet());
			    getPlayer(HUMAN).decCredits(getCurrentBet());
                            addToHandPot(getCurrentBet());

                            // To reset the current bet to zero when continuing
			    // to the next round, uncomment the following
			    // function call, otherwise the bet will not be
			    // reset, which makes sense when the next bet can
			    // not be lower than this one.
			    // resetCurrentBet();

			    // Hack alert: rehide dealer cards
			    dealer_cimages.setHideCards(true);

                            updateUI();
                        }

                    } else {

		      SingleButtonDialog bet_error("You must place a bet before continuing.", "Try Again");
                        getSystemManager().getRenderer().push(&bet_error);
                        getSystemManager().getEventManager().push(&bet_error);

                        while (event_dialog_dismiss != getSystemManager().getEventManager().doEvents()) {
                            getSystemManager().getRenderer().draw();
                        }

                        getSystemManager().getEventManager().pop();
                        getSystemManager().getRenderer().pop();

                    }

	  }//if(PSP Cross || Keyb Enter)

	  if(((SDL_JOYBUTTONDOWN == event.type) &&
	      (psp_square == event.jbutton.button)) ||
	     ((SDL_KEYDOWN == event.type) && 
	      (SDLK_c == event.key.keysym.sym))) {

                    // Call can only happen after the third round
                    if (4 > getRound()) {

                        SingleButtonDialog call_error("You may not call before the fourth round.");
                        getSystemManager().getRenderer().push(&call_error);
                        getSystemManager().getEventManager().push(&call_error);

                        while (event_dialog_dismiss != getSystemManager().getEventManager().doEvents()) {
                            getSystemManager().getRenderer().draw();
                        }

                        getSystemManager().getEventManager().pop();
                        getSystemManager().getRenderer().pop();

                    } else {
                        call();
                    }

	  }//if(PSP Square || Keyb "C")
                   
	  if(((SDL_JOYBUTTONDOWN == event.type) &&
	      (psp_dpad_left == event.jbutton.button)) ||
	     ((SDL_KEYDOWN == event.type) && 
	      (SDLK_LEFT == event.key.keysym.sym))) {
                    // Move card iterator left
                    getPlayer(HUMAN).moveSelectedLeft();

                    updateUI();

                    playSelectSound();

	  }//if(PSP D-Pad Left || Keyb CurLt)

	  if(((SDL_JOYBUTTONDOWN == event.type) &&
	      (psp_dpad_right == event.jbutton.button)) ||
	     ((SDL_KEYDOWN == event.type) &&
	      (SDLK_RIGHT == event.key.keysym.sym))) {

                    // Move card iterator right
                    getPlayer(HUMAN).moveSelectedRight();

                    updateUI();

                    playSelectSound();
	  }//if(PSP D-Pad Right || Keyb CurRt)

	  if(((SDL_JOYBUTTONDOWN == event.type) &&
	      (psp_select == event.jbutton.button)) ||
	     ((SDL_KEYDOWN == event.type) && 
	      (SDLK_ESCAPE == event.key.keysym.sym))) {

                    done = true;

	  }// if(PSP Select || Keyb Escape)

          if(SDL_QUIT == event.type) done = true;

        } // if(SDL_WaitEvent)

	if(0 > getPlayer(HUMAN).getCredits()) {

	  SingleButtonDialog out_of_luck("No credits, no play.", "Get out of here!");

                        getSystemManager().getRenderer().push(&out_of_luck);
                        getSystemManager().getEventManager().push(&out_of_luck);

                        while (event_dialog_dismiss != getSystemManager().getEventManager().doEvents()) {
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

void CasinoGame::updateUI() {

    char ui_output[128];

#if defined(_DEBUG) || defined(_DEBUGDEALERHAND)
    sprintf(ui_output, "\\b\\i%s\\i\\b     %d total in hand", getPlayer(DEALER).getName().c_str(), getPlayer(DEALER).getHandTotal());
    dealer_caption.setText(ui_output);
#endif// debug dealer

    sprintf(ui_output, "\\b\\i%s\\i\\b     %d credits, %d total in hand", getPlayer(HUMAN).getName().c_str(), getPlayer(HUMAN).getCredits(), getPlayer(HUMAN).getHandTotal());
    player_caption.setText(ui_output);

    sprintf(ui_output, "\\bCurrent bet:\\b %dcr\n\\bHand pot:\\b %dcr\n\\bSabacc pot:\\b %dcr", getCurrentBet(), getHandPot(), getSabaccPot());
    round_info.setText(ui_output);

    selected_info.setText(getFullCardName(getPlayer(HUMAN).getSelectedCard()));

    sprintf(ui_output, "\\bRound %d\\b", getRound());
    round_caption.setText(ui_output);

    dealer_cimages.setHideCards(true);

}	// updateUI

void CasinoGame::startNewRound() {

  for (vector<Player>::iterator it = getPlayers().begin(); getPlayers().end() > it; ++it) {

        it->getHand().clear();

    }

  // for_each(getPlayers().begin(), getPlayers().end(), mem_fun(&Player::emptyHand));

    /* Once upon a time (pre 0.41), a round ended but the deck continued to be used.
     * This meant that when cards ran out, bad things happened (usually a crash).
     * Also, the waste deck wasn't emptied, so the deck continued to grow with each round...
     * (WTF? The deck has 100 cards? Now 130!!).
     *
     * This comment is only here to serve as a reminder. */
    getWaste().clear();
    createDeck();

    // v0.41 forgot to shuffle oops hahaha.
    shuffleDeck();

    deal();

    dealer_cimages.init();
    player_cimages.init();

    dealer_cimages.setHideCards(true);

    firstRound();

    dealer_cimages.setHideCards(true);

    updateUI();

}	// startNewRound

// Card operations
void CasinoGame:: deal() {

#ifdef _DEBUGCASINOGAME
    logAppend("Deal in CasinoGame.");
#endif

    if (2 != getPlayers().size()) throw(Exceptions::GamePlay::TooManyPlayers(__FILE__));

    for (int card_cnt = 0; 4 > card_cnt; ++card_cnt) {
        if (getPlayers()[0].takeCard(getDeck().back())) {
            getDeck().pop_back();
        }

        if (getPlayers()[1].takeCard(getDeck().back())) {
            getDeck().pop_back();
        }
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
void CasinoGame::shift() {
//	Random shift_gen;

//	int shift_probability = shift_gen(6);

//	if(shift_probability % 2) return;	// Shift only on even roll

#ifdef _DEBUGCASINOGAME
    char debug_string[64];
    sprintf(debug_string, "Shift is in effect for round %d, in CasinoGame.", getRound());
    logAppend(debug_string);
#endif

    // Each player's cards get shifted in turn
    for (vector<Player>::iterator pl_it = getPlayers().begin(); getPlayers().end() > pl_it; ++pl_it) {
        // Go through cards, determine which cards get shifted
        for (vector<Card>::iterator cd_it = pl_it->getHand().begin(); pl_it->getHand().end() > cd_it; ++cd_it) {

            /* Early versions (pre 0.41) never moved waste cards 
	     * back to the deck, so at some
             * point when the deck no longer had any cards to deal, 
	     * a bad pointer would be passed to
             * the CardImage widget, and well, things were bad. 
	     * Ooops! Led to hours of fruitless debugging.
             *
             * This is only here as a reminder. */
#ifdef _DEBUGCASINOGAME
		logAppend("Checking empty deck");
#endif

            if (getDeck().empty()) {
		
#if _DEBUGCASINOGAME
		logAppend("Replacing empty deck");
		sprintf(debug_string, "Waste size %d", getWaste().size());
		logAppend(debug_string);
#endif
		
		copy(getWaste().begin(), getWaste().end(),
		     back_inserter(getDeck()));

                getWaste().clear();
                shuffleDeck();
		
            }	// if(1 > getDeck().size())

#ifdef _DEBUGCASINOGAME
		logAppend("Good");
#endif

#ifdef _DEBUGCASINOGAME
		sprintf(debug_string, "Deck size %d", getDeck().size());
    logAppend(debug_string);
#endif
	    
            if (!cd_it->hold) {

		shuffleDeck();

                pl_it->dropCard(cd_it);
                getWaste().push_back(*cd_it);

		// Make sure the new card is not held
		getDeck().back().hold = false;
                pl_it->takeCard(getDeck().back());
                getDeck().pop_back();

//				} // if shift card

            }	// cd_it

        } // for(cd_it)

    } // for(pl_it)

    dealer_cimages.init();
    player_cimages.init();

    dealer_cimages.setHideCards(true);

} // shift

// Hand operations
void CasinoGame::dealerCall() {

#ifdef _DEBUGCASINOGAME
    char dealer_call[64];
    logAppend("Dealer thinking about calling in CasinoGame.");

    sprintf(dealer_call, "Player total %d, dealer %d.", getPlayer(HUMAN).getHandTotal(), getPlayer(DEALER).getHandTotal());
    logAppend(dealer_call);
#endif

    // Calculates the probability that the dealer should call.
    float call_likeliness = 0.0f;

    Random call_indicision_gen;

    // Idiot array or sabacc is a definite time to call
    if (getPlayer(DEALER).idiotArray() 
	|| getPlayer(DEALER).pureSabacc() || getPlayer(DEALER).sabacc()) 
	call();

    if ((23 > getPlayer(DEALER).getHandTotal()) 
	&& (21 < getPlayer(DEALER).getHandTotal())) {

#ifdef _DEBUGCASINOGAME
        logAppend("Between 22 and 23");
#endif

        // No brainer.
        call();

    } else if ((22 > getPlayer(DEALER).getHandTotal()) && (18 < getPlayer(DEALER).getHandTotal())) {

#ifdef _DEBUGCASINOGAME
        logAppend("Between 19 and 23");
#endif

        // I need a four or less, "should I or shouldn't I?"
        if (1 == call_indicision_gen(2)) call();	//	"I should"

    } else if ((19 > getPlayer(DEALER).getHandTotal()) && (15 < getPlayer(DEALER).getHandTotal())) {

#ifdef _DEBUGCASINOGAME
        logAppend("Between 16 and 18");
#endif

        // I need between five and seven

        int balls_factor = call_indicision_gen(23);
        float likelihood_5_7 = call_indicision_gen(23) / 12;	// 1.94 greatest

        call_likeliness = (balls_factor / likelihood_5_7) / 23;	// .51 greatest

        // Adjust to scale of 0 to 1 or close to if
        call_likeliness *= 1.9;

#ifdef _DEBUGCASINOGAME
        sprintf(dealer_call, "Dealer (prob: %f)", call_likeliness);
        dealer_caption.setText(dealer_call);
#endif

        if (0.85f < call_likeliness) call();	// All that fancy math for one function call hahahaha

    } else {	// Probably need more cards

        return;

    }	// if(need card)

    /* Notes:
     * - Never call on hand with total greater than 23 if it is not 46. This is a bomb.
     * - Improve call likelood calculation with balls factor.
     */

}	// dealerCall

// Calls the hand, ends play for this hand
void CasinoGame::call() {

#ifdef _DEBUGCASINOGAME
    char debug_string[64];
    logAppend("Call in CasinoGame.");
#endif

    // Call can only happen after the third round
    if (4 > getRound()) return;

#ifdef _DEBUGCASINOGAME
    sprintf(debug_string, "Player total %d, dealer %d.", getPlayer(HUMAN).getHandTotal(), getPlayer(DEALER).getHandTotal());
    logAppend(debug_string);
#endif

    // Update the UI. Sometimes the displayed total and the final don't match
    // maybe we're not updating UI before call.
    updateUI();

    SingleButtonDialog call_dialog("", "Call", "Continue", Rect(30, 85 , 420, 102));

    if ((getPlayer(HUMAN).idiotArray() && !getPlayer(DEALER).idiotArray()) ||
            (getPlayer(HUMAN).pureSabacc() && !getPlayer(DEALER).idiotArray() && !getPlayer(DEALER).pureSabacc()) ||
            (getPlayer(HUMAN).sabacc() && !getPlayer(DEALER).idiotArray() && !getPlayer(DEALER).pureSabacc() && !getPlayer(DEALER).sabacc())) {

#ifdef _DEBUGCASINOGAME
        logAppend("Player wins: Idiot or Sabacc");
#endif

        getPlayer(HUMAN).incCredits(getHandPot() + getSabaccPot());
        emptySabaccPot();

        if(getPlayer(HUMAN).idiotArray()) {
	  call_dialog.setDialogText(getPlayer(HUMAN).getName() + " is holding an Idiot array.");
	} else {
	  call_dialog.setDialogText(getPlayer(HUMAN).getName() + " is holding Sabacc.");
	}

        call_dialog.setDialogTitle(getPlayer(HUMAN).getName() + " wins.");

    } else if ((getPlayer(DEALER).idiotArray() && !getPlayer(HUMAN).idiotArray()) ||
               (getPlayer(DEALER).pureSabacc() && !getPlayer(HUMAN).idiotArray() && !getPlayer(HUMAN).pureSabacc()) ||
               (getPlayer(DEALER).sabacc() && !getPlayer(HUMAN).idiotArray() && !getPlayer(HUMAN).pureSabacc() && !getPlayer(HUMAN).sabacc())) {

#ifdef _DEBUGCASINOGAME
        logAppend("Dealer wins: Idiot or Sabacc");
#endif

        emptySabaccPot();

        if(getPlayer(DEALER).idiotArray()) {
#if defined(_DEBUG) || defined(_DEBUGDEALERHAND)
	  stringstream total;
	  total << "Dealer's total: " << getPlayer(DEALER).getHandTotal();
	  call_dialog.setDialogText(getPlayer(DEALER).getName() + " is holding an Idiot array.\n" + total.str());
#else
	  call_dialog.setDialogText(getPlayer(DEALER).getName() + " is holding an Idiot array.");
#endif
	} else {
#if defined(_DEBUG) || defined(_DEBUGDEALERHAND)
	  stringstream total;
	  total << "Dealer's total: " << getPlayer(DEALER).getHandTotal();
	  call_dialog.setDialogText(getPlayer(DEALER).getName() + " is holding Sabacc.\n" + total.str());
#else
	  call_dialog.setDialogText(getPlayer(DEALER).getName() + " is holding Sabacc.");
#endif
	}

        call_dialog.setDialogTitle(getPlayer(DEALER).getName() + " wins.");

    } else if (getPlayer(HUMAN).bomb() && getPlayer(DEALER).bomb()) {

#ifdef _DEBUGCASINOGAME
        logAppend("Both sides bomb.");
#endif

        getPlayer(HUMAN).decCredits(getHandPot());	// Player pays bomb penalty
        addToSabaccPot(getHandPot());

        call_dialog.setDialogText(getPlayer(HUMAN).getName() + " and Dealer bomb,\nand " + getPlayer(HUMAN).getName() + "pays a penalty to the Sabacc pot\nequal to the hand pot.");
        call_dialog.setDialogTitle("All players lose.");

    } else if (!getPlayer(DEALER).bomb() && getPlayer(HUMAN).bomb()) {

#ifdef _DEBUGCASINOGAME
        logAppend("Dealer wins: Player bombs");
#endif

        getPlayer(HUMAN).decCredits(getHandPot());
        addToSabaccPot(getHandPot());

        call_dialog.setDialogText(getPlayer(HUMAN).getName() + " bombed out,\nand pays a penalty to the Sabacc pot\nequal to the hand pot.");
        call_dialog.setDialogTitle(getPlayer(DEALER).getName() + " wins.");

    } else if (getPlayer(DEALER).bomb() && !getPlayer(HUMAN).bomb()) {

#ifdef _DEBUGCASINOGAME
        logAppend("Player wins: Dealer bombs");
#endif

        addToSabaccPot(getHandPot() / 2);

        call_dialog.setDialogText(getPlayer(HUMAN).getName() + " wins,\n" + getPlayer(DEALER).getName() + " bombed out.");
        call_dialog.setDialogTitle(getPlayer(HUMAN).getName() + " wins.");

    } else if ((getPlayer(DEALER).idiotArray() && getPlayer(HUMAN).idiotArray()) ||
               (getPlayer(DEALER).getHandTotal() == getPlayer(HUMAN).getHandTotal())) {

#ifdef _DEBUGCASINOGAME
        logAppend("Draw");
#endif

        getPlayer(HUMAN).incCredits(getHandPot() / 2);

        call_dialog.setDialogText("Hand is a draw.\n" + getPlayer(HUMAN).getName() + " wins half of the hand pot.");
        call_dialog.setDialogTitle("Draw.");

    } else if ((getPlayer(DEALER).getHandTotal() < getPlayer(HUMAN).getHandTotal()) &&
               (24 > getPlayer(HUMAN).getHandTotal()) && (-24 < getPlayer(HUMAN).getHandTotal())) {

#ifdef _DEBUGCASINOGAME
        logAppend("Player wins: Higher hand");
#endif

        getPlayer(HUMAN).incCredits(getHandPot());
        addToSabaccPot(getHandPot() / 2);

        call_dialog.setDialogText(getPlayer(HUMAN).getName() + " holds the hand with the\nhighest total.");
        call_dialog.setDialogTitle(getPlayer(HUMAN).getName() + " wins.");

    } else {

#ifdef _DEBUGCASINOGAME
        logAppend("Dealer wins");
#endif

        getPlayer(HUMAN).decCredits(getHandPot() / 2);
        addToSabaccPot(getHandPot() / 2);

        call_dialog.setDialogText(getPlayer(DEALER).getName() + " holds the hand with the\nhighest total.");
        call_dialog.setDialogTitle(getPlayer(DEALER).getName() + " wins.");

    }

    emptyHandPot();
    resetCurrentBet();
    resetLastBet();

    dealer_cimages.setHideCards(false);

    getSystemManager().getRenderer().push(&call_dialog);
    getSystemManager().getEventManager().push(&call_dialog);

    while (event_dialog_dismiss != getSystemManager().getEventManager().doEvents()) {
        getSystemManager().getRenderer().draw();
    }

    getSystemManager().getEventManager().pop();
    getSystemManager().getRenderer().pop();

    startNewRound();

} // call

// Hold offers to hold, then places hold
// void CasinoGame::hold(Card&);	// Card
void CasinoGame::hold(vector<Card>::iterator card_iter) {	// Card iterator
    card_iter->hold = true;
}

void CasinoGame::hold(vector<Card>::size_type card_number) {	// Card #
    /* if(pl.getHand().size() < card_number) throw(Exceptions::InvalidHold("Fatal Error: Card not found."));

    pl.getHand()[card_number].hold = true; */
} // hold(Player, int)
