package com.chymeravr.gearvradclient;

/**
 * Created by robin_chimera on 11/29/2016.
 */

import android.content.Context;
import android.support.test.InstrumentationRegistry;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.runners.MockitoJUnitRunner;

/**
 * Instrumentation test, which will execute on an Android device.
 *
 * @see <a href="http://d.android.com/tools/testing">Testing documentation</a>
 */
@RunWith(MockitoJUnitRunner.class)
public class SDKTest {

    @Test(expected = NullPointerException.class)
    public void NullContextSDKInit() throws NullPointerException {
        ChymeraVrSdk.initialize(null, "123");
    }

    @Test()
    public void SDKInit() throws NullPointerException {
        Context appContext = InstrumentationRegistry.getTargetContext();
        ChymeraVrSdk.initialize(appContext, "123");
    }


}
