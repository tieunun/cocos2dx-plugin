#include "HelloWorldScene.h"
#include "PluginManager.h"
#include "ProtocolAnalytics.h"
#include "AppDelegate.h"

#define MAT_ADVERTISER_ID_IOS       "877"
#define MAT_CONVERSION_KEY_IOS      "8c14d6bbe466b65211e781d62e301eec"

#define MAT_ADVERTISER_ID_ANDROID   "877"
#define MAT_CONVERSION_KEY_ANDROID  "8c14d6bbe466b65211e781d62e301eec"

using namespace cocos2d;
using namespace cocos2d::plugin;

extern cocos2d::plugin::PluginProtocol *mat;

USING_NS_CC;

enum {
    TAG_START_TRACKER = 0,
    TAG_TOGGLE_DEBUG,
    TAG_TOGGLE_ALLOW_DUPLICATES,
    TAG_TOGGLE_DELEGATE,
    TAG_MEASURE_SESSION,
    TAG_MEASURE_EVENT,
    TAG_MEASURE_EVENT_WITH_REFERENCE_ID,
    TAG_MEASURE_EVENT_WITH_REVENUE,
    TAG_MEASURE_EVENT_WITH_EVENT_ITEMS,
    TAG_MEASURE_EVENT_WITH_RECEIPT,
    TAG_TEST_SETTERS,
    TAG_TEST_GETTERS
};

typedef struct tagEventMenuItem {
    std::string id;
    int tag;
} EventMenuItem;

static EventMenuItem s_EventMenuItem[] =
{
    {"Start MAT", TAG_START_TRACKER},
    {"Toggle Debug", TAG_TOGGLE_DEBUG},
    {"Toggle Allow Duplicates", TAG_TOGGLE_ALLOW_DUPLICATES},
    {"Toggle Delegate", TAG_TOGGLE_DELEGATE},
    {"Measure Session", TAG_MEASURE_SESSION},
    {"Measure Event", TAG_MEASURE_EVENT},
    {"Measure Event With Ref Id", TAG_MEASURE_EVENT_WITH_REFERENCE_ID},
    {"Measure Event With Revenue", TAG_MEASURE_EVENT_WITH_REVENUE},
    {"Measure Event With Event Items", TAG_MEASURE_EVENT_WITH_EVENT_ITEMS},
    {"Measure Event With IAP Receipt", TAG_MEASURE_EVENT_WITH_RECEIPT},
    {"Test Setter Methods", TAG_TEST_SETTERS},
    {"Test Getter Methods", TAG_TEST_GETTERS}
};

bool isDebug = false;
bool isAllowDuplicates = false;
bool isDelegate = false;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    
    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                 origin.y + closeItem->getContentSize().height/2));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);
    
    /////////////////////////////
    // 3. add your codes below...
    
    // add a label shows "Hello World"
    // create and initialize a label
    
    Point beginPos = Point(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 50);
    float step = 60.0f;
    int nCaseCount = sizeof(s_EventMenuItem) / sizeof(s_EventMenuItem[0]);
    for (int i = 0; i < nCaseCount; ++i) {
        std::string caseName = s_EventMenuItem[i].id;
        MenuItemFont *pItem = MenuItemFont::create(caseName.c_str(), CC_CALLBACK_1(HelloWorld::menuCallback, this));
        pItem->setTag(s_EventMenuItem[i].tag);
        pItem->setPosition(Point(beginPos.x, beginPos.y - i * step));
        menu->addChild(pItem);
    }
    
    return true;
}

void HelloWorld::reloadPluginMenuCallback(Object* pSender)
{
    PluginManager::getInstance()->unloadPlugin("PluginMobileAppTracker");
    
    AppDelegate::loadMobileAppTrackerPlugin();
}


