// ***************************************************************************
// Implementation System Includes
// ***************************************************************************

#include <Directory.h>
#include <Entry.h>
#include <Path.h>
#include <scsi.h>
#include <string.h>

// ***************************************************************************
// Implementation Project Includes
// ***************************************************************************

#include "CddaAddOn.h"
#include "CddaPlayer.h"

// ***************************************************************************
// Implementation Constant Definitions
// ***************************************************************************

// ***************************************************************************
// Implementation Type Definitions
// ***************************************************************************

// ***************************************************************************
// Implementation Variable Definitions
// ***************************************************************************

// ***************************************************************************
// Implementation Function Declarations
// ***************************************************************************

// ***************************************************************************
// Global Variable Definitions
// ***************************************************************************

// ***************************************************************************
// PUBLIC Member Function Definitions
// ***************************************************************************

// ===========================================================================
// Constructor
// ===========================================================================

CddaAddOn::CddaAddOn(void)
	: HustlerAddOn()
{
	// Build the list of installed CD player devices.
	deviceNames = new BList(5);
	SearchForCdPlayer("/dev/disk");
		
	BBG_WRITE(("[CddaAddOn::CddaAddOn] Constructed."));
}

// ===========================================================================
// Destructor
// ===========================================================================

CddaAddOn::~CddaAddOn(void)
{
	// Free the names of the CD player devices.
	int32 countItems = deviceNames->CountItems(); 
	for (int32 t=0; t<countItems; ++t) 
	{
		delete deviceNames->RemoveItem((int32) 0);
    }
	delete deviceNames;

	BBG_WRITE(("[CddaAddOn::~CddaAddOn] Destructed."));
}

// ===========================================================================
// GetInfo
// ===========================================================================

char*
CddaAddOn::GetInfo(void)
{
	return "Compact Disc Digital Audio (CDDA) add-on\n"
		"Written by Matthijs Hollemans. This add-on contains software"
		" components developed by Peter Urbanec and Be, Inc.";
}

// ===========================================================================
// GetPlayerName
// ===========================================================================

int32
CddaAddOn::GetPlayerName(int32 index, char* &name)
{
	// Return the name of the requested player.
	if (index < deviceNames->CountItems())
	{
		name = (char*) deviceNames->ItemAt(index);
		return NAME_OK;
	}

	// No more players.
	return NAME_NO_MORE_NAMES;
}

// ===========================================================================
// GetPlayer
// ===========================================================================
	
HustlerPlayer* 
CddaAddOn::GetPlayer(const char* name)
{
	for (int32 t=0; t<deviceNames->CountItems(); ++t)
	{
		if (strcmp(name, (char*) deviceNames->ItemAt(t)) == 0)
		{
			return new CddaPlayer(name);
		}
	}
	return 0;
}

// ***************************************************************************
// PROTECTED Member Function Definitions
// ***************************************************************************

// ***************************************************************************
// PRIVATE Member Function Definitions
// ***************************************************************************

// ===========================================================================
// SearchForCdPlayer
// ===========================================================================

void 
CddaAddOn::SearchForCdPlayer(const char* dirName) 
{
	// Turn the directory name into a BDirectory object and rewind it.
	BDirectory directory;
	if (directory.SetTo(dirName) != B_OK) 
	{
		return;
	}
	directory.Rewind();

	// Loop through the entries from the directory.
	BEntry entry;
	while (directory.GetNextEntry(&entry) >= 0) 
	{
		// Get the path of the entry.
		BPath path;
		if (entry.GetPath(&path) != B_OK) 
		{
			continue;
		} 
		const char* name = path.Path();

		// Get an entry_ref for the entry.
		entry_ref ref;
		if (entry.GetRef(&ref) != B_OK) 
		{
			continue;
		}

		// Is the entry a directory?
		if (entry.IsDirectory()) 
		{
			// Ignore floppy devices. It's worth to explicitly check for the
			// floppy device and ignore it, because opening it to get its
			// geometry would just make a lot of noise, but not any sense.
			if (strcmp(ref.name, "floppy") == 0) 
			{
				continue;
			}
			SearchForCdPlayer(name);
    	} 
		else 
		{
			// Ignore partitions.
			if (strcmp(ref.name, "raw") != 0) 
			{
				continue;
			}

			// Try to open the device.
			int device = open(name, O_RDONLY);
			if (device < 0) 
			{
				continue;
			}

			// Figure out whether the device is a CD-ROM drive.
			device_geometry geometry;
			if (ioctl(device, 
					B_GET_GEOMETRY, 
					&geometry, 
					sizeof(geometry)) < 0) 
			{
				close(device);
				continue;
			}

			// Hooray, we've found a CD-ROM drive.
			if (geometry.device_type == B_CD) 
			{
				// Store the device's name in the list and close the device. 
				deviceNames->AddItem(strdup(name));
				close(device);        
			}
		}
	}
}

// ***************************************************************************
// Helper Function Definitions
// ***************************************************************************

// ***************************************************************************
// Global Function Definitions
// ***************************************************************************

// ===========================================================================
// InstantiateHustlerAddOn
// ===========================================================================

HustlerAddOn* 
InstantiateHustlerAddOn()
{
	BBG_ENTER("[InstantiateHustlerAddOn]");
	return new CddaAddOn();
}

// ***************************************************************************
// Implementation Function Definitions
// ***************************************************************************
