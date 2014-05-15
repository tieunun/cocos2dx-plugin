package org.cocos2dx.cpp;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.plugin.PluginWrapper;

import android.content.Context;
import android.os.Bundle;

import com.mobileapptracker.gaidwrapper.GAIDFetcher;

public class AppActivity extends Cocos2dxActivity{
    private static Context mContext;

    @Override
    protected void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);

        mContext = getApplicationContext();

        PluginWrapper.init(this); // for plugins
    }

    public static void getGaid() {
        GAIDFetcher fetcher = new GAIDFetcher();
        fetcher.useCocosFetcherInterface();
        fetcher.fetchGAID(mContext);
    }

    static {
        System.loadLibrary("cocos2dcpp");
    }
}
