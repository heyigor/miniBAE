package org.minibae;

public class Sound
{
	private int mReference;

	private native int _newNativeSound();
	
	Sound()
	{
		mReference = _newNativeSound();
		if (mReference != 0)
		{
		}
	}
	
	

}
