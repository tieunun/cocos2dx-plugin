package org.cocos2dx.plugin;

import java.util.ArrayList;
import java.util.Date;
import java.util.Hashtable;
import java.util.List;

import org.json.JSONException;
import org.json.JSONObject;

import android.app.Activity;
import android.content.Context;
import android.util.Log;

import com.mobileapptracker.MATEventItem;
import com.mobileapptracker.MobileAppTracker;

public class PluginMobileAppTracker implements InterfaceAnalytics {
    private static final String LOG_TAG = "PluginMobileAppTracker";
    private static Activity mContext = null;
    private static boolean bDebug = false;
    
    MobileAppTracker mat;
    
    protected static void LogE(String msg, Exception e) {
        Log.e(LOG_TAG, msg, e);
        e.printStackTrace();
    }
    
    protected static void LogD(String msg) {
        if (bDebug) {
            Log.d(LOG_TAG, msg);
        }
    }
    
    public PluginMobileAppTracker(Context context) {
        mContext = (Activity) context;
    }

    public void startTracker(JSONObject eventInfo) {
        try {
            String adId = eventInfo.getString("Param1");
            String convKey = eventInfo.getString("Param2");
            
            MobileAppTracker.init(mContext, adId, convKey);
            mat = MobileAppTracker.getInstance();
            mat.setPluginName("cocos2dx");
            mat.setReferralSources(mContext);
        } catch(Exception e) {
            LogE("Exception in startTracker", e);
        }
    }
    
    public void measureSession() {
        mat.measureSession();
    }
    
    public void measureAction(String eventName) {
        mat.measureAction(eventName);
    }
    
    public void measureActionWithRefId(JSONObject eventInfo) {
        try {
            String eventIdOrName = eventInfo.getString("Param1");
            String refId = eventInfo.getString("Param2");
            
            mat.measureAction(eventIdOrName, 0, "USD", refId);
        } catch(Exception e) {
            LogE("Exception: ", e);
        }
    }
    
    public void measureActionWithRevenue(JSONObject eventInfo) {
        try {
            String eventIdOrName = eventInfo.getString("Param1");
            String refId = eventInfo.getString("Param2");
            double revenueAmount = eventInfo.getDouble("Param3");
            String currencyCode = eventInfo.getString("Param4");
            
            mat.measureAction(eventIdOrName, revenueAmount, currencyCode, refId);
        } catch(Exception e) {
            LogE("Exception: ", e);
        }
    }
    
    public void measureActionWithEventItems(JSONObject eventInfo) {
        try {
            String eventIdOrName = eventInfo.getString("Param1");
            String refId = eventInfo.getString("Param2");
            double revenueAmount = eventInfo.getDouble("Param3");
            String currencyCode = eventInfo.getString("Param4");
            
            int itemsStartPos = 5;
            int countItems = eventInfo.length() - itemsStartPos - 1;
            
            List<MATEventItem> items = new ArrayList<MATEventItem>();
            
            for(int i = 0; i < countItems; ++i)
            {
                JSONObject itemMap = eventInfo.getJSONObject("Param" + (i + itemsStartPos));
                MATEventItem item = matEventItemFromMap(itemMap);
                items.add(item);
            }
            
            mat.measureAction(eventIdOrName, items, revenueAmount, currencyCode, refId);
        } catch(Exception e) {
            LogE("Exception: ", e);
        }
    }
    
    public void measureActionWithIAPReceipt(JSONObject eventInfo) {
        try {
            // items -- is one or more event item hash maps
            // String eventIdOrName, boolean isId, List<MATEventItem> items, String refId, String revenueAmount, String currencyCode, int transactionState, String receipt, String receiptSignature
            
            String eventIdOrName = eventInfo.getString("Param1");
            String refId = eventInfo.getString("Param2");
            double revenueAmount = eventInfo.getDouble("Param3");
            String currencyCode = eventInfo.getString("Param4");
            String receipt = eventInfo.getString("Param6");
            String receiptSignature = eventInfo.getString("Param7");
            
            int itemsStartPos = 8;
            int countItems = eventInfo.length() - itemsStartPos - 1;
            
            List<MATEventItem> items = new ArrayList<MATEventItem>();
            
            for (int i = 0; i < countItems; ++i) {
                JSONObject itemMap = eventInfo.getJSONObject("Param" + (i + itemsStartPos));
                MATEventItem item = matEventItemFromMap(itemMap);
                items.add(item);
            }

            if (receiptSignature != null && receiptSignature.length() > 0) {
                mat.measureAction(eventIdOrName, items, revenueAmount, currencyCode, refId, receipt, receiptSignature);
            } else {
                mat.measureAction(eventIdOrName, items, revenueAmount, currencyCode, refId);
            }
        } catch(Exception e) {
            LogE("Exception: ", e);
        }
    }
    
