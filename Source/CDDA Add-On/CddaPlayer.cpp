/*!
 * [NOTE] Ideas on improving the CD engine.
 *
 * Currently, you constantly have to ask the CD engine for whatever you want
 * to know. This keeps the code for the engine very simple, but the price is
 * a very limited engine, and quite a bit of unnecessary overhead.
 *
 * In retrospect, the approach taken by Be's CDButton sample code is much 
 * better: clients can simply subscribe to one or more notifiers, and are
 * automatically notified when something happens. So instead of reading the
 * table of contents for every call to IsDataTrack() and GetNumberOfTracks(),
 * we would simply read it once after we've received a "CD changed" event and
 * cache it for future reference by our other methods.
 */

// ***************************************************************************
// Implementation System Includes
// ***************************************************************************

#include <Drivers.h>
#include <scsi.h>

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

CddaPlayer::CddaPlayer(const char* deviceName)
	: HustlerPlayer()
{
	// Initialize attributes.
	initCheck = true;
	playlist = new BList(20);
	
	// Allocate memory for the table of contents.
	tableOfContents = (TableOfContents*) malloc(sizeof(scsi_toc));
	if (tableOfContents == 0)
	{
		initCheck = false;
		BBG_WRITE(("[CddaPlayer::CddaPlayer] Can't allocate memory for"
				   " table of contents."));
	}
	else
	{
		// Try to open the device.
		BBG_WRITE(("[CddaPlayer::CddaPlayer] Trying to open device %s",
				   deviceName));
		deviceHandle = open(deviceName, O_RDONLY);
		if (deviceHandle < 0)
		{
			initCheck = false;
			BBG_WRITE(("[CddaPlayer::CddaPlayer] Can't open device."));
		}
	}
	  
	BBG_WRITE(("[CddaPlayer::CddaPlayer] Constructed."));
}

// ===========================================================================
// Destructor
// ===========================================================================

CddaPlayer::~CddaPlayer(void)
{
	// Get rid of the playlist.
	int32 countItems = playlist->CountItems();
	for (int32 t=0; t<countItems; ++t) 
	{
		free(playlist->RemoveItem((int32) 0));
    }
	delete playlist;
	
	// Close the CD player device.
	if (deviceHandle >= 0)
	{
		BBG_WRITE(("[CddaPlayer::~CddaPlayer] Closing device."));
		close(deviceHandle);
	}

	BBG_WRITE(("[CddaPlayer::~CddaPlayer] Destructed."));
}

// ===========================================================================
// Play
// ===========================================================================

void
CddaPlayer::Play(void)
{
	BBG_ENTER("[CddaPlayer::Play]");

	GoTo(1);
}

// ===========================================================================
// Resume
// ===========================================================================

void 
CddaPlayer::Resume(void)
{
	BBG_ENTER("[CddaPlayer::Resume]");

	if (initCheck == true)
	{
		status_t result = ioctl(deviceHandle, B_SCSI_RESUME_AUDIO);
		if (result != B_OK)
		{
			BBG_WRITE(("[CddaPlayer::Resume] Error resuming."));
		}
	}	
}

// ===========================================================================
// Pause
// ===========================================================================

void
CddaPlayer::Pause(void)
{
	BBG_ENTER("[CddaPlayer::Pause]");

	if (initCheck == true)
	{
		status_t result = ioctl(deviceHandle, B_SCSI_PAUSE_AUDIO);
		if (result != B_OK)
		{
			BBG_WRITE(("[CddaPlayer::Pause] Error pausing."));
		}
	}
}

// ===========================================================================
// Stop
// ===========================================================================

void
CddaPlayer::Stop(void)
{
	BBG_ENTER("[CddaPlayer::Stop]");

	if (initCheck == true)
	{
		status_t result = ioctl(deviceHandle, B_SCSI_STOP_AUDIO);
		if (result != B_OK)
		{
			BBG_WRITE(("[CddaPlayer::Stop] Error stopping."));
		}
	}	
}

// ===========================================================================
// Previous
// ===========================================================================
 
void 
CddaPlayer::Previous(void)
{
	BBG_ENTER("[CddaPlayer::Previous]");

	GoTo(GetCurrentTrack() - 1);

	// Make sure we don't start playing if we were paused before.
	if (GetState() == STATE_PAUSED)
	{
		Pause();
	}
}
 
// ===========================================================================
// Next
// ===========================================================================

void
CddaPlayer::Next(void)
{
	BBG_ENTER("[CddaPlayer::Next]");

	GoTo(GetCurrentTrack() + 1);

	// Make sure we don't start playing if we were paused before.
	if (GetState() == STATE_PAUSED)
	{
		Pause();
	}
}

// ===========================================================================
// GoTo
// ===========================================================================

