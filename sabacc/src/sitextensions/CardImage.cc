// Standard library headers
#include <sstream>
#include <string>

// SDL headers
#ifdef __APPLE__
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#else
#include <SDL.h>
#include <SDL_image.h>
#endif

// Sabacc headers
#include "controls/StaticImage.hh"
#include "base/Widget.hh"
#include "defaults/Defaults.hh"
#include "freetype/FontManager.hh"
#include "game/CardConstants.hh"
#include "game/objects/Card.hh"
#include "exceptions/GameImpl.hh"
#include "exceptions/base/WidgetExceptions.hh"
#include "exceptions/io/FileExceptions.hh"
#include "sitextensions/CardImage.hh"
#include "system/SystemManager.hh"
#include "geometry/Rect.hh"

// Constants
#include "defaults/Defaults.hh"

// Image data
#include "images/SmallCardData.h"
#include "images/CoinsData.h"
#include "images/FlasksData.h"
#include "images/SabresData.h"
#include "images/StavesData.h"

#if defined(_DEBUG) || defined(_DEBUGCARDIMAGE)
#include "utility/Log.hh"
#endif

FreeTypeRenderer CardImage::card_renderer = FreeTypeRenderer();

CardImage::CardImage() : StaticImage(small_card_data, size_small_card_data, default_rect, "autosize: true; pixelformat: displayalpha"), card_data(), hide_face(false) { }
CardImage::CardImage(Card* cd) : StaticImage(small_card_data, size_small_card_data, default_rect, "autosize: true; pixelformat: displayalpha"), card_data(cd), hide_face(false) { }
CardImage::CardImage(const CardImage& src) : StaticImage(src), card_data(src.card_data), hide_face(false) { }
CardImage::~CardImage() { }

CardImage& CardImage::operator=(const CardImage& src) {

    if (&src != this) {
        StaticImage::operator=(src);

        card_data = src.card_data;
        setNotInitialized();
    }

    return(*this);
} // operator=

Card* CardImage::getCard() const {
    return(card_data);
}
void CardImage::setCard(Card* src) {
    card_data = src;
}

void CardImage::setHiddenFace(bool hide) { 
  hide_face = hide; 
  setNotInitialized();
}

void CardImage::init() {

    cleanup();

    StaticImage::init();

    if(hide_face) {
      setInitialized();
      return;
    }

#if defined(_DEBUG) || defined(_DEBUGCARDIMAGE)
    char debug_string[128];
    sprintf(debug_string, "Drawing card v: %d, s: %d.", card_data->value,
	    card_data->suite);
    logAppend(debug_string);
#endif

    // draw the suite
    if ((0 < card_data->suite) && (5 > card_data->suite)) {

      SDL_RWops* image_stream = 0;
      
      switch(card_data->suite)
	{
	case 1:
	  image_stream = SDL_RWFromMem(coins_image, size_coins_image);
	  break;
	case 2:
	  image_stream = SDL_RWFromMem(flasks_image, size_flasks_image);
	  break;
	case 3:
	  image_stream = SDL_RWFromMem(sabres_image, size_sabres_image);
	  break;
	case 4:
	  image_stream = SDL_RWFromMem(staves_image, size_staves_image);
	  break;
	default:
	  break;
	}
	
      SDL_Surface* suite_surf = IMG_Load_RW(image_stream, true);

        if (!suite_surf) {

#if defined(_DEBUG) || defined(_DEBUGCARDIMAGE)
            logAppend("CardImage: Could not load suite image.");
#endif

            throw(Exceptions::File::FileNotFound(__FILE__, "Could not load suite image"));

        }	// (!suite_surf)

        SDL_Rect dest = { (getWidth() - suite_surf->w) / 2, (getHeight() - suite_surf->w) / 2, suite_surf->w, suite_surf->h };
        if (-1 == SDL_BlitSurface(suite_surf, NULL, getSurface(), &dest)) {

#if defined(_DEBUG) || defined(_DEBUGCARDIMAGE)
            logAppend("CardImage: Could not blit card suite image.");
#endif

            throw(Exceptions::Widget::BlitError(__FILE__, "Could not blit card suite image."));

        }	// if(Blit)

        SDL_FreeSurface(suite_surf);


        if (!getSurface()) {

#ifdef _DEBUGCARDIMAGE
            logAppend("CardImage: Could not load suite image.");
#endif

            throw(Exceptions::File::FileNotFound(__FILE__, "Could not load suite image"));

        }	// (!suite_surf)

    }	// if(card_data->suite)

    card_renderer.pushState();

    if (1 > card_data->value) {
      //      getCardFont().setColor(default_card_name_color);
      getCardFont().render(face_names_neg[card_data->value + face_neg_offset],
                             getSurface(),
			   (getWidth() - getCardFont().lineWidth(face_names_neg[card_data->value + face_neg_offset])) / 2, getCardFont().getLineHeight() + 1);
    } else if (11 < card_data->value) {
      //      card_renderer.setColor(default_card_name_color);
      getCardFont().render(face_names_pos[card_data->value + face_pos_offset], getSurface(), (getWidth() - getCardFont().lineWidth(face_names_pos[card_data->value + face_pos_offset])) / 2, getCardFont().getLineHeight() + 1);
    } else {

#if !defined(__PSP__)
        card_renderer.setStyle(font_style_bold | font_style_italic | font_style_outline);
#else
        card_renderer.setStyle(font_style_bold | font_style_italic | font_style_outline);
#endif

	card_renderer.setColor(default_card_value_color);
	card_renderer.setStrokeColor(default_card_value_stroke_color);
        card_renderer.setSize(card_renderer.getSize() * 3);


        std::stringstream card_value;
        card_value << card_data->value;

        card_renderer.render(card_value.str(), getSurface(),
                             (getWidth() - card_renderer.lineWidth(card_value.str())) / 2,
                             (getHeight() - card_renderer.textHeight(card_value.str())) / 2);

    } // if(face card)

    card_renderer.popState();

    // Suite name rendering removed. Its not really necessary with the suite icons
    // and help line.

    //    card_renderer.pushState();

    //    card_renderer.setColor(default_card_suite_color);
    //    card_renderer.setSize(card_renderer.getSize() * .75);

    //    getCardFont().render(suite_names[card_data->suite], getSurface(), (getWidth() - getCardFont().lineWidth(suite_names[card_data->suite])) / 2, getHeight() - getCardFont().getLineHeight() * 2.25);

    //    card_renderer.popState();

    setInitialized();

} // init

// Draw the card
void CardImage::draw() {

    StaticImage::draw();

}	// draw()
