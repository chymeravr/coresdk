package com.chymeravr.adclient;


import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.AsyncTask;
import android.support.v4.content.LocalBroadcastManager;
import android.util.Log;

import com.android.volley.toolbox.JsonObjectRequest;
import com.chymeravr.analytics.AnalyticsManager;
import com.chymeravr.common.Config;
import com.chymeravr.common.Util;
import com.chymeravr.schemas.eventreceiver.EventType;
import com.chymeravr.schemas.serving.AdFormat;
import com.chymeravr.schemas.serving.ResponseCode;
import com.google.android.gms.ads.identifier.AdvertisingIdClient;
import com.google.android.gms.common.GooglePlayServicesNotAvailableException;
import com.google.android.gms.common.GooglePlayServicesRepairableException;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.HashMap;

/**
 * Created by robin_chimera on 11/28/2016.
 * This class manages the Image360 ad display for the SDK
 * It is responsible for managing the ad depending on lifecycle
 * events. It listens for lifecycle events and renders ads at an
 * appropriate time.
 * Its core responsibilities are
 *      - maintaining load states
 *      - store an ad
 *      - call display methods for ad
 *      - request a new ad
 */

public final class Image360Ad extends Ad {
    private static final String TAG = "Image360Ad";
    /* below are some private variables used for internal representation of ad data and
    *  server requests */

    private int showRequestCode = 0;

    private RequestGenerator requestGenerator;

    private void adListenerCallbacks(){
        Log.v(TAG, "Ad Closed");
        this.getAdListener().onAdClosed();
    }

    class MessageHandler extends BroadcastReceiver {
        @Override
        public void onReceive(Context context, Intent intent) {
            adListenerCallbacks();
        }
    }

    public Image360Ad(Context context, AdListener adListener) {
        super(AdFormat.IMG_360, context, adListener, ChymeraVrSdk.getSdkInstance().getAnalyticsManager(),
                ChymeraVrSdk.getSdkInstance().getWebRequestQueue());

        // only one instance of image360 ad will be present always
        this.setInstanceId(-1);

        LocalBroadcastManager.getInstance(context).registerReceiver(new Image360Ad.MessageHandler(),
                new IntentFilter("adClosed"));

        this.requestGenerator = new RequestGenerator(this);
    }

    /* Request AdServer for a new image360 ad.
    * It passes targeting parameters from AdRequest for better Ads*/
    public void loadAd(final AdRequest adRequest) {
        this.setLoading(true);
//        this.emitEvent(EventType.AD_REQUEST, Event.Priority.MEDIUM, null);

        Log.i(TAG, "Requesting Server for a New 360 Image Ad");

        // Fetching advertisingId completely asynchronously will lead to race condition.
        // better to wrap them all together. It is important we get the advertId because
        // it helps in tracking app downloads - usage etc.
        AsyncTask<Void, Void, String> task = new AsyncTask<Void, Void, String>() {
            @Override
            protected String doInBackground(Void... params) {
                if(ChymeraVrSdk.getAdvertisingId() != null){
                    return null;
                }

                AdvertisingIdClient.Info idInfo = null;
                try {
                    idInfo = AdvertisingIdClient.getAdvertisingIdInfo(Image360Ad.this.getContext());
                } catch (GooglePlayServicesNotAvailableException|GooglePlayServicesRepairableException e) {
                    Log.e(TAG, "Load Ad Failed due to Exception in Google Play Services : ", e);
                    Image360Ad.this.emitEvent(EventType.ERROR, AnalyticsManager.Priority.LOW, Util.getErrorMap(e));
                } catch (Exception e) {
                    Log.e(TAG, "Load Ad Failed due to an Exception : ", e);
                    Image360Ad.this.emitEvent(EventType.ERROR, AnalyticsManager.Priority.LOW, Util.getErrorMap(e));
                }

                String advertId = null;
                try {
                    assert idInfo != null;
                    advertId = idInfo.getId();
                }
                catch (Exception e) {
                    Log.e(TAG, "Load Ad Failed due to an Exception : ", e);
                    Image360Ad.this.emitEvent(EventType.ERROR, AnalyticsManager.Priority.LOW, Util.getErrorMap(e));
                }
                ChymeraVrSdk.setAdvertisingId(advertId);
                Log.v(TAG, "AdvertisingId : " + advertId);
                return null;
            }

            @Override
            protected void onPostExecute(String advertId) {
                JsonObjectRequest jsonObjRequest =
                        Image360Ad.this.requestGenerator.getAdServerJsonRequest(adRequest);

                Image360Ad.this.getWebRequestQueue().addToRequestQueue(jsonObjRequest);
                Log.v(TAG, "fetching ad . . . ");
            }
        };
        task.execute();
    }

