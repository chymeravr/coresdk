package com.chymeravr.adclient;

import android.Manifest;
import android.content.Context;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.support.annotation.NonNull;
import android.support.v4.content.ContextCompat;

import java.nio.ByteBuffer;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;


/**
 * Created by robin_chimera on 12/2/2016.
 */

class Util {
    public static Set<String> mandatoryPermissions =
            new HashSet<>(Arrays.asList(Manifest.permission.INTERNET,
                                                Manifest.permission.ACCESS_NETWORK_STATE));
    public static Set<String> recommendedPermissions =
            new HashSet<>(Arrays.asList(Manifest.permission.ACCESS_COARSE_LOCATION,
                                                Manifest.permission.ACCESS_FINE_LOCATION,
                                                Manifest.permission.ACCESS_WIFI_STATE,
                                                Manifest.permission.CHANGE_WIFI_STATE,
                                                Manifest.permission.VIBRATE,
                                                Manifest.permission.WRITE_EXTERNAL_STORAGE));

    @NonNull
    public static byte[] convertToByteArray(Bitmap bitmap){
        /* As of KITKAT, the result of this method cannot be used to determine memory usage
           use getAllocationByteCount instead
        */
        assert(bitmap != null);
        int bytes = bitmap.getByteCount();
        ByteBuffer dst = ByteBuffer.allocate(bytes);
        bitmap.copyPixelsToBuffer(dst);
        return dst.array();
    }

    /* a generic boolean test for whether system resource is available to the SDK */
    public static boolean isResourceAccessPermitted(Context context, String permission){
        int accessResource = ContextCompat.checkSelfPermission(context, permission);
        return PackageManager.PERMISSION_GRANTED == accessResource;
    }

    public static boolean checkMandatoryPermissions(Context context) {
        for (String permission : mandatoryPermissions){
            if(!isResourceAccessPermitted(context, permission)) {
                return false;
            }
        }
        return true;
    }

    public static Map<String, Boolean>  checkRecommendedPermissions(Context context){
        Map<String, Boolean> map = new HashMap<>();
        for (String permission : recommendedPermissions){
            map.put(permission, isResourceAccessPermitted(context, permission));
        }
        return map;
    }
}
