/*!
 * [NOTE] Ideas on improving Hustler.
 *
 * One simple improvement would be to put the code that accesses the add-ons
 * in its own thread. This would make the GUI a bit more responsive, because
 * the Deskbar no longer has to wait for the add-ons to complete their ioctls
 * and stuff.
 */

/*!
 * [NOTE] Feature requests.
 *
 * - Offer different color schemes for the deskbar view.
 */

// ***************************************************************************
// Implementation System Includes
// ***************************************************************************

#include <Alert.h>
#include <AppFileInfo.h>
#include <Directory.h>
#include <Entry.h>
#include <File.h>
#include <MenuItem.h>
#include <NodeMonitor.h>
#include <Path.h>
#include <PopUpMenu.h>
#include <String.h>

#include <private/interface/AboutWindow.h>

#include <stdio.h>        // sprintf()
#include <string.h>

// ***************************************************************************
// Implementation Project Includes
// ***************************************************************************

#include "HustlerApp.h"
#include "HustlerMain.h"
#include "HustlerView.h"
#include "Version.h"

// ***************************************************************************
// Implementation Constant Definitions
// ***************************************************************************

const rgb_color COLOR_BACKGROUND = {40, 80, 80};
const rgb_color COLOR_FOREGROUND = {100, 220, 220};

/** The directory where our add-ons are stored. */
const char* ADD_ON_DIR = "/boot/home/config/add-ons/Hustler/";

/** Our settings file. */
const char* SETTINGS_FILE = "/boot/home/config/settings/Hustler";

// ***************************************************************************
// Implementation Type Definitions
// ***************************************************************************

/** Stores the image_id and HustlerAddOn objects of a loaded add-on. */
struct LoadedAddOn
{
	HustlerAddOn* addon;
	image_id image;
};

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

HustlerView::HustlerView(void)
	: BView(BRect(0,0,1,1), 0, 0, 0)
{
	// Initialize attributes.
	isReplicated = false;
	
	BBG_WRITE(("[HustlerView::HustlerView] Constructed."));
}

// ===========================================================================
// Constructor
// ===========================================================================

HustlerView::HustlerView(BMessage* message)
	: BView(BRect(0,0,15,15),
			"Hustler",
			B_FOLLOW_NONE,
			B_WILL_DRAW | B_PULSE_NEEDED) 
{
	BBG_WRITE(("[HustlerView::HustlerView] Constructed."));

	// Initialize attributes.
	mustBlink = false;
	oldState = HustlerPlayer::STATE_NO_TRACKS;
	oldTrack = 0;
	oldPulseTime = real_time_clock_usecs();
	isReplicated = true;
	
	// Set our view's background color.
	SetViewColor(COLOR_BACKGROUND);

	// Create the dummy player.
	dummyPlayer = new DummyPlayer();
	player = dummyPlayer;
		
	// Load all the add-ons.	
	LoadAddOns();

	// Get the first player that is available.
	SelectFirstAvailablePlayer();
}

// ===========================================================================
// Destructor
// ===========================================================================

HustlerView::~HustlerView(void)
{
	// Clean up.
	if (isReplicated)
	{
		stop_watching(this);
		UnloadAddOns();
		delete dummyPlayer;
	}
		
	BBG_WRITE(("[HustlerView::~HustlerView] Destructed."));
}

// ===========================================================================
// Instantiate
// ===========================================================================

__declspec(dllexport) HustlerView* 
HustlerView::Instantiate(BMessage* archive)
{
	if (!validate_instantiation(archive, "HustlerView")) 
	{
		return 0;
	}
	return new HustlerView(archive);
}

// ===========================================================================
// Archive
// ===========================================================================

status_t 
HustlerView::Archive(BMessage* archive, bool deep) const
{
	// Tell the overridden BView to archive itself.
	BView::Archive(archive, deep);
  
	// Store our signature and the name of our class in the archive.
	archive->AddString("add_on", APP_SIGNATURE);
	archive->AddString("class", "HustlerView");

	return B_OK;
}  