void 
CddaPlayer::GoTo(int32 track)
{
	BBG_ENTER("[CddaPlayer::GoTo]");

	if (initCheck == true)
	{
		if (GetState() == STATE_NO_TRACKS)
		{
			return;
		}

		scsi_play_track playTrack;

		if (track < 1)
		{
			playTrack.start_track = 1;
		}
		else if (track > 99)
		{
			playTrack.start_track = 99;
		}
		else
		{
			playTrack.start_track = track;
		}

		// If first track is data track, we skip it.
		GetContents();
		if ((playTrack.start_track == 1) && (IsDataTrack(0)))
		{
			// Don't play the CD if it doesn't contain any audio tracks.
			if (GetNumberOfTracks() <= 1)
			{
				BBG_WRITE(("[CddaPlayer::GoTo] CD does not contain audio"
				           " tracks."));
				return;
			}

			BBG_WRITE(("[CddaPlayer::Play] Skipping data track."));
			playTrack.start_track = 2;
		}

		playTrack.start_index = 1;
		playTrack.end_track   = 99;
		playTrack.end_index   = 1;

		if (ioctl(deviceHandle, B_SCSI_PLAY_TRACK, &playTrack) != B_OK)
		{
			BBG_WRITE(("[CddaPlayer::GoTo] Error playing."));
		}
	}	
}

// ===========================================================================
// SetVolume
// ===========================================================================

void 
CddaPlayer::SetVolume(float volume)
{
	BBG_ENTER("[CddaPlayer::SetVolume]");
}

// ===========================================================================
// GetVolume
// ===========================================================================

float 
CddaPlayer::GetVolume(void)
{
	BBG_ENTER("[CddaPlayer::GetVolume]");
	return 0.0;
}

// ===========================================================================
// SetPanning
// ===========================================================================

void 
CddaPlayer::SetPanning(float panning)
{
	BBG_ENTER("[CddaPlayer::SetPanning]");
}

// ===========================================================================
// GetPanning
// ===========================================================================

float  
CddaPlayer::GetPanning(void)
{
	BBG_ENTER("[CddaPlayer::GetPanning]");
	return 0.0;
}

// ===========================================================================
// Eject
// ===========================================================================

void
CddaPlayer::Eject(void)
{
	BBG_ENTER("[CddaPlayer::Eject]");
	
	if (initCheck == true)
	{
		if (GetState() == STATE_NO_TRACKS)
		{
			// Do nothing. (Media loading is not yet available.)
		}
		else
		{
			status_t result = ioctl(deviceHandle, B_SCSI_EJECT);
			if (result != B_OK)
			{
				BBG_WRITE(("[CddaPlayer::Eject] Error ejecting."));
			}
		}
	}		
}

// ===========================================================================
// GetState
// ===========================================================================
  
CddaPlayer::State
CddaPlayer::GetState(void)
{
	if (initCheck == true)
	{
		status_t mediaStatus = B_DEV_NO_MEDIA;

		ioctl(deviceHandle,
			B_GET_MEDIA_STATUS, 
			&mediaStatus,
			sizeof(mediaStatus));

		if (mediaStatus != B_OK)
		{
			return STATE_NO_TRACKS;
		}

		scsi_position position;

		status_t result = ioctl(deviceHandle,
			B_SCSI_GET_POSITION,
			&position);

		if (result != B_OK)
		{
			return STATE_NO_TRACKS;
		}
		else if ((!position.position[1]) 
			 ||  (position.position[1] >= 0x13) 
			 ||  ((position.position[1] == 0x12) && (!position.position[6])))
		{
			return STATE_STOPPED;
		}
		else if (position.position[1] == 0x11)
		{
			return STATE_PLAYING;
		}
		else
		{
			return STATE_PAUSED;
		}
	}
	return STATE_NO_TRACKS;
}

// ===========================================================================
// GetCurrentTrack
// ===========================================================================

int32
CddaPlayer::GetCurrentTrack(void)
{
	if (initCheck == true)
	{
		status_t mediaStatus = B_DEV_NO_MEDIA;
		
		ioctl(deviceHandle,
			B_GET_MEDIA_STATUS, 
			&mediaStatus, 
			sizeof(mediaStatus));
          
		if (mediaStatus != B_OK)
		{
			return -1;
		}

		scsi_position position;
		
		status_t result = ioctl(deviceHandle,
			B_SCSI_GET_POSITION, 
			&position);

		if (result != B_OK)
		{
			return -1;
		}
		
		if (!position.position[1] 
		||  position.position[1] >= 0x13
		||  (position.position[1] == 0x12 && !position.position[6]))
		{
			return 0;
		}
		else
		{
			return position.position[6];
		}
	}
	return -1;
}

// ===========================================================================
// GetNumberOfTracks
// ===========================================================================

int32 
CddaPlayer::GetNumberOfTracks(void)
{
	GetContents();
	return tableOfContents->lastTrack;
}

// ===========================================================================
// GetPlaylist
// ===========================================================================

