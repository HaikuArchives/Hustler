// ***************************************************************************
// Implementation System Includes
// ***************************************************************************

#include <be/app/Message.h>
#include <be/app/Messenger.h>

// ***************************************************************************
// Implementation Project Includes
// ***************************************************************************

#include "SoundPlayAddOn.h"
#include "SoundPlayPlayer.h"

// ***************************************************************************
// Implementation Constant Definitions
// ***************************************************************************

/** The maximum amount of time to wait on replies from SoundPlay. */
static const bigtime_t TIMEOUT = 1000000;

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

SoundPlayPlayer::SoundPlayPlayer(team_id team)
	: HustlerPlayer()
{
	// Initialize attributes.
	messenger = new BMessenger(0, team);
	timedOut = false;
	
	BBG_WRITE(("[SoundPlayPlayer::SoundPlayPlayer] Constructed."));
}

// ===========================================================================
// Destructor
// ===========================================================================

SoundPlayPlayer::~SoundPlayPlayer(void)
{
	delete messenger;
	
	BBG_WRITE(("[SoundPlayPlayer::~SoundPlayPlayer] Destructed."));
}

// ===========================================================================
// Play
// ===========================================================================

void
SoundPlayPlayer::Play(void)
{
	BBG_ENTER("[SoundPlayPlayer::Play]");

	if (timedOut) return;

	BMessage message;
	BMessage reply;

	message.what = B_SET_PROPERTY;
	message.AddFloat("data", 0.0);
	message.AddSpecifier("pitch");
	message.AddSpecifier("track", (int32) 0);

	if (messenger->SendMessage(&message, &reply, TIMEOUT, TIMEOUT) != B_OK)
	{
		timedOut = true;
		return;
	}

	message.MakeEmpty();
	message.what = B_SET_PROPERTY;
	message.AddFloat("data", 0.0);
	message.AddSpecifier("position");
	message.AddSpecifier("track", (int32) 0);

	if (messenger->SendMessage(&message, &reply, TIMEOUT, TIMEOUT) != B_OK)
	{
		timedOut = true;
		return;
	}
	
	message.MakeEmpty();
	message.what = B_SET_PROPERTY;
	message.AddFloat("data", 1.0);
	message.AddSpecifier("pitch");
	message.AddSpecifier("track", (int32) 0);

	if (messenger->SendMessage(&message, &reply, TIMEOUT, TIMEOUT) != B_OK)
	{
		timedOut = true;
	}
}

// ===========================================================================
// Resume
// ===========================================================================

void 
SoundPlayPlayer::Resume(void)
{
	BBG_ENTER("[SoundPlayPlayer::Resume]");

	if (timedOut) return;

	BMessage message;
	message.what = B_SET_PROPERTY;
	message.AddFloat("data", 1.0);
	message.AddSpecifier("pitch");
	message.AddSpecifier("track", (int32) 0);

	if (messenger->SendMessage(&message, (BHandler*) 0, TIMEOUT) != B_OK)
	{
		timedOut = true;
	}
}

// ===========================================================================
// Pause
// ===========================================================================

void
SoundPlayPlayer::Pause(void)
{
	BBG_ENTER("[SoundPlayPlayer::Pause]");

	if (timedOut) return;

	BMessage message;
	message.what = B_SET_PROPERTY;
	message.AddFloat("data", 0.0);
	message.AddSpecifier("pitch");
	message.AddSpecifier("track", (int32) 0);

	if (messenger->SendMessage(&message, (BHandler*) 0, TIMEOUT) != B_OK)
	{
		timedOut = true;
	}
}

// ===========================================================================
// Stop
// ===========================================================================

