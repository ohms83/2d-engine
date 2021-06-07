//
//  NativeUI.mm
//  MiniRPG
//
//  Created by nuttachai on 12/12/19.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#include "../NativeUI.h"

void platform::showNativePopup(const std::string& title, const std::string& message)
{
#ifndef UNIT_TEST
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle: [NSString stringWithUTF8String: title.c_str() ]
                                                    message: [NSString stringWithUTF8String: message.c_str() ]
                                                   delegate:nil
                                          cancelButtonTitle:@"OK"
                                          otherButtonTitles:nil];
    [alert show];
    [alert release];
#else
    assert(false);
#endif
}
