#ifndef HUSTLER_HUSTLERAPP_H
#define HUSTLER_HUSTLERAPP_H

// ***************************************************************************
// System Includes
// ***************************************************************************

#include <Application.h>

// ***************************************************************************
// Project Includes
// ***************************************************************************

// Standard type and constant definitions.
#include "StyleGuideDefs.h"         

// ***************************************************************************
// External Name Declarations
// ***************************************************************************

// ***************************************************************************
// External Variable Declarations
// ***************************************************************************

// ***************************************************************************
// Constant Definitions
// ***************************************************************************

/** The MIME signature of our application. */
#define APP_SIGNATURE "application/x-vnd.mahlzeit.Hustler"

// ***************************************************************************
// Type Definitions
// ***************************************************************************

// ***************************************************************************
// Class Definition
// ***************************************************************************

/** 
 * The HustlerApp class installs the Hustler replicant in the Deskbar.
 *
 * This class creates a HustlerView object, archives it, and sends it to the 
 * Deskbar where it will be replicated. As soon as the replicant is sent, the
 * application will terminate itself.
 * 
 * Because the application only runs for about a second (probably less),
 * there is no need for its title to appear inside the Deskbar. Therefore, 
 * the application flags inside the resource file Hustler.rsrc should be set
 * to B_BACKGROUND_APP.
 * 
 * @author Matthijs Hollemans <mahlzeit@bigfoot.com>
 * @version 2.0 (Friday, 30 April 1999)
 */
class HustlerApp : public BApplication 
{
public:
	// Constant Definitions ==================================================

	// Type Definitions ======================================================

	// Attribute Definitions =================================================

	// Member Function Definitions ===========================================

	/** Constructor. */
	HustlerApp(void);

	/** Automatically generated copy constructor. */
	// HustlerApp(const HustlerApp& old);

	/** Automatically generated destructor. */
	// virtual ~HustlerApp();

	/** Automatically generated assignment operator. */
	// HustlerApp& operator=(HustlerApp& right);

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

#endif // HUSTLER_HUSTLERAPP_H