// ===========================================================================
// AttachedToWindow
// ===========================================================================

void
HustlerView::AttachedToWindow(void)
{
	// Put a node monitor on the add-ons directory, so we will be notified
	// when the user installs or removes add-ons. (Because watch_node()
	// needs both a BHandler and the BLooper that the handler is attached
	// to, we cannot install the node monitor in our constructor.)
	BDirectory dir(ADD_ON_DIR); 
	if (dir.InitCheck() == B_OK)
	{ 
		node_ref ref; 
		dir.GetNodeRef(&ref); 
		watch_node(&ref, B_WATCH_DIRECTORY, this);
	}	
}

// ===========================================================================
// Draw
// ===========================================================================

void 
HustlerView::Draw(BRect updateRect)
{
	// Only draw the track number if we are not blinking.
	if (mustBlink == false) 
	{
		// Create the string.
		char string[5];
		switch (player->GetState()) 
		{
			case HustlerPlayer::STATE_NO_TRACKS: 
			{
				sprintf(string,"?");
				break;
			}
	
			case HustlerPlayer::STATE_PLAYING:
				// Fall through.
	
			case HustlerPlayer::STATE_PAUSED: 
			{
				sprintf(string, "%d", player->GetCurrentTrack());
				break;
			}
	
			case HustlerPlayer::STATE_STOPPED: 
			{
				sprintf(string,"-");
				break;
			}
		}
		
		// Center the string horizontally and vertically.
		font_height height;
		GetFontHeight(&height);
		BRect rect = Bounds();
		float width = StringWidth(string);
		float x = (rect.Width() - width)/2;
		float y = height.ascent
				+ (rect.Height() - (height.ascent + height.descent))/2;

		// Draw the string. 
		SetLowColor(COLOR_BACKGROUND);
		SetHighColor(COLOR_FOREGROUND);
		SetDrawingMode(B_OP_OVER);
		DrawString(string, BPoint(x,y));
	}
}

// ===========================================================================
// MouseDown
// ===========================================================================

