package com.chymeravr.adclient;

import android.util.Log;

import com.android.volley.RequestQueue;
import com.android.volley.VolleyError;
import com.chymeravr.analytics.Event;
import com.chymeravr.common.Config;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.sql.Timestamp;
import java.util.HashMap;


/**
 * Created by robin_chimera on 1/23/2017.
 */

public class Image360DownloadMediaServerListener extends ServerListener<byte[]> {

    private final String TAG = "Image360MediaListener";

    public Image360DownloadMediaServerListener(Ad ad, RequestQueue requestQueue) {
        super(ad);

        this.setRequestQueue(requestQueue);
    }

    @Override
    public void onErrorResponse(VolleyError error) {

        this.getAd().setLoading(false);
        this.getAd().getAdListener().onAdFailedToLoad();
        HashMap<String, Object> errorMap = new HashMap<String, Object>();
        errorMap.put("Error", error.toString());
        this.getAd().getAnalyticsManager().push(new Event((new Timestamp(System.currentTimeMillis())).getTime(),
                Event.EventType.ERROR, Event.Priority.LOW, errorMap));
        Log.e(TAG, "Error ", error);
    }

    @Override
    public void onResponse(byte[] response) {
        try {
            if (response!=null) {

                File sd_path = this.getAd().getContext().getFilesDir();
                String dest_dir_path = sd_path + addLeadingSlash(Config.Image360AdAssetDirectory);
                File dest_dir = new File(dest_dir_path);
                createDir(dest_dir);

                FileOutputStream outputStream;
                String name= getAd().getPlacementId() + ".jpg";
                Log.d(TAG, "writing file to: " + dest_dir + name);
                outputStream = new FileOutputStream(new File(dest_dir, name));
                outputStream.write(response);
                outputStream.close();

                this.getAd().onMediaServerResponseSuccess(response);
            }
        } catch (IOException e) {
            // send error logs to server
            HashMap<String, Object> errorMap = new HashMap<String, Object>();
            errorMap.put("Error", e.toString());
            this.getAd().getAnalyticsManager().push(new Event((new Timestamp(System.currentTimeMillis())).getTime(),
                    Event.EventType.ERROR, Event.Priority.LOW, errorMap));
            Log.e(TAG, "Error processing download file for media ad : " + e.toString());
        }
    }

    public String addLeadingSlash(String path)
    {
        if (path.charAt(0) != '/')
        {
            path = "/" + path;
        }
        return path;
    }

    public void createDir(File dir) throws IOException
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
