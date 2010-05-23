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

    int x_start = getLeft();
    int x_sel = getLeft();

    std::vector<CardImage>::iterator c_sel = card_images.begin();

    setWidth((card_images.size() * card_images.back().getWidth() / 3) + card_images.back().getWidth());
    setHeight(card_images.back().getHeight());

#ifdef _DEBUGCARDIMAGESET
    char debug_string[128];
    sprintf(debug_string, "Drawing cards for 0x%x @ %d %d", card_holder, getLeft(), getTop());
    logAppend(debug_string);
#endif

    for (std::vector<CardImage>::iterator it = card_images.begin(); card_images.end() > it; ++it) {

      if(card_images.end() != it) {
        it->setLeft(x_start);
        it->setTop(getTop());

        if (it->getCard()->hold) {
            Sint16 l = it->getLeft();
            Sint16 t = it->getTop();
            Sint16 w = it->getWidth();
            Sint16 h = it->getHeight();
            Sint16 m = 5;

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
            filledPolygonRGBA(SDL_GetVideoSurface(), poly_x, poly_y, 8, 0xFF, 0xFF, 0x00, 0x80);

//			boxRGBA(SDL_GetVideoSurface(), x_start, it->getTop() - 1, x_start + it->getWidth() + 1,
//				it->getTop() + it->getWidth() + 1, 0xFF, 0x80, 0x40, 0xB0);
        }

        it->draw();

        if (&(*card_holder->getSelectedCardIterator()) == it->getCard()) {

            // if(!use_selection) it->draw();
            c_sel = it;
            x_sel = x_start;

        }

        x_start += static_cast<int>(static_cast<float>(it->getWidth()) / 1.33);

      } // if(it)
    }	// it

    if ((card_images.end() > c_sel) && use_selection) {
        c_sel->setLeft(x_sel);
        if (c_sel->getCard()->hold) {
            rectangleRGBA(SDL_GetVideoSurface(), x_sel - 1, c_sel->getTop() - 1, x_sel + c_sel->getWidth() + 1,
                          c_sel->getTop() + c_sel->getWidth() + 1, 0xFF, 0xFF, 0x88	, 0xFF);
        } else {
            rectangleRGBA(SDL_GetVideoSurface(), x_sel - 1, c_sel->getTop() - 1, x_sel + c_sel->getWidth() + 1,
                          c_sel->getTop() + c_sel->getWidth() + 1, 0x00, 0xCC, 0xFF, 0xFF);
        }

        c_sel->draw();

    }	// c_sel

}	// draw
