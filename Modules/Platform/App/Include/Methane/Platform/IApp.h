/******************************************************************************

Copyright 2021 Evgeny Gorodetskiy

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

FILE: Methane/Platform/IApp.h
Platform application interface.

******************************************************************************/

#pragma once

#include <Methane/Data/Types.h>
#include <string>

struct android_app;

namespace Methane::Data
{
    struct IProvider;
}

namespace Methane::Platform
{

struct AppSettings
{
    std::string      name;
    Data::FloatSize  size     { 0.8F, 0.8F};   // if dimension < 1.0 use as ratio of desktop size; else use as exact size in pixels/dots
    Data::FrameSize  min_size { 640U, 480U };
    bool             is_full_screen = false;
    Data::IProvider* icon_provider  = nullptr;

    AppSettings& SetName(std::string&& new_name) noexcept;
    AppSettings& SetSize(Data::FloatSize&& new_size) noexcept;
    AppSettings& SetMinSize(Data::FrameSize&& new_min_size) noexcept;
    AppSettings& SetFullScreen(bool new_full_screen) noexcept;
    AppSettings& SetIconProvider(Data::IProvider* new_icon_provider) noexcept;
};

struct AppRunArgs
{
#ifdef __ANDROID__
    android_app* app = nullptr;
#else
    int          cmd_arg_count  = 0;
    const char** cmd_arg_values = nullptr;
#endif
};

struct AppMessage
{
    enum class Type : uint32_t
    {
        Information = 0,
        Warning,
        Error
    };

    Type        type = Type::Information;
    std::string title;
    std::string information;
};

struct AppEnvironment;

struct IApp
{
public:
    using Settings = AppSettings;
    using RunArgs  = AppRunArgs;
    using Message  = AppMessage;

    // IApp interface
    virtual int      Run(const RunArgs& args) = 0;
    virtual void     InitContext(const AppEnvironment& env, const Data::FrameSize& frame_size) = 0;
    virtual void     Init() = 0;
    virtual void     ChangeWindowBounds(const Data::FrameRect& window_bounds) = 0;
    virtual void     StartResizing() = 0;
    virtual void     EndResizing() = 0;
    virtual bool     Resize(const Data::FrameSize& frame_size, bool is_minimized) = 0;
    virtual bool     Update() = 0;
    virtual bool     Render() = 0;
    virtual void     Alert(const Message& msg, bool deferred = false) = 0;
    virtual void     SetWindowTitle(const std::string& title_text) = 0;
    virtual bool     SetFullScreen(bool is_full_screen) = 0;
    virtual bool     SetKeyboardFocus(bool has_keyboard_focus) = 0;
    virtual void     ShowControlsHelp() = 0;
    virtual void     ShowCommandLineHelp() = 0;
    virtual void     ShowParameters() = 0;
    virtual float    GetContentScalingFactor() const = 0;
    virtual uint32_t GetFontResolutionDpi() const = 0;
    virtual void     Close() = 0;

    virtual ~IApp() = default;

};

} // namespace Methane::Platform
