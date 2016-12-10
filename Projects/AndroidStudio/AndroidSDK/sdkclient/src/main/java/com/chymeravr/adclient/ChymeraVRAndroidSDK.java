package com.chymeravr.adclient;

import android.content.Context;
import android.os.Build.VERSION;
import android.util.Log;

//import com.google.android.gms.ads.identifier.AdvertisingIdClient;
//import com.google.android.gms.common.GooglePlayServicesNotAvailableException;
//import com.google.android.gms.common.GooglePlayServicesRepairableException;
//import com.google.android.gms.common.api.GoogleApiClient;
//import com.google.android.gms.drive.Drive;

import java.util.Map;

/**
 * Created by robin_chimera on 11/28/2016.
 */

/**
 * Sets up the SDK for sending and receiving Ad Requests & Response.
 * Mandatory permissions - INTERNET, ACCESS_NETWORK_STATE
 * Recommended permissions - ACCESS_COARSE_LOCATION, ACCESS_FINE_LOCATION,
 *                          ACCESS_WIFI_STATE, CHANGE_WIFI_STATE, VIBRATE,
 *                          WRITE_EXTERNAL_STORAGE
 * Verifies that the publisher provides adequate permissions for the
 * SDK.
 * Verify correct version of Android
 * Verify that the publisher has configured his Application IDs correctly
 * Implemented as a singleton class
 */
public final class ChymeraVRAndroidSDK {
    private static Context context;
    private static String applicationCode;
    private static final String TAG = "chymeravr.com.sdkclient";
    private static String advertisingId;

    public static String getApplicationCode() {
        return applicationCode;
    }

    public static String getAdvertisingID(){
        return advertisingId;
    }

    public static Context getContext(){
        return context;
    }

    /* do we want a pre initialized singleton here? What about a static class? */
    private static ChymeraVRAndroidSDK ourInstance = new ChymeraVRAndroidSDK();

    public static ChymeraVRAndroidSDK getInstance() {
        return ourInstance;
    }

    private ChymeraVRAndroidSDK() {}

    /* Correctly set up Context, verify appCode, check for required permissions*/

    public static void initialize(Context newContext, String newApplicationCode){
        /** TODO: figure out what to do with the app context
        **/

        /* newContext cannot be null - there is no recovering from bad input on this one */
        assert(newContext != null);

        context = newContext;

        //fetchAdvertisingId();

        /* TODO: Verify application code format & validity*/
        applicationCode = newApplicationCode;


        /* Check whether client has granted the mandatory permissions for this SDK to function
            We need internet and network state
        */
        if(!Util.checkMandatoryPermissions(context)){
            Log.e(TAG,
                  "Internet or Network State Access Permision has not been granted." +
                          " The SDK failed to initialize.");
            return;
        }

        Map<String, Boolean> recommendedPermissionsMap = Util.checkRecommendedPermissions(context);
        for (Map.Entry<String, Boolean> entry : recommendedPermissionsMap.entrySet()){
            if(!entry.getValue()){
                Log.w(TAG, entry.getKey() + " unavailable. Enable this for better Ad Targeting");
            }
        }

        if(VERSION.SDK_INT < Config.androidVersionNo) {
            Log.e(TAG, "ChymeraVR SDK is only available with Android Version " + Config.androidVersionNo + " or higher. " +
                    "SDK initialization failed!");
            return;
        } else{
            Log.i(TAG, "Version Check Succeeded! " + VERSION.SDK_INT);
        }

    }

    /* mute the ad */
    public static void setAppMuted(boolean muted){

    }

    /* set the app volume */
    public static void setAppVolume(float volume){

    }


//    private static void fetchAdvertisingId() {
//        AsyncTask<Void, Void, String> task = new AsyncTask<Object, Object, Void>() {
//            @Override
//            protected Void doInBackground(Object... params){}
//            @Override
//            protected String doInBackground(Void... params) {
//                AdvertisingIdClient.Info idInfo = null;
//                try {
//                    idInfo = AdvertisingIdClient.getAdvertisingIdInfo(getContext());
//                } catch (GooglePlayServicesNotAvailableException e) {
//                    e.printStackTrace();
//                } catch (GooglePlayServicesRepairableException e) {
//                    e.printStackTrace();
//                } catch (Exception e) {
//                    e.printStackTrace();
//                }
//                String advertId = null;
//                try {
//                    advertId = idInfo.getId();
//                } catch (Exception e) {
//                    e.printStackTrace();
//                }
//
//                return advertId;
//            }
//            @Override
//            protected void onPostExecute(String advertId) {
//                advertisingId = advertId;
//            }
//        };
//
//        task.execute();
//    }

}
