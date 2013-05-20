package org.minibae;
import java.nio.ByteBuffer;
import android.content.res.AssetManager;

public class Sound
{
	private int mReference;
    private ByteBuffer mFile;
    Mixer mMixer;

	private native int _newNativeSound(int mixerReference);
    private native int _loadSound(int soundReference, ByteBuffer fileData);
    private native int _loadSound(int soundReference, AssetManager assetManager, String file);

	Sound(Mixer mixer)
	{
        mMixer = mixer;
		mReference = _newNativeSound(mMixer.mReference);
		if (mReference == 0)
		{
            // good
		}
        else
        {
            // bad
        }
	}

    int load(String resourceName)
    {
        mFile = ByteBuffer.allocateDirect(10000);
        int status = _loadSound(mReference, mMixer.mAssetManager, resourceName);
        return status;
    }

    int load()
    {
        mFile = ByteBuffer.allocateDirect(10000);
        int status = _loadSound(mReference, mFile);
        return status;
    }
}
