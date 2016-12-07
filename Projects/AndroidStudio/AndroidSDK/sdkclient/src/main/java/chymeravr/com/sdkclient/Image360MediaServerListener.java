package chymeravr.com.sdkclient;

import android.graphics.Bitmap;
import android.util.Log;

import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;

/**
 * Created by robin_chimera on 12/6/2016.
 */

final class Image360MediaServerListener extends ServerListener<Bitmap> {
    private final String TAG = "Image360MediaListener";

    public Image360MediaServerListener(Ad ad) {
        super(ad);
        // Explicitly set the singleton queue;
        this.setRequestQueue(WebRequestQueue.getInstance(ad.getContext()).getRequestQueue());

    }

    @Override
    public void onErrorResponse(VolleyError error) {
        error.printStackTrace();
        this.getAd().setLoading(false);
        this.getAd().getAdListener().onAdFailedToLoad();
        Log.e(TAG, this.getClass() + " Media Server Response Failure!");
    }

    @Override
    public void onResponse(Bitmap response){
        this.getAd().onMediaServerResponseSuccess(response);
        Log.i(TAG, this.getClass() + " Media Server Response Success!");
    }
}
