package chymeravr.com.sdkclient;


import com.android.volley.Request;
import com.android.volley.toolbox.ImageRequest;
import com.android.volley.toolbox.JsonObjectRequest;

import android.content.Context;
import android.graphics.Bitmap;
import android.util.Log;

import org.json.JSONException;
import org.json.JSONObject;


import static android.graphics.Bitmap.Config.ARGB_8888;

/**
 * Created by robin_chimera on 11/28/2016.
 */

/*
    This class manages the Image360 ad display for the SDK
    It is responsible for managing the ad depending on lifecycle
    events. It listens for lifecycle events and renders ads at an
    appropriate time.
    Its core responsibilities are
        - maintaining load states
        - store an ad
        - call display methods for ad
        - request a new ad
 */
public final class Image360Ad extends Ad{
    private static final String TAG = "ChymeraVRImage360";
    /* below are some private variables used for internal representation of ad data and
    *  server requests */
    private byte[] byteArray;
    private Bitmap imageBitmap;
    private ServerListener<JSONObject> jsonResponse;
    private ServerListener<Bitmap> imageResponse;

    Bitmap getImageBitmap() {
        return this.imageBitmap;
    }

    byte[] getByteArray() {
        return this.byteArray;
    }

    public Image360Ad(Context context){
        assert(context != null);
        this.setContext(context);
        this.jsonResponse = new Image360AdServerListener(this);
        this.imageResponse = new Image360MediaServerListener(this);
    }

    /* Request AdServer for a new image360 ad.
    * It passes targeting parameters from AdRequest for better Ads*/
    public void loadAd(AdRequest adRequest){
        this.setLoading(true);
        Log.i(TAG, "Requesting Server for a New 360 Image Ad");

        // TODO: build a meaningful adServerURL or howsoever we want to encode the request
        String adServerURL = Config.adServer;

        // Request a json response from the provided URL.
        JsonObjectRequest jsonObjRequest = new JsonObjectRequest(Request.Method.GET, adServerURL, null,
                this.jsonResponse, this.jsonResponse);
        this.jsonResponse.getRequestQueue().add(jsonObjRequest);
    }

    @Override
    void onAdServerResponseSuccess(JSONObject response){
        try{
            Image360Ad.this.setMediaUrl(response.getString("url"));
            ImageRequest imageRequest = new ImageRequest(this.getMediaUrl(), this.imageResponse, 0, 0,
                    null, ARGB_8888, this.imageResponse);
            this.imageResponse.getRequestQueue().add(imageRequest);

            Log.i(TAG, "Ad Server response succesfully processed. Proceeding to query Media Server");
        }catch(JSONException e){
            this.setLoading(false);
            Image360Ad.this.getAdListener().onAdFailedToLoad();
            e.printStackTrace();
            Log.e(TAG, "An exception occurred while processing the JSON Response from the ad server");
        }
    }

    @Override
    <T> void onMediaServerResponseSuccess(T media) {
        this.imageBitmap = (Bitmap) media;
        this.byteArray = Util.convertToByteArray(this.imageBitmap);
        this.getAdListener().onAdLoaded();
        this.setLoaded(true);
        this.setLoading(false);
        Log.i(TAG, "Media Server Response Successful");
    }


    /* Display ad by calling the native graphics library (or 3rd party API if that is the case)*/
    public void show(){

    }

}