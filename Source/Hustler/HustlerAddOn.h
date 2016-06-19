#ifndef HUSTLER_HUSTLERADDON_H
#define HUSTLER_HUSTLERADDON_H

// ***************************************************************************
// System Includes
// ***************************************************************************

// ***************************************************************************
// Project Includes
// ***************************************************************************

// Standard type and constant definitions.
#include "StyleGuideDefs.h"

#include "HustlerPlayer.h"

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
 * The HustlerAddOn class defines the common behaviors for all Hustler 
 * add-ons that conform to version 1 of the Hustler add-on API.
 *
 * @see HustlerPlayer
 */
class HustlerAddOn
{
public:
	// Constant Definitions ==================================================

	// Type Definitions ======================================================

	/** The possible return values of the GetPlayerName() method. */
	enum
	{
		NAME_OK = 0,
		NAME_NOT_AVAILABLE,
		NAME_NO_MORE_NAMES
	};
	
	// Attribute Definitions =================================================

	// Member Function Definitions ===========================================

	/** Constructor. */
	HustlerAddOn(void)
	{
		// Do nothing.
	}

	/** Automatically generated copy constructor. */
	// HustlerAddOn(const HustlerAddOn& old);

	/** Destructor. */
	virtual ~HustlerAddOn(void)
	{
		// Do nothing.
	}

	/** Automatically generated assignment operator. */
	// HustlerAddOn& operator=(HustlerAddOn& right);

	/**
	 * Returns the version number of the add-on API that
	 * this add-on conforms to.
	 */
	virtual int32 GetVersion(void)
	{
		return 1;
	}

	/**
	 * Returns a string with information about this add-on; what it does,
	 * which version it is, who wrote it, and so on. This information will
	 * be displayed inside Hustler's about box.
	 */	
	virtual char* GetInfo(void) = 0;
	
	/**
	 * Returns the (human-readable) name of a certain player.
	 *
	 * When the user opens the pop-up menu, Hustler calls this method with 
	 * index 0, and then keeps calling it with an ever increasing index until
	 * the method returns NAME_NO_MORE_NAMES.
	 *
	 * @param index The index of the player.
	 * @param name The name of the player. This is an output parameter that
	 *        should return a _unique_ name for the player (so no two players
	 *        may have the same name, not even if they are created by
	 *        different add-ons.)
	 * @return NAME_OK if the name was returned successfully, 
	 *         NAME_NO_MORE_NAMES if there are no more players beyond this 
	 *         index, or NAME_NOT_AVAILABLE if the player at this index 
	 *         cannot be used (for whatever reason).
	 */
	virtual int32 GetPlayerName(int32 index, char* &name) = 0;

	/**
	 * Creates and returns a new HustlerPlayer object.
	 *
	 * The HustlerPlayer object will be deleted by Hustler when it is done
	 * with it. Therefore, GetPlayer() should _always_ return a new object.
	 *
	 * @param name The name of the player.
	 * @return The HustlerPlayer object, or null in case of an error.
	 */
	virtual HustlerPlayer* GetPlayer(const char* name) = 0;

protected:
	// Constant Definitions ==================================================

	// Type Definitions ======================================================

	// Attribute Definitions =================================================

	// Member Function Definitions ===========================================

private:
	// Constant Definitions ==================================================

	// Type Definitions ======================================================

	// Attribute Definitions =================================================

	// Member Function Definitions ===========================================
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

/**
 * Creates and returns an instance of the HustlerAddOn class.
 *
 * This function is called by Hustler after the add-on has been loaded.
 * Derived classes must implement this function to return an instance of
 * themselves.
 */
extern "C" __declspec(dllexport) HustlerAddOn* InstantiateHustlerAddOn();

#endif // HUSTLER_HUSTLERADDON_H
