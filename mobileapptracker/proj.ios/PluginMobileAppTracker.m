//
//  PluginMobileAppTracker.m
//  PluginMobileAppTracker
//
//  Created by Harshal Ogale on 11/20/13.
//  Copyright (c) 2013 HasOffers Inc. All rights reserved.
//

#import "PluginMobileAppTracker.h"
#import "MobileAppTracker.h"

@interface MATSDKDelegate : NSObject <MobileAppTrackerDelegate>

@end


@implementation MATSDKDelegate

#pragma mark - MobileAppTrackerDelegate Methods

- (void)mobileAppTrackerDidSucceedWithData:(NSData *)data
{
    NSString * dataString = [[NSString alloc] initWithData:data encoding:NSASCIIStringEncoding];
    
    NSLog(@"MATPlugin succeeded: %@", dataString);
}

- (void)mobileAppTrackerDidFailWithError:(NSError *)error
{
    NSInteger errorCode = [error code];
    NSString *errorDescr = [error localizedDescription];
    
    NSString *errorURLString = nil;
    NSDictionary *dictError = [error userInfo];
    
    if(dictError)
    {
        errorURLString = [dictError objectForKey:NSURLErrorFailingURLStringErrorKey];
    }
    
    errorURLString = nil == error ? @"" : errorURLString;
    
    NSString *strError = [NSString stringWithFormat:@"{\"code\":\"%ld\",\"localizedDescription\":\"%@\",\"failedURL\":\"%@\"}", (long)errorCode, errorDescr, errorURLString];
    
    NSLog(@"MATPlugin failed: %@", strError);
}

@end

@implementation PluginMobileAppTracker

MATSDKDelegate *matDelegate = nil;

#pragma mark - MAT initialization method

- (void)startTracker:(NSMutableDictionary *)params
{
    NSString* advId = (NSString *) [params objectForKey:@"Param1"];
    NSString* convKey = (NSString *) [params objectForKey:@"Param2"];
    
    [MobileAppTracker initializeWithMATAdvertiserId:advId MATConversionKey:convKey];
    [MobileAppTracker setPluginName:@"cocos2dx"];
}

#pragma mark - MAT event tracking method

- (void)measureSession
{
    [MobileAppTracker measureSession];
}


- (void)measureAction:(NSString *)eventName
{
    // count of params that precede the eventItem params
    const int COUNT_FIXED_PARAMS = 1;
    
    [self measureActionInternal:[NSMutableDictionary dictionaryWithObject:eventName forKey:@"Param1"] countOfFixedParams:COUNT_FIXED_PARAMS];
}

- (void)measureActionWithRefId:(NSMutableDictionary *)params
{
    // count of params that precede the eventItem params
    const int COUNT_FIXED_PARAMS = 2;
    
    [self measureActionInternal:params countOfFixedParams:COUNT_FIXED_PARAMS];
}

- (void)measureActionWithRevenue:(NSMutableDictionary *)params
{
    // count of params that precede the eventItem params
    const int COUNT_FIXED_PARAMS = 4;
    
    [self measureActionInternal:params countOfFixedParams:COUNT_FIXED_PARAMS];
}

- (void)measureActionWithEventItems:(NSMutableDictionary *)params
{
    // count of params that precede the eventItem params
    const int COUNT_FIXED_PARAMS = 4;
    
    [self measureActionInternal:params countOfFixedParams:COUNT_FIXED_PARAMS];
}

- (void)measureActionWithIAPReceipt:(NSMutableDictionary *)params
{
    // count of params that precede the eventItem params
    const int COUNT_FIXED_PARAMS = 7;
    
    [self measureActionInternal:params countOfFixedParams:COUNT_FIXED_PARAMS];
}

