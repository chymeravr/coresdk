package com.chymeravr.adclient;

import android.app.Activity;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.media.AudioManager;
import android.net.Uri;
import android.os.Bundle;
import android.support.v4.app.NotificationCompat;
import android.support.v4.content.LocalBroadcastManager;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.WindowManager;

import com.chymeravr.analytics.AnalyticsManager;
import com.chymeravr.analytics.Event;
import com.chymeravr.common.Config;

import java.sql.Timestamp;
import java.util.HashMap;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

/**
 * Created by robin_chimera on 1/24/2017.
 */

public final class Image360Activity extends Activity implements SurfaceHolder.Callback {

    public static final String TAG = "Image360AdActivity";

    private SurfaceView mView;
    private SurfaceHolder mSurfaceHolder;
    private long mNativeHandle;

    private String clickUrl;

    private static AnalyticsManager analyticsManager;
    private AdListener adListener;

    // load the native library for image 360 ads
    static {
        System.loadLibrary("image360ad");
    }

    private ScheduledExecutorService scheduler;

    private final Runnable hmdPollRunner = new Runnable() {
        public void run() {
            Log.v(TAG, "Fetching HMD Parameters");
            Image360Activity.this.getHMDParams();
        }
    };

    // Native methods for Activity lifecyle
    private native long onCreateNative(Activity activity, String appDir);

    private native void onStartNative(long handle);

    private native void onPauseNative(long handle);

    private native void onResumeNative(long handle);

    private native void onStopNative(long handle);

    private native void onDestroyNative(long handle);

    // Native methods for Surface Lifecycle             WHY ARE THESE METHODS STATIC?
    public native void onSurfaceCreatedNative(long handle, Surface s);

    public native void onSurfaceChangedNative(long handle, Surface s);

    public native void onSurfaceDestroyedNative(long handle);

    // Native methods for handling touch and key events
    public native void onKeyEventNative(long handle, int keyCode, int action);

    public native void onTouchEventNative(long handle, int action, float x, float y);

    public native float[] getHMDParamsNative(long handle);

    private void killActivity() {
        // TODO: 2/2/2017 the surface is closed in correctly here - fix it
        Intent intent = new Intent("adClosed");
        LocalBroadcastManager.getInstance(this).sendBroadcast(intent);
        finish();
    }

    class MessageHandler extends BroadcastReceiver {
        @Override
        public void onReceive(Context context, Intent intent) {
            killActivity();
        }
    }


    @Override
    protected void onCreate(Bundle icicle) {
        Log.d(TAG, "onCreate()");
        super.onCreate(icicle);

        // Fetch url to show when user clicks
        Intent intent = getIntent();
        this.clickUrl = intent.getStringExtra("clickUrl");

        // Register an kill activity intent to destroy the activity when user is done and return to parent
        LocalBroadcastManager.getInstance(this).registerReceiver(new MessageHandler(),
                new IntentFilter("kill"));

        // get analytics manager from sdk manager
        analyticsManager = ChymeraVrSdk.getAnalyticsManager();

        // schedule polling for hmd parameters
        this.scheduler = Executors.newScheduledThreadPool(1);
        scheduler.scheduleAtFixedRate(hmdPollRunner, Config.hmdSamplingDelay,
                Config.hmdSamplingPeriod, TimeUnit.SECONDS);

        // call native creation method for all the HMD magic with OVR
        String basePath = this.getFilesDir().getAbsolutePath();
        this.mNativeHandle = this.onCreateNative(this, basePath);

        // We draw everything in a surface embedded within the activity layout
        this.mView = new SurfaceView(this);
        this.setContentView(mView);
        this.mView.getHolder().addCallback(this);

        // Force the screen to stay on, rather than letting it dim and shut off
        // while the user is watching amovie.
        this.getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        // Force screen brightness to stay at maximum
        WindowManager.LayoutParams params = this.getWindow().getAttributes();
        params.screenBrightness = 1.0f;
        this.getWindow().setAttributes(params);

    }

    @Override
    protected void onStart() {
        Log.d(TAG, "onStart()");
        super.onStart();
        this.onStartNative(this.mNativeHandle);
    }

    @Override
    protected void onResume() {
        Log.d(TAG, "onResume()");
        super.onResume();
        this.onResumeNative(this.mNativeHandle);

    }

    @Override
    protected void onPause() {
        Log.d(TAG, "onPause()");
        super.onPause();
        this.onPauseNative(this.mNativeHandle);
    }

    @Override
    protected void onStop() {
        Log.d(TAG, "onStop()");
        analyticsManager.push(new Event((new Timestamp(System.currentTimeMillis())).getTime(),
                Event.EventType.ADCLOSE,
                Event.Priority.HIGH));
        super.onStop();
        this.onStopNative(this.mNativeHandle);
    }