void
HustlerView::MouseDown(BPoint point) 
{
	// Get the current player's state.
	HustlerPlayer::State state = player->GetState();

	// Create a pop-up menu object that is not in radio mode.
	BPopUpMenu* menu = new BPopUpMenu("HustlerView", false, false);
	BMenuItem* item; 

	// "Play" item. ----------------------------------------------------------

	item = new BMenuItem("Play", new BMessage(MSG_MENU_PLAY));
	menu->AddItem(item);

	if (!(player->CanPlay() || player->CanResume()))
	{
		item->SetEnabled(false);
	} 

	// "Pause" item. ---------------------------------------------------------

	item = new BMenuItem("Pause", new BMessage(MSG_MENU_PAUSE));
	menu->AddItem(item);
	
	if (!player->CanPause())
	{
		item->SetEnabled(false);
	} 
  
	// "Stop" item. ----------------------------------------------------------

	item = new BMenuItem("Stop", new BMessage(MSG_MENU_STOP));
	menu->AddItem(item);

	if (!player->CanStop())
	{
		item->SetEnabled(false);
	} 
  
	// Separator bar. --------------------------------------------------------

	menu->AddSeparatorItem();

	// "Previous" item. ------------------------------------------------------
  
	item = new BMenuItem("Previous", new BMessage(MSG_MENU_PREVIOUS));
	menu->AddItem(item);

	if (!player->CanPrevious())
	{
		item->SetEnabled(false);
	}
	
	// "Next" item. ----------------------------------------------------------
  
	item = new BMenuItem("Next", new BMessage(MSG_MENU_NEXT));
	menu->AddItem(item);

	if (!player->CanNext())
	{
		item->SetEnabled(false);
	}
	
	// "Go to" submenu. ------------------------------------------------------
  
	// Set to "radio mode", so only one item at a time can be selected.
	BMenu* gotoMenu = new BMenu("Go to");
	gotoMenu->SetRadioMode(true);
    menu->AddItem(gotoMenu);

	// Get the current player's list of tracks.
	BList* playList = player->GetPlaylist();

	if ((!player->CanGoTo()) || (playList == 0))
	{
		item = new BMenuItem("No tracks", 0);
		gotoMenu->AddItem(item);
		gotoMenu->SetEnabled(false);
	} 
	else 
	{
		int32 currentTrack = player->GetCurrentTrack();
		
		// Loop through the play list.
		for (int32 t=0; t<playList->CountItems(); ++t)
		{
			// Create the message.
			BMessage* message = new BMessage(MSG_MENU_GO_TO);
			message->AddInt32("Track", (t+1));

			// Create the menu item.
			item = new BMenuItem((char*) playList->ItemAt(t), message);
			gotoMenu->AddItem(item);

			// If this track is currently playing, then mark it.
			if (currentTrack == (t+1))
			{
				item->SetMarked(true);
			}
		}
		gotoMenu->SetEnabled(true);
	    gotoMenu->SetTargetForItems(this);
	}

	// Separator bar. --------------------------------------------------------

	menu->AddSeparatorItem();

	// "Volume" submenu. -----------------------------------------------------
  
	BMenu* volumeMenu = new BMenu("Volume");
	volumeMenu->SetRadioMode(true);
    menu->AddItem(volumeMenu);

	if (!player->CanVolume())
	{
		item = new BMenuItem("Cannot set volume", 0);
		volumeMenu->AddItem(item);
		volumeMenu->SetEnabled(false);
	} 
	else 
	{
		int32 volume = (int32) (player->GetVolume()*100);
		char string[5];

		// Loop through the possible volumes.
		for (int32 t=100; t>=0; t-=10)
		{
			// Create the message.
			BMessage* message = new BMessage(MSG_MENU_VOLUME);
			message->AddFloat("Volume", t/100.0);

			// Create the menu item.
			sprintf(string, "%d %%", t);
			item = new BMenuItem(string, message);
			volumeMenu->AddItem(item);

			// If this is the current volume, then select it.
			if (volume == t)
			{
				item->SetMarked(true);
			}
		}
		volumeMenu->SetEnabled(true);
	    volumeMenu->SetTargetForItems(this);
	}

	// "Panning" submenu. ----------------------------------------------------

	BMenu* panningMenu = new BMenu("Panning");
	panningMenu->SetRadioMode(true);
    menu->AddItem(panningMenu);

	if (!player->CanPan())
	{
		item = new BMenuItem("Cannot set panning", 0);
		panningMenu->AddItem(item);
		panningMenu->SetEnabled(false);
	} 
	else
	{
		int32 panning = (int32) (player->GetPanning()*100);
		char string[8];//6];

		// Loop through the possible panning settings.
		for (int32 t=100; t>=-100; t-=25)
		{
			// Create the message.
			BMessage* message = new BMessage(MSG_MENU_PANNING);
			message->AddFloat("Panning", t/100.0);

			// Create the menu item.
			sprintf(string, "%d %%", t);
			item = new BMenuItem(string, message);
			panningMenu->AddItem(item);

			// If this is the current panning, then select it.
			if (panning == t)
			{
				item->SetMarked(true);
			}
		}
		panningMenu->SetEnabled(true);
	    panningMenu->SetTargetForItems(this);
	}

	// Separator bar. --------------------------------------------------------

	menu->AddSeparatorItem();

	// "Eject" item. ---------------------------------------------------------

	item = new BMenuItem("Eject", new BMessage(MSG_MENU_EJECT));
	menu->AddItem(item);
	
	if (!player->CanEject())
	{
		item->SetEnabled(false);
	}

	// Separator bar. --------------------------------------------------------

	menu->AddSeparatorItem();

	// "Options" submenu. ----------------------------------------------------

	/*!
	 * [TODO] Add "Options" submenu.
	 *
	 *  Possible options are continuous play and random play.
	 */
  
	//BMenu* lpOptionsMenu = new BMenu("Options");
	//
	//lpItem = new BMenuItem("Continous play", 0);
	//lpOptionsMenu->AddItem(lpItem);
	//
	//lpItem = new BMenuItem("Random play", 0);
	//lpOptionsMenu->AddItem(lpItem);
	//
	//lpMenu->AddItem(lpOptionsMenu);
  
	// "Player" submenu. -----------------------------------------------------
  
	BMenu* playerMenu = new BMenu("Player");
	playerMenu->SetRadioMode(true);    
	menu->AddItem(playerMenu);

	// Ask all add-ons to give us the names of the players they can create.
	for (int32 index=0; index < loadedAddOns->CountItems(); ++index)
	{
		// Get the pointer to the HustlerAddOn object.
		HustlerAddOn* addon =
			((LoadedAddOn*) loadedAddOns->ItemAt(index))->addon;

		// Loop until we've got the names of all the players.
		int32 playerIndex=0;
		char* name;
		while (1)
		{
			// Get the name of the player.
			int32 result = addon->GetPlayerName(playerIndex, name);
			if (result == HustlerAddOn::NAME_NO_MORE_NAMES)
			{
				break;
			}
			
			// Create the message.
			BMessage* message = new BMessage(MSG_MENU_PLAYER);
			message->AddPointer("AddOn", addon);
			message->AddString("PlayerName", name);

			// Create the menu item.
			item = new BMenuItem(name, message);
			playerMenu->AddItem(item);

			// If the player is (temporarily) unavailable, we disable it.
			if (result == HustlerAddOn::NAME_NOT_AVAILABLE)
			{
				item->SetEnabled(false);
			}
   
			// If this is the active player, then mark it.
			if (player != dummyPlayer)
			{
				if (strcmp(playerName, name) == 0)
				{
					item->SetMarked(true);
				}
			}

			// Try the next player.
			++playerIndex;
		}
	}

	// No players found.
	if (playerMenu->CountItems() == 0)
	{
		item = new BMenuItem("No players", 0);
		playerMenu->AddItem(item);
		playerMenu->SetEnabled(false);
	}

	playerMenu->SetTargetForItems(this);

	// Separator bar. --------------------------------------------------------

	menu->AddSeparatorItem();

	// "About" item. ---------------------------------------------------------

	item = new BMenuItem("About" B_UTF8_ELLIPSIS, 
		new BMessage(B_ABOUT_REQUESTED));

	menu->AddItem(item);

	// "Quit" item. ----------------------------------------------------------

	/*!
	 * [NOTE] Quitting Hustler is not implemented.
	 *
	 * The reason for this is a Deskbar replicant is not allowed to quit
	 * itself (according to the R4.5 Be Book's chapter on the Deskbar).
	 */
   
	//item = new BMenuItem("Quit", new BMessage(B_QUIT_REQUESTED));
	//menu->AddItem(item);

	// -----------------------------------------------------------------------

	// The messages of the menu items should be sent to ourselves.
	menu->SetTargetForItems(this);

	// Pop up the menu and wait for the user to make a selection.
	ConvertToScreen(&point); 
	menu->Go(point, true, false, true); 

	// Delete the things we don't need anymore.
	delete menu;
}

