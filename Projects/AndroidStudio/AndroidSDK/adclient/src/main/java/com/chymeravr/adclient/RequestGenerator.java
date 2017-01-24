package com.chymeravr.adclient;

import android.graphics.Bitmap;

import com.android.volley.Request;
import com.android.volley.toolbox.ImageRequest;
import com.android.volley.toolbox.JsonObjectRequest;

import org.json.JSONObject;

import java.util.HashMap;

import lombok.AccessLevel;
import lombok.Getter;
import lombok.NonNull;
import lombok.RequiredArgsConstructor;
import lombok.Setter;

import static android.graphics.Bitmap.Config.ARGB_8888;

//import com.google.android.gms.ads.*;

/**
 * Created by robin_chimera on 12/9/2016.
 */

@RequiredArgsConstructor(suppressConstructorProperties = true)
final class RequestGenerator {

    @Getter
    @Setter(AccessLevel.PRIVATE)
    @NonNull
    private final Ad.Type adType;


    public JsonObjectRequest getAdServerJsonRequest(@NonNull AdRequest adRequest,
                                                    @NonNull ServerListener<JSONObject> jsonServerListener) {

        HashMap<String, String> postParams = new HashMap<>();
        postParams.put("gender", adRequest.getGender().getValue());
        postParams.put("dob", adRequest.getBirthday().toString());
        postParams.put("lat", Double.toString(adRequest.getLocation().getLatitude()));
        postParams.put("lon", Double.toString(adRequest.getLocation().getLongitude()));
        postParams.put("adType", adType.name());

        JSONObject jsonObjectParams = new JSONObject(postParams);
        JsonObjectRequest requestResult = new JsonObjectRequest(Request.Method.POST, Config.adServer,
                jsonObjectParams, jsonServerListener, jsonServerListener) {
            // we need to overwrite the default content type.
            @Override
            public String getBodyContentType() {
                return "application/json";
            }
        };

        return requestResult;
    }

    public ImageRequest getMediaServerRequest(@NonNull String mediaUrl,
                                              @NonNull ServerListener<Bitmap> mediaServerListener) {
        return new ImageRequest(mediaUrl, mediaServerListener, 0, 0,
                null, ARGB_8888, mediaServerListener);
    }

    public InputStreamVolleyRequest getMediaDownloadRequest(@NonNull String mediaUrl,
                                                            @NonNull ServerListener<byte[]> mediaDownloadServerListener)
    {
        return new InputStreamVolleyRequest(Request.Method.GET, mediaUrl, mediaDownloadServerListener, mediaDownloadServerListener, null);
    }
}
