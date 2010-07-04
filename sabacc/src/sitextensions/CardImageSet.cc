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

CardImageSet::CardImageSet() :
    Widget(), card_images(), card_holder(), use_selection(false),
    hide_cards(false) { }
CardImageSet::CardImageSet(Rect pos) :
    Widget(pos), card_images(), card_holder(), use_selection(false),
    hide_cards(false) { }
CardImageSet::~CardImageSet() { }

void
CardImageSet::setPlayer(Player* p)
{

  card_holder = p;

  init();

}

void
CardImageSet::setHideCards(bool hide)
{
  for (std::vector<CardImage>::iterator cd_it = card_images.begin();
       card_images.end() > cd_it;
       ++cd_it)
    {
      (*cd_it).HideFace(hide);
    }
}

void
CardImageSet::init()
{

  card_images.clear();

  if (!card_holder) return;

  for (std::vector<Card>::iterator pit = (*card_holder).HandBegin();
       (*card_holder).HandEnd() != pit;
       ++pit)
  {
      card_images.push_back(CardImage(&(*pit)));
  }

}	// clear
void
CardImageSet::cleanup() { }

void
CardImageSet::draw()
{

  if (card_images.empty()) return;

  int x_start = Left();
  int x_sel = Left();

  std::vector<CardImage>::iterator c_sel = card_images.begin();

  Width((card_images.size() * card_images.back().Width() / 3)
        + card_images.back().Width());

  Height(card_images.back().Height());

#ifdef _DEBUGCARDIMAGESET
  char debug_string[128];
  sprintf(debug_string, "Drawing cards for 0x%x @ %d %d",
          card_holder,
          Left(),
          Top());
  logAppend(debug_string);
#endif

  for (std::vector<CardImage>::iterator it = card_images.begin();
       card_images.end() > it;
       ++it)
    {

      if (card_images.end() != it)
        {
          (*it).Left(x_start);
          (*it).Top(Top());

	  (*it).draw();

              Sint16 l = (*it).Left();
              Sint16 t = (*it).Top();
              Sint16 w = (*it).Width();
              Sint16 h = (*it).Height();
              Sint16 m = 3;


          if ((*it).CardData()->Hold())
            {

              Sint16 poly_x[13] =
              {
                static_cast<Sint16>(l + 12),
                static_cast<Sint16>(l + 32),
                static_cast<Sint16>(l + 63),
                static_cast<Sint16>(l + 83),
                static_cast<Sint16>(l + 87),
                static_cast<Sint16>(l + 87),
                static_cast<Sint16>(l + 83),
		static_cast<Sint16>(l + 63),
		static_cast<Sint16>(l + 32),
		static_cast<Sint16>(l + 12),
		static_cast<Sint16>(l + 8),
		static_cast<Sint16>(l + 8),
		static_cast<Sint16>(l + 12)
              };

              Sint16 poly_y[13] =
              {
                static_cast<Sint16>(t + 12),
                static_cast<Sint16>(t + 8),
                static_cast<Sint16>(t + 8),
                static_cast<Sint16>(t + 12),
                static_cast<Sint16>(t + 32),
                static_cast<Sint16>(t + 63),
                static_cast<Sint16>(t + 83),
		static_cast<Sint16>(t + 87),
		static_cast<Sint16>(t + 87),
		static_cast<Sint16>(t + 83),
		static_cast<Sint16>(t + 63),
		static_cast<Sint16>(t + 32),
		static_cast<Sint16>(t + 12)
              };

              filledPolygonRGBA(SDL_GetVideoSurface(),
				poly_x, poly_y, 13, 0x00, 0xA0, 0xFF, 0x60);
            }

	  if ((&(*card_holder->SelectedCard()) == (*it).CardData())
	      && use_selection)
            {

		Sint16 poly_x[13] =
		    {
			static_cast<Sint16>(l + 12),
			static_cast<Sint16>(l + 32),
			static_cast<Sint16>(l + 63),
			static_cast<Sint16>(l + 83),
			static_cast<Sint16>(l + 87),
			static_cast<Sint16>(l + 87),
			static_cast<Sint16>(l + 83),
			static_cast<Sint16>(l + 63),
			static_cast<Sint16>(l + 32),
			static_cast<Sint16>(l + 12),
			static_cast<Sint16>(l + 8),
			static_cast<Sint16>(l + 8),
			static_cast<Sint16>(l + 12)
		    };

		Sint16 poly_y[13] =
		    {
			static_cast<Sint16>(t + 12),
			static_cast<Sint16>(t + 8),
			static_cast<Sint16>(t + 8),
			static_cast<Sint16>(t + 12),
			static_cast<Sint16>(t + 32),
			static_cast<Sint16>(t + 63),
			static_cast<Sint16>(t + 83),
			static_cast<Sint16>(t + 87),
			static_cast<Sint16>(t + 87),
			static_cast<Sint16>(t + 83),
			static_cast<Sint16>(t + 63),
			static_cast<Sint16>(t + 32),
			static_cast<Sint16>(t + 12)
		    };

		filledPolygonRGBA(SDL_GetVideoSurface(),
				  poly_x, poly_y, 13, 0xFF, 0xFF, 0xFF, 0x60);
	}

          x_start += static_cast<int>(static_cast<float>((*it).Width()) / 1.33);

        } // if(it)
    }	// it

}	// draw