void HelloWorld::menuCallback(Object* pSender)
{
    MenuItemFont *pItem = (MenuItemFont*) pSender;
    switch (pItem->getTag()) {
        case TAG_START_TRACKER:
        {
            const char * matAdvId  = "";
            const char * matConvKey = "";
            
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            matAdvId  = MAT_ADVERTISER_ID_IOS;
            matConvKey  = MAT_CONVERSION_KEY_IOS;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            matAdvId  = MAT_ADVERTISER_ID_ANDROID;
            matConvKey  = MAT_CONVERSION_KEY_ANDROID;
#endif
            
            PluginParam pAdvId(matAdvId);
            PluginParam pConvKey(matConvKey);
            AppDelegate::mat()->callFuncWithParam("startTracker", &pAdvId, &pConvKey, NULL);
            
            PluginParam pPackageName("org.cocos2dx.HelloMobileAppTracker");
            AppDelegate::mat()->callFuncWithParam("setPackageName", &pPackageName, NULL);
            
            break;
        }
        case TAG_TOGGLE_DEBUG:
        {
            isDebug = !isDebug;
            PluginParam pDebug(isDebug);
            AppDelegate::mat()->callFuncWithParam("setDebugMode", &pDebug, NULL);
            break;
        }
        case TAG_TOGGLE_ALLOW_DUPLICATES:
        {
            isAllowDuplicates = !isAllowDuplicates;
            PluginParam pDup(isAllowDuplicates);
            AppDelegate::mat()->callFuncWithParam("setAllowDuplicates", &pDup, NULL);
            break;
        }
        case TAG_TOGGLE_DELEGATE:
        {
            isDelegate = !isDelegate;
            PluginParam pDelegate(isDelegate);
            AppDelegate::mat()->callFuncWithParam("setDelegate", &pDelegate, NULL);
            break;
        }
        case TAG_MEASURE_SESSION:
        {
            AppDelegate::mat()->callFuncWithParam("measureSession", NULL);
            break;
        }
        case TAG_MEASURE_EVENT:
        {
            PluginParam pEvent("event");
            AppDelegate::mat()->callFuncWithParam("measureAction", &pEvent, NULL);
            break;
        }
        case TAG_MEASURE_EVENT_WITH_REFERENCE_ID:
        {
            PluginParam pEvent("eventWithRef");
            PluginParam pRef("ref103");
            AppDelegate::mat()->callFuncWithParam("measureActionWithRefId", &pEvent, &pRef, NULL);
            break;
        }
        case TAG_MEASURE_EVENT_WITH_REVENUE:
        {
            PluginParam pEvent("eventWithRevenue");
            PluginParam pRef("ref104");
            PluginParam pRevenue(1.53f);
            PluginParam pCurrency("GBP");
            AppDelegate::mat()->callFuncWithParam("measureActionWithRevenue", &pEvent, &pRef, &pRevenue, &pCurrency, NULL);
            break;
        }
        case TAG_MEASURE_EVENT_WITH_EVENT_ITEMS:
        {
            PluginParam pEvent("eventWithItems");
            PluginParam pRef("ref105");
            PluginParam pRevenue(1.54f);
            PluginParam pCurrency("GBP");
            
            // sample event item using StringMap
            StringMap first;
            first["item"] = "ball1";
            first["unit_price"] = "1.99";
            first["quantity"] = "2";
            first["revenue"] = "1.25";
            PluginParam pItem1(first);
            
            StringMap second;
            second["item"] = "ball2";
            second["unit_price"] = "0.78";
            second["quantity"] = "2";
            second["revenue"] = "1.35";
            second["attribute1"] = "blue";
            second["attribute2"] = "100gm";
            second["attribute3"] = "leather";
            PluginParam pItem2(second);
            
            // sample event item using LogEventParamMap
            LogEventParamMap third;
            third.insert(LogEventParamPair("item", "ball3"));
            third.insert(LogEventParamPair("unit_price", "3.99"));
            third.insert(LogEventParamPair("quantity", "1"));
            third.insert(LogEventParamPair("revenue", "3.99f"));
            third.insert(LogEventParamPair("attribute1", "red"));
            PluginParam pItem3(third);
            
            AppDelegate::mat()->callFuncWithParam("measureActionWithEventItems", &pEvent, &pRef, &pRevenue, &pCurrency, &pItem1, &pItem2, &pItem3, NULL);
            break;
        }
        case TAG_MEASURE_EVENT_WITH_RECEIPT:
        {
            PluginParam pEvent("eventWithReceipt");
            PluginParam pRef("ref106");
            PluginParam pRevenue(1.55f);
            PluginParam pCurrency("GBP");
            
            StringMap first;
            first["item"] = "ball1";
            first["unit_price"] = "1.99";
            first["quantity"] = "1";
            first["revenue"] = "1.79";
            PluginParam pItem1(first);
            
            StringMap second;
            second["item"] = "ball2";
            second["unit_price"] = "0.78";
            second["quantity"] = "2";
            second["revenue"] = "1.47";
            second["attribute1"] = "blue";
            second["attribute2"] = "100gm";
            second["attribute3"] = "leather";
            PluginParam pItem2(second);
            
            int dummyTransactionState = 0;
            PluginParam pTransactionState(dummyTransactionState);
            
            const char *receipt = NULL;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            receipt = getDummyIAPReceipt();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            receipt = "";
#endif
            PluginParam pReceipt(receipt);
            
            const char *receiptSignature = ""; // empty
            PluginParam pReceiptSignature(receiptSignature);
            
            AppDelegate::mat()->callFuncWithParam("measureActionWithIAPReceipt", &pEvent, &pRef, &pRevenue, &pCurrency, &pTransactionState, &pReceipt, &pReceiptSignature, &pItem1, &pItem2, NULL);
            
            break;
        }
        case TAG_TEST_SETTERS:
        {
            PluginParam pAppleAdvId("1234567890-1234567890-1234567890-12");
            PluginParam pAdvTrackingEnabled(true);
            AppDelegate::mat()->callFuncWithParam("setAppleAdvertisingIdentifier", &pAppleAdvId, &pAdvTrackingEnabled, NULL);
            
            PluginParam pAppleVendorId("1234567890-1234567890-1234567890-12");
            AppDelegate::mat()->callFuncWithParam("setAppleVendorIdentifier", &pAppleVendorId, NULL);
            
            PluginParam pAge(57);
            AppDelegate::mat()->callFuncWithParam("setAge", &pAge, NULL);
            
            PluginParam pGoogleAdvId("1234567890-1234567890-1234567890-12");
            AppDelegate::mat()->callFuncWithParam("setGoogleAdvertisingId", &pGoogleAdvId, NULL);
            
            PluginParam pExistingUser(false);
            AppDelegate::mat()->callFuncWithParam("setExistingUser", &pExistingUser, NULL);
            
            PluginParam pJailbroken(false);
            AppDelegate::mat()->callFuncWithParam("setJailbroken", &pJailbroken, NULL);
            
            PluginParam pTrusteTPID("1234567890-1234567890-1234567890-12");
            AppDelegate::mat()->callFuncWithParam("setTRUSTeId", &pTrusteTPID, NULL);
            
            PluginParam pUserEmail("address@companyname.com");
            AppDelegate::mat()->callFuncWithParam("setUserEmail", &pUserEmail, NULL);
            
            PluginParam pUserId("123-456-789-0");
            AppDelegate::mat()->callFuncWithParam("setUserId", &pUserId, NULL);
            
            PluginParam pUserName("username");
            AppDelegate::mat()->callFuncWithParam("setUserName", &pUserName, NULL);
            
            PluginParam pSiteId("0000");
            AppDelegate::mat()->callFuncWithParam("setSiteId", &pSiteId, NULL);
            
            PluginParam pPackageName("com.company.appname");
            AppDelegate::mat()->callFuncWithParam("setPackageName", &pPackageName, NULL);
            
            PluginParam pAttr1("attribute1");
            AppDelegate::mat()->callFuncWithParam("setEventAttribute1", &pAttr1, NULL);
            
            PluginParam pAttr2("attribute2");
            AppDelegate::mat()->callFuncWithParam("setEventAttribute2", &pAttr2, NULL);
            
            PluginParam pAttr3("attribute3");
            AppDelegate::mat()->callFuncWithParam("setEventAttribute3", &pAttr3, NULL);
            
            PluginParam pAttr4("attribute4");
            AppDelegate::mat()->callFuncWithParam("setEventAttribute4", &pAttr4, NULL);
            
            PluginParam pAttr5("attribute5");
            AppDelegate::mat()->callFuncWithParam("setEventAttribute5", &pAttr5, NULL);
            
            PluginParam pFacebookUserId("temp_facebook_user_id");
            AppDelegate::mat()->callFuncWithParam("setFacebookUserId", &pFacebookUserId, NULL);

            PluginParam pGoogleUserId("temp_google_user_id");
            AppDelegate::mat()->callFuncWithParam("setGoogleUserId", &pGoogleUserId, NULL);

            PluginParam pTwitterUserId("temp_twitter_user_id");
            AppDelegate::mat()->callFuncWithParam("setTwitterUserId", &pTwitterUserId, NULL);
            
            PluginParam pLat(11.2f);
            PluginParam pLon(33.4f);
            PluginParam pAlt(55.6f);
            AppDelegate::mat()->callFuncWithParam("setLocation", &pLat, &pLon, &pAlt, NULL);
            
            PluginParam pGender(1); // 1 == female
            AppDelegate::mat()->callFuncWithParam("setGender", &pGender, NULL);
            
            PluginParam pCurrencyCode("AUD");
            AppDelegate::mat()->callFuncWithParam("setCurrencyCode", &pCurrencyCode, NULL);
            
            PluginParam pAppAdTracking(true);
            AppDelegate::mat()->callFuncWithParam("setAppAdTracking", &pAppAdTracking, NULL);
            
            PluginParam pRedirectUrl("http://www.example.com");
            AppDelegate::mat()->callFuncWithParam("setRedirectUrl", &pRedirectUrl, NULL);
            
            PluginParam pAutoDetectJailbroken(true);
            AppDelegate::mat()->callFuncWithParam("setShouldAutoDetectJailbroken", &pAutoDetectJailbroken, NULL);
            
            PluginParam pAutoGenerateAppleVendorId(true);
            AppDelegate::mat()->callFuncWithParam("setShouldAutoGenerateAppleVendorIdentifier", &pAutoGenerateAppleVendorId, NULL);
            
            PluginParam pAppToApp1("targetPackage");
            PluginParam pAppToApp2("targetAdvId");
            PluginParam pAppToApp3("targetOfferId");
            PluginParam pAppToApp4("targetPubId");
            PluginParam pAppToApp5(true);
            AppDelegate::mat()->callFuncWithParam("setTracking", &pAppToApp1, &pAppToApp2, &pAppToApp3, &pAppToApp4, &pAppToApp5, NULL);
            
            break;
        }
        case TAG_TEST_GETTERS:
        {
            std::string matId = AppDelegate::mat()->callStringFuncWithParam("getMatId", NULL);
            std::string openLogId = AppDelegate::mat()->callStringFuncWithParam("getOpenLogId", NULL);
            
            CCLOG("matId = %s", matId.c_str());
            CCLOG("openLogId = %s", openLogId.c_str());
        }
        default:
            break;
    }
}


