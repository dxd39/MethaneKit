/******************************************************************************

Copyright 2019-2022 Evgeny Gorodetskiy

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

FILE: Methane/Graphics/Vulkan/ResourceVK.h
Vulkan implementation of the resource interface.

******************************************************************************/

#pragma once

#include <Methane/Graphics/ResourceBarriers.h>
#include <Methane/Graphics/Resource.h>
#include <Methane/Data/Receiver.hpp>

#include <vulkan/vulkan.hpp>

namespace Methane::Graphics
{

class BufferVK;
struct ITextureVK;

class ResourceBarriersVK
    : public ResourceBarriers
    , private Data::Receiver<IResourceCallback>
{
public:
    explicit ResourceBarriersVK(const Set& barriers);

    // ResourceBarriers overrides
    AddResult Add(const ResourceBarrier::Id& id, const ResourceBarrier& barrier) override;
    bool Remove(const ResourceBarrier::Id& id) override;

    const std::vector<vk::ImageMemoryBarrier>&  GetNativeImageMemoryBarriers() const noexcept  { return m_vk_image_memory_barriers; }
    const std::vector<vk::BufferMemoryBarrier>& GetNativeBufferMemoryBarriers() const noexcept { return m_vk_buffer_memory_barriers; }
    const std::vector<vk::MemoryBarrier>&       GetNativeMemoryBarriers() const noexcept       { return m_vk_memory_barriers; }
    vk::PipelineStageFlags                      GetNativeSrcStageMask() const noexcept         { return m_vk_src_stage_mask; }
    vk::PipelineStageFlags                      GetNativeDstStageMask() const noexcept         { return m_vk_dst_stage_mask; }

private:
    // IResourceCallback
    void OnResourceReleased(Resource& resource) override;

    void SetResourceBarrier(const ResourceBarrier::Id& id, const ResourceBarrier& barrier, bool is_new_barrier);
    void SetBufferMemoryBarrier(BufferVK& buffer, const ResourceBarrier& barrier);
    void SetImageMemoryBarrier(ITextureVK& texture, const ResourceBarrier& barrier);

    void AddBufferMemoryStateChangeBarrier(const BufferVK& buffer, const ResourceBarrier::StateChange& state_change);
    void AddBufferMemoryOwnerChangeBarrier(const BufferVK& buffer, const ResourceBarrier::OwnerChange& owner_change);
    void AddImageMemoryStateChangeBarrier(const ITextureVK& texture, const ResourceBarrier::StateChange& state_change);
    void AddImageMemoryOwnerChangeBarrier(const ITextureVK& texture, const ResourceBarrier::OwnerChange& owner_change);

    void UpdateBufferMemoryStateChangeBarrier(vk::BufferMemoryBarrier& vk_buffer_memory_barrier, const ResourceBarrier::StateChange& state_change);
    void UpdateBufferMemoryOwnerChangeBarrier(vk::BufferMemoryBarrier& vk_buffer_memory_barrier, const ResourceBarrier::OwnerChange& owner_change);
    void UpdateImageMemoryStateChangeBarrier(vk::ImageMemoryBarrier& vk_image_memory_barrier, const ResourceBarrier::StateChange& state_change);
    void UpdateImageMemoryOwnerChangeBarrier(vk::ImageMemoryBarrier& vk_image_memory_barrier, const ResourceBarrier::OwnerChange& owner_change);

    void RemoveBufferMemoryBarrier(const vk::Buffer& vk_buffer, ResourceBarrier::Type barrier_type);
    void RemoveImageMemoryBarrier(const vk::Image& vk_image, ResourceBarrier::Type barrier_type);

    void UpdateStageMasks();

    std::vector<vk::BufferMemoryBarrier> m_vk_buffer_memory_barriers;
    std::vector<vk::ImageMemoryBarrier>  m_vk_image_memory_barriers;
    std::vector<vk::MemoryBarrier>       m_vk_memory_barriers;
    vk::PipelineStageFlags               m_vk_src_stage_mask {};
    vk::PipelineStageFlags               m_vk_dst_stage_mask {};
};

} // namespace Methane::Graphics
