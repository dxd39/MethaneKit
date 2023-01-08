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

FILE: Methane/Graphics/RHI/IContext.cpp
Methane base context interface: wraps graphics device used for GPU interaction.

******************************************************************************/

#include <Methane/Graphics/RHI/IContext.h>
#include <Methane/Graphics/RHI/ICommandList.h>

namespace Methane::Graphics::Rhi
{

ICommandKit& IContext::GetUploadCommandKit() const
{
    return GetDefaultCommandKit(CommandListType::Transfer);
}

} // namespace Methane::Graphics::Rhi
