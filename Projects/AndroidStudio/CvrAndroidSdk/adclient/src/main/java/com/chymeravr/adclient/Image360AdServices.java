package com.chymeravr.adclient;

import android.app.ActivityManager;
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.AsyncTask;
import android.os.Build;
import android.util.Log;

import com.android.volley.NetworkError;
import com.android.volley.NoConnectionError;
import com.android.volley.Request;
import com.android.volley.ServerError;
import com.android.volley.TimeoutError;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.chymeravr.analytics.EventPriority;
import com.chymeravr.common.Config;
import com.chymeravr.common.Util;
import com.chymeravr.common.WebRequestQueue;
import com.chymeravr.schemas.eventreceiver.EventType;
import com.chymeravr.schemas.serving.Demographics;
import com.chymeravr.schemas.serving.Device;
import com.chymeravr.schemas.serving.Location;
import com.chymeravr.schemas.serving.Placement;
import com.chymeravr.schemas.serving.ResponseCode;
import com.chymeravr.schemas.serving.ServingRequest;
import com.google.android.gms.ads.identifier.AdvertisingIdClient;
import com.google.android.gms.common.GooglePlayServicesNotAvailableException;
import com.google.android.gms.common.GooglePlayServicesRepairableException;
import com.google.gson.Gson;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import lombok.NonNull;

import static android.content.Context.ACTIVITY_SERVICE;
import static com.chymeravr.analytics.EventPriority.LOW;


/**
 * Created by robin_chimera on 12/9/2016.
 * Handles generation of requests for image 360 ads - used for daydream, gearvr & cardboard
 * loading ad spans 2 requests - 1st to the ad server : we send vradrequest object
 *                             - 2nd to the media server : dependent on 1st response
 * upon receive a media url (360 jpeg image) - we proceed to download it to device
 * todo: delete the file after ad is shown?
 */

public final class Image360AdServices {
    private static final String TAG = "Image360AdServices";

    private Ad ad;

    private String mediaUrl;

    // these listeners encapsulate response/error listeners for volley class
    private AdServerResponseListener adServerResponseListener;
    private AdServerErrorListener adServerErrorListener;

    private Image360MediaServerResponseListener mediaServerResponseListener;
    private Image360MediaServerErrorListener mediaServerErrorListener;

    private WebRequestQueue webRequestQueue;

    public Image360AdServices(Ad ad, WebRequestQueue webRequestQueue){
        this.ad = ad;
        this.webRequestQueue = webRequestQueue;
        this.adServerResponseListener = new AdServerResponseListener(this);
        this.adServerErrorListener = new AdServerErrorListener(this);

        this.mediaServerResponseListener = new Image360MediaServerResponseListener(this);
        this.mediaServerErrorListener = new Image360MediaServerErrorListener(this);
    }

    public void fetchAd(final VrAdRequest vrAdRequest, final String applicationId){
        // Fetching advertisingId completely asynchronously will lead to race condition.
        // better to wrap them all together. It is important we get the advertId because
        // it helps in tracking app downloads - usage etc.
        // after fetching advertising id, we make our calls to chymeravr ad server which
        // then calls the media server for image360 ad file
        AsyncTask<Void, Void, String> task = new AsyncTask<Void, Void, String>() {
            @Override
            protected String doInBackground(Void... params) {
                if(ad.getAdvertisingId() != null){
                    return null;
                }

                AdvertisingIdClient.Info idInfo = null;
                try {
                    idInfo = AdvertisingIdClient.getAdvertisingIdInfo(ad.getContext());
                } catch (GooglePlayServicesNotAvailableException
                        | GooglePlayServicesRepairableException e) {
                    Log.e(TAG, "Load Ad Failed due to Exception in Google Play Services : ", e);
                    ad.emitEvent(EventType.ERROR, EventPriority.LOW, Util.getErrorMap(e));
                } catch (Exception e) {
                    Log.e(TAG, "Load Ad Failed due to an Exception : ", e);
                    ad.emitEvent(EventType.ERROR, EventPriority.LOW, Util.getErrorMap(e));
                }

                String advertId = null;
                try {
                    assert idInfo != null;
                    advertId = idInfo.getId();
                } catch (Exception e) {
                    Log.e(TAG, "Load Ad Failed due to an Exception : ", e);
                    ad.emitEvent(EventType.ERROR, EventPriority.LOW, Util.getErrorMap(e));
                }
                ad.setAdvertisingId(advertId);
                Log.d(TAG, "AdvertisingId : " + advertId);
                return null;
            }

            @Override
            protected void onPostExecute(String advertId) {
                WifiManager wifiManager = (WifiManager) ad.getContext().getApplicationContext()
                        .getSystemService(Context.WIFI_SERVICE);
                WifiInfo info = wifiManager.getConnectionInfo();
                JsonObjectRequest jsonObjRequest =
                        getAdServerJsonRequest(vrAdRequest, info, applicationId, ad.getAdvertisingId());

                webRequestQueue.addToRequestQueue(jsonObjRequest);
                Log.d(TAG, "Fetching ad from media server ");
            }
        };
        task.execute();
    }

