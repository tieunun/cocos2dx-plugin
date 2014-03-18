#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "cocos2d.h"
#include "PluginManager.h"

using namespace cocos2d::plugin;

USING_NS_CC;

PluginProtocol *pluginMAT = NULL;

AppDelegate::AppDelegate() {
    
}

AppDelegate::~AppDelegate()
{
    unloadMobileAppTrackerPlugin();
}

PluginProtocol* AppDelegate::mat()
{
    return pluginMAT;
}

void AppDelegate::loadMobileAppTrackerPlugin()
{
    // load plugin PluginMobileAppTracker
    pluginMAT = dynamic_cast<ProtocolAnalytics*>(PluginManager::getInstance()->loadPlugin("PluginMobileAppTracker"));
    printf("MAT plugin = %s", pluginMAT->getPluginName());
}

void AppDelegate::unloadMobileAppTrackerPlugin()
{
    // unload plugin PluginMobileAppTracker
    PluginManager::getInstance()->unloadPlugin("PluginMobileAppTracker");
    pluginMAT = NULL;
}

bool AppDelegate::applicationDidFinishLaunching() {
    // load the MobileAppTracker plugin
    loadMobileAppTrackerPlugin();
    
    // initialize director
    auto director = Director::getInstance();
    auto eglView = EGLView::getInstance();
    
    director->setOpenGLView(eglView);
    
    // turn on display FPS
    director->setDisplayStats(true);
    
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    
    // create a scene. it's an autorelease object
    auto scene = HelloWorld::createScene();
    
    // run
    director->runWithScene(scene);
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
    
    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
    
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}