package chymeravr.com.sdkclient;

/**
 * Created by robin_chimera on 11/29/2016.
 */

import android.content.Context;
import android.support.test.InstrumentationRegistry;

import static org.hamcrest.MatcherAssert.assertThat;
import org.mockito.runners.MockitoJUnitRunner;


import org.junit.Test;
import org.junit.runner.RunWith;

import static org.junit.Assert.*;

/**
 * Instrumentation test, which will execute on an Android device.
 *
 * @see <a href="http://d.android.com/tools/testing">Testing documentation</a>
 */
//@RunWith(AndroidJUnit4.class)
@RunWith(MockitoJUnitRunner.class)
public class SDKTest {

    @Test
    public void useAppContext() throws Exception {
        // Context of the app under test.
        Context appContext = InstrumentationRegistry.getTargetContext();

        assertEquals("chymeravr.com.sdkclient", appContext.getPackageName());
    }

    @Test(expected=NullPointerException.class)
    public void NullContextSDKInit() throws NullPointerException{
        ChymeraVRAndroidSDK.initialize(null, "123");
    }

    @Test()
    public void SDKInit() throws NullPointerException{
        Context appContext = InstrumentationRegistry.getTargetContext();
        ChymeraVRAndroidSDK.initialize(appContext, "123");
    }


}