    public void setAge(int age) {
        mat.setAge(age);
    }
    
    public void setAndroidId(String androidId) {
        mat.setAndroidId(androidId);
    }
    
    public void setAllowDuplicates(boolean allowDuplicates) {
        mat.setAllowDuplicates(allowDuplicates);
    }
    
    public void setCurrencyCode(String currencyCode) {
        mat.setCurrencyCode(currencyCode);
    }
    
    public void setDebugMode(boolean shouldDebug) {
        mat.setDebugMode(shouldDebug);
    }
    
    public void setFacebookUserId(String userId)
    {
        mat.setFacebookUserId(userId);
    }
    
    public void setGoogleUserId(String userId)
    {
        mat.setGoogleUserId(userId);
    }
    
    public void setTwitterUserId(String userId)
    {
        mat.setTwitterUserId(userId);
    }
    
    public void setEventAttribute1(String attribute) {
        mat.setEventAttribute1(attribute);
    }
    
    public void setEventAttribute2(String attribute) {
        mat.setEventAttribute2(attribute);
    }
    
    public void setEventAttribute3(String attribute) {
        mat.setEventAttribute3(attribute);
    }
    
    public void setEventAttribute4(String attribute) {
        mat.setEventAttribute4(attribute);
    }
    
    public void setEventAttribute5(String attribute) {
        mat.setEventAttribute5(attribute);
    }
    
    public void setEventContentId(String contentId) {
        mat.setEventContentId(contentId);
    }
    
    public void setEventContentType(String contentType) {
        mat.setEventContentType(contentType);
    }
    
    public void setEventDate1(String dateMillis) {
    	long millis = Long.parseLong(dateMillis);
    	Date date = new Date(millis);
        mat.setEventDate1(date);
    }
    
    public void setEventDate2(String dateMillis) {
    	long millis = Long.parseLong(dateMillis);
    	Date date = new Date(millis);
        mat.setEventDate2(date);
    }
    
    public void setEventLevel(int level) {
        mat.setEventLevel(level);
    }
    
    public void setEventQuantity(int quantity) {
        mat.setEventQuantity(quantity);
    }
    
    public void setEventRating(float rating) {
        mat.setEventRating(rating);
    }
    
    public void setEventSearchString(String searchString) {
        mat.setEventSearchString(searchString);
    }
    
    public void setExistingUser(boolean existingUser) {
        mat.setExistingUser(existingUser);
    }
    
    public void setGender(int gender) {
        mat.setGender(1 == gender ? MobileAppTracker.GENDER_FEMALE : MobileAppTracker.GENDER_MALE);
    }
    
    public void setGoogleAdvertisingId(String googleAid, boolean isLimitAdTrackingEnabled) {
        mat.setGoogleAdvertisingId(googleAid, isLimitAdTrackingEnabled);
    }
    
    public void setLocation(JSONObject eventInfo) {
        try {
            double latitude = eventInfo.getInt("Param1");
            double longitude = eventInfo.getInt("Param2");
            double altitude = eventInfo.getInt("Param3");
            
            mat.setLatitude(latitude);
            mat.setLongitude(longitude);
            mat.setAltitude(altitude);
        } catch(Exception e) {
            LogE("Exception: ", e);
        }
    }
    
    public void setPackageName(String packageName) {
        mat.setPackageName(packageName);
    }
    
    public void setPayingUser(boolean isPaying) {
        mat.setIsPayingUser(isPaying);
    }
    
