#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "PluginManager.h"
#include "ProtocolAnalytics.h"

using namespace cocos2d;

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    const char * getDummyIAPReceipt();
    
    // a selector callback
    void menuCloseCallback(Object* pSender);
    void menuCallback(Object* pSender);
    void reloadPluginMenuCallback(Object* pSender);

    void loadMobileAppTrackerPlugin();
    void unloadMobileAppTrackerPlugin();
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

private:
    cocos2d::plugin::ProtocolAnalytics* _pluginMAT;
};

#endif // __HELLOWORLD_SCENE_H__