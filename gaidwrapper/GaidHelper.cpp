#include "GaidHelper.h"
#include <jni.h>
#include <android/log.h>
#include "platform/android/jni/JniHelper.h"

using namespace cocos2d;

void GaidHelper::getGaid()
{
    JniMethodInfo t;
    
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/Cocos2dxActivity", "getGaid", "()V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        // delete reference
        t.env->DeleteLocalRef(t.classID);
    }
}

extern "C"
JNIEXPORT void JNICALL Java_com_mobileapptracker_gaidwrapper_CocosGAIDInterface_nativeRetrievedGAID(JNIEnv *env, jobject obj, jstring gaid, jboolean isLimitAdTrackingEnabled)
{
    // Get a MobileAppTracker instance
    jclass cls_MobileAppTracker = env->FindClass("com/mobileapptracker/MobileAppTracker");
    
    jmethodID getInstanceMethod = env->GetStaticMethodID(cls_MobileAppTracker, "getInstance", "()Lcom/mobileapptracker/MobileAppTracker;");
    jobject obj_MobileAppTracker = env->CallStaticObjectMethod(cls_MobileAppTracker, getInstanceMethod);
    
    jmethodID setGoogleAdvertisingIdMethod = env->GetMethodID(cls_MobileAppTracker, "setGoogleAdvertisingId", "(Ljava/lang/String;Z)V");
    env->CallVoidMethod(obj_MobileAppTracker, setGoogleAdvertisingIdMethod, gaid, isLimitAdTrackingEnabled);
}