void HelloWorld::menuCloseCallback(Object* pSender)
{
    PluginManager::end();
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

const char * HelloWorld::getDummyIAPReceipt()
{
    return "{\"signature\" = \"AiuR/oePW4lQq2qAFerYcL/lU7HB7rqPKoddrjnqLUqvLywbSukO3OUwWwiRGE8iFiNvaqVF2CaG8siBkfkP5KYaeiTHT2RNLCIKyCfhOIr4q0wYCKwxNp2vdo3S8b+4boeSAXzgzBHCR1S1hTN5LuoeRzDeIWHoYT66CBweqDetAAADVzCCA1MwggI7oAMCAQICCGUUkU3ZWAS1MA0GCSqGSIb3DQEBBQUAMH8xCzAJBgNVBAYTAlVTMRMwEQYDVQQKDApBcHBsZSBJbmMuMSYwJAYDVQQLDB1BcHBsZSBDZXJ0aWZpY2F0aW9uIEF1dGhvcml0eTEzMDEGA1UEAwwqQXBwbGUgaVR1bmVzIFN0b3JlIENlcnRpZmljYXRpb24gQXV0aG9yaXR5MB4XDTA5MDYxNTIyMDU1NloXDTE0MDYxNDIyMDU1NlowZDEjMCEGA1UEAwwaUHVyY2hhc2VSZWNlaXB0Q2VydGlmaWNhdGUxGzAZBgNVBAsMEkFwcGxlIGlUdW5lcyBTdG9yZTETMBEGA1UECgwKQXBwbGUgSW5jLjELMAkGA1UEBhMCVVMwgZ8wDQYJKoZIhvcNAQEBBQADgY0AMIGJAoGBAMrRjF2ct4IrSdiTChaI0g8pwv/cmHs8p/RwV/rt/91XKVhNl4XIBimKjQQNfgHsDs6yju++DrKJE7uKsphMddKYfFE5rGXsAdBEjBwRIxexTevx3HLEFGAt1moKx509dhxtiIdDgJv2YaVs49B0uJvNdy6SMqNNLHsDLzDS9oZHAgMBAAGjcjBwMAwGA1UdEwEB/wQCMAAwHwYDVR0jBBgwFoAUNh3o4p2C0gEYtTJrDtdDC5FYQzowDgYDVR0PAQH/BAQDAgeAMB0GA1UdDgQWBBSpg4PyGUjFPhJXCBTMzaN+mV8k9TAQBgoqhkiG92NkBgUBBAIFADANBgkqhkiG9w0BAQUFAAOCAQEAEaSbPjtmN4C/IB3QEpK32RxacCDXdVXAeVReS5FaZxc+t88pQP93BiAxvdW/3eTSMGY5FbeAYL3etqP5gm8wrFojX0ikyVRStQ+/AQ0KEjtqB07kLs9QUe8czR8UGfdM1EumV/UgvDd4NwNYxLQMg4WTQfgkQQVy8GXZwVHgbE/UC6Y7053pGXBk51NPM3woxhd3gSRLvXj+loHsStcTEqe9pBDpmG5+sk4tw+GK3GMeEN5/+e1QT9np/Kl1nj+aBw7C0xsy0bFnaAd1cSS6xdory/CUvM6gtKsmnOOdqTesbp0bs8sn6Wqs0C9dgcxRHuOMZ2tm8npLUm7argOSzQ==\";\"purchase-info\" = \"ewoJIm9yaWdpbmFsLXB1cmNoYXNlLWRhdGUtcHN0IiA9ICIyMDEzLTA2LTE5IDEzOjMyOjE5IEFtZXJpY2EvTG9zX0FuZ2VsZXMiOwoJInVuaXF1ZS1pZGVudGlmaWVyIiA9ICJjODU0OGI1YWExZjM5NDA2NmY1ZWEwM2Q3ZGU0YTBiYzdjMTEyZDk5IjsKCSJvcmlnaW5hbC10cmFuc2FjdGlvbi1pZCIgPSAiMTAwMDAwMDA3Nzk2MDgzNSI7CgkiYnZycyIgPSAiMS4xIjsKCSJ0cmFuc2FjdGlvbi1pZCIgPSAiMTAwMDAwMDA4MzE1MjA1NCI7CgkicXVhbnRpdHkiID0gIjEiOwoJIm9yaWdpbmFsLXB1cmNoYXNlLWRhdGUtbXMiID0gIjEzNzE2NzM5MzkwMDAiOwoJInVuaXF1ZS12ZW5kb3ItaWRlbnRpZmllciIgPSAiQTM3MjFCQzctNDA3Qi00QzcyLTg4RDAtMTdGNjIwRUMzNzEzIjsKCSJwcm9kdWN0LWlkIiA9ICJjb20uaGFzb2ZmZXJzLmluYXBwcHVyY2hhc2V0cmFja2VyMS5iYWxsIjsKCSJpdGVtLWlkIiA9ICI2NTMyMzA4MjkiOwoJImJpZCIgPSAiY29tLkhhc09mZmVycy5JbkFwcFB1cmNoYXNlVHJhY2tlcjEiOwoJInB1cmNoYXNlLWRhdGUtbXMiID0gIjEzNzU4MTM2NDcxMDIiOwoJInB1cmNoYXNlLWRhdGUiID0gIjIwMTMtMDgtMDYgMTg6Mjc6MjcgRXRjL0dNVCI7CgkicHVyY2hhc2UtZGF0ZS1wc3QiID0gIjIwMTMtMDgtMDYgMTE6Mjc6MjcgQW1lcmljYS9Mb3NfQW5nZWxlcyI7Cgkib3JpZ2luYWwtcHVyY2hhc2UtZGF0ZSIgPSAiMjAxMy0wNi0xOSAyMDozMjoxOSBFdGMvR01UIjsKfQ==\";\"environment\" = \"Sandbox\";\"pod\" = \"100\";\"signing-status\" = \"0\";}";
}