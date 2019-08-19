/******************************************************************************

Copyright 2019 Evgeny Gorodetskiy

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************

FILE: Methane/Platform/MacOS/KeyboardMac.h
MacOS platform specific types and implementation of Keyboard abstractions.

******************************************************************************/

#pragma once

#include <stdint.h>

namespace Methane
{
namespace Platform
{
namespace Keyboard
{
    
struct NativeKey
{
    unsigned int  code;
    unsigned long flags;
};

} // namespace Keyboard
} // namespace Platform
} // namespace Methane