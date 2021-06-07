//
//  NativeUI.mm
//  MiniRPG
//
//  Created by nuttachai on 12/12/19.
//

#include "../NativeUI.h"
#include "jni/JniHelper.h"

void platform::showNativePopup(const std::string& title, const std::string& message)
{
#ifndef UNIT_TEST
     cocos2d::JniMethodInfo t;

     if ( cocos2d::JniHelper::getStaticMethodInfo( t, "ohms/engine/platform/NativeUI", "showNativePopup",
             "(Landroid/app/Activity;Ljava/lang/String;Ljava/lang/String;)V" ) )
     {
         jobject activity = cocos2d::JniHelper::getActivity();
         jstring jniText = t.env->NewStringUTF( message.c_str() );
         jstring jniTitle = t.env->NewStringUTF( title.c_str() );
         t.env->CallStaticVoidMethod( t.classID, t.methodID, activity, jniTitle, jniText);
         t.env->DeleteLocalRef(jniText);
         t.env->DeleteLocalRef(jniTitle);
         t.env->DeleteLocalRef(t.classID);
     }
#else
    assert(false);
#endif
}
