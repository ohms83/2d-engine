list(APPEND ENGINE_HEADER
    engine/platform/NativeUI.h
)

if(ANDROID)
    list(APPEND ENGINE_SOURCE
        engine/platform/android/NativeUI.android.cpp
    )
elseif(APPLE)
    if(IOS)
        list(APPEND ENGINE_SOURCE
            engine/platform/ios/NativeUI.ios.mm
        )
    else()
        list(APPEND ENGINE_SOURCE
            engine/platform/mac/NativeUI.mac.mm
        )
    endif()
endif()