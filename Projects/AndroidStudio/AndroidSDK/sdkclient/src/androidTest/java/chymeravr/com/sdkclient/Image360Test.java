package chymeravr.com.sdkclient;

/**
 * Created by robin_chimera on 11/29/2016.
 */

import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.support.test.InstrumentationRegistry;
import android.support.test.runner.AndroidJUnit4;
import android.util.Log;

import com.android.volley.Request;

import junit.framework.Assert;

import org.json.JSONObject;
import org.junit.Before;
import org.mockito.Mockito;
import org.mockito.runners.MockitoJUnitRunner;


import org.junit.Test;
import org.junit.runner.RunWith;

//import static org.hamcrest.core.IsInstanceOf.any;
import java.util.Arrays;

import static org.mockito.Matchers.any;
import static org.mockito.Matchers.anyString;
import static org.mockito.Mockito.doNothing;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

/**
 * Instrumentation test, which will execute on an Android device.
 *
 * @see <a href="http://d.android.com/tools/testing">Testing documentation</a>
 */

/* This class currently does not require much test code since its only job is
to make network requests
 */
@RunWith(AndroidJUnit4.class)
//@RunWith(MockitoJUnitRunner.class)
public class Image360Test {
    private static final String TAG = "Image360Test";
    private Context appContext = InstrumentationRegistry.getTargetContext();
    private Image360Ad image360Ad;

    private WebRequestQueue mockRequestQueue;

    @Before
    public void setUp() throws Exception {
        Log.v(TAG, "Setting up testing environment");
        image360Ad = new Image360Ad(appContext);
        image360Ad.setAdListener(new AdListener() {
            @Override
            public void onAdLoaded() {}
            @Override
            public void onAdFailedToLoad() {}
            @Override
            public void onAdOpened() {}
            @Override
            public void onAdClosed() {}
            @Override
            public void onAdLeftApplication() {}
        });
        this.mockRequestQueue = Mockito.mock(WebRequestQueue.class);
        doNothing().when(mockRequestQueue).addToRequestQueue(any(Request.class));
        when(mockRequestQueue.getRequestQueue()).thenReturn(null);
    }

    @Test()
    public void onAdServerResponseSuccessTest() throws Exception{
        Log.v(TAG, "Commencing image 360 ad server response success test");
        JSONObject jsonResponseObject = new JSONObject();
        String mediaUrl = "http://foo.bar";
        jsonResponseObject.put("url", mediaUrl);
        image360Ad.onAdServerResponseSuccess(jsonResponseObject);
        Assert.assertEquals(image360Ad.getMediaUrl(), mediaUrl);
    }

    @Test()
    public void onMediaServerResponseSuccessTest() throws Exception{
        Log.v(TAG, "Commencing image 360 media server response success test");
        Config.adServer = "ASD";
        Bitmap testBitmap = Bitmap.createBitmap(8, 1, Bitmap.Config.ARGB_8888);
        testBitmap.setPixel(0, 0, Color.BLACK);
        testBitmap.setPixel(1, 0, Color.BLUE);
        testBitmap.setPixel(2, 0, Color.RED);
        testBitmap.setPixel(3, 0, Color.BLACK);
        testBitmap.setPixel(4, 0, Color.BLUE);
        testBitmap.setPixel(5, 0, Color.GREEN);
        testBitmap.setPixel(6, 0, Color.BLACK);
        testBitmap.setPixel(7, 0, Color.BLUE);

        image360Ad.onMediaServerResponseSuccess(testBitmap);

        byte[] result = Util.convertToByteArray(testBitmap);

        Assert.assertTrue(Arrays.equals(result, image360Ad.getByteArray()));
        Assert.assertTrue(testBitmap.sameAs(image360Ad.getImageBitmap()));

    }


}