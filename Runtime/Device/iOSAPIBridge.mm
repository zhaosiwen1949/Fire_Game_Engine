#import "iOSAPIBridge.h"
#import <Foundation/Foundation.h>
#ifdef ALICE_IPHONE
#import <UIKit/UIKit.h>
namespace  Alice{
    void iOSAPIBridge::KeeyScreenOn(bool bOn){
        if(bOn){
            [[UIApplication sharedApplication] setIdleTimerDisabled:YES];//tell device do not sleep
        }else{
            [[UIApplication sharedApplication] setIdleTimerDisabled:NO];//tell device do not sleep
        }
    }
}
#endif
