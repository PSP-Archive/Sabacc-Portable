// Standard headers
#include <vector>

// SDL headers
#ifdef __APPLE__
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#else
#include <SDL.h>
#include <SDL_gfxPrimitives.h>
#endif

// Sabacc headers
#include "base/Widget.hh"
#include "game/objects/Card.hh"
#include "game/objects/Player.hh"
#include "sitextensions/CardImage.hh"
#include "sitextensions/CardImageSet.hh"
#include "geometry/Rect.hh"

// Debug
#ifdef _DEBUGCARDIMAGESET
#include "utility/Log.hh"
#include <cstdio>
#endif

CardImageSet::CardImageSet() : Widget(), card_images(), card_holder(), use_selection(false), hide_cards(false) { }
CardImageSet::CardImageSet(Rect pos) : Widget(pos), card_images(), card_holder(), use_selection(false), hide_cards(false) { }
CardImageSet::~CardImageSet() { }

void CardImageSet::setPlayer(Player* p) {

    card_holder = p;

    init();

}
void CardImageSet::setHideCards(bool hide) {
    for (std::vector<CardImage>::iterator cd_it = card_images.begin(); card_images.end() > cd_it; ++cd_it) {
	cd_it->setHiddenFace(hide);
    }
}  
void CardImageSet::init() {

    card_images.clear();

    if (!card_holder) return;

    for (std::vector<Card>::iterator pit = card_holder->getHand().begin(); card_holder->getHand().end() > pit; ++pit) {
        card_images.push_back(CardImage(&(*pit)));
    }

}	// clear
void CardImageSet::cleanup() { }

void CardImageSet::draw() {

    if (card_images.empty()) return;

    int x_start = Left();
    int x_sel = Left();

    std::vector<CardImage>::iterator c_sel = card_images.begin();

    Width((card_images.size() * card_images.back().Width() / 3) + card_images.back().Width());
    Height(card_images.back().Height());

#ifdef _DEBUGCARDIMAGESET
    char debug_string[128];
    sprintf(debug_string, "Drawing cards for 0x%x @ %d %d", card_holder, Left(), Top());
    logAppend(debug_string);
#endif

    for (std::vector<CardImage>::iterator it = card_images.begin(); card_images.end() > it; ++it) {

      if(card_images.end() != it) {
        it->Left(x_start);
        it->Top(Top());

	if (it->getCard()->Hold()) {
            Sint16 l = it->Left();
            Sint16 t = it->Top();
            Sint16 w = it->Width();
            Sint16 h = it->Height();
            Sint16 m = 4;

            Sint16 poly_x[8] = { 
		static_cast<Sint16>(l - m),
		static_cast<Sint16>(l + m),
		static_cast<Sint16>(l + w - m),
		static_cast<Sint16>(l + w + m),
		static_cast<Sint16>(l + w + m),
		static_cast<Sint16>(l + w - m),
		static_cast<Sint16>(l + m),
		static_cast<Sint16>(l - m)
	    };
            Sint16 poly_y[8] = {
		static_cast<Sint16>(t + m),
		static_cast<Sint16>(t - m),
		static_cast<Sint16>(t - m),
		static_cast<Sint16>(t + m),
		static_cast<Sint16>(t + h - m),
		static_cast<Sint16>(t + h + m),
		static_cast<Sint16>(t + h + m),
		static_cast<Sint16>(t + h - m)
	    };
            filledPolygonRGBA(SDL_GetVideoSurface(),
			      poly_x, poly_y, 8, 0xFF, 0xFF, 0x00, 0x80);

        }

        it->draw();

        if (&(*card_holder->getSelectedCardIterator()) == it->getCard()) {

            // if(!use_selection) it->draw();
            c_sel = it;
            x_sel = x_start;

        }

        x_start += static_cast<int>(static_cast<float>(it->Width()) / 1.33);

      } // if(it)
    }	// it

    if ((card_images.end() > c_sel) && use_selection) {
        c_sel->Left(x_sel);
	if (c_sel->getCard()->Hold()) {
            rectangleRGBA(SDL_GetVideoSurface(), x_sel - 1, c_sel->Top() - 1, x_sel + c_sel->Width() + 1,
                          c_sel->Top() + c_sel->Width() + 1, 0xFF, 0xFF, 0x88	, 0xFF);
        } else {
            rectangleRGBA(SDL_GetVideoSurface(), x_sel - 1, c_sel->Top() - 1, x_sel + c_sel->Width() + 1,
                          c_sel->Top() + c_sel->Width() + 1, 0x00, 0xCC, 0xFF, 0xFF);
        }

        c_sel->draw();

    }	// c_sel

}	// draw
