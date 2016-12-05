package chymeravr.com.sdkclient;


import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
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
public final class Image360Ad extends Ad {
    private static final String TAG = "ChymeraVRImage360";
    //private String imageURL;
    /* below are some private variables used for internal representation of ad data and
    *  server requests */
    private byte[] byteArray;
    private Bitmap imageBitmap;
    private RequestQueue queue;
    private AdServerResponseListener jsonResponse = new AdServerResponseListener();
    private MediaServerResponseListener imageResponse = new MediaServerResponseListener();

//    public void setImageURL(String imageURL) {
//        this.imageURL = imageURL;
//    }
//
//    public String getImageURL() {
//        return imageURL;
//    }

    public Image360Ad(Context context){
        this.setContext(context);
        this.queue = VolleyRequestQueue.getInstance(this.getContext()).getRequestQueue();
    }

    /* Request AdServer for a new image360 ad.
    * It passes targeting parameters from AdRequest for better Ads*/
    public void loadAd(AdRequest adRequest){

        Log.i(TAG, "Requesting Server for a New 360 Image Ad");

        // TODO: build a meaningful adServerURL or howsoever we want to encode the request
        String adServerURL = Config.adServer;

        // Request a string response from the provided URL.
        JsonObjectRequest jsonObjRequest = new JsonObjectRequest(Request.Method.GET,
                adServerURL, null, jsonResponse, jsonResponse);
        this.queue.add(jsonObjRequest);

    }

    void onAdServerResponseSuccess(){
        ImageRequest imageRequest = new ImageRequest(this.getMediaUrl(), this.imageResponse, 0, 0,
                null, ARGB_8888, this.imageResponse);
        this.queue.add(imageRequest);
    }

    @Override
    <T> void onMediaServerResponseSuccess(T media) {
        this.imageBitmap = (Bitmap) media;
        this.byteArray = Util.convertToByteArray(this.imageBitmap);
    }


    /* Display ad by calling the native graphics library (or 3rd party API if that is the case)*/
    public void show(){

    }

    /* A nested class to implement Response Listeners for image response from ad server*/
    private class MediaServerResponseListener implements
            Response.ErrorListener,
            Response.Listener<Bitmap>{

        @Override
        public void onErrorResponse(VolleyError error) {
            Image360Ad.this.getAdListener().onAdFailedToLoad();
        }

        @Override
        public void onResponse(Bitmap bitmap) {
            Image360Ad.this.onMediaServerResponseSuccess(bitmap);
            Image360Ad.this.getAdListener().onAdLoaded();
        }
    }

    /* A nested class to implement Response Listeners for json response from media server*/
    private class AdServerResponseListener implements
            Response.ErrorListener,
            Response.Listener<JSONObject>{

        @Override
        public void onErrorResponse(VolleyError error) {
            Image360Ad.this.getAdListener().onAdFailedToLoad();
        }

        @Override
        public void onResponse(JSONObject response) {
            try{
                Image360Ad.this.setMediaUrl(response.getString("url"));
                Image360Ad.this.onAdServerResponseSuccess();
            }catch(JSONException e){
                e.printStackTrace();
                Log.e(TAG, "An exception occurred while processing the JSON Response from the ad server");
            }
        }
    }
}