    @Override
    void onAdServerResponseSuccess(JSONObject response){
        try {
            String responseCodeString = response.getString("responseCode");
            ResponseCode responseCode = ResponseCode.BAD_REQUEST;

            if( responseCodeString.equals("NO_AD")){
                responseCode = ResponseCode.NO_AD;
            }else if(responseCodeString.equals("SERVED")){
                responseCode = ResponseCode.SERVED;
            }

            switch(responseCode){
                case BAD_REQUEST:
                    Log.v(TAG, "Ad Server responded with BAD_REQUEST");
                    this.getAdListener().onAdFailedToLoad();
                    break;
                case NO_AD:
                    Log.v(TAG, "Ad Server responded with NO_AD");
                    this.getAdListener().onAdFailedToLoad();
                    break;
                case SERVED:
                    JSONObject responseAdJson = response.getJSONObject("ads").getJSONObject(this.getPlacementId());
                    this.setServingId(responseAdJson.getString("servingId"));
                    this.setMediaUrl(responseAdJson.getString("mediaUrl"));
                    // TODO: 2/10/2017 change this when server changes
                    // this.setClickUrl(responseAdJson.getString("clickUrl"));

                    // download media from url and save in internal memory
                    InputStreamVolleyRequest mediaDownloadRequest = this.requestGenerator
                            .getMediaDownloadRequest(this.getMediaUrl());
                    this.getWebRequestQueue().addToRequestQueue(mediaDownloadRequest);

                    Log.i(TAG, "Ad Server response successfully processed. Proceeding to query Media Server");
                    break;
            }
//        try {
//            JSONObject responseAdJson = response.getJSONObject("ads").getJSONObject(this.getPlacementId());
//            this.setServingId(responseAdJson.getString("servingId"));
//            this.setMediaUrl(responseAdJson.getString("mediaUrl"));
//            // TODO: 2/10/2017 change this when server changes
////            this.setClickUrl(responseAdJson.getString("clickUrl"));
//
//            // download media from url and save in internal memory
//            InputStreamVolleyRequest mediaDownloadRequest = this.requestGenerator
//                    .getMediaDownloadRequest(this.getMediaUrl());
//            this.getWebRequestQueue().addToRequestQueue(mediaDownloadRequest);
//
//            Log.i(TAG, "Ad Server response successfully processed. Proceeding to query Media Server");
        } catch (JSONException e) {
            this.setLoading(false);
            this.getAdListener().onAdFailedToLoad();

            HashMap<String, String> errorMap = new HashMap<>();
            errorMap.put("Error", e.toString());
            this.emitEvent(EventType.ERROR, AnalyticsManager.Priority.LOW, errorMap);

            Log.e(TAG, "Adserver Success Response Handler encountered Exception : ", e);
        }
    }

    @Override
    void onMediaServerResponseSuccess() {
        this.getAdListener().onAdLoaded();
        this.setLoaded(true);
        this.setLoading(false);
//        this.emitEvent(EventType.AD_LOAD, Event.Priority.MEDIUM, null);
        Log.i(TAG, "Media Server Response Successful");
    }

    /* Display ad by calling the native graphics library (or 3rd party API if that is the case)*/
    public void show() {
        Intent intent = new Intent(this.getContext(), Image360Activity.class);
        intent.putExtra("clickUrl", this.getClickUrl());
        intent.putExtra("imageAdFilePath", Config.Image360AdAssetDirectory + this.getPlacementId() + ".jpg");
        intent.putExtra("servingId", this.getServingId());
        intent.putExtra("instanceId", this.getInstanceId());
        ((Activity)this.getContext()).startActivityForResult(intent, showRequestCode);

        this.getAdListener().onAdOpened();
    }

}
