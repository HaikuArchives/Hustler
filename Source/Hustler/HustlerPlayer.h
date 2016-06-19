#ifndef HUSTLER_HUSTLERPLAYER_H
#define HUSTLER_HUSTLERPLAYER_H

// ***************************************************************************
// System Includes
// ***************************************************************************

#include <List.h>

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

// ***************************************************************************
// Type Definitions
// ***************************************************************************

// ***************************************************************************
// Class Definition
// ***************************************************************************

/**
 * The HustlerPlayer class defines the common behaviors of all players that
 * can be created by Hustler add-ons that conform to version 1 of the Hustler
 * add-on API.
 *
 * @see HustlerAddOn
 */
class HustlerPlayer
{
public:
	// Constant Definitions ==================================================

	// Type Definitions ======================================================

	/** The possible states of the player. */
	enum State 
	{
		STATE_NO_TRACKS = 0,
		STATE_PLAYING,
		STATE_PAUSED,
		STATE_STOPPED
	};
  
	// Attribute Definitions =================================================

	// Member Function Definitions ===========================================

	/** Constructor. */
	HustlerPlayer(void)
	{
		// Do nothing.
	}

	/** Automatically generated copy constructor. */
	// HustlerPlayer(const HustlerPlayer& old);

	/** Destructor. */
	virtual ~HustlerPlayer(void)
	{
		// Do nothing.
	}

	/** Automatically generated assignment operator. */
	// HustlerPlayer& operator=(HustlerPlayer& right);

	/** Starts playing the first track. */
	virtual void Play(void) = 0;

	/** Continues playback after the player has been paused. */
	virtual void Resume(void) = 0;

	/** Pauses playback. */
	virtual void Pause(void) = 0;

	/** Stops playback. */
	virtual void Stop(void) = 0;
  
	/** Skips to the previous track. */
	virtual void Previous(void) = 0;
 
	/** Skips to the next track. */
	virtual void Next(void) = 0;

	/** 
	 * Starts playing a specific track.
	 *
	 * @param track The number of the track to play. Valid track numbers
	 *        start at 1.
	 */
	virtual void GoTo(int32 track) = 0;

	/**
	 * Sets the volume of the player.
	 * 
	 * @param volume The new volume, which ranges from 0.0 (silent) to 1.0
	 *        (maximum loudness).
	 */
	virtual void SetVolume(float volume) = 0;
	
	/**
	 * Returns the volume of the player.
	 *
	 * @return The volume, which ranges from 0.0 (silent) to 1.0
	 *         (maximum loudness).
	 */
	virtual float GetVolume(void) = 0;

	/**
	 * Sets the panning of the player.
	 * 
	 * @param panning The new panning, which ranges from -1.0 (totally left)
	 *        to 1.0 (totally right).
	 */
	virtual void SetPanning(float panning) = 0;
	
	/**
	 * Returns the panning of the player.
	 *
	 * @return The panning, which ranges from -1.0 (totally left) to
	 *         1.0 (totally right).
	 */
	virtual float GetPanning(void) = 0;

	/**
	 * What this method does is totally up to the add-on. The CDDA add-on
	 * ejects the CD, for example. Other add-ons could show a file dialog
	 * that allows users to choose the files to play.
	 */
	virtual void Eject(void) = 0;

	/**
	 * Returns the player's state.
	 *
	 * @see State
	 */
	virtual State GetState(void) = 0;
  
	/** 
	 * Returns the number of the track that is currently playing or
	 * -1 if no track is playing. Valid track numbers start at 1.
	 */
	virtual int32 GetCurrentTrack(void) = 0;

	/**
	 * Returns the number of tracks in the playlist.
	 */
	virtual int32 GetNumberOfTracks(void) = 0;

	/**
	 * Returns the player's playlist.
	 *
	 * @return The pointer to the BList object that stores pointers to the
	 *         names of the tracks, or null when no playlist is available.
	 */
	virtual BList* GetPlaylist(void) = 0;

	/** Determines whether it is possible to call the Play() method. */
	virtual bool CanPlay(void) = 0;

	/** Determines whether it is possible to call the Resume() method. */
	virtual bool CanResume(void) = 0;

	/** Determines whether it is possible to call the Pause() method. */
	virtual bool CanPause(void) = 0;

	/** Determines whether it is possible to call the Stop() method. */
	virtual bool CanStop(void) = 0;

	/** Determines whether it is possible to call the Previous() method. */
	virtual bool CanPrevious(void) = 0;

	/** Determines whether it is possible to call the Next() method. */
	virtual bool CanNext(void) = 0;

	/** Determines whether it is possible to call the GoTo() method. */
	virtual bool CanGoTo(void) = 0;

	/**
	 * Determines whether it is possible to call the SetVolume() and
	 * GetVolume() methods.
	 */
	virtual bool CanVolume(void) = 0;

	/**
	 * Determines whether it is possible to call the SetPanning() and
	 * GetPanning() methods.
	 */
	virtual bool CanPan(void) = 0;

	/** Determines whether it is possible to call the Eject() method. */
	virtual bool CanEject(void) = 0;

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

#endif // HUSTLER_HUSTLERPLAYER_H