    public void setSiteId(String siteId) {
        mat.setSiteId(siteId);
    }
    
    public void setTRUSTeId(String tpid) {
        mat.setTRUSTeId(tpid);
    }
    
    public void setUserEmail(String email) {
        mat.setUserEmail(email);
    }
    
    public void setUserId(String userId) {
        mat.setUserId(userId);
    }
    
    public void setUserName(String name) {
        mat.setUserName(name);
    }
    
    public void setAppAdTracking(boolean enable) {
        mat.setAppAdTrackingEnabled(enable);
    }
    
    public void setShouldAutoDetectJailbroken(boolean shouldAutoDetect) {
        // not available in android
    }
    
    public void setShouldAutoGenerateAppleVendorIdentifier(boolean shouldAutoGenerate) {
        // not available in android
    }
    
    public void setAppleAdvertisingIdentifier(String appleAdvertisingId, boolean isTrackingEnabled) {
        // not available in android
    }
    
    public void setAppleVendorIdentifier(String appleVendorId) {
        // not available in android
    }
    
    public void setJailbroken(boolean isJailbroken) {
        // not available in android
    }
    
    public void setRedirectUrl(String redirectUrl) {
        // not available in android
    }
    
    public void setUseCookieTracking(boolean useCookieTracking) {
        // not available in android
    }
    
    private MATEventItem matEventItemFromMap(JSONObject item) {
        MATEventItem eventItem = null;
        
        try {
            String itemName = item.getString("item");
            int quantity = 0;
            double unitPrice = 0;
            double revenue = 0;
            String attribute1 = null;
            String attribute2 = null;
            String attribute3 = null;
            String attribute4 = null;
            String attribute5 = null;

            if (item.has("quantity")) {
                quantity = item.getInt("quantity");
            }
            if (item.has("unit_price")) {
                unitPrice = item.getDouble("unit_price");
            }
            if (item.has("revenue")) {
                revenue = item.getDouble("revenue");
            }

            if (item.has("attribute_sub1")) {
                attribute1 = item.getString("attribute_sub1");
            }
            if (item.has("attribute_sub2")) {
                attribute2 = item.getString("attribute_sub2");
            }
            if (item.has("attribute_sub3")) {
                attribute3 = item.getString("attribute_sub3");
            }
            if (item.has("attribute_sub4")) {
                attribute4 = item.getString("attribute_sub4");
            }
            if (item.has("attribute_sub5")) {
                attribute5 = item.getString("attribute_sub5");
            }

            eventItem = new MATEventItem(itemName, quantity, unitPrice, revenue, attribute1, attribute2, attribute3, attribute4, attribute5);
                
        } catch (JSONException e) {
            e.printStackTrace();
        }
        
        return eventItem;
    }
    
    public String getMatId() {
        return mat.getMatId();
    }
    
    public String getOpenLogId() {
        return mat.getOpenLogId();
    }
    
    public String getIsPayingUser() {
        return new String("" + mat.getIsPayingUser());
    }
    
    @Override
    public void startSession(String appKey) {
        // TODO Auto-generated method stub
    }

    @Override
    public void stopSession() {
        // TODO Auto-generated method stub
    }

    @Override
    public void setSessionContinueMillis(int millis) {
        // TODO Auto-generated method stub
    }

    @Override
    public void setCaptureUncaughtException(boolean isEnabled) {
        // TODO Auto-generated method stub
    }

    @Override
    public void logError(String errorId, String message) {
        // TODO Auto-generated method stub
    }

    @Override
    public void logEvent(String eventId) {
        // TODO Auto-generated method stub
    }

    @Override
    public void logEvent(String eventId, Hashtable<String, String> paramMap) {
        // TODO Auto-generated method stub
    }

    @Override
    public void logTimedEventBegin(String eventId) {
        // TODO Auto-generated method stub
    }

    @Override
    public void logTimedEventEnd(String eventId) {
        // TODO Auto-generated method stub
    }

    @Override
    public String getSDKVersion() {
        return mat.getSDKVersion();
    }

    @Override
    public String getPluginVersion() {
        return mat.getSDKVersion();
    }
}
