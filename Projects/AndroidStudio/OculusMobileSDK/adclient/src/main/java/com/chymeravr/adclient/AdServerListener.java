package com.chymeravr.adclient;

import android.util.Log;

import com.android.volley.RequestQueue;
import com.android.volley.VolleyError;
import com.chymeravr.analytics.Event;

import org.json.JSONObject;

import java.sql.Timestamp;
import java.util.HashMap;

import lombok.NonNull;

/**
 * Created by robin_chimera on 12/6/2016.
 */

class AdServerListener extends ServerListener<JSONObject> {
    private final String TAG = "AdServListener";

    public AdServerListener(@NonNull Ad ad, RequestQueue requestQueue) {
        super(ad);
        this.setRequestQueue(requestQueue);
    }

    @Override
    public void onErrorResponse(VolleyError error) {
        this.getAd().setLoading(false);
        this.getAd().getAdListener().onAdFailedToLoad();

        // send error logs to server
        HashMap<String, Object> errorMap = new HashMap<String, Object>();
        errorMap.put("Error", error.toString());
        this.getAd().getAnalyticsManager()
                .push(new Event((new Timestamp(System.currentTimeMillis())).getTime(),
                Event.EventType.ERROR, Event.Priority.LOW, errorMap));
        Log.e(TAG, "Error", error);
    }

    @Override
    public void onResponse(JSONObject response) {
        this.getAd().onAdServerResponseSuccess(response);
        Log.i(TAG, "Ad Server Response Success!");
    }
}
