/******************************************************************************

Copyright 2019-2020 Evgeny Gorodetskiy

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

FILE: Methane/Graphics/Metal/RenderCommandListMT.mm
Metal implementation of the render command list interface.

******************************************************************************/

#include "RenderCommandListMT.hh"
#include "CommandListMT.hh"
#include "ParallelRenderCommandListMT.hh"
#include "RenderPassMT.hh"
#include "CommandQueueMT.hh"
#include "RenderContextMT.hh"
#include "BufferMT.hh"

#include <Methane/Instrumentation.h>
#include <Methane/Platform/MacOS/Types.hh>

namespace Methane::Graphics
{

static MTLPrimitiveType PrimitiveTypeToMetal(RenderCommandList::Primitive primitive) noexcept
{
    META_FUNCTION_TASK();

    switch (primitive)
    {
        case RenderCommandList::Primitive::Point:          return MTLPrimitiveTypePoint;
        case RenderCommandList::Primitive::Line:           return MTLPrimitiveTypeLine;
        case RenderCommandList::Primitive::LineStrip:      return MTLPrimitiveTypeLineStrip;
        case RenderCommandList::Primitive::Triangle:       return MTLPrimitiveTypeTriangle;
        case RenderCommandList::Primitive::TriangleStrip:  return MTLPrimitiveTypeTriangleStrip;
    }
    return MTLPrimitiveTypeTriangleStrip;
}

Ptr<RenderCommandList> RenderCommandList::Create(CommandQueue& command_queue, RenderPass& render_pass)
{
    META_FUNCTION_TASK();
    return std::make_shared<RenderCommandListMT>(static_cast<CommandQueueBase&>(command_queue), static_cast<RenderPassBase&>(render_pass));
}

Ptr<RenderCommandList> RenderCommandList::Create(ParallelRenderCommandList& parallel_render_command_list)
{
    META_FUNCTION_TASK();
    return std::make_shared<RenderCommandListMT>(static_cast<ParallelRenderCommandListBase&>(parallel_render_command_list));
}

RenderCommandListMT::RenderCommandListMT(CommandQueueBase& command_queue, RenderPassBase& render_pass)
    : RenderCommandListBase(command_queue, render_pass)
{
    META_FUNCTION_TASK();
}

RenderCommandListMT::RenderCommandListMT(ParallelRenderCommandListBase& parallel_render_command_list)
    : RenderCommandListBase(parallel_render_command_list)
{
    META_FUNCTION_TASK();
}

void RenderCommandListMT::Reset(const Ptr<RenderState>& sp_render_state, DebugGroup* p_debug_group)
{
    META_FUNCTION_TASK();
    
    RenderCommandListBase::ResetCommandState();

    if (m_mtl_render_encoder)
    {
        RenderCommandListBase::Reset(sp_render_state, p_debug_group);
        return;
    }

    if (IsParallel())
    {
        Ptr<ParallelRenderCommandListMT> sp_parallel_render_cmd_list = std::static_pointer_cast<ParallelRenderCommandListMT>(GetParallelRenderCommandList());
        assert(!!sp_parallel_render_cmd_list);
        id <MTLParallelRenderCommandEncoder>& mtl_parallel_render_command_encoder = sp_parallel_render_cmd_list->GetNativeParallelRenderEncoder();
        m_mtl_render_encoder = [mtl_parallel_render_command_encoder renderCommandEncoder];
    }
    else
    {
        // If command buffer was not created for current frame yet,
        // then render pass descriptor should be reset with new frame drawable
        MTLRenderPassDescriptor* mtl_render_pass = GetRenderPassMT().GetNativeDescriptor(m_mtl_cmd_buffer == nil);
        
        if (!m_mtl_cmd_buffer)
        {
            m_mtl_cmd_buffer = [GetCommandQueueMT().GetNativeCommandQueue() commandBuffer];

            assert(m_mtl_cmd_buffer != nil);
            m_mtl_cmd_buffer.label = m_ns_name;
        }

        assert(mtl_render_pass != nil);
        m_mtl_render_encoder = [m_mtl_cmd_buffer renderCommandEncoderWithDescriptor:mtl_render_pass];
    }

    assert(m_mtl_render_encoder != nil);
    m_mtl_render_encoder.label = m_ns_name;

    RenderCommandListBase::Reset(sp_render_state, p_debug_group);
}

void RenderCommandListMT::SetName(const std::string& name)
{
    META_FUNCTION_TASK();

    RenderCommandListBase::SetName(name);
    
    m_ns_name = MacOS::ConvertToNsType<std::string, NSString*>(name);
    
    if (m_mtl_render_encoder != nil)
    {
        m_mtl_render_encoder.label = m_ns_name;
    }
    
    if (m_mtl_cmd_buffer != nil)
    {
        m_mtl_cmd_buffer.label = m_ns_name;
    }
}

void RenderCommandListMT::PushDebugGroup(DebugGroup& debug_group)
{
    META_FUNCTION_TASK();

    CommandListBase::PushDebugGroup(debug_group);

    assert(m_mtl_render_encoder != nil);
    [m_mtl_render_encoder pushDebugGroup:static_cast<CommandListMT::DebugGroupMT&>(debug_group).GetNSName()];
}

void RenderCommandListMT::PopDebugGroup()
{
    META_FUNCTION_TASK();

    CommandListBase::PopDebugGroup();

    assert(m_mtl_render_encoder != nil);
    [m_mtl_render_encoder popDebugGroup];
}

void RenderCommandListMT::SetVertexBuffers(const Buffers& vertex_buffers)
{
    META_FUNCTION_TASK();

    RenderCommandListBase::SetVertexBuffers(vertex_buffers);

    if (!(GetDrawingState().changes & DrawingState::Changes::VertexBuffers))
        return;

    assert(m_mtl_render_encoder != nil);
    const BuffersMT& metal_vertex_buffers = static_cast<const BuffersMT&>(vertex_buffers);
    const std::vector<id<MTLBuffer>>& mtl_buffers = metal_vertex_buffers.GetNativeBuffers();
    const std::vector<NSUInteger>&    mtl_offsets = metal_vertex_buffers.GetNativeOffsets();
    const NSRange                     mtl_range{ 0u, metal_vertex_buffers.GetCount() };
    [m_mtl_render_encoder setVertexBuffers:mtl_buffers.data() offsets:mtl_offsets.data() withRange:mtl_range];
}

void RenderCommandListMT::DrawIndexed(Primitive primitive, Buffer& index_buffer,
                                      uint32_t index_count, uint32_t start_index, uint32_t start_vertex,
                                      uint32_t instance_count, uint32_t start_instance)
{
    META_FUNCTION_TASK();
    
    const BufferMT& metal_index_buffer = static_cast<const BufferMT&>(index_buffer);
    if (index_count == 0)
    {
        index_count = metal_index_buffer.GetFormattedItemsCount();
    }

    RenderCommandListBase::DrawIndexed(primitive, index_buffer, index_count, start_index, start_vertex, instance_count, start_instance);
    
    const MTLPrimitiveType mtl_primitive_type   = PrimitiveTypeToMetal(primitive);
    const MTLIndexType     mtl_index_type       = metal_index_buffer.GetNativeIndexType();
    const id<MTLBuffer>&   mtl_index_buffer     = metal_index_buffer.GetNativeBuffer();
    const uint32_t         mtl_index_stride     = mtl_index_type == MTLIndexTypeUInt32 ? 4 : 2;

    assert(m_mtl_render_encoder != nil);
    [m_mtl_render_encoder drawIndexedPrimitives: mtl_primitive_type
                                     indexCount: index_count
                                      indexType: mtl_index_type
                                    indexBuffer: mtl_index_buffer
                              indexBufferOffset: start_index * mtl_index_stride
                                  instanceCount: instance_count
                                     baseVertex: start_vertex
                                   baseInstance: start_instance];
}

void RenderCommandListMT::Draw(Primitive primitive, uint32_t vertex_count, uint32_t start_vertex,
                               uint32_t instance_count, uint32_t start_instance)
{
    META_FUNCTION_TASK();

    RenderCommandListBase::Draw(primitive, vertex_count, start_vertex, instance_count, start_instance);

    const MTLPrimitiveType mtl_primitive_type = PrimitiveTypeToMetal(primitive);

    assert(m_mtl_render_encoder != nil);
    [m_mtl_render_encoder drawPrimitives: mtl_primitive_type
                             vertexStart: start_vertex
                             vertexCount: vertex_count
                           instanceCount: instance_count
                            baseInstance: start_instance];
}

void RenderCommandListMT::Commit()
{
    META_FUNCTION_TASK();
    
    assert(!IsCommitted());

    RenderCommandListBase::Commit();

    if (m_mtl_render_encoder)
    {
        [m_mtl_render_encoder endEncoding];
        m_mtl_render_encoder = nil;
    }

    if (!m_mtl_cmd_buffer || IsParallel())
        return;

    [m_mtl_cmd_buffer enqueue];
}

void RenderCommandListMT::Execute(uint32_t frame_index, const CompletedCallback& completed_callback)
{
    META_FUNCTION_TASK();

    RenderCommandListBase::Execute(frame_index, completed_callback);

    if (IsParallel() || !m_mtl_cmd_buffer)
        return;

    [m_mtl_cmd_buffer addCompletedHandler:^(id<MTLCommandBuffer>) {
        Complete(frame_index);
    }];

    [m_mtl_cmd_buffer commit];
    m_mtl_cmd_buffer  = nil;
}

CommandQueueMT& RenderCommandListMT::GetCommandQueueMT() noexcept
{
    META_FUNCTION_TASK();
    return static_cast<class CommandQueueMT&>(GetCommandQueue());
}

RenderPassMT& RenderCommandListMT::GetRenderPassMT()
{
    META_FUNCTION_TASK();
    return static_cast<class RenderPassMT&>(GetPass());
}

} // namespace Methane::Graphics
