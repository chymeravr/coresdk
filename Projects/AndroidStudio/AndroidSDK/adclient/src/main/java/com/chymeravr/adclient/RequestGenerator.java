package com.chymeravr.adclient;

import android.graphics.Bitmap;
import android.widget.Toast;

import com.android.volley.Request;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.ImageRequest;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.StringRequest;
//import com.google.android.gms.ads.*;

import org.json.JSONObject;

import java.util.HashMap;
import java.util.Map;

import static android.graphics.Bitmap.Config.ARGB_8888;

/**
 * Created by robin_chimera on 12/9/2016.
 */

final class RequestGenerator {
    private Ad.Type adType;
    //private HashMap<String, String> postParams;

    public RequestGenerator(Ad.Type adType) {
        this.adType = adType;
    }

    public JsonObjectRequest getAdServerJsonRequest(final AdRequest adRequest,
                                                    final ServerListener<JSONObject> jsonServerListener) {
        assert (adRequest != null && adType != null);

        HashMap<String, String> postParams = new HashMap<String, String>();
        //postParams.put("Content-Type", "application/json; charset=utf-8");
        postParams.put("gender", adRequest.getGender().getValue());
        postParams.put("dob", adRequest.getBirthday().toString());
        postParams.put("lat", Double.toString(adRequest.getLocation().getLatitude()));
        postParams.put("lon", Double.toString(adRequest.getLocation().getLongitude()));
        postParams.put("adType", adType.name());

        JSONObject jsonObjectParams = new JSONObject(postParams);
        JsonObjectRequest res = new JsonObjectRequest(
                Request.Method.POST, Config.adServer, jsonObjectParams,
                jsonServerListener, jsonServerListener){
            // we need to overwrite the default content type.
            @Override
            public String getBodyContentType(){
                return "application/json";
            }
        };

        return res;
    }

    public ImageRequest getMediaServerRequest(String mediaUrl, ServerListener<Bitmap> mediaServerListener) {
        return new ImageRequest(mediaUrl, mediaServerListener, 0, 0,
                null, ARGB_8888, mediaServerListener);
    }
}
