#ifndef HUSTLER_DUMMYPLAYER_H
#define HUSTLER_DUMMYPLAYER_H

// ***************************************************************************
// System Includes
// ***************************************************************************

// ***************************************************************************
// Project Includes
// ***************************************************************************

// Standard type and constant definitions.
#include "StyleGuideDefs.h"

// Our base class.
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
 * The DummyPlayer class is used when no real players are available.
 */
class DummyPlayer : public HustlerPlayer
{
public:
	// Constant Definitions ==================================================

	// Type Definitions ======================================================
  
	// Attribute Definitions =================================================

	// Member Function Definitions ===========================================

	/** Constructor. */
	DummyPlayer(void);

	/** Automatically generated copy constructor. */
	// DummyPlayer(const DummyPlayer& old);

	/** Destructor. */
	virtual ~DummyPlayer(void);

	/** Automatically generated assignment operator. */
	// DummyPlayer& operator=(DummyPlayer& right);

	/** Does nothing. */
	virtual void Play(void);

	/** Does nothing. */
	virtual void Resume(void);

	/** Does nothing. */
	virtual void Pause(void);

	/** Does nothing. */
	virtual void Stop(void);
  
	/** Does nothing. */
	virtual void Previous(void);
 
	/** Does nothing. */
	virtual void Next(void);

	/** Does nothing. */
	virtual void GoTo(int32 track);

	/** Does nothing. */
	virtual void Eject(void);

	/** Does nothing. */
	virtual void SetVolume(float volume);
	
	/** Does nothing. */
	virtual float GetVolume(void);

	/** Does nothing. */
	virtual void SetPanning(float panning);
	
	/** Does nothing. */
	virtual float GetPanning(void);
	
	/** Always returns STATE_NO_TRACKS. */
	virtual State GetState(void);

	/** Always returns -1. */
	virtual int32 GetCurrentTrack(void);

	/** Always returns 0. */
	virtual int32 GetNumberOfTracks(void);

	/** Always returns null. */
	virtual BList* GetPlaylist(void);

	/** Always returns false. */
	virtual bool CanPlay(void);

	/** Always returns false. */
	virtual bool CanResume(void);

	/** Always returns false. */
	virtual bool CanPause(void);

	/** Always returns false. */
	virtual bool CanStop(void);

	/** Always returns false. */
	virtual bool CanPrevious(void);

	/** Always returns false. */
	virtual bool CanNext(void);

	/** Always returns false. */
	virtual bool CanGoTo(void);

	/** Always returns false. */
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

#endif // HUSTLER_DUMMYPLAYER_H
