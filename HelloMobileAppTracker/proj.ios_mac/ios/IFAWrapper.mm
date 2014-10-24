//
//  IFAWrapper.mm
//  HelloMobileAppTracker
//
//  Created by HasOffers Inc on 4/10/14.
//
//

#import "IFAWrapper.h"
#import <AdSupport/AdSupport.h>

namespace IFAWrapperiOS
{
    const char* IFAWrapper::advertisingIdentifier()
    {
        return [[[[ASIdentifierManager sharedManager] advertisingIdentifier] UUIDString] UTF8String];
    }
    
    bool IFAWrapper::isAdvertisingTrackingEnabled()
    {
        return [[ASIdentifierManager sharedManager] isAdvertisingTrackingEnabled];
    }
}