- (void)measureActionInternal:(NSMutableDictionary *)params countOfFixedParams:(NSUInteger)countFixedParams
{
    NSString* eventIdOrName = (NSString *) [params objectForKey:@"Param1"];
    
    NSString* refId = nil;
    
    if(2 <= countFixedParams && [params objectForKey:@"Param2"])
    {
        refId = (NSString *) [params objectForKey:@"Param2"];
    }
    
    NSNumber *numRevenue = nil;
    NSString *currencyCode = nil;
    
    float revenue = 0.0f;
    
    if(4 <= countFixedParams)
    {
        if([params objectForKey:@"Param3"])
        {
            numRevenue = (NSNumber *) [params objectForKey:@"Param3"];
            
            revenue = [numRevenue floatValue];
        }
        
        if([params objectForKey:@"Param4"])
        {
            currencyCode = (NSString *) [params objectForKey:@"Param4"];
        }
    }
    
    NSNumber *numTransactionState = nil;
    NSString *strReceipt = nil;
    
    int transactionState = -1;
    NSData *receipt = nil;
    
    if(7 == countFixedParams)
    {
        if([params objectForKey:@"Param5"])
        {
            numTransactionState = (NSNumber *) [params objectForKey:@"Param5"];
            transactionState = [numTransactionState intValue];
        }
        
        if([params objectForKey:@"Param6"])
        {
            strReceipt = (NSString *) [params objectForKey:@"Param6"];
            receipt = [strReceipt dataUsingEncoding:NSUTF8StringEncoding];
        }
        
        // ignore @"Param7" used for receiptSignature in Android
    }
    
    NSArray *eventItems = nil;
    if(4 <= countFixedParams)
    {
        eventItems = [self convertToMATEventItems:params countOfFixedParams:countFixedParams];
    }
    
    if(7 == countFixedParams)
    {
        [MobileAppTracker measureAction:eventIdOrName eventItems:eventItems referenceId:refId revenueAmount:revenue currencyCode:currencyCode transactionState:transactionState receipt:receipt];
    }
    else
    {
        [MobileAppTracker measureAction:eventIdOrName eventItems:eventItems referenceId:refId revenueAmount:revenue currencyCode:currencyCode];
    }
}

#pragma mark - MAT Debugging methods

- (void)setDebugMode:(NSNumber *)yesorno
{
    BOOL enable = [yesorno boolValue];
    
    [MobileAppTracker setDebugMode:enable];
}

- (void)setAllowDuplicates:(NSNumber *)yesorno
{
    BOOL enable = [yesorno boolValue];
    
    [MobileAppTracker setAllowDuplicateRequests:enable];
}

- (void)setDelegate:(NSNumber *)yesorno
{
    BOOL enable = [yesorno boolValue];
    
    if(enable && !matDelegate)
    {
        matDelegate = [[MATSDKDelegate alloc] init];
    }
    
    [MobileAppTracker setDelegate:enable ? matDelegate : nil];
}

#pragma mark - MAT Setter methods

- (void)setAppleAdvertisingIdentifier:(NSMutableDictionary *)ifaAndTrackingEnabled
{
    id classNSUUID = NSClassFromString(@"NSUUID");
    
    if(classNSUUID)
    {
        NSString* ifa = (NSString *) [ifaAndTrackingEnabled objectForKey:@"Param1"];
        NSNumber* numEnabled = (NSNumber *) [ifaAndTrackingEnabled objectForKey:@"Param2"];
        
        BOOL trackingEnabled = [numEnabled boolValue];
        
        [MobileAppTracker setAppleAdvertisingIdentifier:[[classNSUUID alloc] initWithUUIDString:ifa] advertisingTrackingEnabled:trackingEnabled];
    }
}

- (void)setAppleVendorIdentifier:(NSString *)ifv
{
    id classNSUUID = NSClassFromString(@"NSUUID");
    
    if(classNSUUID)
    {
        [MobileAppTracker setAppleVendorIdentifier:[[classNSUUID alloc] initWithUUIDString:ifv]];
    }
}

- (void)setAge:(NSNumber *)age
{
    NSInteger intAge = [age integerValue];
    
    [MobileAppTracker setAge:intAge];
}

- (void)setJailbroken:(NSNumber *)jailbroken
{
    BOOL isJailbroken = [jailbroken boolValue];
    
    [MobileAppTracker setJailbroken:isJailbroken];
}

