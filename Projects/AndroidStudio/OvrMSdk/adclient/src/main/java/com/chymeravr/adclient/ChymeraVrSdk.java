package com.chymeravr.adclient;

import android.content.Context;
import android.content.res.AssetManager;
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

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Iterator;

import lombok.AccessLevel;
import lombok.Getter;
import lombok.NonNull;
import lombok.Setter;

import static com.chymeravr.common.Util.createDir;

/**
 * Created by robin_chimera on 11/28/2016.

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
    private static String applicationId;

    @Getter(AccessLevel.PACKAGE)
    @Setter(AccessLevel.PACKAGE)
    private static String advertisingId;

    @Getter
    private AnalyticsManager analyticsManager;

    @Getter
    private WebRequestQueue webRequestQueue;

    @Getter
    private static final ChymeraVrSdk sdkInstance = new ChymeraVrSdk();

    private ChymeraVrSdk() {
    }

    /* Correctly set up Context, verify appCode, check for required permissions*/
    public static void initialize(final Context context, final String applicationId) {

        setApplicationId(applicationId);

        copyAssets(context);
        /* Check whether client has granted the mandatory permissions for this SDK to function
            We need internet and network state
        */
        if (!Util.checkMandatoryPermissions(context)) {
            Log.e(TAG,
                    "One or more of the Mandatory Permisions required for SDK has not been granted." +
                            " The SDK failed to initialize.");
            return;
        }


        if (VERSION.SDK_INT < Config.androidVersionNo) {
            Log.e(TAG, "ChymeraVR Ad Client SDK is only available with Android Version "
                    + Config.androidVersionNo + " or higher. " + "SDK initialization failed!");
            return;
        } else {
            Log.i(TAG, "Version Check Succeeded! " + VERSION.SDK_INT);
        }

        // initialize volley asynchronous request queue
        sdkInstance.webRequestQueue = WebRequestQueue.setInstance(context);

        // Get configuration options from server
        fetchSdkConfig(sdkInstance.webRequestQueue);

        sdkInstance.analyticsManager = AnalyticsManager.getInstance(sdkInstance.webRequestQueue, applicationId);
        AnalyticsManager.initialize();

        Log.i(TAG, "SDK successfully initialized");

    }

    public static void shutdown(){
        // Shutdown must be called after intialize to release any allocated resources
        Log.i(TAG, "Shutting down ChymerVrSdk. Goodbye!");
        AnalyticsManager.shutdown();
    }

    private static void fetchSdkConfig(WebRequestQueue requestQueue){
        JsonObjectRequest jsonRequest = new JsonObjectRequest(Request.Method.GET,
                Config.configServer, null,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(final JSONObject response) {

                        Log.d(TAG, "Config Server responded with : " + response.toString());

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

                        AnalyticsManager.reConfigure();
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

    private static void copyAssets(Context context) {
        AssetManager assetManager = context.getAssets();
        String[] files = null;
        try {
            files = assetManager.list("fonts");
        } catch (IOException e) {
            Log.e("tag", "Failed to get asset file list.", e);
        }
        for(String filename : files) {
            InputStream in = null;
            OutputStream out = null;
            try {
                in = assetManager.open("fonts/" + filename);

                File appPath = context.getFilesDir();
                String appSdkPath = appPath + Util.addLeadingSlash(Config.getFontPath());
                File dest_dir = new File(appSdkPath);
                createDir(dest_dir);

                File outFile = new File(dest_dir, filename);
                Log.d(TAG, "Absolute font file path : " + outFile.getAbsolutePath());
                out = new FileOutputStream(outFile);
                copyFile(in, out);

                in.close();

                out.flush();
                out.close();
            } catch(IOException e) {
                Log.e("tag", "Failed to copy asset file: " + filename, e);
            }
        }
        Log.v(TAG, "Copied Fonts Successfully");
    }
    private static void copyFile(InputStream in, OutputStream out) throws IOException {
        byte[] buffer = new byte[1024];
        int read;
        while((read = in.read(buffer)) != -1){
            out.write(buffer, 0, read);
        }
    }
}
