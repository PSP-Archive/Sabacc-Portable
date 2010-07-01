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

CardImage::CardImage() :
    StaticImage(small_card_data, size_small_card_data, default_rect,
		"autosize: true; pixelformat: displayalpha"), card_data(),
    hide_face(false) { }
CardImage::CardImage(Card* cd) :
    StaticImage(small_card_data, size_small_card_data, default_rect,
		"autosize: true; pixelformat: displayalpha"), card_data(cd),
    hide_face(false) { }
CardImage::CardImage(const CardImage& src) :
    StaticImage(src), card_data(src.card_data), hide_face(false) { }
CardImage::~CardImage() { }

CardImage
&CardImage::operator=(const CardImage& src) {

    if (&src != this) {
	StaticImage::operator=(src);

	card_data = src.card_data;
	setNotInitialized();
    }

    return(*this);
} // operator=

Card
*CardImage::CardData() const {
    return(card_data);
}

void
CardImage::CardData(Card *c) {
    card_data = c;
}

bool
CardImage::HideFace()
{
    return hide_face;
}

void
CardImage::HideFace(bool hide) {
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
    sprintf(debug_string, "Drawing card v: %d, s: %d.", card_data->Value(),
	    card_data->Suite());
    logAppend(debug_string);
#endif

    // There should be a valid surface, otherwise something went wrong,
    if (!getSurface()) {

#ifdef _DEBUGCARDIMAGE
	logAppend("CardImage: Error.");
#endif

	throw
	    Exceptions::File::FileNotFound(__FILE__,
					   "Invalid card image surface.");

    }	// (!getSurface)


    // draw the suite
    if ((0 < card_data->Suite()) && (5 > card_data->Suite()))
    {

	SDL_RWops* image_stream = 0;

	switch(card_data->Suite())
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
	    logAppend("CardImage: Did not load suite image.");
	    logAppend(SDL_GetError());
#endif
	}	// (!suite_surf)

	SDL_Rect dest = {
	    static_cast<Sint16>((Width() - suite_surf->w) / 2),
	    static_cast<Sint16>((Height() - suite_surf->w) / 2),
	    static_cast<Sint16>(suite_surf->w),
	    static_cast<Sint16>(suite_surf->h)
	};
	
	if (-1 == SDL_BlitSurface(suite_surf, NULL, getSurface(), &dest)) {
#if defined(_DEBUG) || defined(_DEBUGCARDIMAGE)
	    logAppend("CardImage: Could not blit card suite image.");
	    logAppend(SDL_GetError());
#endif
	}	// if(Blit)

	SDL_FreeSurface(suite_surf);

    }	// if(card_data->Suite())

    card_renderer.pushState();

    int temp_width = 0;
    int temp_height = getCardFont().getLineHeight() + 1;

    if (1 > card_data->Value()) {
	temp_width =
	    getCardFont().lineWidth(face_names_neg[card_data->Value()
						   + face_neg_offset]);

	getCardFont().render(face_names_neg[card_data->Value()
					    + face_neg_offset],
			     getSurface(),
			     (Width() - temp_width) / 2,
			     temp_height);

    } else if (11 < card_data->Value()) {
	temp_width = Width()
	    - getCardFont().lineWidth(face_names_pos[card_data->Value()
						     + face_pos_offset]);

	getCardFont().render(face_names_pos[card_data->Value()
					    + face_pos_offset],
			     getSurface(),
			     temp_width / 2,
			     temp_height);
    } else {

	card_renderer.setStyle(font_style_bold | font_style_italic
			       | font_style_outline);

	card_renderer.setColor(default_card_value_color);
	card_renderer.setStrokeColor(default_card_value_stroke_color);
	card_renderer.setSize(card_renderer.getSize() * 3);

	std::stringstream card_value;
	card_value << card_data->Value();

	temp_width = Width() - card_renderer.lineWidth(card_value.str());
	temp_height = Height() - card_renderer.textHeight(card_value.str());

	card_renderer.render(card_value.str(), getSurface(),
			     temp_width / 2,
			     temp_height / 2);

    } // if(face card)

    card_renderer.popState();

    setInitialized();

} // init

// Draw the card
void
CardImage::draw()
{
    StaticImage::draw();
}	// draw()