    @Override
    protected void onDestroy() {
        Log.d(TAG, "onDestroy()");
        super.onDestroy();
        this.onDestroyNative(this.mNativeHandle);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        Log.v(TAG, "ActivityGearVR::onSurfaceCreated()");
        this.analyticsManager.push(new Event((new Timestamp(System.currentTimeMillis())).getTime(),
                Event.EventType.ADSHOW,
                Event.Priority.HIGH));
        if (this.mNativeHandle != 0) {
            onSurfaceCreatedNative(this.mNativeHandle, holder.getSurface());
            this.mSurfaceHolder = holder;
        }
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        Log.v(TAG, "ActivityGearVR::onSurfaceChanged()");
        if (this.mNativeHandle != 0) {
            onSurfaceChangedNative(this.mNativeHandle, holder.getSurface());
            this.mSurfaceHolder = holder;
        }
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        Log.v(TAG, "ActivityGearVR::onSurfaceDestroyed()");
        if (this.mNativeHandle != 0) {
            onSurfaceDestroyedNative(this.mNativeHandle);
            this.mSurfaceHolder = null;
        }
    }


    private void adjustVolume(int direction) {
        AudioManager audio = (AudioManager) this.getSystemService(Context.AUDIO_SERVICE);
        audio.adjustStreamVolume(AudioManager.STREAM_MUSIC, direction, 0);
    }

    @Override
    public boolean dispatchKeyEvent(KeyEvent event) {
        Log.d(TAG, "dispatchKeyEvent");
        if (mNativeHandle != 0) {
            int keyCode = event.getKeyCode();
            int action = event.getAction();
            analyticsManager.push(new Event((new Timestamp(System.currentTimeMillis())).getTime(),
                    Event.EventType.ADCLICK,
                    Event.Priority.HIGH));
            if (keyCode == KeyEvent.KEYCODE_VOLUME_UP) {
                adjustVolume(1);
                this.notifyUser();
            }
            if (keyCode == KeyEvent.KEYCODE_VOLUME_DOWN) {
                adjustVolume(-1);
                Intent intent = new Intent("kill");
                LocalBroadcastManager.getInstance(this).sendBroadcast(intent);
            }
            if (action == KeyEvent.ACTION_UP) {
                Log.v(TAG, "dispatchKeyEvent( " + keyCode + ", " + action + " )");
            }
            this.onKeyEventNative(this.mNativeHandle, keyCode, action);
        }

        return false;
    }

    @Override
    public boolean dispatchTouchEvent(MotionEvent event) {
        if (this.mNativeHandle != 0) {
            int action = event.getAction();
            float x = event.getRawX();
            float y = event.getRawY();
            if (action == MotionEvent.ACTION_UP) {
                Log.v(TAG, "GLES3JNIActivity::dispatchTouchEvent( " + action + ", " + x + ", " + y + " )");
            }
            this.onTouchEventNative(this.mNativeHandle, action, x, y);
        }
        return true;
    }

    private void notifyUser() {
        Intent notificationIntent = new Intent(Intent.ACTION_VIEW);
        notificationIntent.setData(Uri.parse(this.clickUrl));
        PendingIntent pi = PendingIntent.getActivity(this, 0, notificationIntent, 0);

        Notification notification = new NotificationCompat.Builder(this)
                .setTicker("<Yourtext>")
                .setSmallIcon(android.R.drawable.ic_menu_report_image)
                .setContentTitle("<Yourtext>")
                .setContentText("<ContextText>")
                .setContentIntent(pi)
                .setAutoCancel(true)
                .build();

        NotificationManager notificationManager2 = (NotificationManager) this.getSystemService(Service.NOTIFICATION_SERVICE);
        notificationManager2.notify(0, notification);
    }

    public static void sendHMDParams(int param) {
        Log.v(TAG, "JNI Sent Param " + param);
        analyticsManager.push(new Event((new Timestamp(System.currentTimeMillis())).getTime(),
                Event.EventType.ADCLICK,
                Event.Priority.HIGH));
    }

    public void getHMDParams() {
        float[] hmdParams = this.getHMDParamsNative(this.mNativeHandle);
        HashMap<String, Object> hmdEyeMap = new HashMap<>();

        String[] parameterKeys =
                {"L00", "L01", "L02", "L03",
                 "L10", "L11", "L12", "L13",
                 "L20", "L21", "L22", "L23",
                 "L30", "L31", "L32", "L33",

                 "R00", "R01", "R02", "R03",
                 "R10", "R11", "R12", "R13",
                 "R20", "R21", "R22", "R23",
                 "R30", "R31", "R32", "R33"};

        int i = 0;
        for( String key : parameterKeys){
            hmdEyeMap.put(key, hmdParams[i++]);
        }

        Event event = new Event((new Timestamp(System.currentTimeMillis())).getTime(),
                Event.EventType.ADVIEWMETRICS,
                Event.Priority.LOW, hmdEyeMap);
        analyticsManager.push(event);
    }
}
