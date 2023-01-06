/******************************************************************************

Copyright 2023 Evgeny Gorodetskiy

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

FILE: Methane/Graphics/Null/CommandQueue.h
Null implementation of the command queue interface.

******************************************************************************/

#pragma once

#include <Methane/Graphics/Base/CommandQueue.h>

namespace Methane::Graphics::Null
{

class CommandQueue final
    : public Base::CommandQueue
{
public:
    using Base::CommandQueue::CommandQueue;

    // ICommandQueue interface
    uint32_t GetFamilyIndex() const noexcept override { return 0U; }
    Rhi::ITimestampQueryPool* GetTimestampQueryPool() const noexcept override { return nullptr; }
};

} // namespace Methane::Graphics::Null
