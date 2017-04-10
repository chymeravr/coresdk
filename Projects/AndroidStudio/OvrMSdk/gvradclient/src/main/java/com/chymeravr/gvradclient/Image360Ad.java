package com.chymeravr.gvradclient;

import android.app.Activity;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.AsyncTask;
import android.support.v4.content.LocalBroadcastManager;
import android.util.Log;

import com.android.volley.toolbox.JsonObjectRequest;
import com.chymeravr.adclient.Ad;
import com.chymeravr.adclient.InputStreamVolleyRequest;
import com.chymeravr.adclient.RequestGenerator;
import com.chymeravr.adclient.VrAdListener;
import com.chymeravr.adclient.VrAdRequest;
import com.chymeravr.analytics.AnalyticsManager;
import com.chymeravr.common.Config;
import com.chymeravr.common.Util;
import com.chymeravr.schemas.eventreceiver.EventType;
import com.chymeravr.schemas.serving.AdFormat;
import com.chymeravr.schemas.serving.ResponseCode;
import com.google.android.gms.ads.identifier.AdvertisingIdClient;
import com.google.android.gms.common.GooglePlayServicesNotAvailableException;
import com.google.android.gms.common.GooglePlayServicesRepairableException;
import com.google.vr.ndk.base.DaydreamApi;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.File;
import java.util.HashMap;

/**
 * Created by robin_chimera on 3/22/2017.
 * Image360Ad class
 */

public class Image360Ad extends Ad {
    private static final String TAG = "Image360AdGvr";
    /* below are some private variables used for internal representation of ad data and
    *  server requests */

    private RequestGenerator requestGenerator;

    static final int IMAGE360_ACTIVIT_REQUEST = 1;

    DaydreamApi daydreamApi;

    private void adListenerCallbacks() {
        this.getVrAdListener().onAdClosed();
    }

    class MessageHandler extends BroadcastReceiver {
        @Override
        public void onReceive(Context context, Intent intent) {
            //((Activity) Image360Ad.this.getContext()).finishActivity(IMAGE360_ACTIVIT_REQUEST);
            //((Activity)Image360Ad.this.getContext()).overridePendingTransition(R.anim.image360fadein, R.anim.image360fadeout);
            Log.d(TAG, "Received broadcast signal to close ad activity");
            ((Activity) Image360Ad.this.getContext()).finishActivity(IMAGE360_ACTIVIT_REQUEST);
            //daydreamApi.exitFromVr((Activity) Image360Ad.this.getContext(), IMAGE360_ACTIVIT_REQUEST, null);
            adListenerCallbacks();
            Image360Ad.this.daydreamApi.close();
        }
    }

    public Image360Ad(Context context, VrAdListener vrAdListener) {
        super(AdFormat.IMG_360, context, vrAdListener, ChymeraVrSdk.getSdkInstance().getAnalyticsManager(),
                ChymeraVrSdk.getSdkInstance().getWebRequestQueue());

        // only one instance of image360 ad will be present always
        this.setInstanceId(-1);

        LocalBroadcastManager.getInstance(context).registerReceiver(new Image360Ad.MessageHandler(),
                new IntentFilter("adClosed"));

        this.requestGenerator = new RequestGenerator(this);

    }

    /* Request AdServer for a new image360 ad.
    * It passes targeting parameters from VrAdRequest for better Ads*/
    public void loadAd(final VrAdRequest vrAdRequest) {
        this.setLoading(true);

        Log.i(TAG, "Requesting Server for a New 360 Image Ad");

        // Fetching advertisingId completely asynchronously will lead to race condition.
        // better to wrap them all together. It is important we get the advertId because
        // it helps in tracking app downloads - usage etc.
        AsyncTask<Void, Void, String> task = new AsyncTask<Void, Void, String>() {
            @Override
            protected String doInBackground(Void... params) {
                if (ChymeraVrSdk.getAdvertisingId() != null) {
                    return null;
                }

                AdvertisingIdClient.Info idInfo = null;
                try {
                    idInfo = AdvertisingIdClient.getAdvertisingIdInfo(Image360Ad.this.getContext());
                } catch (GooglePlayServicesNotAvailableException | GooglePlayServicesRepairableException e) {
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
                } catch (Exception e) {
                    Log.e(TAG, "Load Ad Failed due to an Exception : ", e);
                    Image360Ad.this.emitEvent(EventType.ERROR, AnalyticsManager.Priority.LOW, Util.getErrorMap(e));
                }
                ChymeraVrSdk.setAdvertisingId(advertId);
                Log.d(TAG, "AdvertisingId : " + advertId);
                return null;
            }

            @Override
            protected void onPostExecute(String advertId) {
                WifiManager wifiManager = (WifiManager) Image360Ad.this.getContext().getApplicationContext()
                        .getSystemService(Context.WIFI_SERVICE);
                WifiInfo info = wifiManager.getConnectionInfo();
                JsonObjectRequest jsonObjRequest =
                        Image360Ad.this.requestGenerator.getAdServerJsonRequest(vrAdRequest, info,
                                ChymeraVrSdk.getApplicationId(), ChymeraVrSdk.getAdvertisingId());

                Image360Ad.this.getWebRequestQueue().addToRequestQueue(jsonObjRequest);
                Log.d(TAG, "Fetching ad from media server ");
            }
        };
        task.execute();
    }