void
SoundPlayPlayer::Stop(void)
{
	if (timedOut) return;

	BMessage message;
	BMessage reply;
	
	message.what = B_SET_PROPERTY;
	message.AddFloat("data", 0.0);
	message.AddSpecifier("pitch");
	message.AddSpecifier("track", (int32) 0);

	if (messenger->SendMessage(&message, &reply, TIMEOUT, TIMEOUT) != B_OK)
	{
		timedOut = true;
		return;
	}

	// This is required because the user may have turned on "smooth pitch"
	// changes, so setting the pitch to 0 might take a second or so. If we
	// reset the position before the pitch is 0, SoundPlay will still play
	// a few milliseconds, so position won't be 0 anymore, and GetState()
	// will think we have paused instead of stopped.
	snooze(1000000);

	message.MakeEmpty();
	message.what = B_SET_PROPERTY;
	message.AddFloat("data", 0.0);
	message.AddSpecifier("position");
	message.AddSpecifier("track", (int32) 0);

	if (messenger->SendMessage(&message, &reply, TIMEOUT, TIMEOUT) != B_OK)
	{
		timedOut = true;
	}
}

// ===========================================================================
// Previous
// ===========================================================================
 
void 
SoundPlayPlayer::Previous(void)
{
	BBG_ENTER("[SoundPlayPlayer::Previous]");
}
 
// ===========================================================================
// Next
// ===========================================================================

void
SoundPlayPlayer::Next(void)
{
	BBG_ENTER("[SoundPlayPlayer::Next]");
}

// ===========================================================================
// GoTo
// ===========================================================================

void 
SoundPlayPlayer::GoTo(int32 track)
{
	BBG_ENTER("[SoundPlayPlayer::GoTo]");
}

// ===========================================================================
// SetVolume
// ===========================================================================

void 
SoundPlayPlayer::SetVolume(float volume)
{
	BBG_ENTER("[SoundPlayPlayer::SetVolume]");

	if (timedOut) return;
	
	BMessage message;
	BMessage reply;
	
	message.what = B_SET_PROPERTY;
	message.AddFloat("data", volume*2.0);
	message.AddSpecifier("volume");
	message.AddSpecifier("track", (int32) 0);
	
	if (messenger->SendMessage(&message, &reply, TIMEOUT, TIMEOUT) != B_OK)
	{
		timedOut = true;
	}
}

// ===========================================================================
// GetVolume
// ===========================================================================

float 
SoundPlayPlayer::GetVolume(void)
{
	BBG_ENTER("[SoundPlayPlayer::GetVolume]");

	if (timedOut) return 0.0;

	BMessage message;
	BMessage reply;
		
	message.what = B_GET_PROPERTY;
	message.AddSpecifier("volume");
	message.AddSpecifier("track", (int32) 0);
	
	if (messenger->SendMessage(&message, &reply, TIMEOUT, TIMEOUT) != B_OK)
	{
		timedOut = true;
		return 0.0;
	}

	float volume;		
	if (reply.FindFloat("result", &volume) != B_OK)
	{
		return 0.0;
	}

	// SoundPlay's volume goes from 0.0 to 2.0.
	return volume/2.0;
}

// ===========================================================================
// SetPanning
// ===========================================================================

void 
SoundPlayPlayer::SetPanning(float panning)
{
	BBG_ENTER("[SoundPlayPlayer::SetPanning]");
}

// ===========================================================================
// GetPanning
// ===========================================================================

float  
SoundPlayPlayer::GetPanning(void)
{
	BBG_ENTER("[SoundPlayPlayer::GetPanning]");
	return 0.0;
}

// ===========================================================================
// Eject
// ===========================================================================

void
SoundPlayPlayer::Eject(void)
{
	BBG_ENTER("[SoundPlayPlayer::Eject]");
}

// ===========================================================================
// GetState
// ===========================================================================
  
SoundPlayPlayer::State
SoundPlayPlayer::GetState(void)
{
	if (timedOut) return STATE_NO_TRACKS;

	BMessage message;
	BMessage reply;

	if (GetNumberOfTracks() <= 0)
	{
		return STATE_NO_TRACKS;
	}
	
	message.what = B_GET_PROPERTY;
	message.AddSpecifier("pitch");
	message.AddSpecifier("track", (int32) 0);
	
	if (messenger->SendMessage(&message, &reply, TIMEOUT, TIMEOUT) != B_OK)
	{
		timedOut = true;
		return STATE_NO_TRACKS;
	}

	float pitch;		
	if (reply.FindFloat("result", &pitch) != B_OK)
	{
		return STATE_NO_TRACKS;
	}

	message.MakeEmpty();
	message.what = B_GET_PROPERTY;
	message.AddSpecifier("position");
	message.AddSpecifier("track", (int32) 0);

	if (messenger->SendMessage(&message, &reply, TIMEOUT, TIMEOUT) != B_OK)
	{
		timedOut = true;
		return STATE_NO_TRACKS;
	}

	float position;
	if (reply.FindFloat("result", &position) != B_OK)
	{
		return STATE_NO_TRACKS;
	}

	if (pitch == 0.0)
	{
		if (position == 0.0)
		{
			return STATE_STOPPED;
		}
		else
		{
			return STATE_PAUSED;
		}
	}
	else
	{
		return STATE_PLAYING;
	}
}

