#ifndef HUSTLER_SOUNDPLAYPLAYER_H
#define HUSTLER_SOUNDPLAYPLAYER_H

// ***************************************************************************
// System Includes
// ***************************************************************************

// ***************************************************************************
// Project Includes
// ***************************************************************************

// Standard type and constant definitions.
#include "../Hustler/StyleGuideDefs.h"

// Our base class.
#include "../Hustler/HustlerPlayer.h"

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
 * The SoundPlayPlayer class represents an instance of the SoundPlay
 * application that is used by the SoundPlay remote control add-on.
 *
 * @see SoundPlayAddOn
 */
class SoundPlayPlayer : public HustlerPlayer
{
public:
	// Constant Definitions ==================================================

	// Type Definitions ======================================================
  
	// Attribute Definitions =================================================

	// Member Function Definitions ===========================================

	/**
	 * Constructor.
	 *
	 * @param team The team_id of this player's SoundPlay instance.
	 */
	SoundPlayPlayer(team_id team);

	/** Automatically generated copy constructor. */
	// SoundPlayPlayer(const SoundPlayPlayer& old);

	/** Destructor. */
	virtual ~SoundPlayPlayer(void);

	/** Automatically generated assignment operator. */
	// SoundPlayPlayer& operator=(SoundPlayPlayer& right);

	/** Starts playing the first track. */
	virtual void Play(void);

	/** Continues playback after the player has been paused. */
	virtual void Resume(void);

	/** Pauses playback. */
	virtual void Pause(void);

	/** Stops playback. */
	virtual void Stop(void);
  
	/** Does nothing. */
	virtual void Previous(void);
 
	/** Does nothing. */
	virtual void Next(void);

	/** Does nothing. */
	virtual void GoTo(int32 track);

	/** Sets the volume of the player. */
	virtual void SetVolume(float volume);
	
	/** Returns the volume of the player. */
	virtual float GetVolume(void);

	/** Does nothing. */
	virtual void SetPanning(float panning);
	
	/** Does nothing. */
	virtual float GetPanning(void);
	
	/** Does nothing. */
	virtual void Eject(void);

	/** Returns the player's state. */
	virtual State GetState(void);

	/** Does nothing. */
	virtual int32 GetCurrentTrack(void);

	/** Does nothing. */
	virtual int32 GetNumberOfTracks(void);

	/** Does nothing. */
	virtual BList* GetPlaylist(void);

	/** Determines whether it is possible to start the player. */
	virtual bool CanPlay(void);

	/** Determines whether it is possible to resume the player. */
	virtual bool CanResume(void);

	/** Determines whether it is possible to pause the player. */
	virtual bool CanPause(void);

	/** Determines whether it is possible to stop the player. */
	virtual bool CanStop(void);

	/** Always returns false. */
	virtual bool CanPrevious(void);

	/** Always returns false. */
	virtual bool CanNext(void);

	/** Always returns false. */
	virtual bool CanGoTo(void);

	/** Determines whether it's possible to change the volume. */
	virtual bool CanVolume(void);

	/** Always returns false. */
	virtual bool CanPan(void);
	
	/** Always returns false. */
	virtual bool CanEject(void);

protected:
	// Constant Definitions ==================================================

	// Type Definitions ======================================================

	// Attribute Definitions =================================================

	// Member Function Definitions ===========================================

private:
	// Constant Definitions ==================================================

	// Type Definitions ======================================================

	// Attribute Definitions =================================================

	/** A messenger to the SoundPlay instance that we target. */
	BMessenger* messenger;

	/** Whether SoundPlay has timed out on us. */
	bool timedOut;

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

#endif // HUSTLER_SOUNDPLAYPLAYER_H