- (void)setTRUSTeId:(NSString *)tpid
{
    [MobileAppTracker setTRUSTeId:tpid];
}

- (void)setUserEmail:(NSString *)userEmail
{
    [MobileAppTracker setUserEmail:userEmail];
}

- (void)setUserId:(NSString *)userId
{
    [MobileAppTracker setUserId:userId];
}

- (void)setUserName:(NSString *)userName
{
    [MobileAppTracker setUserName:userName];
}

- (void)setSiteId:(NSString *)siteId
{
    [MobileAppTracker setSiteId:siteId];
}

- (void)setPackageName:(NSString *)packageName
{
    [MobileAppTracker setPackageName:packageName];
}

- (void)setFacebookUserId:(NSString *)userId
{
    [MobileAppTracker setFacebookUserId:userId];
}

- (void)setGoogleUserId:(NSString *)userId
{
    [MobileAppTracker setGoogleUserId:userId];
}

- (void)setTwitterUserId:(NSString *)userId
{
    [MobileAppTracker setTwitterUserId:userId];
}

- (void)setLocation:(NSMutableDictionary *)location
{
    NSNumber* numLat = (NSNumber *) [location objectForKey:@"Param1"];
    NSNumber* numLon = (NSNumber *) [location objectForKey:@"Param2"];
    NSNumber* numAlt = (NSNumber *) [location objectForKey:@"Param3"];
    
    double lat = [numLat doubleValue];
    double lon = [numLon doubleValue];
    double alt = [numAlt doubleValue];
    
    if(numAlt)
    {
        [MobileAppTracker setLatitude:lat longitude:lon altitude:alt];
    }
    else
    {
        [MobileAppTracker setLatitude:lat longitude:lon];
    }
}

- (void)setGender:(NSNumber *)gender
{
    int nGender = [gender intValue];
    MATGender matGender = 0 == nGender ? MAT_GENDER_MALE : MAT_GENDER_FEMALE;
    
    [MobileAppTracker setGender:matGender];
}

- (void)setCurrencyCode:(NSString *)currencyCode
{
    [MobileAppTracker setCurrencyCode:currencyCode];
}

- (void)setEventAttribute1:(NSString *)attr
{
    [MobileAppTracker setEventAttribute1:attr];
}

- (void)setEventAttribute2:(NSString *)attr
{
    [MobileAppTracker setEventAttribute2:attr];
}

- (void)setEventAttribute3:(NSString *)attr
{
    [MobileAppTracker setEventAttribute3:attr];
}

- (void)setEventAttribute4:(NSString *)attr
{
    [MobileAppTracker setEventAttribute4:attr];
}

- (void)setEventAttribute5:(NSString *)attr
{
    [MobileAppTracker setEventAttribute5:attr];
}

- (void)setExistingUser:(NSNumber *)yesorno
{
    BOOL isExisting = [yesorno boolValue];
    
    [MobileAppTracker setExistingUser:isExisting];
}

- (void)setAppAdTracking:(NSNumber *)yesorno
{
    BOOL enable = [yesorno boolValue];
    
    [MobileAppTracker setAppAdTracking:enable];
}

- (void)setRedirectUrl:(NSString *)redirectUrl
{
    [MobileAppTracker setRedirectUrl:redirectUrl];
}

- (void)setShouldAutoDetectJailbroken:(NSNumber *)yesorno
{
    BOOL enable = [yesorno boolValue];
    
    [MobileAppTracker setShouldAutoDetectJailbroken:enable];
}

- (void)setShouldAutoGenerateAppleVendorIdentifier:(NSNumber *)yesorno
{
    BOOL enable = [yesorno boolValue];
    
    [MobileAppTracker setShouldAutoGenerateAppleVendorIdentifier:enable];
}

