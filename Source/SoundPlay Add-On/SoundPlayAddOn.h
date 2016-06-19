#ifndef HUSTLER_SOUNDPLAYADDON_H
#define HUSTLER_SOUNDPLAYADDON_H

// ***************************************************************************
// System Includes
// ***************************************************************************

// ***************************************************************************
// Project Includes
// ***************************************************************************

// Standard type and constant definitions.
#include "../Hustler/StyleGuideDefs.h" 

// Debugging output macros.
#define  BUGSBGONE
#define  BBG_PREFIX "Hustler SoundPlayAddOn"
#undef   BBG_SERIAL
#include "../Hustler/Bugs-B-Gone.h"

// Our base class.
#include "../Hustler/HustlerAddOn.h"

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
 * The SoundPlayAddOn class is a Hustler add-on that can remote control the
 * SoundPlay audio player application.
 *
 * @see SoundPlayPlayer
 */
class SoundPlayAddOn : public HustlerAddOn
{
public:
	// Constant Definitions ==================================================

	// Type Definitions ======================================================

	// Attribute Definitions =================================================

	// Member Function Definitions ===========================================

	/** Constructor. */
	SoundPlayAddOn(void);

	/** Automatically generated copy constructor. */
	// SoundPlayAddOn(const SoundPlayAddOn& old);

	/** Automatically generated destructor. */
	virtual ~SoundPlayAddOn(void);

	/** Automatically generated assignment operator. */
	// SoundPlayAddOn& operator=(SoundPlayAddOn& right);

	/** See HustlerAddOn. */
	virtual char* GetInfo(void);
	
	/** See HustlerAddOn. */
	virtual int32 GetPlayerName(int32 index, char* &name);
	
	/** See HustlerAddOn. */
	virtual HustlerPlayer* GetPlayer(const char* name);

protected:
	// Constant Definitions ==================================================

	// Type Definitions ======================================================

	// Attribute Definitions =================================================

	// Member Function Definitions ===========================================

private:
	// Constant Definitions ==================================================

	// Type Definitions ======================================================

	// Attribute Definitions =================================================

	/** The team_id of the currently running instance of SoundPlay. */
	team_id team;

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

#endif // HUSTLER_SOUNDPLAYADDON_H
