package com.chymeravr.analytics;

/**
 * Created by robin_chimera on 1/31/2017.
 */

import android.content.Context;
import android.support.test.InstrumentationRegistry;
import android.support.test.runner.AndroidJUnit4;

import com.android.volley.Request;
import com.chymeravr.common.WebRequestQueue;

import org.junit.Test;
import org.junit.runner.RunWith;

import java.sql.Timestamp;
import java.util.HashMap;

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

        ArrayEventQueue eventQueue = new ArrayEventQueue(4, requestQueue);

        assertEquals(eventQueue.getCurrentSize(), 0);
        Event m1 = eventGenerator(1);
        eventQueue.enqueue(m1);
        Event m2 = eventGenerator(2);
        eventQueue.enqueue(m2);
        Event m3 = eventGenerator(3);
        eventQueue.enqueue(m3);
        Event m4 = eventGenerator(4);
        eventQueue.enqueue(m4);
        assertEquals(eventQueue.getCurrentSize(), 4);
    }

    @Test
    public void flushTest() throws Exception {
        WebRequestQueue requestQueue = mock(WebRequestQueue.class);
        doNothing().when(requestQueue).addToRequestQueue(any(Request.class));


        ArrayEventQueue messageQueue = new ArrayEventQueue(4, requestQueue);

        Event m1 = eventGenerator(1);
        messageQueue.enqueue(m1);
        Event m2 = eventGenerator(2);
        messageQueue.enqueue(m1);
        Event m3 = eventGenerator(3);
        messageQueue.enqueue(m3);
        Event m4 = eventGenerator(4);
        messageQueue.enqueue(m4);

        messageQueue.flush();
        assertTrue(!messageQueue.isFull());
        assertEquals(messageQueue.getCurrentSize(), 0);
    }

    private Event eventGenerator(int var){
        Timestamp timestamp = new Timestamp(System.currentTimeMillis());
        HashMap<String, String> params = new HashMap<>();
        params.put("dummy", "dummy");
        Event dummyEvent = new Event(timestamp.getTime(), Event.EventType.ADREQUEST, Event.Priority.HIGH);

        return dummyEvent;
    }
}
