package com.chymeravr.adclient;

import android.graphics.Bitmap;
import android.util.Log;

import com.android.volley.RequestQueue;
import com.android.volley.VolleyError;
import com.chymeravr.analytics.Event;

import java.sql.Timestamp;
import java.util.HashMap;

/**
 * Created by robin_chimera on 12/6/2016.
 */

final class Image360MediaServerListener extends ServerListener<Bitmap> {
    private final String TAG = "Image360MediaListener";

    public Image360MediaServerListener(Ad ad, RequestQueue requestQueue) {
        super(ad);
        // Explicitly set the singleton queue;
        this.setRequestQueue(requestQueue);

    }

    @Override
    public void onErrorResponse(VolleyError error) {
        this.getAd().setLoading(false);
        this.getAd().getAdListener().onAdFailedToLoad();
        HashMap<String, Object> errorMap = new HashMap<String, Object>();
        errorMap.put("Error", error.toString());
        this.getAd().getAnalyticsManager().push(new Event((new Timestamp(System.currentTimeMillis())).getTime(),
                Event.EventType.ERROR, Event.Priority.LOW, errorMap));
        Log.e(TAG, "Error ", error);
    }

    @Override
    public void onResponse(Bitmap response) {
        this.getAd().onMediaServerResponseSuccess(response);
        Log.i(TAG, this.getClass() + " Media Server Response Success!");
    }
}
