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

FILE: Methane/Platform/Android/Utils.cpp
Android platform utility functions.

******************************************************************************/

#include <Methane/Platform/Android/Utils.h>
#include <Methane/Instrumentation.h>

#include <string_view>

#include <android/log.h>

namespace Methane::Platform
{

static const char* log_tag = "Methane";

void PrintToDebugOutput(std::string_view msg)
{
    META_FUNCTION_TASK();
    __android_log_print(ANDROID_LOG_DEBUG, log_tag, "%s", msg.data());
    TracyMessage(msg.data(), msg.size());
}

std::string GetExecutableDir()
{
    return "";
}

std::string GetExecutableFileName()
{
    return "";
}

std::string GetResourceDir()
{
    return "";
}

} // namespace Methane::Platform
