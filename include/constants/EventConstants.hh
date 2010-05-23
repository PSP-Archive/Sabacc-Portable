#ifndef __SMF_EVENTCONSTANTS_HH
#define __SMF_EVENTCONSTANTS_HH

/*
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: August 30, 2008
 *
 * File: EventConstants.hh
 *
 * Event identifiers.
 */

typedef int smf_event_t;

/* Generic events */
static const smf_event_t event_none = 0x0;
static const smf_event_t event_quit = 0x1;

/* Menus 0x10 - 0x1F */
static const smf_event_t event_menu_item_selected = 0x10;

/* Dialogs 0x20 - 0x */
static const smf_event_t event_dialog_dismiss = 0x20;
static const smf_event_t event_dialog_ok = 0x21;
static const smf_event_t event_dialog_cancel = 0x22;

#endif // __SMF_EVENTCONSTANTS_HH
