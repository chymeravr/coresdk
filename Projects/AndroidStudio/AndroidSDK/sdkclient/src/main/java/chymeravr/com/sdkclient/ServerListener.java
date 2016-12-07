package chymeravr.com.sdkclient;

import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;

import org.jetbrains.annotations.Contract;
import org.json.JSONObject;

/**
 * Created by robin_chimera on 12/6/2016.
 */

abstract class ServerListener<T> implements Response.ErrorListener, Response.Listener<T> {
    private Ad ad;
    private RequestQueue requestQueue;

    public ServerListener(Ad ad) {
        assert(ad != null);
        this.ad = ad;
    }

    public ServerListener(Ad ad, RequestQueue requestQueue) {
        assert(ad != null && requestQueue != null);
        this.ad = ad;
        this.requestQueue = requestQueue;
    }

    public Ad getAd() {
        return ad;
    }

    public void setAd(Ad ad) {
        assert(ad != null);
        this.ad = ad;
    }

    public RequestQueue getRequestQueue() {
        return requestQueue;
    }

    public void setRequestQueue(RequestQueue requestQueue) {
        assert(requestQueue != null);
        this.requestQueue = requestQueue;
    }

    //public abstract void onErrorResponse(VolleyError error);
}

