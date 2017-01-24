package com.chymeravr.adclient;

/**
 * Created by robin_chimera on 12/1/2016.
 */

import android.content.Context;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.toolbox.Volley;

import lombok.NonNull;

/*
    A singleton class to hold the request queue for Android's Volley Request Queue
    This results in reuse of the request queue for requests - efficiency gains
 */
public final class WebRequestQueue {
    private static WebRequestQueue mInstance;
    private RequestQueue mRequestQueue;


    private static Context mCtx;

    private WebRequestQueue(@NonNull Context context) {
        mCtx = context;
        mRequestQueue = getRequestQueue();
    }

    public static synchronized WebRequestQueue getInstance(Context context) {
        if (mInstance == null) {
            mInstance = new WebRequestQueue(context);
        }
        return mInstance;
    }

    public RequestQueue getRequestQueue() {
        if (mRequestQueue == null) {
            // getApplicationContext() is key, it keeps you from leaking the
            // Activity or BroadcastReceiver if someone passes one in.
            mRequestQueue = Volley.newRequestQueue(mCtx.getApplicationContext());
        }
        return mRequestQueue;
    }

    public <T> void addToRequestQueue(Request<T> req) {
        getRequestQueue().add(req);
    }


}