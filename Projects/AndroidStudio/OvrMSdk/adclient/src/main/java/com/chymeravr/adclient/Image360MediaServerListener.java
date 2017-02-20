package com.chymeravr.adclient;

import android.util.Log;

import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.chymeravr.analytics.AnalyticsManager;
import com.chymeravr.common.Config;
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
        this.ad.getAdListener().onAdFailedToLoad();
        HashMap<String, String> errorMap = new HashMap<>();
        errorMap.put("Error", error.toString());
        this.ad.emitEvent(EventType.ERROR, AnalyticsManager.Priority.LOW, errorMap);
        Log.e(TAG, "Error ", error);
    }

    @Override
    public void onResponse(byte[] response) {
        try {
            if (response!=null) {

                File sd_path = this.ad.getContext().getFilesDir();
                String dest_dir_path = sd_path + addLeadingSlash(Config.Image360AdAssetDirectory);
                File dest_dir = new File(dest_dir_path);
                createDir(dest_dir);

                FileOutputStream outputStream;
                String name= this.ad.getPlacementId() + ".jpg";
                Log.d(TAG, "writing file to: " + dest_dir + name);
                outputStream = new FileOutputStream(new File(dest_dir, name));
                outputStream.write(response);
                outputStream.close();

                this.ad.onMediaServerResponseSuccess();
            }
        } catch (IOException e) {
            this.ad.getAdListener().onAdFailedToLoad();

            // send error logs to server

            HashMap<String, String> errorMap = new HashMap<>();
            errorMap.put("Error", e.toString());
            this.ad.emitEvent(EventType.ERROR, AnalyticsManager.Priority.LOW, errorMap);
            Log.e(TAG, "Error processing download file for media ad : " + e.toString());
        }
    }

    private String addLeadingSlash(String path)
    {
        if (path.charAt(0) != '/')
        {
            path = "/" + path;
        }
        return path;
    }

    private void createDir(File dir) throws IOException
    {
        if (dir.exists())
        {
            if (!dir.isDirectory())
            {
                throw new IOException("Can't create directory, a file is in the way");
            }
        } else
        {
            dir.mkdirs();
            if (!dir.isDirectory())
            {
                throw new IOException("Unable to create directory");
            }
        }
    }
}