- (void)startAppToAppTracking:(NSMutableDictionary *)trackingInfo
{
    NSString* targetPackageName = (NSString *) [trackingInfo objectForKey:@"Param1"];
    NSString* targetAdvId = (NSString *) [trackingInfo objectForKey:@"Param2"];
    NSString* targetOfferId = (NSString *) [trackingInfo objectForKey:@"Param3"];
    NSString* targetPublisherId = (NSString *) [trackingInfo objectForKey:@"Param4"];
    
    NSNumber* numRedirect = (NSNumber *) [trackingInfo objectForKey:@"Param5"];
    BOOL redirect = [numRedirect boolValue];
    
    [MobileAppTracker startAppToAppTracking:targetPackageName advertiserId:targetAdvId offerId:targetOfferId publisherId:targetPublisherId redirect:redirect];
}

#pragma mark - MAT Getter methods

- (NSString*)getMatId
{
    return [MobileAppTracker matId];
}

- (NSString*)getOpenLogId
{
    return [MobileAppTracker openLogId];
}

#pragma mark - Helper Methods

- (NSArray *)convertToMATEventItems:(NSMutableDictionary *)params countOfFixedParams:(NSUInteger)countFixedParams
{
    NSMutableArray *arrItems = [NSMutableArray array];
    
    NSUInteger start = countFixedParams;
    NSUInteger end = [params count];
    
    for(NSUInteger i = start; i < end ; ++i)
    {
        // dictionary of eventItem dictionaries
        NSMutableDictionary* dictItem = (NSMutableDictionary *) [params objectForKey:[NSString stringWithFormat:@"Param%u", (unsigned int)(i + 1)]];
        
        MATEventItem *item = [self matEventItemFromDictionary:dictItem];
        
        [arrItems addObject:item];
    }
    
    return arrItems;
}

- (MATEventItem *)matEventItemFromDictionary:(NSDictionary *)dict
{
    MATEventItem *item = nil;
    
    NSString *name = [dict valueForKey:@"item"];
    NSString *strUnitPrice = [dict valueForKey:@"unit_price"];
    float unitPrice = [NSNull null] == (id)strUnitPrice ? 0 : [strUnitPrice floatValue];
    
    NSString *strQuantity = [dict valueForKey:@"quantity"];
    int quantity = [NSNull null] == (id)strQuantity ? 0 : [strQuantity intValue];
    
    NSString *strRevenue = [dict valueForKey:@"revenue"];
    float revenue = [NSNull null] == (id)strRevenue ? 0 : [strRevenue floatValue];
    
    NSString *attribute1 = [dict valueForKey:@"attribute1"];
    NSString *attribute2 = [dict valueForKey:@"attribute2"];
    NSString *attribute3 = [dict valueForKey:@"attribute3"];
    NSString *attribute4 = [dict valueForKey:@"attribute4"];
    NSString *attribute5 = [dict valueForKey:@"attribute5"];
    
    item = [MATEventItem eventItemWithName:name
                                 unitPrice:unitPrice
                                  quantity:quantity
                                   revenue:revenue
                                attribute1:attribute1
                                attribute2:attribute2
                                attribute3:attribute3
                                attribute4:attribute4
                                attribute5:attribute5];
    
    return item;
}

#pragma mark - Android-only methods

- (void)setGoogleAdvertisingId:(NSString *)gid
{
    // Android-only method, no-op in iOS
}

#pragma mark - InterfaceAnalytics methods - Used Methods

- (NSString*)getSDKVersion
{
    return @"3.0.6";
}

- (NSString*)getPluginVersion
{
    return @"3.0.6";
}

#pragma mark - InterfaceAnalytics methods - Ignored Methods

- (void)startSession:(NSString*)appKey
{
    // no-op
}

- (void)stopSession
{
    // no-op
}

- (void)setSessionContinueMillis:(long)millis
{
    // no-op
}

- (void)setCaptureUncaughtException:(BOOL)isEnabled
{
    // no-op
}

- (void)logError: (NSString*) errorId withMsg:(NSString*) message;
{
    // no-op
}

- (void)logEvent: (NSString*) eventId
{
    // no-op
}

- (void)logEvent: (NSString*) eventId withParam:(NSMutableDictionary*) paramMap
{
    // no-op
}

- (void)logTimedEventBegin: (NSString*) eventId
{
    // no-op
}

- (void)logTimedEventEnd: (NSString*) eventId
{
    // no-op
}

@end
