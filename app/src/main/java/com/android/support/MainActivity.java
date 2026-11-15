package com.android.support;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.provider.Settings;
import android.widget.Toast;

public class MainActivity extends Activity
    {
        public String GameActivity = "com.unity3d.player.UnityPlayerActivity";
        public boolean hasLaunched = false;
        static {
                // When you change the lib name, change also on Android.mk file
                // Both must have same name
                System.loadLibrary ( "HarshitController" );

            }

        //To call onCreate, please refer to README.md
        @Override
        protected void onCreate ( Bundle savedInstanceState )
            {
                super.onCreate ( savedInstanceState );
                if ( !hasLaunched )
                    {
                        try
                            {
                                //Start service
                                //MainActivity.this.startActivity(new Intent(MainActivity.this, Class.forName(MainActivity.this.GameActivity)));
                                MainActivity.this.startActivity ( new Intent ( MainActivity.this, Class.forName ( MainActivity.this.GameActivity ) ) );
                                hasLaunched = true;
                            }
                        catch (ClassNotFoundException e)
                            {
                                //Uncomment this if you are following METHOD 2 of CHANGING FILES
                                //Toast.makeText(MainActivity.this, "Error. Game's main activity does not exist", Toast.LENGTH_LONG).show();
                                e.printStackTrace ( );
                                return;
                            }
                    }
            }


    }
