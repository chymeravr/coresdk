package com.chymeravr.gearvradclient;

/**
 * Created by robin_chimera on 11/28/2016.
 * This class manages the Image360 ad display for the SDK
 * It is responsible for managing the ad depending on lifecycle
 * events. It listens for lifecycle events and renders ads at an
 * appropriate time.
 * Its core responsibilities are
 * - maintaining load states
 * - store an ad
 * - call display methods for ad
 * - request a new ad
 */

import android.app.Activity;
import android.content.BroadcastReceiver;
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

import java.io.File;
import java.sql.Timestamp;
import java.util.Map;

public final class Image360Ad extends Ad {
    private static final String TAG = "Image360AdGearVr";
    /* below are some private variables used for internal representation of ad data and
    *  server requests */

    private Image360AdServices image360AdServices;

    static final int IMAGE360_ACTIVIT_REQUEST = 1;

    private void adListenerCallbacks() {
        this.getVrAdListener().onVrAdClosed();
    }

    class MessageHandler extends BroadcastReceiver {
        @Override
        public void onReceive(Context context, Intent intent) {
            ((Activity) Image360Ad.this.getContext()).finishActivity(IMAGE360_ACTIVIT_REQUEST);
            //((Activity)Image360Ad.this.getContext()).overridePendingTransition(R.anim.image360fadein, R.anim.image360fadeout);
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

        File appPath = this.getContext().getFilesDir();

        String filePath = null;
        if(BuildConfig.NETWORK_ENABLED) {
            Log.i(TAG, "Fetching ad from server");
            filePath = Util.addLeadingSlash(Config.getImage360AdAssetDirectory())
                    + this.getPlacementId() + ".jpg";
        }
        else{
            // demo mode ~ choose any of the 9 images at random

            // TODO: 5/31/2017 reconfigure for use with non stereo images
            // for non stereo images - use this
            // String files[] = {"equirectangular_desert2"}
            // int fileIndex = (int)(Math.random() * 0);
            Log.i(TAG, "Fetching ad from internal storage");
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

            filePath =  Config.getImage360AdAssetDirectory() + files[fileIndex] + ".jpg";
            this.setClickUrl("https://www.chymeravr.com");
        }

        Log.i(TAG, "Ad File stored at : " + appPath + "/" + filePath);
        File file = new File(appPath, filePath);

        if (file.exists()) {
            // send relevant ad data to activity with intent
            Intent intent = new Intent(this.getContext(), Image360Activity.class);
            intent.putExtra("clickUrl", this.getClickUrl());
            intent.putExtra("imageAdFilePath", filePath);
            intent.putExtra("servingId", this.getServingId());
            intent.putExtra("instanceId", this.getInstanceId());

            // start activity for showing ad
            ((Activity) this.getContext()).startActivityForResult(intent, IMAGE360_ACTIVIT_REQUEST);
            //((Activity)this.getContext()).overridePendingTransition(R.anim.image360fadein, R.anim.image360fadeout);

            // client callback - could there be a possible race condition with using same file for all ads?
            this.getVrAdListener().onVrAdOpened();
        } else {
            Log.i(TAG, "No Ad to Show");
            this.getVrAdListener().onVrAdLoadFailed(VrAdRequest.Error.NO_AD_TO_SHOW, "There is no ad Loaded.");
        }

    }

}