// ===========================================================================
// MessageReceived
// ===========================================================================

void
HustlerView::MessageReceived(BMessage* message)
{
	// Figure out what kind of message was received.
	switch(message->what) 
	{
		// "Play" item selected. ---------------------------------------------
		case MSG_MENU_PLAY: 
		{
			if (player->GetState() == HustlerPlayer::STATE_PAUSED) 
			{
				player->Resume();
			}
			else 
			{
				player->Play();
			}
			mustBlink = false;
			break;
		}

		// "Pause" item selected. --------------------------------------------
		case MSG_MENU_PAUSE: 
		{
			player->Pause();
			break;
		}

		// "Stop" item selected. ---------------------------------------------
		case MSG_MENU_STOP: 
		{
			player->Stop();
			break;
		}

		// "Previous" item selected. -----------------------------------------
		case MSG_MENU_PREVIOUS: 
		{
			player->Previous();
			break;
		}

		// "Next" item selected. ---------------------------------------------
		case MSG_MENU_NEXT: 
		{
			player->Next();
			break;
		}

		// "Go to" item selected. --------------------------------------------
		case MSG_MENU_GO_TO: 
		{
			int32 newTrack;
			if (message->FindInt32("Track", &newTrack) == B_OK) 
			{
				player->GoTo(newTrack);
			}
			break;
		}

		// "Volume" item selected. -------------------------------------------
		case MSG_MENU_VOLUME: 
		{
			float newVolume;
			if (message->FindFloat("Volume", &newVolume) == B_OK) 
			{
				player->SetVolume(newVolume);
			}
			break;
		}

		// "Panning" item selected. ------------------------------------------
		case MSG_MENU_PANNING: 
		{
			float newPanning;
			if (message->FindFloat("Panning", &newPanning) == B_OK) 
			{
				player->SetPanning(newPanning);
			}
			break;
		}
		
		// "Eject" item selected. --------------------------------------------
		case MSG_MENU_EJECT: 
		{
			player->Eject();
			break;
		}

		// "Player" item selected. -------------------------------------------
		case MSG_MENU_PLAYER: 
		{
			HustlerAddOn* addon;
			if (message->FindPointer("AddOn", (void**) &addon) == B_OK)
			{
				const char* name = message->FindString("PlayerName");
				if (name != 0)
				{
					// Change players only if the user did not
					// select the current one.
					if (strcmp(name, playerName) != 0)
					{
						SelectPlayer(addon, name);
						
						// Save the name of the chosen player into the
						// settings file.
						BFile file(
							SETTINGS_FILE,
							B_CREATE_FILE | B_WRITE_ONLY);
							
						if (file.InitCheck() == B_OK)
						{
							if (file.Lock() == B_OK)
							{
								file.WriteAttr("PreferredPlayer",
									B_STRING_TYPE,
									0,
									playerName,
									strlen(playerName));
					      
								file.Sync();
								file.Unlock();
							}
						}
					}
				}
			}
			break;
		}
    
		// Show the about window. --------------------------------------------
		case B_ABOUT_REQUESTED: 
		{
			BString text;
			
			text.Append("Hustler -- Simple Deskbar Audio Player\n\n"
				"Version " VERSION_STRING "\n"
				"Released on " VERSION_DATE_STRING "\n\n"
				"Created by Matthijs Hollemans\n"
				"matthijs@weirdexperiments.com\n"
				"http://www.weirdexperiments.com\n\n"
				"Hustler is public domain. No warranties expressed"
				" or implied.");
		
			if (loadedAddOns->CountItems() == 0)
			{
				text.Append("\n\nNo add-ons loaded.");
			}
			else
			{
				for (int32 t=0; t < loadedAddOns->CountItems(); ++t)
				{
					text.Append("\n\n");
	
					// Get the pointer to the HustlerAddOn object.
					HustlerAddOn* addon =
						((LoadedAddOn*) loadedAddOns->ItemAt(t))->addon;
	
					text.Append(addon->GetInfo());
				}
			}
									
			BAboutWindow* about = new BAboutWindow(B_TRANSLATE("Hustler"), APP_SIGNATURE);
	        about->AddDescription(text.String());
            about->Show();

			break;
		}

		// Node monitor notifications. ---------------------------------------
		case B_NODE_MONITOR:
		{
			BBG_WRITE(("[HustlerView::MessageReceived] Node monitor"
			           " notification."));

			// If I didn't do this, the add-ons sometimes wouldn't be loaded.
			snooze(50000);
			
			// Each time we receive a node monitor notification, we get rid
			// of all the loaded add-ons, and re-read the add-ons directory.
			// It would probably be better to remove or load only those
			// add-ons that changed, but I think we can get away with it.
			// (After all, how often do users change add-ons?)
			UnloadAddOns();
			LoadAddOns();
			SelectFirstAvailablePlayer();
			break;
		}

		// Let the original BView handle the message. ------------------------
		default: 
		{
			BView::MessageReceived(message);
			break;
		}
	}
  
	// Most of the messages above require an immediate redraw of the view.
	Invalidate();
}