    @Override
    public void onAdServerResponseSuccess(JSONObject response) {
        try {
            String responseCodeString = response.getString("responseCode");
            ResponseCode responseCode = ResponseCode.BAD_REQUEST;

            if (responseCodeString.equals("NO_AD")) {
                responseCode = ResponseCode.NO_AD;
            } else if (responseCodeString.equals("SERVED")) {
                responseCode = ResponseCode.SERVED;
            }

            switch (responseCode) {
                case BAD_REQUEST:
                    Log.v(TAG, "Ad Server responded with BAD_REQUEST");
                    this.getVrAdListener().onAdLoadFailed(VrAdRequest.Error.ADSERVER_FAILURE, "Ad Server responded with BAD_REQUEST");
                    break;
                case NO_AD:
                    Log.v(TAG, "Ad Server responded with NO_AD");
                    this.getVrAdListener().onAdLoadFailed(VrAdRequest.Error.NO_AD_TO_SHOW, "Ad Server Responded with NO_AD");
                    break;
                case SERVED:
                    JSONObject responseAdJson = response.getJSONObject("ads").getJSONObject(this.getPlacementId());
                    this.setServingId(responseAdJson.getString("servingId"));
                    this.setMediaUrl(responseAdJson.getString("mediaUrl"));
                    this.setClickUrl(responseAdJson.getString("clickUrl"));

                    // download media from url and save in internal memory
                    InputStreamVolleyRequest mediaDownloadRequest = this.requestGenerator
                            .getMediaDownloadRequest(this.getMediaUrl());
                    this.getWebRequestQueue().addToRequestQueue(mediaDownloadRequest);

                    Log.i(TAG, "Ad Server response successfully processed. Proceeding to query Media Server");
                    break;
            }

        } catch (JSONException e) {
            this.setLoading(false);
            this.getVrAdListener().onAdLoadFailed(VrAdRequest.Error.UNKNOWN_FAILURE, e.toString());

            HashMap<String, String> errorMap = new HashMap<>();
            errorMap.put("Error", e.toString());
            this.emitEvent(EventType.ERROR, AnalyticsManager.Priority.LOW, errorMap);

            Log.e(TAG, "Adserver Success Response Handler encountered Exception : ", e);
        }
    }

    @Override
    public void onMediaServerResponseSuccess() {
        this.getVrAdListener().onAdLoaded();
        this.setLoaded(true);
        this.setLoading(false);
        Log.i(TAG, "Media Server Response Successful");
    }

    /* Display ad by calling the native graphics library (or 3rd party API if that is the case)*/
    public void show() {

        File appPath = this.getContext().getFilesDir();

        // TODO: 3/16/2017 reverse this in release

        //String filePath = Util.addLeadingSlash(Config.Image360AdAssetDirectory) + this.getPlacementId() + ".jpg";

        //File file = new File(appPath, filePath);

        //if(file.exists()) {

        daydreamApi = DaydreamApi.create(this.getContext());

//        String files[] = {"Witcher-BoatSunset-SmartPhone-360-Stereo",
//                "Witcher-CiriForestSunset-Smartphone-360-Stereo",
//                "Witcher-Fireball-SmartPhone-360-Stereo",
//                "Witcher-LightHouse-SmartPhone-360-Stereo",
//                "Witcher-Tower-Smartphone-360-Stereo",
//                "Witcher-Valley-Smartphone-360-Stereo",
//                "WitnessCreek-Smartphone-360-Stereo",
//                "WitnessHand-Smartphone-360-Stereo",
//                "WitnessSquare-Smartphone-360-Stereo"};
//        int fileIndex = (int)(Math.random() * 9);

        String files[] = {"equirectangular_desert2",
                };
        int fileIndex = (int)(Math.random() * 0);

        String filePath =  Config.Image360AdAssetDirectory + files[fileIndex] + ".jpg";

        this.setClickUrl("https://www.chymeravr.com");
        File file = new File(appPath, filePath);

        if (file.exists()) {
            // send relevant ad data to activity with intent
            Intent intent = new Intent(this.getContext(), Image360Activity.class);
            intent.putExtra("clickUrl", this.getClickUrl());
            intent.putExtra("imageAdFilePath", filePath);
            intent.putExtra("servingId", this.getServingId());
            intent.putExtra("instanceId", this.getInstanceId());

            // start activity for showing ad
            //((Activity) this.getContext()).startActivityForResult(intent, IMAGE360_ACTIVIT_REQUEST);
            Intent vrIntent =  DaydreamApi.setupVrIntent(intent);
            PendingIntent pendingIntent = PendingIntent.getActivity(this.getContext(), IMAGE360_ACTIVIT_REQUEST, vrIntent, PendingIntent.FLAG_ONE_SHOT);
            daydreamApi.launchInVrForResult((Activity)this.getContext(), pendingIntent, IMAGE360_ACTIVIT_REQUEST);
            this.getVrAdListener().onAdOpened();
        } else {
            Log.i(TAG, "No Ad to Show");
            this.getVrAdListener().onAdLoadFailed(VrAdRequest.Error.NO_AD_TO_SHOW, "There is no ad Loaded.");
        }

    }
}
