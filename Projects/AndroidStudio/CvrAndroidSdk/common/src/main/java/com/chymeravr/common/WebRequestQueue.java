package com.chymeravr.common;

import com.android.volley.Request;

/**
 * Created by robin_chimera on 5/27/2017.
 * a simple interface that wraps a method to add request to queue
 * we are forcing an interface for better testing - there will only
 * ever be one implementation of this
 */

public interface WebRequestQueue {
    public <T> void addToRequestQueue(Request<T> req);
}
