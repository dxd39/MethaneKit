/******************************************************************************

Copyright 2022 Evgeny Gorodetskiy

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

FILE: Methane/Graphics/RHI/RenderContext.h
Methane RenderContext PIMPL wrappers for direct calls to final implementation.

******************************************************************************/

#pragma once

#include "Pimpl.h"

#include <Methane/Graphics/RHI/IRenderContext.h>
#include <Methane/Data/Transmitter.hpp>

namespace Methane::Graphics::Rhi
{

class Device;
class CommandKit;
class CommandQueue;
class ObjectRegistry;

class RenderContext
    : public Data::Transmitter<Rhi::IObjectCallback>
    , public Data::Transmitter<Rhi::IContextCallback>
{
public:
    using Settings              = RenderContextSettings;
    using Type                  = ContextType;
    using WaitFor               = ContextWaitFor;
    using DeferredAction        = ContextDeferredAction;
    using Option                = ContextOption;
    using OptionMask            = ContextOptionMask;
    using IncompatibleException = ContextIncompatibleException;

    META_PIMPL_DEFAULT_CONSTRUCT_METHODS_DECLARE(RenderContext);
    META_PIMPL_METHODS_COMPARE_DECLARE(RenderContext);

    RenderContext(const Ptr<IRenderContext>& render_context_ptr);
    RenderContext(IRenderContext& render_context);
    RenderContext(const Platform::AppEnvironment& env, const Device& device, tf::Executor& parallel_executor, const Settings& settings);

    void Init(const Platform::AppEnvironment& env, const Device& device, tf::Executor& parallel_executor, const Settings& settings);
    void Release();

    bool IsInitialized() const META_PIMPL_NOEXCEPT;
    IRenderContext& GetInterface() const META_PIMPL_NOEXCEPT;
    Ptr<IRenderContext> GetInterfacePtr() const META_PIMPL_NOEXCEPT;

    // IObject interface methods
    bool SetName(std::string_view name) const;
    std::string_view GetName() const META_PIMPL_NOEXCEPT;

    // IContext interface methods
    [[nodiscard]] OptionMask GetOptions() const META_PIMPL_NOEXCEPT;
    [[nodiscard]] tf::Executor& GetParallelExecutor() const META_PIMPL_NOEXCEPT;
    [[nodiscard]] IObjectRegistry& GetObjectRegistry() const META_PIMPL_NOEXCEPT;
    void RequestDeferredAction(DeferredAction action) const META_PIMPL_NOEXCEPT;
    void CompleteInitialization() const;
    [[nodiscard]] bool IsCompletingInitialization() const META_PIMPL_NOEXCEPT;
    void WaitForGpu(WaitFor wait_for) const;
    void Reset(const Device& device) const;
    void Reset() const;
    [[nodiscard]] Device GetDevice() const;
    [[nodiscard]] CommandKit GetDefaultCommandKit(CommandListType type) const;
    [[nodiscard]] CommandKit GetDefaultCommandKit(const CommandQueue& cmd_queue) const;
    [[nodiscard]] CommandKit GetUploadCommandKit() const;
    [[nodiscard]] CommandKit GetRenderCommandKit() const;

    // IRenderContext interface methods
    [[nodiscard]] bool ReadyToRender() const;
    void Resize(const FrameSize& frame_size) const;
    void Present() const;
    [[nodiscard]] Platform::AppView  GetAppView() const;
    [[nodiscard]] const Settings&    GetSettings() const META_PIMPL_NOEXCEPT;
    [[nodiscard]] uint32_t           GetFrameBufferIndex() const META_PIMPL_NOEXCEPT;
    [[nodiscard]] uint32_t           GetFrameIndex() const META_PIMPL_NOEXCEPT;
    [[nodiscard]] const IFpsCounter& GetFpsCounter() const META_PIMPL_NOEXCEPT;
    bool SetVSyncEnabled(bool vsync_enabled) const;
    bool SetFrameBuffersCount(uint32_t frame_buffers_count) const;
    bool SetFullScreen(bool is_full_screen) const;

private:
    class Impl;

    RenderContext(ImplPtr<Impl>&& impl_ptr);

    ImplPtr<Impl> m_impl_ptr;
};

} // namespace Methane::Graphics::Rhi
