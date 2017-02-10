package com.chymeravr.adclient;

import android.app.ActivityManager;
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Build;
import android.util.Log;

import com.android.volley.Request;
import com.android.volley.toolbox.JsonObjectRequest;
import com.chymeravr.analytics.Event;
import com.chymeravr.common.Config;
import com.chymeravr.common.Util;
import com.chymeravr.schemas.serving.Demographics;
import com.chymeravr.schemas.serving.Device;
import com.chymeravr.schemas.serving.Location;
import com.chymeravr.schemas.serving.Placement;
import com.chymeravr.schemas.serving.ServingRequest;
import com.google.gson.Gson;

import org.json.JSONException;
import org.json.JSONObject;

import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.List;

import lombok.NonNull;

import static android.content.Context.ACTIVITY_SERVICE;


/**
 * Created by robin_chimera on 12/9/2016.
 */

final class RequestGenerator {
    private static final String TAG = "RequestGenerator";

    private Ad ad;

    private AdServerListener adServerListener;

    private Image360MediaServerListener mediaServerListener;

    public RequestGenerator(Ad ad){
        this.ad = ad;

        this.adServerListener = new AdServerListener(this.ad);
        this.mediaServerListener = new Image360MediaServerListener(this.ad);
    }

    public JsonObjectRequest getAdServerJsonRequest(@NonNull AdRequest adRequest) {

        long timestamp = new Timestamp(System.currentTimeMillis()).getTime();

        int sdkVersion = Config.SdkVersion;

        String appId = ChymeraVrSdk.getApplicationId();

        List<Placement> placements = new ArrayList<Placement>();
        placements.add(new Placement(this.ad.getPlacementId(), this.ad.getAdFormat()));

        Location loc = new Location(adRequest.getLocation().getLatitude(),
                adRequest.getLocation().getLongitude(),
                adRequest.getLocation().getAccuracy());

        Demographics demo = new Demographics(adRequest.getBirthday().toString(),
                adRequest.getGender().getValue(),
                adRequest.getEmail());

        ActivityManager actManager = (ActivityManager) ChymeraVrSdk.getContext().getSystemService(ACTIVITY_SERVICE);
        ActivityManager.MemoryInfo memInfo = new ActivityManager.MemoryInfo();
        actManager.getMemoryInfo(memInfo);
        long totalMemory = memInfo.totalMem;

        Device device = new Device(Build.MANUFACTURER, Build.MODEL, Double.toString(totalMemory));

        String connectivity = null;
        ConnectivityManager cm = (ConnectivityManager) ChymeraVrSdk.getContext().getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo activeNetwork = cm.getActiveNetworkInfo();
        if (activeNetwork != null) { // connected to the internet
            if (activeNetwork.getType() == ConnectivityManager.TYPE_WIFI) {
                connectivity = "Wifi";
            } else if (activeNetwork.getType() == ConnectivityManager.TYPE_MOBILE) {
                connectivity = "Mobile";
            }
        } else {
            Log.e("RequestGenerator", "User currently not connected to the internet");
        }

        WifiManager wifiManager = (WifiManager) ChymeraVrSdk.getContext().getSystemService(Context.WIFI_SERVICE);
        WifiInfo info = wifiManager.getConnectionInfo();
        String ssid = info.getSSID();

        ServingRequest servingRequest =
                new ServingRequest(timestamp, (short) sdkVersion, appId, placements,
                        Config.osId, String.valueOf(Build.VERSION.SDK_INT),
                        ChymeraVrSdk.getAdvertisingId(), Config.hmdId,
                        loc, demo, device, connectivity, ssid);

        String servingRequestJson = new Gson().toJson(servingRequest);
        JSONObject jsonAdRequest = null;
        try {
            jsonAdRequest = new JSONObject(servingRequestJson);
        } catch (JSONException e) {
            Log.e(TAG, "Encountered a JSONException in creating ad request", e);
            this.ad.emitEvent(Event.EventType.ERROR, Event.Priority.HIGH, Util.getErrorMap(e));
        }
        JsonObjectRequest requestResult = new JsonObjectRequest(Request.Method.POST, Config.adServer,
                jsonAdRequest, this.adServerListener, this.adServerListener) {
            // we need to overwrite the default content type.
            @Override
            public String getBodyContentType() {
                return "application/json";
            }
        };

        return requestResult;
    }


    public InputStreamVolleyRequest getMediaDownloadRequest(@NonNull String mediaUrl) {
        return new InputStreamVolleyRequest(Request.Method.GET, mediaUrl,
                this.mediaServerListener,
                this.mediaServerListener, null);
    }
}