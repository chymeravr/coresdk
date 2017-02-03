package com.chymeravr.analytics;

import android.util.Log;

import com.android.volley.Request;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.chymeravr.common.Config;
import com.chymeravr.common.WebRequestQueue;
import com.google.gson.Gson;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

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

        JSONArray jsonArray = new JSONArray();
        for (Event event : this.eventArray) {
            Gson gson = new Gson();
            String eventJson = gson.toJson(event);
            jsonArray.put(eventJson);
        }

        JSONObject jsonEventObjects = new JSONObject();
        try {
            jsonEventObjects.put("eventList", jsonArray.toString());
        } catch (JSONException e) {
            Log.e(TAG, "Error converting event list to Json Object", e);
        }finally {
            JsonObjectRequest jsonRequest = new JsonObjectRequest(Request.Method.POST,
                    Config.analyticsServer, jsonEventObjects,
                    new Response.Listener<JSONObject>() {

                        @Override
                        public void onResponse(JSONObject response) {
                                Log.v(TAG, "Event List Sent. Server Response " + response.toString());
                        }
                    },
                    new Response.ErrorListener() {
                        @Override
                        public void onErrorResponse(VolleyError error) {
                            Log.e(TAG, "Error posting event list to server : ", error);
                        }
                    }) {
                // we need to overwrite the default content type.
                @Override
                public String getBodyContentType() {
                    return "application/json";
                }
            };
            requestQueue.addToRequestQueue(jsonRequest);

            // reset the current Size
            this.currentSize = 0;
        }
    }
}