// ===========================================================================
// Pulse
// ===========================================================================

void 
HustlerView::Pulse(void)
{
	// Ignore the pulse if less than a second passed.
	bigtime_t newPulseTime = real_time_clock_usecs();
	if ((newPulseTime - oldPulseTime) < 1000000)
	{
		return;
	}
	
	oldPulseTime = newPulseTime;

	// Get the state of the current player.
	HustlerPlayer::State state = player->GetState();

	// Determine whether we need to blink the track number.
	if (state == HustlerPlayer::STATE_PAUSED) 
	{
		if (mustBlink == false) 
		{
			mustBlink = true;
		}
		else
		{
			mustBlink = false;
		}
		Invalidate();
	} 
	else
	{
		mustBlink = false;
	}

	// Determine whether we need to redraw ourselves.
	int32 track = player->GetCurrentTrack();
	if ((oldState != state) || (oldTrack != track)) 
	{
		Invalidate();
	}
	oldState = state;
	oldTrack = track;
}

// ***************************************************************************
// PROTECTED Member Function Definitions
// ***************************************************************************

// ***************************************************************************
// PRIVATE Member Function Definitions
// ***************************************************************************

// ===========================================================================
// LoadAddOns
// ===========================================================================

void
HustlerView::LoadAddOns(void)
{
	// Create the list of LoadedAddOn objects.
	loadedAddOns = new BList(5);

	// Get the directory where our add-ons are stored.
	BDirectory dir(ADD_ON_DIR);
	if (dir.InitCheck() == B_OK)
	{
		// Loop through the directory and get its entries.
		BEntry entry;
		dir.Rewind();
		while (dir.GetNextEntry(&entry, false) == B_OK)
		{
			BPath path;
			entry.GetPath(&path);

			// Load the add-on.
			image_id image = load_add_on(path.Path());
			if (image < 0)
			{
				BBG_WRITE(("[HustlerView::LoadAddOns] Could not load add-on"
				           " \"%s\".", path.Path()));
				continue;
			}
		
			// Try to find the InstantiateAddOn function.
			HustlerAddOn* (*InstantiateAddOn)(); 
		
			status_t result = get_image_symbol(image, 
				"InstantiateHustlerAddOn", 
				B_SYMBOL_TYPE_TEXT,
				(void**) &InstantiateAddOn);
				
			if (result != B_OK) 
			{
				BBG_WRITE(("[HustlerView::LoadAddOns] Could not find the"
				           " InstantiateAddOn function of add-on \"%s\".",
				           path.Path()));
				continue;
			}
		
			// Call the InstantiateAddOn function.
			HustlerAddOn* addon = InstantiateAddOn();
			if (addon == 0)
			{
				BBG_WRITE(("[HustlerView::LoadAddOns] Could not instantiate"
				           " add-on \"%s\".", path.Path()));
				continue;
			}
		
			// Fill a new LoadedAddOn structure and add it to the list.
			LoadedAddOn* loadedAddOn = new LoadedAddOn;
			loadedAddOn->image = image;
			loadedAddOn->addon = addon;
			loadedAddOns->AddItem(loadedAddOn);
		
			BBG_WRITE(("[HustlerView::LoadAddOns] Successfully loaded add-on"
			           " \"%s\".", path.Path()));
		}
	}
}

