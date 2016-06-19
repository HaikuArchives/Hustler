#ifndef HUSTLER_HUSTLERVIEW_H
#define HUSTLER_HUSTLERVIEW_H

// ***************************************************************************
// System Includes
// ***************************************************************************

#include <Bitmap.h>
#include <List.h>
#include <View.h>

// ***************************************************************************
// Project Includes
// ***************************************************************************

// Standard type and constant definitions.
#include "StyleGuideDefs.h"

#include "HustlerAddOn.h"
#include "HustlerPlayer.h"
#include "DummyPlayer.h"

// ***************************************************************************
// External Name Declarations
// ***************************************************************************

// ***************************************************************************
// External Variable Declarations
// ***************************************************************************

// ***************************************************************************
// Constant Definitions
// ***************************************************************************

// ***************************************************************************
// Type Definitions
// ***************************************************************************

// ***************************************************************************
// Class Definition
// ***************************************************************************

/**
 * The HustlerView class forms the heart of the Hustler replicant. 
 *
 * When instantiated from the Deskbar (using the Instantiate() function),
 * this class creates a small view that contains the number of the currently 
 * playing track (or "?" when no tracks are available, and "-" when playback
 * is stopped).
 *
 * The Hustler object also pops up a menu when the user clicks the mouse on
 * the view, and handles the selected item.
 *
 * @author Matthijs Hollemans <matthijs@weirdexperiments.com>
 * @version 2.0 (Saturday, 21 August 1999)
 *
 * @see Be's CDButton sample code on how to make Deskbar replicants.
 */
class HustlerView : public BView 
{
public:
	// Constant Definitions ==================================================

	// Type Definitions ======================================================

	// Attribute Definitions =================================================

	// Member Function Definitions ===========================================

	/**
	 * Default constructor. 
	 *
	 * This constructor is used when the Hustler application is run.
	 */
	HustlerView();

	/**
	 * Creates a new HustlerView object from a BMessage. 
	 *
	 * This constructor is used to "rehydrate" the archived HustlerView
	 * object after the Deskbar (or any other BShelf) has received it. It is
	 * called by Instantiate().
	 */
	HustlerView(BMessage* message);
	
	/** Automatically generated copy constructor. */
	// HustlerView(const HustlerView& old);

	/** Destructor. */
	virtual ~HustlerView();

	/** Automatically generated assignment operator. */
	// HustlerView& operator=(HustlerView& right);

	/** 
	 * Creates a new HustlerView object from an archived BMessage.
	 *
	 * This function is called by the Deskbar's Shelf after it has received
	 * the message that contains our replicant. (To enable the Deskbar to 
	 * find this function, we should export it from the executable.) 
	 */
	static HustlerView* Instantiate(BMessage* archive);

	/** 
	 * Creates an archive of the data that we need in order to instantiate 
	 * ourselves.
	 */
	virtual status_t Archive(BMessage* archive, bool deep = true) const;

	/** Invoked after the view has been added to the window. */
	virtual void AttachedToWindow(void);

	/** Draws the view. */
	virtual void Draw(BRect updateRect);

	/**
	 * Creates the pop-up menu on-the-fly when the mouse is clicked inside 
	 * the view, and waits for the user to make a selection.
	 */
	virtual void MouseDown(BPoint point);

	/**
	 * Handles the messages that the view receives. 
	 *
	 * All items from the pop-up menu send their messages directly to the
	 * view.
	 */
	virtual void MessageReceived(BMessage* message);

	/**
	 * Does the things that need to happen at regular intervals (such as
	 * telling the view to redraw itself, and blinking the track number in
	 * pause mode).
	 */
	virtual void Pulse(void);

protected:
	// Constant Definitions ==================================================

	// Type Definitions ======================================================

	// Attribute Definitions =================================================

	// Member Function Definitions ===========================================

private:
	// Constant Definitions ==================================================

	// Type Definitions ======================================================

	/** The messages that are sent by the menu items. */
	enum {
		MSG_MENU_PLAY = 10000,
		MSG_MENU_PAUSE,
		MSG_MENU_STOP,
		MSG_MENU_PREVIOUS,
		MSG_MENU_NEXT,
		MSG_MENU_GO_TO,
		MSG_MENU_VOLUME,
		MSG_MENU_PANNING,
		MSG_MENU_EJECT,
		MSG_MENU_PLAYER
	};
  
	// Attribute Definitions =================================================

	/**
	 * Whether the HustlerView has been instantiated from HustlerApp or from
	 * the Deskbar.
	 */
	bool isReplicated;
	
	/** List of pointers to LoadedAddOn objects. */
	BList* loadedAddOns;

	/** Pointer to the active HustlerPlayer object. */
	HustlerPlayer* player;
	
	/** Name of the active HustlerPlayer object. */
	char* playerName;
	
	/**
	 * Whether we need to draw the track number. This is used to make the 
	 * display blink in pause mode.
	 */
	bool mustBlink;

	/** The state of the player at the time Pulse() was last called. */
	HustlerPlayer::State oldState;

	/** The track that was playing at the time Pulse() was last called. */  
	int32 oldTrack;

	/** The system clock time the last time Pulse() was called. */
	bigtime_t oldPulseTime;

	/**
	 * Pointer to the dummy player. This player is always available, and is
	 * used when no add-ons can be loaded, or when none of the add-ons has
	 * an available player.
	 */
	DummyPlayer* dummyPlayer;
	      
	// Member Function Definitions ===========================================

	/** Loads all add-ons into memory. */
	void LoadAddOns(void);

	/** Removes all add-ons from memory. */
	void UnloadAddOns(void);

	/** Selects the first available player the loaded add-ons provide. */
	void SelectFirstAvailablePlayer(void);

	/**
	 * Selects a new player.
	 *
	 * @param addon The HustlerAddOn object that the player belongs to.
	 * @param name The name of the new player.
	 */
	void SelectPlayer(HustlerAddOn* addon, const char* name);

	/**
	 * Deletes the active HustlerPlayer object.
	 */
	void DeleteActivePlayer(void);
};

// ***************************************************************************
// Global Variable Declarations
// ***************************************************************************

// ***************************************************************************
// Inline Function Definitions
// ***************************************************************************

// ***************************************************************************
// Helper Function Declarations
// ***************************************************************************

// ***************************************************************************
// Global Function Declarations
// ***************************************************************************

#endif // HUSTLER_HUSTLERVIEW_H
