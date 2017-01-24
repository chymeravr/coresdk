package com.chymeravr.adclient;

import android.app.Activity;
import android.util.Log;
import android.view.SurfaceHolder;

/**
 * Created by robin_chimera on 1/24/2017.
 */

public final class Image360Activity extends Activity implements SurfaceHolder.Callback{


    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        Log.v(TAG, "ActivityGearVR::onSurfaceCreated()");
        if (this.mNativeHandle != 0) {
            onSurfaceCreatedNative(this.mNativeHandle, holder.getSurface());
            this.mSurfaceHolder = holder;
        }
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        Log.v(TAG, "ActivityGearVR::onSurfaceChanged()");
        if (this.mNativeHandle != 0) {
            onSurfaceChangedNative(this.mNativeHandle, holder.getSurface());
            this.mSurfaceHolder = holder;
        }
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        Log.v(TAG, "ActivityGearVR::onSurfaceDestroyed()");
        if (this.mNativeHandle != 0) {
            onSurfaceDestroyedNative(this.mNativeHandle);
            this.mSurfaceHolder = null;
        }

    }
}
