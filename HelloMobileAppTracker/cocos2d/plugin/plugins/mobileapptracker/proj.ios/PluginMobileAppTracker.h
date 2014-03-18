//
//  PluginMobileAppTracker.h
//  PluginMobileAppTracker
//
//  Created by Harshal Ogale on 11/20/13.
//  Copyright (c) 2013 HasOffers Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "InterfaceAnalytics.h"

@interface PluginMobileAppTracker : NSObject <InterfaceAnalytics>


#pragma mark - MAT initialization method

- (void)startTracker:(NSMutableDictionary *)params;

#pragma mark - MAT event measurement methods

- (void)measureSession;
- (void)measureAction:(NSMutableDictionary *)params;
- (void)measureActionWithRefId:(NSMutableDictionary *)params;
- (void)measureActionWithRevenue:(NSMutableDictionary *)params;
- (void)measureActionWithEventItems:(NSMutableDictionary *)params;
- (void)measureActionWithIAPReceipt:(NSMutableDictionary *)params;

#pragma mark - MAT app-to-app tracking method

- (void)startAppToAppTracking:(NSMutableDictionary *)trackingInfo;

#pragma mark - MAT Debugging methods

- (void)setDebugMode:(NSNumber *)yesorno;
- (void)setAllowDuplicates:(NSNumber *)yesorno;
- (void)setDelegate:(NSNumber *)yesorno;

#pragma mark - MAT Setter methods

- (void)setAppleAdvertisingIdentifier:(NSMutableDictionary *)ifaAndTrackingEnabled;
- (void)setAppleVendorIdentifier:(NSString *)ifv;
- (void)setAge:(NSNumber *)age;
- (void)setJailbroken:(NSNumber *)jailbroken;
- (void)setTRUSTeId:(NSString *)tpid;
- (void)setUserEmail:(NSString *)userEmail;
- (void)setUserId:(NSString *)userId;
- (void)setUserName:(NSString *)userName;
- (void)setSiteId:(NSString *)siteId;
- (void)setPackageName:(NSString *)packageName;
- (void)setLocation:(NSMutableDictionary *)location;
- (void)setGender:(NSNumber *)gender;
- (void)setCurrencyCode:(NSString *)currencyCode;
- (void)setAppAdTracking:(NSNumber *)yesorno;
- (void)setRedirectUrl:(NSString *)redirectUrl;
- (void)setShouldAutoDetectJailbroken:(NSNumber *)yesorno;
- (void)setShouldAutoGenerateAppleVendorIdentifier:(NSNumber *)yesorno;
- (void)setExistingUser:(NSNumber *)yesorno;
- (void)setFacebookUserId:(NSString *)userId;
- (void)setGoogleUserId:(NSString *)userId;
- (void)setTwitterUserId:(NSString *)userId;

- (void)setEventAttribute1:(NSString *)attr;
- (void)setEventAttribute2:(NSString *)attr;
- (void)setEventAttribute3:(NSString *)attr;
- (void)setEventAttribute4:(NSString *)attr;
- (void)setEventAttribute5:(NSString *)attr;

// Android-only method, no-op in iOS
- (void)setGoogleAdvertisingId:(NSString *)gid;

#pragma mark - MAT Getter methods

- (NSString*)getMatId;
- (NSString*)getOpenLogId;

#pragma mark - InterfaceAnalytics methods

//- (void) startSession: (NSString*) appKey;
//- (void) stopSession;
//- (void) setSessionContinueMillis: (long) millis;
//- (void) setCaptureUncaughtException: (BOOL) isEnabled;
//- (void) setDebugMode: (BOOL) debug;
//- (void) logError: (NSString*) errorId withMsg:(NSString*) message;
//- (void) logEvent: (NSString*) eventId;
//- (void) logEvent: (NSString*) eventId withParam:(NSMutableDictionary*) paramMap;
//- (void) logTimedEventBegin: (NSString*) eventId;
//- (void) logTimedEventEnd: (NSString*) eventId;
//- (NSString*) getSDKVersion;
//- (NSString*) getPluginVersion;

@end