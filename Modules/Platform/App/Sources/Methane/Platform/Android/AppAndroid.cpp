/******************************************************************************

Copyright 2019-2020 Evgeny Gorodetskiy

Licensed under the Apache License, Version 2.0 (the "License"),
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************

FILE: Methane/Platform/Android/AppAndroid.cpp
Android application implementation.

******************************************************************************/

#include <Methane/Platform/Android/AppAndroid.h>
#include <Methane/Platform/Utils.h>
#include <Methane/Instrumentation.h>

#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <game-activity/native_app_glue/android_native_app_glue.h>

namespace Methane::Platform
{

AppAndroid::AppAndroid(const AppBase::Settings& settings)
    : AppBase(settings)
{
    META_FUNCTION_TASK();
}

AppAndroid::~AppAndroid()
{
}

int AppAndroid::Run(const RunArgs& args)
{
    android_app* app = args.app;
    app->userData = this;
    app->onAppCmd = HandleCmd;

    m_android_app = app;

    android_app_set_key_event_filter(app, NULL);
    android_app_set_motion_event_filter(app, NULL);

    while (true)
    {
        // Read all pending events.
        int                  id;
        int                  events;
        android_poll_source* source;

        // Block forever waiting for events.
        while ((id = ALooper_pollAll(m_env.window ? 0 : -1, NULL, &events, (void**)&source)) >= 0)
        {
            // Process this event.
            if (source != NULL)
            {
                source->process(app, source);
            }

            // Check if we are exiting.
            if (app->destroyRequested != 0)
            {
                return 0;
            }
        }

        HandleEvent(app);

        if (m_is_active)
        {
            // If there's a deferred message, schedule it to show for the current window event loop
            if (HasDeferredMessage())
            {
                ShowAlert(GetDeferredMessage());
                ResetDeferredMessage();
            }

            UpdateAndRenderWithErrorHandling();
        }
        
    }
    return 0;
}

void AppAndroid::Alert(const Message& msg, bool deferred)
{
    META_FUNCTION_TASK();
    AppBase::Alert(msg, deferred);
    if (!deferred)
    {
        ShowAlert(msg);
    }
}

void AppAndroid::SetWindowTitle([[maybe_unused]]const std::string& title_text)
{
}

bool AppAndroid::SetFullScreen(bool is_full_screen)
{
    META_FUNCTION_TASK();
    if (!AppBase::SetFullScreen(is_full_screen))
        return false;

    return true;
}

float AppAndroid::GetContentScalingFactor() const
{
    if (m_android_app)
    {
        return AConfiguration_getDensity(m_android_app->config) / static_cast<float>(ACONFIGURATION_DENSITY_MEDIUM);
    }
    return 1.0f;
}

uint32_t AppAndroid::GetFontResolutionDpi() const
{
    return 72U * GetContentScalingFactor();
}

void AppAndroid::Close()
{
    
}

void AppAndroid::ShowAlert(const Message& msg)
{
    JNIEnv *env;
    m_android_app->activity->vm->AttachCurrentThread(&env, NULL);
    jclass cls = env->GetObjectClass(m_android_app->activity->javaGameActivity);
    jmethodID j_showAlert = env->GetMethodID(cls, "showAlert", "(Ljava/lang/String;)V");
    env->CallVoidMethod(m_android_app->activity->javaGameActivity, j_showAlert, env->NewStringUTF(msg.title.c_str()), env->NewStringUTF(msg.information.c_str()));
    m_android_app->activity->vm->DetachCurrentThread();
}

void AppAndroid::HandleEvent(android_app* app)
{
    auto input_buffers = android_app_swap_input_buffers(app);
    if (!input_buffers)
    {
        return;
    }

    // Just Clear now
    if (input_buffers->motionEventsCount)
    {
        android_app_clear_motion_events(input_buffers);
    }
    
    if (input_buffers->keyEventsCount)
    {
        android_app_clear_key_events(input_buffers);
    }

}

void AppAndroid::HandleCmd(android_app* app, int32_t cmd)
{
    auto kit = (AppAndroid*)(app->userData);
    switch (cmd)
    {
        case APP_CMD_INIT_WINDOW:
        {
            META_LOG("app state changed: APP_CMD_INIT_WINDOW");

            kit->m_env.window = app->window;
            if (!kit->IsInitialized())
            {
                const Data::FrameSize frame_size(
                    static_cast<uint32_t>(ANativeWindow_getWidth(app->window)),
                    static_cast<uint32_t>(ANativeWindow_getHeight(app->window))
                );

                // Application Initialization
                if (kit->InitContextWithErrorHandling(kit->m_env, frame_size)
                    && kit->InitWithErrorHandling())
                {
                    kit->m_is_active = true;
                }
            }
            else 
            {
                kit->Resume(kit->m_env);
                kit->m_is_active = true;
            }
            
            break;
        }
        case APP_CMD_TERM_WINDOW:
        {
            META_LOG("app state changed: APP_CMD_TERM_WINDOW");
            kit->m_is_active = false;
            break;
        }
        case APP_CMD_WINDOW_RESIZED:
        {
            META_LOG("app state changed: APP_CMD_WINDOW_RESIZED");
            // now has bug here
            const Data::FrameSize frame_size(
                static_cast<uint32_t>(ANativeWindow_getWidth(app->window)),
                static_cast<uint32_t>(ANativeWindow_getHeight(app->window))
            );
            kit->Resize(frame_size, false);
            break;
        }
        case APP_CMD_WINDOW_REDRAW_NEEDED:
            META_LOG("app state changed: APP_CMD_WINDOW_REDRAW_NEEDED");
            break;
        case APP_CMD_CONTENT_RECT_CHANGED:
            META_LOG("app state changed: APP_CMD_CONTENT_RECT_CHANGED");
            // Get the new size
            break;
        case APP_CMD_GAINED_FOCUS:
            META_LOG("app state changed: APP_CMD_GAINED_FOCUS");
            break;
        case APP_CMD_LOST_FOCUS:
            META_LOG("app state changed: APP_CMD_LOST_FOCUS");
            break;
        case APP_CMD_CONFIG_CHANGED:
            META_LOG("app state changed: APP_CMD_CONFIG_CHANGED");
            break;
        case APP_CMD_LOW_MEMORY:
            META_LOG("app state changed: APP_CMD_LOW_MEMORY");
            break;
        case APP_CMD_START:
            META_LOG("app state changed: APP_CMD_START");
            break;
        case APP_CMD_RESUME:
            META_LOG("app state changed: APP_CMD_RESUME");
            break;
        case APP_CMD_SAVE_STATE:
            META_LOG("app state changed: APP_CMD_SAVE_STATE");
            break;
        case APP_CMD_PAUSE:
            META_LOG("app state changed: APP_CMD_PAUSE");
            kit->m_is_active = false;
            break;
        case APP_CMD_STOP:
            META_LOG("app state changed: APP_CMD_STOP");
            break;
        case APP_CMD_DESTROY:
            META_LOG("app state changed: APP_CMD_DESTROY");
            break;
        case APP_CMD_WINDOW_INSETS_CHANGED:
            META_LOG("app state changed: APP_CMD_WINDOW_INSETS_CHANGED");
            break;
        default:
            break;
    }
}

} // namespace Methane::Platform