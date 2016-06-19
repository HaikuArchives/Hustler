#ifndef HUSTLER_CDDAPLAYER_H
#define HUSTLER_CDDAPLAYER_H

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
 * The CddaPlayer class represents a CD player device that is used by the
 * CDDA add-on.
 *
 * @see CddaAddOn
 */
class CddaPlayer : public HustlerPlayer
{
public:
	// Constant Definitions ==================================================

	// Type Definitions ======================================================
  
	// Attribute Definitions =================================================

	// Member Function Definitions ===========================================

	/**
	 * Constructor.
	 *
	 * @param deviceName The name of the CD player's device.
	 */
	CddaPlayer(const char* deviceName);

	/** Automatically generated copy constructor. */
	// CddaPlayer(const CddaPlayer& old);

	/** Destructor. */
	virtual ~CddaPlayer(void);

	/** Automatically generated assignment operator. */
	// CddaPlayer& operator=(CddaPlayer& right);

	/** Starts playing the first track. */
	virtual void Play(void);

	/** Continues playback after the player has been paused. */
	virtual void Resume(void);

	/** Pauses playback. */
	virtual void Pause(void);

	/** Stops playback. */
	virtual void Stop(void);
  
	/** Skips to the previous track. */
	virtual void Previous(void);
 
	/** Skips to the next track. */
	virtual void Next(void);

	/** Starts playing a specific track. */
	virtual void GoTo(int32 track);

	/** Does nothing. */
	virtual void SetVolume(float volume);
	
	/** Does nothing. */
	virtual float GetVolume(void);

	/** Does nothing. */
	virtual void SetPanning(float panning);
	
	/** Does nothing. */
	virtual float GetPanning(void);
	
	/** Ejects the CD. */
	virtual void Eject(void);

	/** Returns the player's state. */
	virtual State GetState(void);

	/** Returns the number of the track that is currently playing. */
	virtual int32 GetCurrentTrack(void);

	/** Returns the number of tracks in the playlist. */
	virtual int32 GetNumberOfTracks(void);

	/** Returns the player's playlist. */
	virtual BList* GetPlaylist(void);

	/** Determines whether it is possible to start the player. */
	virtual bool CanPlay(void);

	/** Determines whether it is possible to resume the player. */
	virtual bool CanResume(void);

	/** Determines whether it is possible to pause the player. */
	virtual bool CanPause(void);

	/** Determines whether it is possible to stop the player. */
	virtual bool CanStop(void);

	/** Determines whether it is possible to play the previous track. */
	virtual bool CanPrevious(void);

	/** Determines whether it is possible to play the next track. */
	virtual bool CanNext(void);

	/** Determines whether it is possible to go to another track. */
	virtual bool CanGoTo(void);

	/** Always returns false. */
	virtual bool CanVolume(void);

	/** Always returns false. */
	virtual bool CanPan(void);
	
	/** Determines whether it is possible to eject the player. */
	virtual bool CanEject(void);

protected:
	// Constant Definitions ==================================================

	// Type Definitions ======================================================

	// Attribute Definitions =================================================

	// Member Function Definitions ===========================================

private:
	// Constant Definitions ==================================================

	/** Flags for an entry in the table of contents. */
	enum
	{
		FLAG_AUDIO_PREEMPHASIS = 0x01,
		FLAG_DIGITAL_COPY_OK = 0x02,
		FLAG_DATA_TRACK = 0x04,
		FLAG_AUDIO_FOUR_CHAN = 0x08
	};

	// Type Definitions ======================================================

	/**
	 * A CDDA address. 
	 * 
	 * The ioctl calls to the CD driver use a positioning type called MSF, 
	 * which stands for minute, second, frame. A minute is 60 seconds, a
	 * second is 75 frames, and a frame is one 2352-byte sector, which
	 * contains 588 samples of 16-bit stereo audio. 
	 * 
	 * @see Be Newsletter Volume 1 Issue 66.
	 * @see The ANSI SCSI standard or ATAPI specs.
	 * @see Peter Urbanec's DumpTrack sample code.
	 */
	typedef struct Address
	{
		uint8 reserved;
		uint8 minute;
		uint8 second;
		uint8 frame;
	};

	/** An entry in the table of contents. */
	typedef struct Track
	{
		uint8   reserved1;
		uint8   flags;
		uint8   trackNumber;
		uint8   reserved2;
		Address address;
	};

	/** The table of contents for an entire CD. */
	typedef struct TableOfContents
	{
		uint16 length;      // Number of bytes in the table of contents.  
		uint8  firstTrack;  // The track number which will appear in track[0].
		uint8  lastTrack;   // The last valid entry in the table of contents.
		Track  track[100];
	};

	// Attribute Definitions =================================================

	/** Whether the object was created successfully. */
	bool initCheck;
	
	/** The handle of the CD player's device. */
	int deviceHandle;

	/** Pointer to the current CD's table of contents. */
	TableOfContents* tableOfContents;

	/** The object that stores our playlist. */
	BList* playlist;

	// Member Function Definitions ===========================================

	/**
	 * Reads the current CD's table of contents.
	 *
	 * Note that, although CD track numbers (usually) start at 1, the index
	 * of the first entry from the table of contents is always 0.
	 *
	 * @return True if the table of contents was read successfully, or false
	 *         if the table of contents could not be read.
	 */
	bool GetContents();

	/**
	 * Returns the flags of a certain entry in the CD's table of contents.
	 */
	int8 GetTrackFlags(int8 index);

	/**
	 * Determines whether a certain entry in the CD's table of contents is a 
	 * data track instead of an audio track.
	 */
	bool IsDataTrack(int8 index);
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

#endif // HUSTLER_CDDAPLAYER_H
