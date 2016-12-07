package chymeravr.com.sdkclient;

import android.content.Context;

import org.json.JSONObject;

/**
 * Created by robin_chimera on 12/6/2016.
 */

/* A fake ad class implementation for testing. this doesnt do anything*/
public class FakeAd extends Ad {
    public FakeAd(Context context){
        this.setContext(context);
    }
    @Override
    public void loadAd(AdRequest adRequest) {

    }

    @Override
    public void show() {

    }

    @Override
    void onAdServerResponseSuccess(JSONObject response) {

    }

    @Override
    <T> void onMediaServerResponseSuccess(T media) {

    }
}
