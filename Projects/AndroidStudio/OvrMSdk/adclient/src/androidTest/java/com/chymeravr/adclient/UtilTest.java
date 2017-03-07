//package com.chymeravr.adclient;
//
//import android.Manifest;
//import android.content.Context;
//import android.graphics.Bitmap;
//import android.graphics.Color;
//import android.os.Build;
//import android.support.test.filters.LargeTest;
//import android.support.test.runner.AndroidJUnit4;
//import android.util.Log;
//
//import junit.framework.Assert;
//
//import org.junit.Before;
//import org.junit.Test;
//import org.junit.runner.RunWith;
//
//import java.nio.ByteBuffer;
//import java.util.Map;
//
//import static android.support.test.InstrumentationRegistry.getInstrumentation;
//import static android.support.test.InstrumentationRegistry.getTargetContext;
//
///**
// * Created by robin_chimera on 12/2/2016.
// */
//
//@RunWith(AndroidJUnit4.class)
//@LargeTest
//public class UtilTest {
//    private static final String TAG = "UtilTest";
//    Context context = getTargetContext();
//
//    /* grant permissions so that tests may pass*/
//    @Before
//    public void grantPhonePermission() {
//        // In M+, trying to call a number will trigger a runtime dialog. Make sure
//        // the permission is granted before running this test.
//        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
//            getInstrumentation().getUiAutomation().executeShellCommand(
//                    "pm grant " + getTargetContext().getPackageName()
//                            + " android.permission.ACCESS_COARSE_LOCATION");
//            getInstrumentation().getUiAutomation().executeShellCommand(
//                    "pm grant " + getTargetContext().getPackageName()
//                            + " android.permission.ACCESS_FINE_LOCATION");
//            getInstrumentation().getUiAutomation().executeShellCommand(
//                    "pm grant " + getTargetContext().getPackageName()
//                            + " android.permission.ACCESS_WIFI_STATE");
//            getInstrumentation().getUiAutomation().executeShellCommand(
//                    "pm grant " + getTargetContext().getPackageName()
//                            + " android.permission.CHANGE_WIFI_STATE");
//            getInstrumentation().getUiAutomation().executeShellCommand(
//                    "pm grant " + getTargetContext().getPackageName()
//                            + " android.permission.VIBRATE");
//            getInstrumentation().getUiAutomation().executeShellCommand(
//                    "pm grant " + getTargetContext().getPackageName()
//                            + " android.permission.WRITE_EXTERNAL_STORAGE");
//        }
//    }
//
//    @Test()
//    public void getByteArrayTest() throws Exception {
//
//        Bitmap testBitmap = Bitmap.createBitmap(8, 1, Bitmap.Config.ARGB_8888);
//        testBitmap.setPixel(0, 0, Color.BLACK);
//        testBitmap.setPixel(1, 0, Color.BLUE);
//        testBitmap.setPixel(2, 0, Color.RED);
//        testBitmap.setPixel(3, 0, Color.BLACK);
//        testBitmap.setPixel(4, 0, Color.BLUE);
//        testBitmap.setPixel(5, 0, Color.GREEN);
//        testBitmap.setPixel(6, 0, Color.BLACK);
//        testBitmap.setPixel(7, 0, Color.BLUE);
//
//        byte[] result = Util.convertToByteArray(testBitmap);
//        String resultString = "";
//        for (int i = 0; i < result.length; i++) {
//            resultString += " " + result[i];
//        }
//        Bitmap bitmap = Bitmap.createBitmap(8, 1, Bitmap.Config.ARGB_8888);
//        bitmap.copyPixelsFromBuffer(ByteBuffer.wrap(result));
//
//        Log.v(TAG, "Bitmap Test Successful!");
//        Assert.assertTrue(bitmap.sameAs(testBitmap));
//
//    }
//
//    @Test()
//    public void isResourceAccessPermittedTest() throws Exception {
//
//        // Network access
//        String permission = Manifest.permission.INTERNET;
//        Assert.assertTrue(Util.isResourceAccessPermitted(context, permission));
//
//        // Network State Access
//        permission = Manifest.permission.ACCESS_NETWORK_STATE;
//        Assert.assertTrue(Util.isResourceAccessPermitted(context, permission));
//
//        // Coarse Location Access
//        permission = Manifest.permission.ACCESS_COARSE_LOCATION;
//        Assert.assertTrue(Util.isResourceAccessPermitted(context, permission));
//
//        // Fine Location Access
//        permission = Manifest.permission.ACCESS_FINE_LOCATION;
//        Assert.assertTrue(Util.isResourceAccessPermitted(context, permission));
//
//        // Wifi State Access
//        permission = Manifest.permission.ACCESS_WIFI_STATE;
//        Assert.assertTrue(Util.isResourceAccessPermitted(context, permission));
//
//        // Wifi Change Access
//        permission = Manifest.permission.CHANGE_WIFI_STATE;
//        Assert.assertTrue(Util.isResourceAccessPermitted(context, permission));
//
//        // Vibrate Access
//        permission = Manifest.permission.VIBRATE;
//        Assert.assertTrue(Util.isResourceAccessPermitted(context, permission));
//
//        // External Storage Access
//        permission = Manifest.permission.WRITE_EXTERNAL_STORAGE;
//        Assert.assertTrue(Util.isResourceAccessPermitted(context, permission));
//
//        Log.v(TAG, "Resource Access Test Complete!");
//    }
//
//    @Test()
//    public void checkMandatoryPermissionsTest() throws Exception {
//        Assert.assertTrue(Util.checkMandatoryPermissions(context));
//    }
//
//    @Test()
//    public void checkRecommendedPermissionsTest() throws Exception {
//        boolean result = true;
//        Map<String, Boolean> map = Util.checkRecommendedPermissions(context);
//        for (Map.Entry<String, Boolean> entry : map.entrySet()) {
//            Assert.assertTrue(entry.getValue());
//        }
//    }
//}