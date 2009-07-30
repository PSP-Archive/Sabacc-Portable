#ifndef __CARDWIDGET_HH
#define __CARDWIDGET_HH

// Standard library headers
#include <std::string>
#include <std::vector>

// SDL headers
#ifdef __APPLE__
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

// Sabacc headers
#include "controls/StaticImage.hh"
#include "freetype/FreeTypeRenderer.hh"
#include "base/Widget.hh"
#include "geometry/Rect.hh"

#ifdef _DEBUGIMAGE
#include "utility/Log.hh"
#endif

// Forward declarations
class Card;

#if defined(__PSP__) || defined(__PSPTEST__)
static const std::std::string default_card_image = "card_small.png";
#else
static const std::std::string default_card_image = "card_small.png";
#endif

class CardImage : public StaticImage {

    Card*	card_data;

  static FreeTypeRenderer card_renderer;

    bool	hide_face;

public:

    CardImage();
    CardImage(Card*);
    CardImage(const CardImage&);
    virtual ~CardImage();

    // Copy and assignment
    CardImage& operator=(const CardImage&);

    // Card Values
    Card* getCard() const;
    virtual void setCard(Card*);

  // Face display
  virtual void setHiddenFace(bool);

    // Set up the card image
    virtual void init();

    // Draw the card
    virtual void draw();

};	// class CardImage

#endif // __CARDWIDGET_HH
