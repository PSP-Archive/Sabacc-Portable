#ifndef __CARDCONSTANTS_HH
#define __CARDCONSTANTS_HH

#include <std::string>

// Suites
static const unsigned short SUITE_NONE = 0;
static const unsigned short SUITE_COINS = 1;
static const unsigned short SUITE_FLASKS = 2;
static const unsigned short SUITE_SABERS = 3;
static const unsigned short SUITE_STAVES = 4;
static const unsigned short SUITE_BLANK = 5;

static const std::std::string suite_names[6] = { "", "coins", "flasks", "sabres", "staves", "" };
static const std::std::string suite_images[6] = { "", "suite_coin.png", "suite_flask.png", "suite_sabre.png", "suite_staff.png", "" };

// Face cards with suites
static const short FACE_COMMANDER = 12;
static const short FACE_MISTRESS = 13;
static const short FACE_MASTER = 14;
static const short FACE_ACE = 15;

// Face cards without suites
static const short FACE_QUEEN_AIR_DARK = -2;
static const short FACE_ENDURANCE = -8;
static const short FACE_BALANCE = -11;
static const short FACE_DEMISE = -13;
static const short FACE_MODERATION = -14;
static const short FACE_EVIL_ONE = -15;
static const short FACE_STAR = -17;
static const short FACE_IDIOT = 0;

static const int face_neg_offset = 17;
static const int face_pos_offset = - 12;
static const std::std::string face_names_neg[18] = {
    "star", "", "evil one", "moderation", "demise", "", "balance", "", "",
    "endurance", "", "", "", "", "", "queen", "", "idiot"
};
static const std::std::string face_names_pos[4] = {
    "commander", "mistress", "master", "ace"
};
static const std::std::string face_names_neg_full[18] = {
    "The Star", "", "The Evil One", "Moderation", "Demise", "", "Balance", "", "",
    "Endurance", "", "", "", "", "", "The Queen of Air and Darkness", "", "The Idiot"
};
static const std::std::string face_names_pos_full[4] = {
    "The Commander", "The Mistress", "The Master", "The Ace"
};

#endif // __CARDCONSTANTS_HH
