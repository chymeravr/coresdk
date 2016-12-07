package chymeravr.com.sdkclient;

/**
 * Created by robin_chimera on 12/6/2016.
 */

import android.content.Context;
import android.content.Intent;
import android.support.test.InstrumentationRegistry;
import android.support.test.runner.AndroidJUnit4;
import android.util.Log;

import com.android.volley.Request;
import com.android.volley.VolleyError;

import org.json.JSONObject;
import org.junit.Before;
import org.mockito.Mock;
import org.mockito.Mockito;
import org.mockito.runners.MockitoJUnitRunner;


import org.junit.Test;
import org.junit.runner.RunWith;

import static org.mockito.Matchers.any;
import static org.mockito.Mockito.doNothing;
import static org.mockito.Mockito.doThrow;
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
public class AdServerListenerTest {
    private static final String TAG = "AdServerListenerTest";
    private Context appContext = InstrumentationRegistry.getTargetContext();
    private ServerListener adServerListener;

    private VolleyError mockVolleyError;
    private WebRequestQueue mockRequestQueue;
    private Ad mockAd;
    //private Ad spyAd;
    private AdListener mockAdListener;
    private JSONObject jsonResponseObject;

    @Before
    public void setUp() throws Exception {
        this.mockRequestQueue = Mockito.mock(WebRequestQueue.class);
        this.mockVolleyError = Mockito.mock(VolleyError.class);
        this.mockAdListener = new AdListener() {
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
        };
        this.mockAd = new FakeAd(appContext);

        this.jsonResponseObject = new JSONObject();
        this.jsonResponseObject.put("url", "");

        doNothing().when(mockRequestQueue).addToRequestQueue(any(Request.class));
        when(mockRequestQueue.getRequestQueue()).thenReturn(null);

//        doNothing().when(mockAdListener).onAdFailedToLoad();
//        doNothing().when(mockAdListener).onAdClosed();
//        doNothing().when(mockAdListener).onAdOpened();
//        doNothing().when(mockAdListener).onAdLoaded();
//        doNothing().when(mockAdListener).onAdLeftApplication();

        mockAd.setAdListener(mockAdListener);
        this.adServerListener = new Image360AdServerListener(mockAd);

        this.adServerListener.setRequestQueue(mockRequestQueue.getRequestQueue());

        doThrow(UnsupportedOperationException.class).when(mockVolleyError).printStackTrace();
    }

    @Test()
    public void onResponseTest() throws Exception{
        this.adServerListener.onResponse(jsonResponseObject);
    }

    @Test(expected=UnsupportedOperationException.class)
    public void onErrorResponseTest() throws Exception{
        this.adServerListener.onErrorResponse(mockVolleyError);
    }
}