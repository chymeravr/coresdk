package com.chymeravr.adclient;

/**
 * Created by robin_chimera on 11/29/2016.
 */

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.location.Location;
import android.support.test.InstrumentationRegistry;
import android.support.test.runner.AndroidJUnit4;
import android.util.Log;

import junit.framework.Assert;

import org.json.JSONObject;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;

import java.util.Arrays;
import java.util.Date;

import static com.chymeravr.adclient.TestUtils.createDoB;
import static com.chymeravr.adclient.TestUtils.createLocation;


/**
 * Instrumentation test, which will execute on an Android device.
 *
 * @see <a href="http://d.android.com/tools/testing">Testing documentation</a>
 */

/* This class currently does not require much test code since its only job is
to make network requests
 */
@RunWith(AndroidJUnit4.class)
public class Image360Test {
    private static final String TAG = "Image360Test";
    private Context appContext = InstrumentationRegistry.getTargetContext();
    private Image360Ad image360Ad;

    @Before
    public void setUp() throws Exception {
        Log.v(TAG, "Setting up testing environment");
        this.image360Ad = new Image360Ad("image360test", appContext, new AdListener() {
            @Override
            public void onAdLoaded() {
            }

            @Override
            public void onAdFailedToLoad() {
            }

            @Override
            public void onAdOpened() {
            }

            @Override
            public void onAdClosed() {
            }

            @Override
            public void onAdLeftApplication() {
            }
        });
    }

    @Test()
    public void onAdServerResponseSuccessTest() throws Exception {
        Log.v(TAG, "Commencing image 360 ad server response success test");
        JSONObject testJsonResponseObject = new JSONObject();
        testJsonResponseObject.put("mediaUrl", "test");
        this.image360Ad.onAdServerResponseSuccess(testJsonResponseObject);
        Assert.assertEquals(this.image360Ad.getMediaUrl(), "test");
    }

    @Test()
    public void onMediaServerResponseSuccessTest() throws Exception {
        Log.v(TAG, "Commencing image 360 media server response success test");
        Bitmap testBitmap = Bitmap.createBitmap(8, 1, Bitmap.Config.ARGB_8888);
        testBitmap.setPixel(0, 0, Color.BLACK);
        testBitmap.setPixel(1, 0, Color.BLUE);
        testBitmap.setPixel(2, 0, Color.RED);
        testBitmap.setPixel(3, 0, Color.BLACK);
        testBitmap.setPixel(4, 0, Color.BLUE);
        testBitmap.setPixel(5, 0, Color.GREEN);
        testBitmap.setPixel(6, 0, Color.BLACK);
        testBitmap.setPixel(7, 0, Color.BLUE);

        this.image360Ad.onMediaServerResponseSuccess(testBitmap);

        byte[] result = Util.convertToByteArray(testBitmap);

        Assert.assertTrue(Arrays.equals(result, image360Ad.getByteArray()));
        Assert.assertTrue(testBitmap.sameAs(image360Ad.getImageBitmap()));

    }

    @Test()
    public void onLoadTest() throws Exception {
        Log.v(TAG, "testing on load method");
        Location location = createLocation();
        Date dob = createDoB();
        AdRequest adRequest = AdRequest.builder()
                .location(location)
                .birthday(dob)
                .gender(AdRequest.Gender.MALE).build();

        this.image360Ad.loadAd(adRequest);
        Thread.sleep(5000);
        byte[] byteArray = image360Ad.getByteArray();
        Assert.assertNotNull(byteArray);


    }
}