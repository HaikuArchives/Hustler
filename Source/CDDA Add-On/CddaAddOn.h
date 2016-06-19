#ifndef HUSTLER_CDDAADDON_H
#define HUSTLER_CDDAADDON_H

// ***************************************************************************
// System Includes
// ***************************************************************************

#include <List.h>

// ***************************************************************************
// Project Includes
// ***************************************************************************

// Standard type and constant definitions.
#include "../Hustler/StyleGuideDefs.h" 

// Debugging output macros.
#define  BUGSBGONE
#define  BBG_PREFIX "Hustler CddaAddOn"
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
 * The CddaAddOn class is a Hustler add-on for playing compact discs.
 *
 * @see CddaPlayer
 */
class CddaAddOn : public HustlerAddOn
{
public:
	// Constant Definitions ==================================================

	// Type Definitions ======================================================

	// Attribute Definitions =================================================

	// Member Function Definitions ===========================================

	/** Constructor. */
	CddaAddOn(void);

	/** Automatically generated copy constructor. */
	// CddaAddOn(const CddaAddOn& old);

	/** Automatically generated destructor. */
	virtual ~CddaAddOn(void);

	/** Automatically generated assignment operator. */
	// CddaAddOn& operator=(CddaAddOn& right);

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

	/** The names of the installed CD player devices. */
	BList* deviceNames;

	// Member Function Definitions ===========================================

	/** 
	 * Recursively searches directories for CD player devices and adds their
	 * names to the list of installed CD players.
	 *
	 * @param dirName The name directory to start the search in.
	 */
	void SearchForCdPlayer(const char* dirName);
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

#endif // HUSTLER_CDDAADDON_H
