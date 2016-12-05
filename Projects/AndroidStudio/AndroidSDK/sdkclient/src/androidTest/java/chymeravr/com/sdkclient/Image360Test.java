package chymeravr.com.sdkclient;

/**
 * Created by robin_chimera on 11/29/2016.
 */

import android.content.Context;
import android.support.test.InstrumentationRegistry;

import org.junit.Before;
import org.mockito.runners.MockitoJUnitRunner;


import org.junit.Test;
import org.junit.runner.RunWith;

/**
 * Instrumentation test, which will execute on an Android device.
 *
 * @see <a href="http://d.android.com/tools/testing">Testing documentation</a>
 */

/* This class currently does not require much test code since its only job is
to make network requests
 */
//@RunWith(AndroidJUnit4.class)
@RunWith(MockitoJUnitRunner.class)
public class Image360Test {
    private static final String TAG = "Image360Test";
    private Context appContext = InstrumentationRegistry.getTargetContext();
    private Image360Ad image360Ad;

    @Before
    public void setUp() throws Exception {
        image360Ad = new Image360Ad(appContext);
    }

    @Test()
    public void loadAdTest() throws Exception{
        Config.adServer = "ASD";
    }


}