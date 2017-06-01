package com.chymeravr.analytics;

/**
 * Created by robin_chimera on 1/31/2017.
 */

import android.content.Context;
import android.support.test.InstrumentationRegistry;
import android.support.test.runner.AndroidJUnit4;

import com.android.volley.Request;
import com.chymeravr.common.WebRequestQueue;
import com.chymeravr.schemas.eventreceiver.SDKEvent;

import org.junit.Assert;
import org.junit.Test;
import org.junit.runner.RunWith;

import static junit.framework.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.mockito.Matchers.any;
import static org.mockito.Mockito.doNothing;
import static org.mockito.Mockito.mock;

/**
 * Created by robin_chimera on 1/31/2017.
 */

@RunWith(AndroidJUnit4.class)
public class ArrayEventQueueTest {

    @Test
    public void useAppContext() throws Exception {
        // Context of the app under test.
        Context appContext = InstrumentationRegistry.getTargetContext();

        assertEquals("com.chymeravr.analytics.test", appContext.getPackageName());
    }

    @Test
    public void initialize() throws Exception {
        WebRequestQueue requestQueue = mock(WebRequestQueue.class);
        doNothing().when(requestQueue).addToRequestQueue(any(Request.class));

        ArrayEventQueue eventQueue = new ArrayEventQueue(requestQueue, 4, "test");

        assertEquals(eventQueue.getCurrentSize(), 0);
        SDKEvent m1 = new SDKEvent();
        eventQueue.enqueue(m1);
        SDKEvent m2 = new SDKEvent();
        eventQueue.enqueue(m2);
        SDKEvent m3 = new SDKEvent();
        eventQueue.enqueue(m3);
        SDKEvent m4 = new SDKEvent();
        eventQueue.enqueue(m4);
        assertEquals(eventQueue.getCurrentSize(), 4);
    }

    @Test
    public void reSize() throws Exception {
        WebRequestQueue requestQueue = mock(WebRequestQueue.class);
        doNothing().when(requestQueue).addToRequestQueue(any(Request.class));

        ArrayEventQueue eventQueue = new ArrayEventQueue(requestQueue, 4, "Test");

        Assert.assertEquals(eventQueue.getSize(), 4);
        SDKEvent m1 = new SDKEvent();
        eventQueue.enqueue(m1);
        SDKEvent m2 = new SDKEvent();
        eventQueue.enqueue(m2);
        eventQueue.reSize(10);
        Assert.assertEquals(eventQueue.getCurrentSize(), 0);
        Assert.assertEquals(eventQueue.getSize(), 10);
    }

    @Test
    public void flushTest() throws Exception {
        WebRequestQueue requestQueue = mock(WebRequestQueue.class);
        doNothing().when(requestQueue).addToRequestQueue(any(Request.class));


        ArrayEventQueue messageQueue = new ArrayEventQueue(requestQueue, 4, "Test");

        SDKEvent m1 = new SDKEvent();
        messageQueue.enqueue(m1);
        SDKEvent m2 = new SDKEvent();
        messageQueue.enqueue(m2);
        SDKEvent m3 = new SDKEvent();
        messageQueue.enqueue(m3);
        SDKEvent m4 = new SDKEvent();
        messageQueue.enqueue(m4);

        messageQueue.flush();
        assertTrue(!messageQueue.isFull());
        assertEquals(messageQueue.getCurrentSize(), 0);
    }

}
