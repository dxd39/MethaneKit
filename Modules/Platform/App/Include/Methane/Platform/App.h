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

FILE: Methane/Platform/App.h
Methane platform application alias.

******************************************************************************/

#pragma once

#if defined _WIN32

#include <Methane/Platform/Windows/AppWin.h>

#elif defined __APPLE__

#ifdef APPLE_MACOS
#include <Methane/Platform/MacOS/AppMac.hh>
#else
#include <Methane/Platform/iOS/AppIOS.hh>
#endif

#elif __ANDROID__

#include <Methane/Platform/Android/AppAndroid.h>

#elif defined __linux__

#include <Methane/Platform/Linux/AppLin.h>

#endif

namespace Methane::Platform
{

#if defined _WIN32

using App = AppWin;

#elif defined __APPLE__

#ifdef APPLE_MACOS
using App = AppMac;
#else
using App = AppIOS;
#endif

#elif __ANDROID__

using App = AppAndroid;

#elif defined __linux__

using App = AppLin;

#endif

} // namespace Methane::Platform
