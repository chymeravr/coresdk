package com.chymeravr.analytics;

import android.util.Log;

import com.android.volley.Request;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.chymeravr.common.Config;
import com.chymeravr.common.WebRequestQueue;
import com.chymeravr.schemas.eventreceiver.EventPing;
import com.chymeravr.schemas.eventreceiver.SDKEvent;
import com.google.gson.Gson;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.sql.Timestamp;
import java.util.Arrays;
import java.util.List;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

import lombok.Getter;

/**
 * Created by robin_chimera on 1/31/2017.
 */

public class ArrayEventQueue implements EventQueue {

    private static final String TAG = "AnalyticsSDK";

    @Getter
    private int size;

    @Getter
    private int currentSize;

    private SDKEvent eventArray[];

    private Lock queueLock = new ReentrantLock();

    private WebRequestQueue requestQueue;

    public ArrayEventQueue(int size, WebRequestQueue requestQueue) {
        assert (size > 0);
        this.size = size;
        this.currentSize = 0;

        this.requestQueue = requestQueue;

        this.eventArray = new SDKEvent[this.size];
    }

    public void reSize(int size){
        this.queueLock.lock();
        if(size != this.getSize()) {
            this.flush();
            this.eventArray = new SDKEvent[size];
            this.size = size;
        }
        this.queueLock.unlock();
    }

    // enqueu message to the queue
    public void enqueue(SDKEvent msg) {
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
        Log.d(TAG, "Flushing the Event Queue");

        JSONArray jsonArray = new JSONArray();
        for (SDKEvent event : this.eventArray) {
            Gson gson = new Gson();
            String eventJson = gson.toJson(event);
            jsonArray.put(eventJson);
        }
        List<SDKEvent> test = Arrays.asList(eventArray);
        EventPing ping = new EventPing( new Timestamp(System.currentTimeMillis()).getTime(),
                (short)Config.SdkVersion, AnalyticsManager.getApplicationId(), test);

        String eventRequestJson = new Gson().toJson(ping);

        JSONObject jsonEventObjects = null;
        try {
            jsonEventObjects = new JSONObject(eventRequestJson);
        } catch (JSONException e) {
            Log.e(TAG, "Error converting event list to Json Object", e);
        }finally {
            JsonObjectRequest jsonRequest = new JsonObjectRequest(Request.Method.POST,
                    Config.analyticsServer, jsonEventObjects,
                    new Response.Listener<JSONObject>() {

                        @Override
                        public void onResponse(JSONObject response) {
                                Log.d(TAG, "Event List Sent. Server Response " + response.toString());
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
        }

        // all events sent to server - reset the current Size
        this.currentSize = 0;
    }
}
