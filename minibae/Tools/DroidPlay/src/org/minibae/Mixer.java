package org.minibae;
import org.minibae.Sound;
import org.minibae.Song;

public class Mixer
{
	static private int mReference;

	static
	{
		System.loadLibrary("miniBAE");
	}

	private static native int _newMixer();
	private static native void _deleteMixer(int reference);
	private static native int _openMixer(int sampleRate, int terpMode, int modifiers, int maxSongVoices, int maxSoundVoices, int mixLevel);
	
	// keep static constructor private.
	private Mixer()
	{
	}
	
	public static int create(int sampleRate, int terpMode, int modifiers, int maxSongVoices, int maxSoundVoices, int mixLevel)
	{
		int status = 0;
		mReference = _newMixer();
		if (mReference != 0)
		{
			status = _openMixer(sampleRate, terpMode, modifiers, maxSongVoices, maxSoundVoices, mixLevel);
		}
		return status;
    }

	public static void delete()
	{
        if (mReference != 0)
		{
			_deleteMixer(mReference);
			mReference = 0;
		}
	}

	public static Sound create()
	{
		Sound snd = null;
        if (mReference != 0)
		{
			snd = new Sound();
		}
		return snd;
	}	
}
