package com.chymeravr.daydreamadclient;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.support.v4.content.LocalBroadcastManager;
import android.util.Log;

import com.chymeravr.adclient.Ad;
import com.chymeravr.adclient.Image360AdServices;
import com.chymeravr.adclient.VrAdListener;
import com.chymeravr.adclient.VrAdRequest;
import com.chymeravr.analytics.EventPriority;
import com.chymeravr.common.Config;
import com.chymeravr.common.Util;
import com.chymeravr.schemas.eventreceiver.EventType;
import com.chymeravr.schemas.eventreceiver.RuntimeAdMeta;
import com.chymeravr.schemas.eventreceiver.SDKEvent;
import com.chymeravr.schemas.serving.AdFormat;
import com.google.vr.ndk.base.DaydreamApi;

import java.io.File;
import java.sql.Timestamp;
import java.util.Map;

/**
 * Created by robin_chimera on 3/22/2017.
 * Image360Ad class
 */

public final class Image360Ad extends Ad {
    private static final String TAG = "Image360AdGvr";
    /* below are some private variables used for internal representation of ad data and
    *  server requests */

    private Image360AdServices image360AdServices;

    static final int IMAGE360_ACTIVIT_REQUEST = 1;

    // TODO: 4/24/2017 test whetehr this is null or not before calling functions
    //@Setter
    private DaydreamApi daydreamApi;
    Runnable initializeDaydream = new Runnable(){

        @Override
        public void run() {
            Image360Ad.this.daydreamApi = DaydreamApi.create(Image360Ad.this.getContext());
        }
    };

    private void adListenerCallbacks() {
        this.getVrAdListener().onVrAdClosed();
    }

    class MessageHandler extends BroadcastReceiver {
        @Override
        public void onReceive(Context context, Intent intent) {
            Log.d(TAG, "Received broadcast signal to close ad activity");
            ((Activity) Image360Ad.this.getContext()).finishActivity(IMAGE360_ACTIVIT_REQUEST);

            adListenerCallbacks();
        }
    }

    public Image360Ad(Context context, VrAdListener vrAdListener) {
        super(AdFormat.IMG_360, context, vrAdListener);

        // only one instance of image360 ad will be present always
        this.setInstanceId(-1);

        LocalBroadcastManager.getInstance(context).registerReceiver(new Image360Ad.MessageHandler(),
                new IntentFilter("adClosed"));

        this.image360AdServices = new Image360AdServices(this, ChymeraVrSdk.getWebRequestQueue());

        ((Activity)context).runOnUiThread(initializeDaydream);

    }

    @Override
    protected void emitEvent(EventType eventType, EventPriority priority, Map<String, String> map) {
        long currTime = new Timestamp(System.currentTimeMillis()).getTime();
        RuntimeAdMeta adMeta = new RuntimeAdMeta(this.getServingId(), this.getInstanceId());
        SDKEvent event = new SDKEvent(currTime, eventType, adMeta);
        event.setParamsMap(map);
        ChymeraVrSdk.getAnalyticsManager().push(event, priority);
    }

    /* Request AdServer for a new image360 ad.
    * It passes targeting parameters from VrAdRequest for better Ads*/
    public void loadAd(final VrAdRequest vrAdRequest) {
        if(BuildConfig.NETWORK_ENABLED) {
            this.setLoading(true);
            Log.i(TAG, "Requesting Server for a New 360 Image Ad");
            this.image360AdServices.fetchAd(vrAdRequest, ChymeraVrSdk.getApplicationId());
            return;
        }
        this.setLoaded(true);
    }

    /* Display ad by calling the native graphics library (or 3rd party API if that is the case)*/
    public void show() {
        if(daydreamApi == null){
            Log.e(TAG, "Daydream API is null ~ unable to show ad. This is probably" +
                    "not a daydream enabled phone");
            return;
        }
        File appPath = this.getContext().getFilesDir();

        String filePath = null;
        if(BuildConfig.NETWORK_ENABLED){
            filePath = Util.addLeadingSlash(Config.getImage360AdAssetDirectory())
                    + this.getPlacementId() + ".jpg";
        }
        else {
            String files[] = {"Witcher-BoatSunset-SmartPhone-360-Stereo",
                    "Witcher-CiriForestSunset-Smartphone-360-Stereo",
                    "Witcher-Fireball-SmartPhone-360-Stereo",
                    "Witcher-LightHouse-SmartPhone-360-Stereo",
                    "Witcher-Tower-Smartphone-360-Stereo",
                    "Witcher-Valley-Smartphone-360-Stereo",
                    "WitnessCreek-Smartphone-360-Stereo",
                    "WitnessHand-Smartphone-360-Stereo",
                    "WitnessSquare-Smartphone-360-Stereo"};
            int fileIndex = (int) (Math.random() * 9);

            // for non stereo 360 images - use this
            // String files[] = {"equirectangular_desert2"}
            // int fileIndex = (int)(Math.random() * 0);

           filePath = Config.getImage360AdAssetDirectory()
                   + files[fileIndex] + ".jpg";
        }

        File file = new File(appPath, filePath);

        if (file.exists()) {
            // send relevant ad data to activity with intent

            ComponentName c= new ComponentName(this.getContext(), Image360Activity.class);
            Intent vrIntent = daydreamApi.createVrIntent(c);

            if(BuildConfig.NETWORK_ENABLED) {
                vrIntent.putExtra("clickUrl", "com.google.android.apps.youtube.vr");
            }else{
                vrIntent.putExtra("clickUrl", this.getClickUrl());
            }

            vrIntent.putExtra("imageAdFilePath", filePath);
            vrIntent.putExtra("servingId", this.getServingId());
            vrIntent.putExtra("instanceId", this.getInstanceId());
            vrIntent.putExtra("returningClass", this.getContext().getClass().getName());


            // start activity for showing ad
            daydreamApi.launchInVr(vrIntent);
            this.getVrAdListener().onVrAdOpened();
        } else {
            Log.i(TAG, "No Ad to Show");
            this.getVrAdListener().onVrAdLoadFailed(VrAdRequest.Error.NO_AD_TO_SHOW, "There is no ad Loaded.");
        }

    }
}
