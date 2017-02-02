package com.chymeravr.analytics;

import android.util.Log;

import com.android.volley.Request;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonArrayRequest;
import com.chymeravr.common.Config;
import com.chymeravr.common.WebRequestQueue;

import org.json.JSONArray;
import org.json.JSONException;

import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

import lombok.Getter;
import lombok.NonNull;

/**
 * Created by robin_chimera on 1/31/2017.
 */

public class ArrayEventQueue implements EventQueue {

    private static final String TAG = "ChymeraVR::AnalyticsSDK";

    @NonNull
    private final int size;

    @Getter
    private int currentSize;

    private Event eventArray[];

    private Lock queueLock = new ReentrantLock();

    private WebRequestQueue requestQueue;

    public ArrayEventQueue(int size, WebRequestQueue requestQueue) {
        assert (size > 0);
        this.size = size;
        this.currentSize = 0;

        this.requestQueue = requestQueue;

        this.eventArray = new Event[this.size];
    }

    // enqueu message to the queue
    public void enqueue(Event msg) {
        this.queueLock.lock();
        if (this.isFull()) {
            this.flush();
        }
        eventArray[currentSize] = msg;
        this.currentSize++;
        this.queueLock.unlock();
    }

    // boolean to check if queue is full or not
    public boolean isFull() {
        this.queueLock.lock();
        boolean result = (this.size == this.currentSize);
        this.queueLock.unlock();
        return result;
    }

    // flushes the entire queue - send all messages to the analytics server
    public void flush() {
        Log.v(TAG, "Attempting to flush the Event Queue");
        try {
            JSONArray jsonArray = new JSONArray(this.eventArray);
            JsonArrayRequest jsonArrayRequest = new JsonArrayRequest(Request.Method.POST,
                    Config.analyticsServer, jsonArray, new Response.Listener<JSONArray>() {
                @Override
                public void onResponse(JSONArray response) {
                }
            }, new Response.ErrorListener() {
                @Override
                public void onErrorResponse(VolleyError error) {
                    Log.e(TAG, "Analytics Server encountered a VolleyError: " + error.toString());
                }
            }) {
                // we need to overwrite the default content type.
                @Override
                public String getBodyContentType() {
                    return "application/json";
                }
            };

            requestQueue.addToRequestQueue(jsonArrayRequest);

            // reset the current Size
            this.currentSize = 0;
        } catch (JSONException e) {
            Log.e(TAG, "Analytics Server encountered a JSONException: " + e.toString());
        }
    }
}
