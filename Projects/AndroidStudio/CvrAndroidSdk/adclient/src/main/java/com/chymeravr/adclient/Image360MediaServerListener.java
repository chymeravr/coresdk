package com.chymeravr.adclient;

import android.util.Log;

import com.android.volley.NetworkError;
import com.android.volley.NoConnectionError;
import com.android.volley.Response;
import com.android.volley.ServerError;
import com.android.volley.TimeoutError;
import com.android.volley.VolleyError;
import com.chymeravr.analytics.AnalyticsManager;
import com.chymeravr.common.Config;
import com.chymeravr.common.Util;
import com.chymeravr.schemas.eventreceiver.EventType;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.HashMap;

import lombok.NonNull;
import lombok.RequiredArgsConstructor;


/**
 * Created by robin_chimera on 1/23/2017.
 * Defines callbacks for downloading media from server
 */

@RequiredArgsConstructor(suppressConstructorProperties = true)
final class Image360MediaServerListener implements Response.ErrorListener,
        Response.Listener<byte[]> {

    private final String TAG = "Image360MediaListener";

    @NonNull
    private Ad ad;

    @Override
    public void onErrorResponse(VolleyError error) {

        this.ad.setLoading(false);

        VrAdRequest.Error errorCode = null;
        if (error instanceof TimeoutError || error instanceof NoConnectionError || error instanceof ServerError) {
            errorCode = VrAdRequest.Error.ADSERVER_FAILURE;
        } else if (error instanceof NetworkError) {
            errorCode = VrAdRequest.Error.NETWORK_FAILURE;
        } else {
            errorCode = VrAdRequest.Error.UNKNOWN_FAILURE;            // screwed
        }

        this.ad.getVrAdListener().onVrAdLoadFailed(errorCode, error.toString());
        HashMap<String, String> errorMap = new HashMap<>();
        errorMap.put("Error", error.toString());
        this.ad.emitEvent(EventType.ERROR, AnalyticsManager.Priority.LOW, errorMap);
        Log.e(TAG, "Error ", error);
    }

    @Override
    public void onResponse(byte[] response) {
        try {
            if (response!=null) {

                File appPath = this.ad.getContext().getFilesDir();
                String appSdkPath = appPath + Util.addLeadingSlash(Config.Image360AdAssetDirectory);
                File dest_dir = new File(appSdkPath);
                Util.createDir(dest_dir);

                FileOutputStream outputStream;
                String name= this.ad.getPlacementId() + ".jpg";
                Log.d(TAG, "writing file to: " + dest_dir + name);
                outputStream = new FileOutputStream(new File(dest_dir, name));
                outputStream.write(response);
                outputStream.close();

                this.ad.onMediaServerResponseSuccess();
            }
        } catch (IOException e) {
            // this should not really happen
            this.ad.getVrAdListener().onVrAdLoadFailed(VrAdRequest.Error.UNKNOWN_FAILURE, e.toString());

            // send error logs to server

            HashMap<String, String> errorMap = new HashMap<>();
            errorMap.put("Error", e.toString());
            this.ad.emitEvent(EventType.ERROR, AnalyticsManager.Priority.LOW, errorMap);
            Log.e(TAG, "Error processing download file for media ad : " + e.toString());
        }
    }

//    private String addLeadingSlash(String path)
//    {
//        if (path.charAt(0) != '/')
//        {
//            path = "/" + path;
//        }
//        return path;
//    }
//
//    private void createDir(File dir) throws IOException
//    {
//        if (dir.exists())
//        {
//            if (!dir.isDirectory())
//            {
//                throw new IOException("Can't create directory, a file is in the way");
//            }
//        } else
//        {
//            dir.mkdirs();
//            if (!dir.isDirectory())
//            {
//                throw new IOException("Unable to create directory");
//            }
//        }
//    }
}
