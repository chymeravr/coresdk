package com.chymeravr.adclient;

import android.content.Context;
import android.os.Build.VERSION;
import android.util.Log;

import com.chymeravr.common.Config;
import com.chymeravr.common.Util;

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

public final class ChymeraVRAndroidSDK {

    private static final String TAG = "com.chymeravr.adclient";

    @Getter
    @Setter(AccessLevel.PRIVATE)
    @NonNull
    private static Context context;

    @Getter
    @Setter(AccessLevel.PRIVATE)
    @NonNull
    private static String applicationCode;

    @Getter
    private static String advertisingId;

    @Getter
    private static final ChymeraVRAndroidSDK ourInstance = new ChymeraVRAndroidSDK();

    private ChymeraVRAndroidSDK() {
    }

    /* Correctly set up Context, verify appCode, check for required permissions*/
    public static void initialize(Context newContext, String newApplicationCode) {
        /** TODO: figure out what to do with the app context
         *        Initialize only once
         **/

        setContext(newContext);
        setApplicationCode(newApplicationCode);

        /* TODO: Verify application code format & validity*/

        /* Check whether client has granted the mandatory permissions for this SDK to function
            We need internet and network state
        */
        if (!Util.checkMandatoryPermissions(context)) {
            Log.e(TAG,
                    "Internet or Network State Access Permision has not been granted." +
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

        Log.i(TAG, "SDK successfully initialized");

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

}