BList*
CddaPlayer::GetPlaylist(void)
{
	// Get rid of the old playlist.
	int32 countItems = playlist->CountItems();
	for (int32 t=0; t<countItems; ++t) 
	{
		free(playlist->RemoveItem((int32) 0));
    }

	// Loop through the CD's table of contents.
	if (GetContents())
	{
		for (int8 t=0; t<tableOfContents->lastTrack; ++t)
		{
			// Create the string for the track name. Note that we need to look up
			// the actual track number for this entry because the number of the
			// track does not necessarily correspond with its index in the table
			// of contents.
			char* trackName = (char*) malloc(5*sizeof(char));
			sprintf(trackName, "%d", tableOfContents->track[t].trackNumber);
	
			// Add the track name to the playlist.
			playlist->AddItem(trackName);
		}
	}
	
	// And return the playlist.
	return playlist;
}

// ===========================================================================
// CanPlay
// ===========================================================================

bool
CddaPlayer::CanPlay(void)
{
	State state = GetState();
	if ((state == STATE_NO_TRACKS)
	||  (state == STATE_PLAYING)
	||  ((GetNumberOfTracks() == 1) && (IsDataTrack(0) == true)))
	{
		return false;
	}
	return true;
}

// ===========================================================================
// CanResume
// ===========================================================================

bool
CddaPlayer::CanResume(void)
{
	State state = GetState();
	if ((state == STATE_NO_TRACKS)
	||  (state == STATE_PLAYING)
	||  (state == STATE_STOPPED)
	||  ((GetNumberOfTracks() == 1) && (IsDataTrack(0) == true)))
	{
		return false;
	}
	return true;
}

// ===========================================================================
// CanPause
// ===========================================================================

bool
CddaPlayer::CanPause(void)
{
	State state = GetState();
	if ((state == STATE_NO_TRACKS)
	||  (state == STATE_PAUSED)
	||  (state == STATE_STOPPED))
	{
		return false;
	}
	return true;
}

// ===========================================================================
// CanStop
// ===========================================================================

bool
CddaPlayer::CanStop(void)
{
	/*!
	 * [BUG] Stop is disabled because the CD driver doesn't stop correctly.
	 *
	 * When the CD engine sends a B_SCSI_STOP_AUDIO command to a CD player
	 * device, it goes into a sort-of not-working pause mode instead of
	 * stopping the CD. I believe that this is a problem with the CD driver,
	 * because Be's CDPlayer application also does this.
	 *
	 * This bug also affects switching between CD players: I cannot stop the
	 * old device, so both keep playing.
	 * 
	 * @date Tuesday, 16 March 1999
	 */

	return false;
}

// ===========================================================================
// CanPrevious
// ===========================================================================

bool
CddaPlayer::CanPrevious(void)
{
	State state = GetState();
	if ((state == STATE_NO_TRACKS)
	||  (GetCurrentTrack() < 2))
	{
		return false;
	}
	return true;
}

// ===========================================================================
// CanNext
// ===========================================================================

bool
CddaPlayer::CanNext(void)
{
	State state = GetState();
	if ((state == STATE_NO_TRACKS)
	||  (GetCurrentTrack() >= GetNumberOfTracks()))
	{
		return false;
	}
	return true;
}

// ===========================================================================
// CanGoTo
// ===========================================================================

bool
CddaPlayer::CanGoTo(void)
{
	if (GetState() == STATE_NO_TRACKS)
	{
		return false;
	}
	return true;
}

// ===========================================================================
// CanVolume
// ===========================================================================

bool
CddaPlayer::CanVolume(void)
{
	return false;
}

// ===========================================================================
// CanPan
// ===========================================================================

bool
CddaPlayer::CanPan(void)
{
	return false;
}

// ===========================================================================
// CanEject
// ===========================================================================

bool
CddaPlayer::CanEject(void)
{
	if (GetState() == STATE_NO_TRACKS)
	{
		return false;
	}
	return true;
}

// ***************************************************************************
// PROTECTED Member Function Definitions
// ***************************************************************************

// ***************************************************************************
// PRIVATE Member Function Definitions
// ***************************************************************************

// ===========================================================================
// GetContents
// ===========================================================================

bool
CddaPlayer::GetContents()
{
	if (initCheck == true)
	{
		if (ioctl(deviceHandle, B_SCSI_GET_TOC, tableOfContents) != 0)
		{
			BBG_WRITE(("[CddaPlayer::GetContents] Can't read table of"
			           " contents."));
			return false;
		}
		return true;
	}
	return false;
}

// ===========================================================================
// GetTrackFlags
// ===========================================================================

int8
CddaPlayer::GetTrackFlags(int8 index)
{
	GetContents();
	return tableOfContents->track[index].flags;
}

// ===========================================================================
// IsDataTrack
// ===========================================================================

bool
CddaPlayer::IsDataTrack(int8 index)
{
	GetContents();
	if ((GetTrackFlags(index) & FLAG_DATA_TRACK) != 0)
	{
		return true;
	} 
	return false;
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
