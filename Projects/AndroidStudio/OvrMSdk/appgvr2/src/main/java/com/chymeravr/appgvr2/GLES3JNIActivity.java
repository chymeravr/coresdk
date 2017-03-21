
package com.chymeravr.appgvr2;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.location.Location;
import android.media.AudioManager;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.WindowManager;
import android.view.animation.AlphaAnimation;
import android.view.animation.Animation;
import android.widget.ImageView;

import com.chymeravr.adclient.AdListener;
import com.chymeravr.adclient.AdRequest;
import com.chymeravr.adclient.ChymeraVrSdk;
import com.chymeravr.adclient.Image360Ad;

import java.text.SimpleDateFormat;
import java.util.Date;

public class GLES3JNIActivity extends Activity implements SurfaceHolder.Callback
{
	// Load the gles3jni library right away to make sure JNI_OnLoad() gets called as the very first thing.
	static
	{
		System.loadLibrary( "appgvr2" );
	}

	private static final String TAG = "ActivityGearVR";

	private SurfaceView mView;
	private SurfaceHolder mSurfaceHolder;
	private long mNativeHandle;

    Animation anim;
    ImageView fadeImage;

    Image360Ad image360TestAd;
    public boolean isShowing = false;

	private void adjustVolume(int direction)
	{
		AudioManager audio = (AudioManager) getSystemService(Context.AUDIO_SERVICE);
		audio.adjustStreamVolume(AudioManager.STREAM_MUSIC, direction, 0);
	}

	@Override protected void onCreate( Bundle icicle )
	{
		Log.v( TAG, "----------------------------------------------------------------" );
		Log.v( TAG, "GLES3JNIActivity::onCreate()" );
		super.onCreate( icicle );
        setContentView(R.layout.gl_context);


        ChymeraVrSdk.initialize(this, "89ec2db1-284e-44af-968e-0618103f89c8");

        this.image360TestAd = new Image360Ad(this, new AdListener() {
            @Override
            public void onAdLoaded() {
            }

            @Override
            public void onAdLoadFailed(AdRequest.Error error, String errorReason) {
            }

            @Override
            public void onAdOpened() {
            }

            @Override
            public void onAdClosed() {
            }
        });
        this.image360TestAd.setPlacementId("3efc7f15-33a6-4480-bb71-3bd74aca4f1f");

        final String PROVIDER = "flp";
        final double lat = 37.377166;
        final double lng = -122.086966;
        final float accuracy = 3.0f;

         /* From input arguments, create a single Location with provider set to
         * "flp"*/

        // Create a new Location
        Location newLocation = new Location(PROVIDER);
        newLocation.setLatitude(lat);
        newLocation.setLongitude(lng);
        newLocation.setAccuracy(accuracy);

        Location location = newLocation;

        String date = 1989 + "/" + 7 + "/" + 15;
        SimpleDateFormat formatter = new SimpleDateFormat("yyyy/MM/dd");
        Date dob = new Date();
        try {
            dob = formatter.parse(date);
        } catch (Exception e) {
            e.printStackTrace();
        }
        final AdRequest adRequest = AdRequest.builder()
                .location(location)
                .birthday(dob)
                .gender(AdRequest.Gender.MALE).build();


        this.image360TestAd.loadAd(adRequest);


		mView = (SurfaceView) findViewById(R.id.mView);//new SurfaceView( this );

        fadeImage = (ImageView)findViewById(R.id.fadeview);

        //setContentView( mView );
		mView.getHolder().addCallback( this );

		// Force the screen to stay on, rather than letting it dim and shut off
		// while the user is watching a movie.
		getWindow().addFlags( WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON );

		// Force screen brightness to stay at maximum
		WindowManager.LayoutParams params = getWindow().getAttributes();
		params.screenBrightness = 1.0f;
		getWindow().setAttributes( params );

		mNativeHandle = GLES3JNILib.onCreate( this );
	}

	@Override protected void onStart()
	{
		Log.v( TAG, "GLES3JNIActivity::onStart()" );
		super.onStart();

		GLES3JNILib.onStart( mNativeHandle );
	}

	@Override protected void onResume()
	{
		Log.v( TAG, "GLES3JNIActivity::onResume()" );
		super.onResume();
//        if(fadeImage != null) {
//			//fadeImage = null;
//            fadeImage.setAlpha(0.0f);
//        }
        fadeImage.setImageBitmap(null);
		GLES3JNILib.onResume( mNativeHandle );
	}

	@Override protected void onPause()
	{
		Log.v( TAG, "GLES3JNIActivity::onPause()" );
        GLES3JNILib.onPause( mNativeHandle );
        GLES3JNIActivity.super.onPause();

	}

