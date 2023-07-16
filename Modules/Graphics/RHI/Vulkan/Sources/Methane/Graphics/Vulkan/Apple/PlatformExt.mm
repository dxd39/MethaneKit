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

FILE: Methane/Graphics/Vulkan/MacOS/PlatformExt.mm
Vulkan platform dependent functions for MacOS.

******************************************************************************/

#include <Methane/Graphics/Vulkan/Platform.h>

#include <Methane/Platform/MacOS/AppViewMetal.hh>
#include <Methane/Graphics/Metal/RenderContextAppView.hh>
#include <Methane/Instrumentation.h>
#include <Methane/Checks.hpp>

namespace Methane::Graphics::Vulkan
{

const std::vector<std::string_view>& Platform::GetVulkanInstanceRequiredLayers()
{
    static const std::vector<std::string_view> s_instance_layers = {};
    return s_instance_layers;
}

const std::vector<std::string_view>& Platform::GetVulkanInstanceRequiredExtensions()
{
    META_FUNCTION_TASK();
    static const std::vector<std::string_view> s_instance_extensions = GetPlatformInstanceExtensions({
        VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME,
        VK_EXT_METAL_SURFACE_EXTENSION_NAME
    });
    return s_instance_extensions;
}

vk::UniqueSurfaceKHR Platform::CreateVulkanSurfaceForWindow(const vk::Instance& vk_instance, const Methane::Platform::AppEnvironment& env)
{
    META_FUNCTION_TASK();
    AppViewMetal* metal_view = nil;
    if (!env.ns_app_delegate.isViewLoaded)
    {
        // Create temporary application view for Window if it was not created yet
        metal_view = Metal::CreateTemporaryAppView(env);
        env.ns_app_delegate.view = metal_view;
    }
    else
    {
        metal_view = static_cast<AppViewMetal*>(env.ns_app_delegate.view);
    }

    CAMetalLayer* metal_layer = metal_view.metalLayer;
    return vk_instance.createMetalSurfaceEXTUnique(
        vk::MetalSurfaceCreateInfoEXT(
            vk::MetalSurfaceCreateFlagsEXT{},
            metal_layer
        )
    );
}

} // namespace Methane::Graphics::Vulkan
