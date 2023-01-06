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

FILE: Methane/Graphics/Null/Shader.cpp
Null implementation of the shader interface.

******************************************************************************/

#include <Methane/Graphics/Null/Shader.h>

#include <Methane/Graphics/Base/Context.h>

namespace Methane::Graphics::Rhi
{

Ptr<IShader> IShader::Create(Rhi::ShaderType shader_type, const Rhi::IContext& context, const Settings& settings)
{
    return std::make_shared<Null::Shader>(shader_type, dynamic_cast<const Base::Context&>(context), settings);
}

} // namespace Methane::Graphics::Rhi

namespace Methane::Graphics::Null
{

Ptrs<Base::ProgramArgumentBinding> Shader::GetArgumentBindings(const Rhi::ProgramArgumentAccessors&) const
{
    return {};
}

} // namespace Methane::Graphics::Null
