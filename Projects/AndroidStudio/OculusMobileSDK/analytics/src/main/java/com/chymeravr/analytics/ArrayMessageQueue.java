package com.chymeravr.analytics;

import android.util.Log;

import com.android.volley.Request;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonArrayRequest;
import com.chymeravr.common.WebRequestQueue;

import org.json.JSONArray;
import org.json.JSONException;

import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

import lombok.Getter;
import lombok.Setter;

/**
 * Created by robin_chimera on 1/31/2017.
 */

public class ArrayMessageQueue implements MessageQueue{

    private static final String TAG = "ChymeraVR::AnalyticsSDK";

    private int size;
    private int currentSize;

    private Message messageArray[];

    private Lock queueLock = new ReentrantLock();

    @Getter
    @Setter
    private WebRequestQueue requestQueue;

    public ArrayMessageQueue(int size) {
        assert (size > 0);
        this.size = size;
        this.currentSize = 0;

        this.messageArray = new Message[this.size];
    }

    // enqueu message to the queue
    public void enqueue(Message msg) {
        this.queueLock.lock();
        if (this.isFull()) {
            this.flush();
        }
        messageArray[currentSize] = msg;
        this.currentSize++;
        this.queueLock.unlock();
    }

    // boolean to check if queue is full or not
    public boolean isFull() {
        return this.size == this.currentSize;
    }

    // flushes the entire queue - send all messages to the analytics server
    public void flush() {
        JSONArray myArray = null;
        try {
            myArray = new JSONArray(this.messageArray);
        } catch (JSONException e) {
            Log.e(TAG, "Analytics Server Unable to flush Message queue : " + e.toString());
        }

        try {
            JSONArray jsonArray = new JSONArray(this.messageArray);
            JsonArrayRequest jsonArrayRequest = new JsonArrayRequest(Request.Method.POST,
                    Config.analyticsServer, jsonArray, new Response.Listener<JSONArray>() {
                @Override
                public void onResponse(JSONArray response) {
                }
            }, new Response.ErrorListener() {
                @Override
                public void onErrorResponse(VolleyError error) {
                    Log.e(TAG, "Analytics Server responeded with error : " + error.toString());
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
            e.printStackTrace();
        }
    }
}