// ===========================================================================
// UnloadAddOns
// ===========================================================================

void
HustlerView::UnloadAddOns(void)
{
	// Delete the active player.
	DeleteActivePlayer();

	// Delete the list of loaded add-ons.
	int32 countItems = loadedAddOns->CountItems();
	for (int32 t=0; t<countItems; ++t)
	{
		// Unload the add-on and delete its HustlerAddOn object.
		LoadedAddOn* loadedAddOn = (LoadedAddOn*) loadedAddOns->ItemAt(0);
		delete loadedAddOn->addon;
	    unload_add_on(loadedAddOn->image);

		// And finally delete the LoadedAddOn structure.
		delete loadedAddOns->RemoveItem((int32) 0);
    }
	delete loadedAddOns;
}

// ===========================================================================
// SelectFirstAvailablePlayer
// ===========================================================================

void
HustlerView::SelectFirstAvailablePlayer(void)
{
	// We'll use the dummy player if we can find no available player.
	player = dummyPlayer;

	// Read the name of the preferred player from the settings file.
	char preferredName[B_PATH_NAME_LENGTH];
	preferredName[0] = '\000';
	
	BFile file(SETTINGS_FILE, B_READ_ONLY);
	if (file.InitCheck() == B_OK)
	{
		if (file.Lock() == B_OK)
		{
			file.ReadAttr("PreferredPlayer",
				B_STRING_TYPE,
				0,
				&preferredName,
				B_PATH_NAME_LENGTH);

			file.Unlock();
		}
	}

	// Make sure that strcmp() doesn't bomb out if no preferred name found.
	if (preferredName == 0)
	{
		preferredName[0] = '\000';
	}

	// Loop through the add-ons and their available players.
	HustlerAddOn* firstAddOn = 0;
	char* firstPlayer = 0;
	
	for (int32 index=0; index < loadedAddOns->CountItems(); ++index)
	{
		// Get the pointer to the HustlerAddOn object.
		HustlerAddOn* addon =
			((LoadedAddOn*) loadedAddOns->ItemAt(index))->addon;

		// Loop until this add-on returns the name of an 
		// available player, or until it has no more players.		
		int32 playerIndex=0;
		char* name;
		while (1)
		{
			int32 result = addon->GetPlayerName(playerIndex, name);
			if (result == HustlerAddOn::NAME_OK)
			{
				// Remember the name of the first player we find,
				// in case we can't find the preferred player.
				if (firstPlayer == 0)
				{
					firstAddOn = addon;
					firstPlayer = name;
				}

				// If we've found the preferred player, then we're done.				
				if (strcmp(name, preferredName) == 0)
				{
					SelectPlayer(addon, name);
					break;
				}
			}
			if (result == HustlerAddOn::NAME_NO_MORE_NAMES)
			{
				break;
			}
			++playerIndex;
		}
		
		// If we've just found a player, then we can exit the loop.
		if (player != dummyPlayer)
		{
			break;
 		}
	}

	// If we haven't found the preferred player, but we did find another
	// one, then we'll select that.
	if ((player == dummyPlayer) && (firstPlayer != 0))
	{
		SelectPlayer(firstAddOn, firstPlayer);
	}
}

// ===========================================================================
// SelectPlayer
// ===========================================================================

void
HustlerView::SelectPlayer(HustlerAddOn* addon, const char* name)
{
	// Delete the player that was previously active.
	DeleteActivePlayer();
	
	// Ask the add-on to give us a new HustlerPlayer object.
	player = addon->GetPlayer(name);

	// If the player could not be found, we'll use the dummy player.
	if (player == 0)
	{
		player = dummyPlayer;
	}
	else
	{
		// Store a copy of the player's name.
		playerName = strdup(name);
	}
}

// ===========================================================================
// DeleteActivePlayer
// ===========================================================================

void
HustlerView::DeleteActivePlayer(void)
{
	if (player != dummyPlayer)
	{
		delete player;
		free(playerName);

		// Make sure that player never becomes null.
		player = dummyPlayer;
	}
}

// ***************************************************************************
// Helper Function Definitions
// ***************************************************************************

// ***************************************************************************
// Global Function Definitions
// ***************************************************************************

// ***************************************************************************
// Implementation Function Definitions
// ***************************************************************************
