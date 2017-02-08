package com.chymeravr.adclient;

import android.content.Context;
import android.os.Build.VERSION;
import android.util.Log;

import com.android.volley.Request;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.chymeravr.analytics.AnalyticsManager;
import com.chymeravr.common.Config;
import com.chymeravr.common.Util;
import com.chymeravr.common.WebRequestQueue;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.Iterator;
import java.util.Map;

import lombok.AccessLevel;
import lombok.Getter;
import lombok.NonNull;
import lombok.Setter;

/**
 * Created by robin_chimera on 11/28/2016.
 */

/**
 * Sets up the SDK for sending and receiving Ad Requests & Response.
 * Client Activity must initialize this class by calling the initialize
 * method before creating ads; Failure to do so will result in inability to
 * monetize their application
 * Mandatory permissions - INTERNET, ACCESS_NETWORK_STATE
 * Recommended permissions - ACCESS_COARSE_LOCATION, ACCESS_FINE_LOCATION,
 * ACCESS_WIFI_STATE, CHANGE_WIFI_STATE, VIBRATE,
 * WRITE_EXTERNAL_STORAGE
 * Verifies that the publisher provides adequate permissions for the
 * SDK.
 * Verify correct version of Android
 * Verify that the publisher has configured his Application IDs correctly
 * Implemented as a singleton class
 */

public final class ChymeraVrSdk {

    private static final String TAG = "ChymeraVrSdk";

    @Getter
    @Setter(AccessLevel.PRIVATE)
    @NonNull
    private static Context context;

    @Getter
    @Setter(AccessLevel.PRIVATE)
    @NonNull
    private static String applicationId;

    @Getter(AccessLevel.PACKAGE)
    @Setter(AccessLevel.PACKAGE)
    private static String advertisingId;

    @Getter
    private static AnalyticsManager analyticsManager;

    @Getter
    private static WebRequestQueue webRequestQueue;

    @Getter
    private static final ChymeraVrSdk ourInstance = new ChymeraVrSdk();

    private static int retryNo = 10;
    private ChymeraVrSdk() {
    }

    /* Correctly set up Context, verify appCode, check for required permissions*/
    public static void initialize(final Context context, final String applicationId) {
        /** TODO: figure out what to do with the app context
         *        Initialize only once
         **/

        final boolean[] isCompleted = {false};

        setContext(context);
        setApplicationId(applicationId);

        /* TODO: Verify application code format & validity*/

        /* Check whether client has granted the mandatory permissions for this SDK to function
            We need internet and network state
        */
        if (!Util.checkMandatoryPermissions(context)) {
            Log.e(TAG,
                    "One or more of the Mandatory Permisions required for SDK has not been granted." +
                            " The SDK failed to initialize.");
            return;
        }

        Map<String, Boolean> recommendedPermissionsMap = Util.checkRecommendedPermissions(context);
        for (Map.Entry<String, Boolean> entry : recommendedPermissionsMap.entrySet()) {
            if (!entry.getValue()) {
                Log.w(TAG, entry.getKey() + " unavailable. Enable this for better Ad Targeting");
            }
        }

        if (VERSION.SDK_INT < Config.androidVersionNo) {
            Log.e(TAG, "ChymeraVR Ad Client SDK is only available with Android Version "
                    + Config.androidVersionNo + " or higher. " + "SDK initialization failed!");
            return;
        } else {
            Log.i(TAG, "Version Check Succeeded! " + VERSION.SDK_INT);
        }

        // initialize volley asynchronous request queue
        webRequestQueue = WebRequestQueue.setInstance(context);

        // Get configuration options from server
        fetchSdkConfig(webRequestQueue);

        analyticsManager = AnalyticsManager.getInstance(context, webRequestQueue, applicationId);
        analyticsManager.initialize();

        Log.i(TAG, "SDK successfully initialized");

    }

    public static void shutdown(){
        // Shutdown must be called after intialize to release any allocated resources
        analyticsManager.shutdown();
    }

    /* mute the ad */
    public static void setAppMuted(boolean muted) {

    }

    /* set the app volume */
    public static void setAppVolume(float volume) {

    }

    public static boolean isApplicationCodeValid(String applicationCode) {
        return true;
    }

    public static void fetchSdkConfig(WebRequestQueue requestQueue){
        JsonObjectRequest jsonRequest = new JsonObjectRequest(Request.Method.GET,
                Config.configServer, null,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(final JSONObject response) {

                        Log.v(TAG, "Config Server responeded with : " + response.toString());

                        Iterator<?> keys = response.keys();
                        try {
                            while (keys.hasNext()) {
                                String key = (String) keys.next();
                                String value = response.getString(key);
                                Config.setParam(key, value);
                            }
                        }catch (JSONException e){
                            Log.e(TAG, "Error parsing Server Config response : ", e);
                        }

                        analyticsManager.reConfigure();
                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        Log.e(TAG, "Error fetching configs from server : ", error);
                    }
                }) {
            // we need to overwrite the default content type.
            @Override
            public String getBodyContentType() {
                return "application/json";
            }
        };
        requestQueue.addToRequestQueue(jsonRequest);
    }
}
