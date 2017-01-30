package com.chymeravr.adclient;

import android.util.Log;

import com.android.volley.VolleyError;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

/**
 * Created by robin_chimera on 1/23/2017.
 */

public class Image360DownloadMediaServerListener extends ServerListener<byte[]> {

    private final String TAG = "Image360MediaListener";
    private Ad ad;

    public Image360DownloadMediaServerListener(Ad ad) {
        super(ad);
        this.ad = ad;
        // Explicitly set the singleton queue;
        this.setRequestQueue(WebRequestQueue.getInstance(ad.getContext()).getRequestQueue());

    }

    @Override
    public void onErrorResponse(VolleyError error) {

        this.getAd().setLoading(false);
        this.getAd().getAdListener().onAdFailedToLoad();
        Log.e(TAG, "Error ", error);
    }

    @Override
    public void onResponse(byte[] response) {
        try {
            if (response!=null) {

                File sd_path = this.ad.getContext().getFilesDir(); //Environment.getExternalStorageDirectory();
                String dest_dir_path = sd_path + addLeadingSlash("chymeraSDKAssets/image360/");
                File dest_dir = new File(dest_dir_path);
                createDir(dest_dir);

                Log.d(TAG, "image destination created");

                FileOutputStream outputStream;
                String name="image360Ad.jpg";
                Log.d(TAG, "writing file to: " + dest_dir + name);
                outputStream = new FileOutputStream(new File(dest_dir, name));//this.ad.getContext().openFileOutput(name, Context.MODE_PRIVATE);
                outputStream.write(response);
                outputStream.close();
                Log.i(TAG, "File download complete");
                File lister = this.ad.getContext().getFilesDir();
//                Log.i(TAG, lister.getAbsolutePath());
//                for (String list : lister.list())
//                {
//                    Log.i(TAG, list);
//                }

                this.getAd().onMediaServerResponseSuccess(response);

            }
        } catch (Exception e) {
            // TODO Auto-generated catch block
            Log.d("KEY_ERROR", "UNABLE TO DOWNLOAD FILE");
            Log.e(TAG, "Error ", e);
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
