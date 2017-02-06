package com.chymeravr.common;

import android.util.Log;

import com.android.volley.Request;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.Iterator;

import lombok.Setter;

/**
 * Created by robin_chimera on 11/30/2016.
 * Contains Configuration Options to be used by projects : adclient & analytics
 */

// TODO: 2/3/2017 Get Configuration Parameters from Server
public class Config {
    public static final String TAG = "Config";

    public static final int SdkVersion = 1;

    public static final String Image360AdAssetDirectory = "chymeraSDKAssets/image360/";

    public static final String configServer = "http://amock.io/api/v1/robin/sdkConfigOptions";

    @Setter
    public static String adServer = "http://www.amock.io/api/v1/robin/adServer/";
    @Setter
    public static String analyticsServer = "http://www.amock.io/api/v1/robin/analyticsServer/";
    @Setter
    public static int androidVersionNo = 23;

    @Setter
    public static int highPriorityQueueSize = 1;
    @Setter
    public static int medPriorityQueueSize = 5;
    @Setter
    public static int lowPriorityQueueSize = 10;

    // no of threads to allocate for analytics manager
    @Setter
    public static int analyticsManagerThreadPoolSize = 1;

    // period between calls for HMD parameters, initial delay for sampling
    @Setter
    public static long hmdSamplingPeriod = 1;

    @Setter
    public static long hmdSamplingDelay = 1;

    private static void setParam(String key, String value){
        switch(key) {
            case "adServer": setAdServer(value);
                break;
            case "analyticsServer": setAnalyticsServer(value);
                break;
            case "androidVersionNo": setAndroidVersionNo(Integer.parseInt(value));
                break;
            case "highPriorityQueue": setHighPriorityQueueSize(Integer.parseInt(value));
                break;
            case "medPriorityQueue": setMedPriorityQueueSize(Integer.parseInt(value));
                break;
            case "lowPriorityQueue": setLowPriorityQueueSize(Integer.parseInt(value));
                break;
            case "analyticsManagerThreadPoolSize": setAnalyticsManagerThreadPoolSize(Integer.parseInt(value));
                break;
            case "hmdSamplingPeriod": setHmdSamplingPeriod(Long.parseLong(value));
                break;
            case "hmdSamplingDelay": setHmdSamplingDelay(Long.parseLong(value));
                break;
            default:
                break;
        }
    }
    public static void fetchSdkConfig(WebRequestQueue requestQueue){
        JsonObjectRequest jsonRequest = new JsonObjectRequest(Request.Method.GET,
                Config.configServer, null,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        Log.v(TAG, "Config Server responeded with : " + response.toString());

                        Iterator<?> keys = response.keys();
                        try {
                            while (keys.hasNext()) {
                                String key = (String) keys.next();
                                String value = response.getString(key);
                                setParam(key, value);
                            }
                        }catch (JSONException e){
                            Log.e(TAG, "Error parsing Server Config response : ", e);
                        }
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
