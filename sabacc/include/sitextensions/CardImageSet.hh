#ifndef __CARDIMAGESET_HH
#define __CARDIMAGESET_HH

// Standard headers
#include <vector>

// Sabacc headers
#include "base/Widget.hh"
#include "geometry/Rect.hh"

// Forward declarations
class CardImage;
class Player;

/* This class uses a vector of CardImage objects and is derived from widget so that it may
 * be added to the render queue. */
class CardImageSet : public Widget
  {

    std::vector<CardImage>	card_images;
    Player*					card_holder;

    // No need to copy or assign card image sets
    CardImageSet(const CardImageSet&);
    CardImageSet& operator=(const CardImageSet&);

  public:

    bool					use_selection;
    bool hide_cards;

    CardImageSet();
    CardImageSet(Rect);
    ~CardImageSet();

    void setPlayer(Player*);

    // Hide the faces of the cards
    virtual void setHideCards(bool);

    // Required overrides
    virtual void init();
    virtual void cleanup();

    // Draw
    virtual void draw();

  };	// class CardImageSet

#endif // __CARDIMAGESET_HH