    // construct a jsonobjectrequest that our ad server will understand
    private JsonObjectRequest getAdServerJsonRequest(@NonNull VrAdRequest vrAdRequest,
                                                    @NonNull WifiInfo wifiInfo, @NonNull String appId,
                                                    @NonNull String advertId) {

        long timestamp = new Timestamp(System.currentTimeMillis()).getTime();

        int sdkVersion = Config.getSdkVersion();

        //String appId = ChymeraVrSdk.getApplicationId();

        List<Placement> placements = new ArrayList<>();
        placements.add(new Placement(this.ad.getPlacementId(), this.ad.getAdFormat()));

        ServingRequest servingRequest
                = new ServingRequest(timestamp, (short) sdkVersion, appId, placements,
                Config.getOsId(), String.valueOf(Build.VERSION.SDK_INT),
                advertId, Config.getHmdId());

        if(vrAdRequest.getLocation() != null) {
            Location loc = new Location(vrAdRequest.getLocation().getLatitude(),
                    vrAdRequest.getLocation().getLongitude(),
                    vrAdRequest.getLocation().getAccuracy());
            servingRequest.setLocation(loc);
        }


        Demographics demo = new Demographics(vrAdRequest.getBirthday().toString(),
                vrAdRequest.getGender().getValue(),
                vrAdRequest.getEmail());
        servingRequest.setDemographics(demo);

        ActivityManager actManager = (ActivityManager) this.ad.getContext().getSystemService(ACTIVITY_SERVICE);
        ActivityManager.MemoryInfo memInfo = new ActivityManager.MemoryInfo();
        actManager.getMemoryInfo(memInfo);
        long totalMemory = memInfo.totalMem;
        Device device = new Device(Build.MANUFACTURER, Build.MODEL, Double.toString(totalMemory));
        servingRequest.setDevice(device);

        String connectivity = null;
        ConnectivityManager cm = (ConnectivityManager) this.ad.getContext().getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo activeNetwork = cm.getActiveNetworkInfo();
        if (activeNetwork != null) { // connected to the internet
            if (activeNetwork.getType() == ConnectivityManager.TYPE_WIFI) {
                connectivity = "Wifi";
            } else if (activeNetwork.getType() == ConnectivityManager.TYPE_MOBILE) {
                connectivity = "Mobile";
            }
        } else {
            Log.e("Image360AdServices", "User currently not connected to the internet");
        }
        servingRequest.setConnectivity(connectivity);

        String ssid = wifiInfo.getSSID();
        servingRequest.setWifiName(ssid);

        String servingRequestJson = new Gson().toJson(servingRequest);
        JSONObject jsonAdRequest = null;
        try {
            jsonAdRequest = new JSONObject(servingRequestJson);
        } catch (JSONException e) {
            Log.e(TAG, "Encountered a JSONException in creating ad request", e);
            this.ad.emitEvent(EventType.ERROR, EventPriority.HIGH, Util.getErrorMap(e));
        }
        return new JsonObjectRequest(Request.Method.POST, Config.getAdServer(), jsonAdRequest,
                this.adServerResponseListener, this.adServerErrorListener) {
            // we need to overwrite the default content type.
            @Override
            public String getBodyContentType() {
                return "application/json";
            }
        };
    }

