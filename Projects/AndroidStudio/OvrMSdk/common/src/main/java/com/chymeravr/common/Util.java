package com.chymeravr.common;

import android.Manifest;
import android.content.Context;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.support.v4.content.ContextCompat;

import java.nio.ByteBuffer;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

import lombok.NonNull;


/**
 * Created by robin_chimera on 12/2/2016.
 */

// TODO: 2/3/2017 Update permissions with only relevant permissions 
public class Util {
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

    public static byte[] convertToByteArray(@NonNull Bitmap bitmap) {
        /* As of KITKAT, the result of this method cannot be used to determine memory usage
           use getAllocationByteCount instead
        */
        assert (bitmap != null);
        int bytes = bitmap.getByteCount();
        ByteBuffer dst = ByteBuffer.allocate(bytes);
        bitmap.copyPixelsToBuffer(dst);
        return dst.array();
    }

    /* a generic boolean test for whether system resource is available to the SDK */
    public static boolean isResourceAccessPermitted(@NonNull Context context, String permission) {
        int accessResource = ContextCompat.checkSelfPermission(context, permission);
        return PackageManager.PERMISSION_GRANTED == accessResource;
    }

    public static boolean checkMandatoryPermissions(@NonNull Context context) {
        for (String permission : mandatoryPermissions) {
            if (!isResourceAccessPermitted(context, permission)) {
                return false;
            }
        }
        return true;
    }

    public static Map<String, Boolean> checkRecommendedPermissions(@NonNull Context context) {
        Map<String, Boolean> map = new HashMap<>();
        for (String permission : recommendedPermissions) {
            map.put(permission, isResourceAccessPermitted(context, permission));
        }
        return map;
    }

    public static Map<String, Object> getErrorMap(@NonNull Exception e){
        Map<String, Object> errorMap = new HashMap<>();
        errorMap.put("Error", e.toString());
        return errorMap;
    }
}
