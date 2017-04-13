package com.chymeravr.adclient;

import com.android.volley.NetworkResponse;
import com.android.volley.Request;
import com.android.volley.Response;
import com.android.volley.toolbox.HttpHeaderParser;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by robin_chimera on 1/23/2017.
 * Contains definition of a request acceting a bytearray and saving it as a
 * jpeg file to app internal directory
 */

public final class InputStreamVolleyRequest extends Request<byte[]>{

    private final Response.Listener<byte[]> mListener;
    private Map<String, String> mParams;

    // Create a static map for directly accessing headers
    public Map<String, String> responseHeaders;

    public InputStreamVolleyRequest(int method, String mUrl, Response.Listener<byte[]> listener,
                                    Response.ErrorListener errorListener, HashMap<String, String> params)
    {
        super(method, mUrl, errorListener);
        // TODO: 1/23/2017 figure out what to do about caching?
        //setShouldCache(false);
        mListener = listener;
        mParams=params;

    }

    @Override
    protected Map<String, String> getParams()
            throws com.android.volley.AuthFailureError {
        return mParams;
    };

    @Override
    protected Response<byte[]> parseNetworkResponse(NetworkResponse response) {
        //Initialise local responseHeaders map with response headers received
        responseHeaders = response.headers;

        //Pass the response data here
        return Response.success( response.data, HttpHeaderParser.parseCacheHeaders(response));
    }

    @Override
    protected void deliverResponse(byte[] response) {
        mListener.onResponse(response);
    }
}
