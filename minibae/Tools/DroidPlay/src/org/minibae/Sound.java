package org.minibae;
import java.nio.ByteBuffer;

public class Sound
{
	private int mReference;
    private ByteBuffer mFile;

	private native int _newNativeSound();
    private native int _loadSound(ByteBuffer fileData);

	Sound()
	{
		mReference = _newNativeSound();
		if (mReference != 0)
		{
		}
	}

    int load(String resource)
    {
        mFile = ByteBuffer.allocateDirect(10000);
        int status = _loadSound(mFile);
        return status;
    }
}