	@Override protected void onStop()
	{
		Log.v( TAG, "GLES3JNIActivity::onStop()" );
		GLES3JNILib.onStop( mNativeHandle );
		super.onStop();
	}

	@Override protected void onDestroy()
	{
		Log.v( TAG, "GLES3JNIActivity::onDestroy()" );
		if ( mSurfaceHolder != null )
		{
			GLES3JNILib.onSurfaceDestroyed( mNativeHandle );
		}
		GLES3JNILib.onDestroy( mNativeHandle );
		super.onDestroy();
		mNativeHandle = 0;
	}

	@Override public void surfaceCreated( SurfaceHolder holder )
	{
        Log.v( TAG, "GLES3JNIActivity::surfaceCreated()" );
		if ( mNativeHandle != 0 )
		{
			GLES3JNILib.onSurfaceCreated( mNativeHandle, holder.getSurface() );
			mSurfaceHolder = holder;
		}
	}

	@Override public void surfaceChanged( SurfaceHolder holder, int format, int width, int height )
	{
		Log.v( TAG, "GLES3JNIActivity::surfaceChanged()" );
		if ( mNativeHandle != 0 )
		{
			GLES3JNILib.onSurfaceChanged( mNativeHandle, holder.getSurface() );
			mSurfaceHolder = holder;
		}
	}
	
	@Override public void surfaceDestroyed( SurfaceHolder holder )
	{
		Log.v( TAG, "GLES3JNIActivity::surfaceDestroyed()" );
		if ( mNativeHandle != 0 )
		{
			GLES3JNILib.onSurfaceDestroyed( mNativeHandle );
			mSurfaceHolder = null;
		}
	}

	@Override public boolean dispatchKeyEvent( KeyEvent event )
	{
        //super.dispatchKeyEvent(event);
		if ( mNativeHandle != 0 )
		{
			int keyCode = event.getKeyCode();
			int action = event.getAction();
			if ( action != KeyEvent.ACTION_DOWN && action != KeyEvent.ACTION_UP )
			{
				return super.dispatchKeyEvent( event );
			}

            if(event.getAction()==KeyEvent.ACTION_UP){
                return false;
            }

			if ( action == KeyEvent.ACTION_UP )
			{
				Log.v( TAG, "GLES3JNIActivity::dispatchKeyEvent( " + keyCode + ", " + action + " )" );
			}
			//GLES3JNILib.onKeyEvent( mNativeHandle, keyCode, action );

            if ( keyCode == KeyEvent.KEYCODE_VOLUME_UP )
            {
                adjustVolume( 1 );
                //if (this.image360TestAd.isLoaded()){// && !this.isShowing) {
                    Log.d(TAG, "Going to show u an ad now . . . .");
                    //this.isShowing = true;


                //fadeImage.setAlpha(0.0f);
                    Bitmap image = Bitmap.createBitmap(fadeImage.getWidth(), fadeImage.getHeight(), Bitmap.Config.ARGB_8888);
                    image.eraseColor(android.graphics.Color.BLACK);

				fadeImage.setImageBitmap(image);

                    anim = new AlphaAnimation(0.0f, 1.0f);
                    anim.setDuration(1500);
                    anim.setRepeatCount(0);

                    anim.setAnimationListener(new Animation.AnimationListener() {

                        @Override
                        public void onAnimationStart(Animation animation) {
                            // TODO Auto-generated method stub

                        }

                        @Override
                        public void onAnimationRepeat(Animation animation) {
                            // TODO Auto-generated method stub

                        }

                        @Override
                        public void onAnimationEnd(Animation animation) {
                            Log.d(TAG, "VISIBILITY IS GONE");
                            //mView.setVisibility(View.INVISIBLE);
                            GLES3JNIActivity.this.image360TestAd.show();
                        }
                    });

				fadeImage.startAnimation(anim);
                    //setContentView(mView);
                    //this.image360TestAd.show();
//                } else {
//                    Log.d(TAG, "No Ad Loaded!! Try again after some time.");
//                }
                return true;
            }

			return true;
		}
		return false;
	}

	@Override public boolean dispatchTouchEvent( MotionEvent event )
	{
        super.dispatchTouchEvent(event);
		if ( mNativeHandle != 0 )
		{
			int action = event.getAction();
			float x = event.getRawX();
			float y = event.getRawY();
			if ( action == MotionEvent.ACTION_UP )
			{
				Log.v( TAG, "GLES3JNIActivity::dispatchTouchEvent( " + action + ", " + x + ", " + y + " )" );
                return false;
			}
			GLES3JNILib.onTouchEvent( mNativeHandle, action, x, y );
		}
		return true;
	}
}
