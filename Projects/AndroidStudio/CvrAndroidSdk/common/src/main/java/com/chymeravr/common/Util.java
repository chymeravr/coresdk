package com.chymeravr.common;

import android.Manifest;
import android.content.Context;
import android.content.pm.PackageManager;

import android.support.v4.content.ContextCompat;
import android.util.Log;

import java.io.File;
import java.io.IOException;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

import lombok.NonNull;


/**
 * Created by robin_chimera on 12/2/2016.
 * Commonly used utility functions
 */

public class Util {
    private static Set<String> mandatoryPermissions =
            new HashSet<>(Arrays.asList(Manifest.permission.INTERNET,
                    Manifest.permission.ACCESS_NETWORK_STATE));

    /* a generic boolean test for whether system resource is available to the SDK */
    static boolean isResourceAccessPermitted(@NonNull Context context, String permission) {
        int accessResource = ContextCompat.checkSelfPermission(context, permission);
        return PackageManager.PERMISSION_GRANTED == accessResource;
    }

    public static boolean checkMandatoryPermissions(@NonNull Context context) {
        for (String permission : mandatoryPermissions) {
            if (!isResourceAccessPermitted(context, permission)) {
                Log.e("CheckRequiredPermission", "Permission " + permission + " not granted!");
                return false;
            }
        }
        return true;
    }

    public static Map<String, String> getErrorMap(@NonNull Exception e){
        Map<String, String> errorMap = new HashMap<>();
        errorMap.put("Error", e.toString());
        return errorMap;
    }

    public static String addLeadingSlash(String path)
    {
        if (path.charAt(0) != '/')
        {
            path = "/" + path;
        }
        return path;
    }

    public static void createDir(File dir) throws IOException
    {
        if (dir.exists())
        {
            if (!dir.isDirectory())
            {
                throw new IOException("Can't create directory, a file is in the way");
            }
        } else
        {
            dir.mkdirs();
            if (!dir.isDirectory())
            {
                throw new IOException("Unable to create directory");
            }
        }
    }
}
