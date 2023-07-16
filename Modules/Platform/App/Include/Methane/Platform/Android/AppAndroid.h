/******************************************************************************

Copyright 2019-2021 Evgeny Gorodetskiy

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

FILE: Methane/Platform/Android/AppLin.h
Android application implementation.

******************************************************************************/

#pragma once

#include <Methane/Platform/AppBase.h>
#include <Methane/Platform/AppEnvironment.h>

struct android_app;
struct AInputEvent;

namespace Methane::Platform
{

class AppAndroid : public AppBase
{
public:
    explicit AppAndroid(const Settings& settings);
    ~AppAndroid() override;

    // AppBase interface
    int  Run(const RunArgs& args) override;
    void Alert(const Message& msg, bool deferred = false) override;
    void SetWindowTitle(const std::string& title_text) override;
    bool SetFullScreen(bool is_full_screen) override;
    float GetContentScalingFactor() const override;
    uint32_t GetFontResolutionDpi() const override;
    void Close() override;

protected:
    // AppBase interface
    void ShowAlert(const Message& msg) override;

    void HandleEvent(android_app* app);

    static void HandleCmd(android_app* app, int32_t cmd);

private:
    AppEnvironment m_env;
    android_app*   m_android_app;
    bool           m_is_active = false;
};

} // namespace Methane::Platform