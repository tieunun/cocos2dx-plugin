package org.cocos2dx.cpp;

import com.mobileapptracker.gaidwrapper.GAIDFetcher;

import android.app.NativeActivity;
import android.content.Context;
import android.os.Bundle;

public class Cocos2dxActivity extends NativeActivity{
    private static Context mContext;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        // TODO Auto-generated method stub
        super.onCreate(savedInstanceState);
        
        mContext = getApplicationContext();
    }
    
    public static void getGaid() {
        GAIDFetcher fetcher = new GAIDFetcher();
        fetcher.useCocosFetcherInterface();
        fetcher.fetchGAID(mContext);
    }
}
