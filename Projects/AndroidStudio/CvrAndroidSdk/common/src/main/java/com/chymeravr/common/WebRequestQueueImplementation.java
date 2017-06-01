package com.chymeravr.common;

/*
 * Created by robin_chimera on 12/1/2016.
 * Google's android volley library makes it easier to make network requests.
 * Instead of just using a singleton ~ i have made a factory that hides the
 * singleton ness of this class (this implementation was given on volley tutorial
 * site) and used an interface to make testing easier for other clients.
 */

//public class WebRequestQueueImplementation implements WebRequestQueue{
//    @NonNull
//    private RequestQueue mRequestQueue;
//
//    public WebRequestQueueImplementation(@NonNull Context context) {
//        this.mRequestQueue = Volley.newRequestQueue(context.getApplicationContext());
//    }
//
//    public <T> void addToRequestQueue(Request<T> req) {
//        mRequestQueue.add(req);
//    }
//
//}

import android.content.Context;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.toolbox.Volley;

import lombok.NonNull;

public class WebRequestQueueImplementation implements WebRequestQueue{
    private static WebRequestQueue mInstance;
    private RequestQueue mRequestQueue;

    private WebRequestQueueImplementation(@NonNull Context context) {
        this.mRequestQueue = getRequestQueue(context);
    }

    public static synchronized WebRequestQueue setInstance(Context context) {
        if (mInstance == null) {
            mInstance = new WebRequestQueueImplementation(context);
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

    @Override
    public <T> void addToRequestQueue(Request<T> req) {
        if (mRequestQueue == null){
            throw new NullPointerException("WebRequestQueue has not been initialized");
        }
        mRequestQueue.add(req);
    }

}