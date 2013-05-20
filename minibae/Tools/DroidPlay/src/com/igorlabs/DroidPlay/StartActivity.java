package com.igorlabs.DroidPlay;

import android.app.Activity;
import android.os.Bundle;
import org.minibae.Mixer;

public class StartActivity extends Activity
{

    /**
     * Called when the activity is first created.
     */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        int status = Mixer.create(44100, 0, 0, 4, 4, 4);
        if (status == 0)
            System.out.println("good");

        setContentView(R.layout.main);
    }
}