    void onAdServerResponseSuccess(JSONObject response) {
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
                    this.ad.getVrAdListener().onVrAdLoadFailed(VrAdRequest.Error.ADSERVER_FAILURE,
                            "Ad Server responded with BAD_REQUEST");
                    break;
                case NO_AD:
                    Log.v(TAG, "Ad Server responded with NO_AD");
                    this.ad.getVrAdListener().onVrAdLoadFailed(VrAdRequest.Error.NO_AD_TO_SHOW,
                            "Ad Server Responded with NO_AD");
                    break;
                case SERVED:
                    JSONObject responseAdJson =
                            response.getJSONObject("ads").getJSONObject(this.ad.getPlacementId());
                    this.ad.setServingId(responseAdJson.getString("servingId"));
                    this.ad.setClickUrl(responseAdJson.getString("clickUrl"));

                    this.mediaUrl = responseAdJson.getString("mediaUrl");

                    // download media from url and save in internal memory
                    InputStreamVolleyRequest mediaDownloadRequest =
                        new InputStreamVolleyRequest(Request.Method.GET, this.mediaUrl,
                                this.mediaServerResponseListener, this.mediaServerErrorListener, null);

                    webRequestQueue.addToRequestQueue(mediaDownloadRequest);

                    Log.i(TAG, "Ad Server response successfully processed. "
                            + "Proceeding to query Media Server");
                    break;
            }

        } catch (JSONException e) {
            this.ad.setLoading(false);
            this.ad.getVrAdListener().onVrAdLoadFailed(VrAdRequest.Error.UNKNOWN_FAILURE,
                    e.toString());

            HashMap<String, String> errorMap = new HashMap<>();
            errorMap.put("Error", e.toString());
            this.ad.emitEvent(EventType.ERROR, EventPriority.LOW, errorMap);

            Log.e(TAG, "Adserver Success Response Handler encountered Exception : ", e);
        }
    }

    void onAdServerResponseError(VolleyError error){
        VrAdRequest.Error errorCode;
        if (error instanceof TimeoutError || error instanceof NoConnectionError
                || error instanceof ServerError) {
            errorCode = VrAdRequest.Error.ADSERVER_FAILURE;
        } else if (error instanceof NetworkError) {
            errorCode = VrAdRequest.Error.NETWORK_FAILURE;
        } else {
            errorCode = VrAdRequest.Error.UNKNOWN_FAILURE;            // screwed
        }

        this.ad.setLoading(false);
        this.ad.getVrAdListener().onVrAdLoadFailed(errorCode, error.toString());
        this.ad.emitEvent(EventType.ERROR, EventPriority.LOW, Util.getErrorMap(error));
    }

    void onMediaServerResponseSuccess(byte[] response) {
        try {
            if (response!=null) {

                File appPath = this.ad.getContext().getFilesDir();
                String appSdkPath = appPath + Util.addLeadingSlash(Config.getImage360AdAssetDirectory());
                File dest_dir = new File(appSdkPath);
                Util.createDir(dest_dir);

                FileOutputStream outputStream;
                String name= this.ad.getPlacementId() + ".jpg";
                Log.d(TAG, "writing file to: " + dest_dir + name);
                outputStream = new FileOutputStream(new File(dest_dir, name));
                outputStream.write(response);
                outputStream.close();

                this.ad.getVrAdListener().onVrAdLoaded();
                this.ad.setLoaded(true);

            }
        } catch (IOException e) {
            // this should not really happen catch block
            this.ad.getVrAdListener().onVrAdLoadFailed(VrAdRequest.Error.UNKNOWN_FAILURE, e.toString());
            // send error logs to server
            this.ad.emitEvent(EventType.ERROR, LOW, Util.getErrorMap(e));
            Log.e(TAG, "Error processing download file for media ad : " + e.toString());
        }finally{
            this.ad.setLoading(false);
            Log.i(TAG, "Media Server Response Successful");
        }
    }


    void onMediaServerResponseError(VolleyError error){
        this.ad.setLoading(false);

        VrAdRequest.Error errorCode;
        if (error instanceof TimeoutError || error instanceof NoConnectionError
                || error instanceof ServerError) {
            errorCode = VrAdRequest.Error.ADSERVER_FAILURE;
        } else if (error instanceof NetworkError) {
            errorCode = VrAdRequest.Error.NETWORK_FAILURE;
        } else {
            errorCode = VrAdRequest.Error.UNKNOWN_FAILURE;            // screwed
        }

        this.ad.getVrAdListener().onVrAdLoadFailed(errorCode, error.toString());
        this.ad.emitEvent(EventType.ERROR, LOW, Util.getErrorMap(error));
    }


}