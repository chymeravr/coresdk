package chymeravr.com.sdkclient;

import android.util.Log;

import com.android.volley.RequestQueue;
import com.android.volley.VolleyError;

import org.json.JSONObject;

/**
 * Created by robin_chimera on 12/6/2016.
 */

class Image360AdServerListener extends ServerListener<JSONObject> {
    private final String TAG = "Image360AdListener";

    public Image360AdServerListener(Ad ad) {
        super(ad);
        this.setRequestQueue(WebRequestQueue.getInstance(ad.getContext()).getRequestQueue());
    }

    @Override
    public void onErrorResponse(VolleyError error) {
        Log.e(TAG, "Ad Server Response Failure!");
        this.getAd().setLoading(false);
        this.getAd().getAdListener().onAdFailedToLoad();
        error.printStackTrace();
    }

    @Override
    public void onResponse(JSONObject response) {
        this.getAd().onAdServerResponseSuccess(response);
        Log.i(TAG, "Ad Server Response Success!");
    }
}
