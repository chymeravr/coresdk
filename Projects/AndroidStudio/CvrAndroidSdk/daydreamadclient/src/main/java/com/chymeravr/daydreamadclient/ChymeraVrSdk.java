package com.chymeravr.daydreamadclient;

import android.content.Context;
import android.content.res.AssetManager;
import android.os.Build;
import android.util.Log;

import com.android.volley.Request;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.chymeravr.analytics.AnalyticsManager;
import com.chymeravr.analytics.AnalyticsManagerFactory;
import com.chymeravr.common.Config;
import com.chymeravr.common.Util;
import com.chymeravr.common.WebRequestQueue;
import com.chymeravr.common.WebRequestQueueFactory;

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
 * Created by robin_chimera on 3/22/2017.
 * this class marshalls critical resources - analytics and networking for
 * our adclient to function correctly.
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

    @Getter(AccessLevel.MODULE)
    private static AnalyticsManager analyticsManager;

    @Getter(AccessLevel.MODULE)
    private static WebRequestQueue webRequestQueue;

    private static boolean isInitialized = false;

    //@Getter
    private static final ChymeraVrSdk sdkInstance = new ChymeraVrSdk();

    private ChymeraVrSdk() {
    }

    /* Correctly set up Context, verify appCode, check for required permissions*/
    public static void initialize(final Context context, final String applicationId) {
        if(isInitialized){
            Log.i(TAG, "SDK already initialized. Ignore request.");
            return;
        }

        setApplicationId(applicationId);


        copyAssets(context, Config.getFontDir());
        if(!BuildConfig.NETWORK_ENABLED) {
            if(!copyAssets(context, Config.getImage360Dir())){
                Log.e(TAG, "Failed to copy SDK Assets. Initialize Exit");
                return;
            }
        }
        /* Check whether client has granted the mandatory permissions for this SDK to function
            We need internet and network state
        */
        if (!Util.checkMandatoryPermissions(context)) {
            Log.e(TAG,
                    "One or more of the Mandatory Permisions required for SDK has not been granted." +
                            " The SDK failed to initialize.");
            return;
        }


        if (Build.VERSION.SDK_INT < Config.getDaydreamAndroidVersionNo()) {
            Log.e(TAG, "ChymeraVR Ad Client SDK is only available with Android Version "
                    + Config.getDaydreamAndroidVersionNo() + " or higher. " + "SDK initialization failed!");
            return;
        } else {
            Log.i(TAG, "Version Check Succeeded! " + Build.VERSION.SDK_INT);
        }

        // initialize volley asynchronous request queue
        WebRequestQueueFactory webFactory = new WebRequestQueueFactory(context);
        webRequestQueue = webFactory.getDefaultWebRequestQueue();

        // Get configuration options from server
        fetchSdkConfig(webRequestQueue);

        AnalyticsManagerFactory analyticsManagerFactory =
                new AnalyticsManagerFactory(webRequestQueue, applicationId);
        analyticsManager = analyticsManagerFactory.getArrayQAnalyticsManager();

        analyticsManager.initialize();

        Log.i(TAG, "SDK successfully initialized");

    }

    public static void shutdown(){
        // Shutdown must be called after intialize to release any allocated resources
        Log.i(TAG, "Shutting down ChymerVrSdk. Goodbye!");
        final boolean shutdown = analyticsManager.shutdown();
        // maybe return a bool indicating shutdown success or not
    }

    private static void fetchSdkConfig(WebRequestQueue requestQueue){
        JsonObjectRequest jsonRequest = new JsonObjectRequest(Request.Method.GET,
                Config.getConfigServer(), null,
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

    // this should probably be util functions
    private static boolean copyAssets(Context context, String assetDir) {
        AssetManager assetManager = context.getAssets();
        String[] files;

        try {
            files = assetManager.list(assetDir);
        } catch (IOException e) {
            Log.e(TAG, "Failed to get asset file list.", e);
            return false;
        }
        for (String filename : files) {
            InputStream in;
            OutputStream out;
            try {

                File appPath = context.getFilesDir();
                String appSdkPath = appPath + Util.addLeadingSlash(Config.getChymeraFolder())
                        + Util.addLeadingSlash(assetDir);
                File dest_dir = new File(appSdkPath);

                if(!dest_dir.exists()) {
                    createDir(dest_dir);
                }

                File outFile = new File(dest_dir, filename);
                Log.d(TAG, "Absolute font file path : " + outFile.getAbsolutePath());

                if(!outFile.exists()) {
                    in = assetManager.open(assetDir + "/" + filename);
                    out = new FileOutputStream(outFile);
                    copyFile(in, out);

                    in.close();

                    out.flush();
                    out.close();
                }
            } catch (IOException e) {
                Log.e(TAG, "Failed to copy asset file: " + filename, e);
                return false;
            }
        }

        Log.v(TAG, "Copied Fonts Successfully");
        return true;
    }

    private static void copyFile(InputStream in, OutputStream out) throws IOException {
        byte[] buffer = new byte[1024];
        int read;
        while((read = in.read(buffer)) != -1){
            out.write(buffer, 0, read);
        }
    }
}
