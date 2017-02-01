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

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.mockito.Matchers.any;
import static org.mockito.Mockito.doNothing;
import static org.mockito.Mockito.mock;

/**
 * Created by robin_chimera on 1/31/2017.
 */

@RunWith(AndroidJUnit4.class)
public class ArrayMessageQueueTest {



    @Test
    public void useAppContext() throws Exception {
        // Context of the app under test.
        Context appContext = InstrumentationRegistry.getTargetContext();

        MessageQueue messageQueue = new ArrayMessageQueue(4);

        assertEquals("com.chymeravr.analytics.test", appContext.getPackageName());
    }

    @Test
    public void initialize() throws Exception {
        MessageQueue messageQueue = new ArrayMessageQueue(4);

        Message m1 = messageGenerator(1);
        messageQueue.enqueue(m1);
        Message m2 = messageGenerator(2);
        messageQueue.enqueue(m2);
        Message m3 = messageGenerator(3);
        messageQueue.enqueue(m3);
        Message m4 = messageGenerator(4);
        messageQueue.enqueue(m4);
        assertTrue(true);
    }

    @Test
    public void flushTest() throws Exception {
        WebRequestQueue requestQueue = mock(WebRequestQueue.class);
        doNothing().when(requestQueue).addToRequestQueue(any(Request.class));


        ArrayMessageQueue messageQueue = new ArrayMessageQueue(4);
        messageQueue.setRequestQueue(requestQueue);

        Message m1 = messageGenerator(1);
        messageQueue.enqueue(m1);
        Message m2 = messageGenerator(2);
        messageQueue.enqueue(m1);
        Message m3 = messageGenerator(3);
        messageQueue.enqueue(m3);
        Message m4 = messageGenerator(4);
        messageQueue.enqueue(m4);

        messageQueue.flush();
        assertTrue(!messageQueue.isFull());
    }

    private Message messageGenerator(int var){
        Timestamp timestamp = new Timestamp(System.currentTimeMillis());
        HashMap<String, String> params = new HashMap<>();
        params.put("dummy", "dummy");
        Message dummyMessage = new Message(timestamp.getTime(), Message.EventType.ADREQUEST, params);

        return dummyMessage;
    }
}
