package chymeravr.com.sdkclient;

import android.Manifest;
import android.content.Context;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.support.annotation.NonNull;
import android.support.v4.content.ContextCompat;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;


/**
 * Created by robin_chimera on 12/2/2016.
 */

public class Util {
    public static ArrayList<String> mandatoryPermissions =
            new ArrayList<String>(Arrays.asList(Manifest.permission.INTERNET,
                                                Manifest.permission.ACCESS_NETWORK_STATE));
    public static ArrayList<String> recommendedPermissions =
            new ArrayList<String>(Arrays.asList(Manifest.permission.ACCESS_COARSE_LOCATION,
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
        boolean result = true;
        for (String permission : mandatoryPermissions){
            result = result && isResourceAccessPermitted(context, permission);
        }
        return result;
    }

    public static Map<String, Boolean>  checkRecommendedPermissions(Context context){
        Map<String, Boolean> map = new HashMap<String, Boolean>();
        for (String permission : recommendedPermissions){
            map.put(permission, isResourceAccessPermitted(context, permission));
        }
        return map;
    }
}
