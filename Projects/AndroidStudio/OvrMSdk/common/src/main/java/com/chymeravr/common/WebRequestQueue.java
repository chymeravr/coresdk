package com.chymeravr.common;
/*
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
public class WebRequestQueue {
    private static WebRequestQueue mInstance;
    private RequestQueue mRequestQueue;

//    private Context mCtx;

    private WebRequestQueue(@NonNull Context context) {
//        mCtx = context;
        mRequestQueue = getRequestQueue(context);
    }

    public static synchronized WebRequestQueue setInstance(Context context) {
        if (mInstance == null) {
            mInstance = new WebRequestQueue(context);
        }
        return mInstance;
    }

    public static synchronized WebRequestQueue getInstance() {
        if (mInstance == null) {
            throw new NullPointerException("WebRequestQueue has not been initialized");
        }
        return mInstance;
    }

    private RequestQueue getRequestQueue(Context context) {
        if (mRequestQueue == null) {
            // getApplicationContext() is key, it keeps you from leaking the
            // Activity or BroadcastReceiver if someone passes one in.
            mRequestQueue = Volley.newRequestQueue(context.getApplicationContext());
        }
        return mRequestQueue;
    }


    public <T> void addToRequestQueue(Request<T> req) {
        if (mRequestQueue == null){
            throw new NullPointerException("WebRequestQueue has not been initialized");
        }
        mRequestQueue.add(req);
    }

}