// ===========================================================================
// GetCurrentTrack
// ===========================================================================

int32
SoundPlayPlayer::GetCurrentTrack(void)
{
	if (timedOut) return -1;

	if (GetNumberOfTracks() > 0)
	{
		return 1;
	}
	return -1;
}

// ===========================================================================
// GetNumberOfTracks
// ===========================================================================

int32 
SoundPlayPlayer::GetNumberOfTracks(void)
{
	if (timedOut) return 0;

	BMessage message;
	BMessage reply;

	message.what = B_COUNT_PROPERTIES;
	message.AddSpecifier("track");
	
	if (messenger->SendMessage(&message, &reply, TIMEOUT, TIMEOUT) != B_OK)
	{
		timedOut = true;
		return 0;
	}

	int32 count;
	if (reply.FindInt32("result", &count) == B_OK)
	{
		return count;
	}
	return 0;
}

// ===========================================================================
// GetPlaylist
// ===========================================================================

BList*
SoundPlayPlayer::GetPlaylist(void)
{
	return 0;
}

// ===========================================================================
// CanPlay
// ===========================================================================

bool
SoundPlayPlayer::CanPlay(void)
{
	if (timedOut) return false;

	State state = GetState();
	if ((state == STATE_NO_TRACKS)
	||  (state == STATE_PLAYING))
	{
		return false;
	}
	return true;
}

// ===========================================================================
// CanResume
// ===========================================================================

bool
SoundPlayPlayer::CanResume(void)
{
	if (timedOut) return false;

	State state = GetState();
	if ((state == STATE_NO_TRACKS)
	||  (state == STATE_PLAYING)
	||  (state == STATE_STOPPED))
	{
		return false;
	}
	return true;
}

// ===========================================================================
// CanPause
// ===========================================================================

bool
SoundPlayPlayer::CanPause(void)
{
	if (timedOut) return false;

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
SoundPlayPlayer::CanStop(void)
{
	if (timedOut) return false;

	State state = GetState();
	if ((state == STATE_NO_TRACKS)
	||  (state == STATE_STOPPED))
	{
		return false;
	}
	return true;
}

// ===========================================================================
// CanPrevious
// ===========================================================================

bool
SoundPlayPlayer::CanPrevious(void)
{
	return false;
}

// ===========================================================================
// CanNext
// ===========================================================================

bool
SoundPlayPlayer::CanNext(void)
{
	return false;
}

// ===========================================================================
// CanGoTo
// ===========================================================================

bool
SoundPlayPlayer::CanGoTo(void)
{
	return false;
}

// ===========================================================================
// CanVolume
// ===========================================================================

bool
SoundPlayPlayer::CanVolume(void)
{
	if (timedOut) return false;

	return true;
}

// ===========================================================================
// CanPan
// ===========================================================================

bool
SoundPlayPlayer::CanPan(void)
{
	return false;
}

// ===========================================================================
// CanEject
// ===========================================================================

bool
SoundPlayPlayer::CanEject(void)
{
	return false;
}

// ***************************************************************************
// PROTECTED Member Function Definitions
// ***************************************************************************

// ***************************************************************************
// PRIVATE Member Function Definitions
// ***************************************************************************

// ***************************************************************************
// Helper Function Definitions
// ***************************************************************************

// ***************************************************************************
// Global Function Definitions
// ***************************************************************************

// ***************************************************************************
// Implementation Function Definitions
// ***************************************************************************
