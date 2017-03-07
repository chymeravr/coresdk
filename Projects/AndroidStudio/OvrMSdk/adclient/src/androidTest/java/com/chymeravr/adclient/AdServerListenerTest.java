package com.chymeravr.adclient;

/**
 * Created by robin_chimera on 12/6/2016.
 */

import android.content.Context;
import android.support.test.InstrumentationRegistry;
import android.support.test.runner.AndroidJUnit4;

import com.android.volley.VolleyError;

import junit.framework.Assert;

import org.json.JSONObject;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mockito;

/**
 * Instrumentation test, which will execute on an Android device.
 *
 * @see <a href="http://d.android.com/tools/testing">Testing documentation</a>
 */

/* This class currently does not require much test code since its only job is
to make network requests
 */
@RunWith(AndroidJUnit4.class)
public class AdServerListenerTest {
    private static final String TAG = "AdServerListenerTest";
    private Context appContext = InstrumentationRegistry.getTargetContext();
    private AdServerListener adServerListener;

    private VolleyError mockVolleyError;
    private Ad mockAd;
    private AdListener mockAdListener;
    private JSONObject jsonResponseObject;

    @Before
    public void setUp() throws Exception {
        this.mockVolleyError = Mockito.mock(VolleyError.class);
        this.mockAdListener = new AdListener() {
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
        };
        this.mockAd = new FakeAd(appContext, mockAdListener, null, null);
        this.mockAd.loadAd(null);
        this.jsonResponseObject = new JSONObject();
        this.jsonResponseObject.put("url", "");

        this.adServerListener = new AdServerListener(mockAd);

//        this.adServerListener.setRequestQueue(null);

    }

    @Test()
    public void onResponseTest() throws Exception {
        this.adServerListener.onResponse(jsonResponseObject);
    }

    @Test()
    public void onErrorResponseTest() throws Exception {
        Assert.assertTrue(this.mockAd.isLoading());
        this.adServerListener.onErrorResponse(mockVolleyError);
        Assert.assertFalse(this.mockAd.isLoading());
    }
}