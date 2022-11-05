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

FILE: Methane/Graphics/Metal/RenderPassMT.mm
Metal implementation of the render pass interface.

******************************************************************************/

#include <Methane/Graphics/RenderPassMT.hh>
#include <Methane/Graphics/RenderContextMT.hh>
#include <Methane/Graphics/TextureMT.hh>
#include <Methane/Graphics/TypesMT.hh>

#include <Methane/Instrumentation.h>
#include <Methane/Checks.hpp>

namespace Methane::Graphics
{

static MTLStoreAction GetMTLStoreAction(IRenderPass::Attachment::StoreAction store_action)
{
    META_FUNCTION_TASK();
    switch(store_action)
    {
        case IRenderPass::Attachment::StoreAction::DontCare: return MTLStoreActionDontCare;
        case IRenderPass::Attachment::StoreAction::Store:    return MTLStoreActionStore;
        case IRenderPass::Attachment::StoreAction::Resolve:  return MTLStoreActionMultisampleResolve;
        default:                                             META_UNEXPECTED_ARG_RETURN(store_action, MTLStoreActionUnknown);
    }
}

static MTLLoadAction GetMTLLoadAction(IRenderPass::Attachment::LoadAction load_action)
{
    META_FUNCTION_TASK();
    switch(load_action)
    {
        case IRenderPass::Attachment::LoadAction::DontCare: return MTLLoadActionDontCare;
        case IRenderPass::Attachment::LoadAction::Load:     return MTLLoadActionLoad;
        case IRenderPass::Attachment::LoadAction::Clear:    return MTLLoadActionClear;
        default:                                            META_UNEXPECTED_ARG_RETURN(load_action, MTLLoadActionDontCare);
    }
}

static void ConvertRenderPassAttachmentToMetal(const Base::RenderPass& render_pass, const IRenderPattern::Attachment& attachment, MTLRenderPassAttachmentDescriptor* mtl_attachment_desc)
{
    META_FUNCTION_TASK();
    const ITexture::View& texture_location = render_pass.GetAttachmentTextureView(attachment);
    const SubResource::Index& sub_resource_index = texture_location.GetSubresourceIndex();
    
    if (texture_location.GetTexture().GetSettings().type == ITexture::Type::FrameBuffer)
    {
        static_cast<TextureMT&>(texture_location.GetTexture()).UpdateFrameBuffer();
    }
    
    META_CHECK_ARG_NOT_NULL(mtl_attachment_desc);
    mtl_attachment_desc.texture       = static_cast<const TextureMT&>(texture_location.GetTexture()).GetNativeTexture();
    mtl_attachment_desc.level         = sub_resource_index.GetMipLevel();
    mtl_attachment_desc.loadAction    = GetMTLLoadAction(attachment.load_action);
    mtl_attachment_desc.storeAction   = GetMTLStoreAction(attachment.store_action);
    
    if (mtl_attachment_desc.texture.textureType == MTLTextureTypeCube ||
        mtl_attachment_desc.texture.textureType == MTLTextureTypeCubeArray)
    {
        mtl_attachment_desc.slice = sub_resource_index.GetArrayIndex() * 6U + sub_resource_index.GetDepthSlice();
    }
    else
    {
        mtl_attachment_desc.slice      = texture_location.GetSubresourceIndex().GetArrayIndex();
        mtl_attachment_desc.depthPlane = texture_location.GetSubresourceIndex().GetDepthSlice();
    }
}

Ptr<IRenderPattern> IRenderPattern::Create(IRenderContext& render_context, const Settings& settings)
{
    META_FUNCTION_TASK();
    return std::make_shared<Base::RenderPattern>(dynamic_cast<Base::RenderContext&>(render_context), settings);
}

Ptr<IRenderPass> IRenderPass::Create(IRenderPattern& render_pattern, const Settings& settings)
{
    META_FUNCTION_TASK();
    return std::make_shared<RenderPassMT>(dynamic_cast<Base::RenderPattern&>(render_pattern), settings);
}

RenderPassMT::RenderPassMT(Base::RenderPattern& render_pattern, const Settings& settings)
    : Base::RenderPass(render_pattern, settings)
{
    META_FUNCTION_TASK();
    Reset();
}

bool RenderPassMT::Update(const Settings& settings)
{
    META_FUNCTION_TASK();
    const bool settings_changed = Base::RenderPass::Update(settings);
    Reset();
    Data::Emitter<IRenderPassCallback>::Emit(&IRenderPassCallback::OnRenderPassUpdated, *this);
    return settings_changed;
}

void RenderPassMT::Reset()
{
    META_FUNCTION_TASK();

    m_mtl_pass_descriptor = [MTLRenderPassDescriptor renderPassDescriptor];
    const Pattern::Settings& pattern_settings = GetPatternBase().GetSettings();

    uint32_t color_attach_index = 0;
    for(const ColorAttachment& color_attach : pattern_settings.color_attachments)
    {
        MTLRenderPassColorAttachmentDescriptor* mtl_color_attachment_desc = m_mtl_pass_descriptor.colorAttachments[color_attach_index++];
        ConvertRenderPassAttachmentToMetal(*this, color_attach, mtl_color_attachment_desc);
        mtl_color_attachment_desc.clearColor  = TypeConverterMT::ColorToMetalClearColor(color_attach.clear_color);
    }
    
    if (pattern_settings.depth_attachment)
    {
        ConvertRenderPassAttachmentToMetal(*this, *pattern_settings.depth_attachment, m_mtl_pass_descriptor.depthAttachment);
        m_mtl_pass_descriptor.depthAttachment.clearDepth = pattern_settings.depth_attachment->clear_value;
    }
    
    if (pattern_settings.stencil_attachment)
    {
        ConvertRenderPassAttachmentToMetal(*this, *pattern_settings.stencil_attachment, m_mtl_pass_descriptor.stencilAttachment);
        m_mtl_pass_descriptor.stencilAttachment.clearStencil  = pattern_settings.stencil_attachment->clear_value;
    }
}

MTLRenderPassDescriptor* RenderPassMT::GetNativeDescriptor(bool reset)
{
    META_FUNCTION_TASK();
    if (reset)
    {
        Reset();
    }
    return m_mtl_pass_descriptor;
}

const IContextMT& RenderPassMT::GetContextMT() const noexcept
{
    META_FUNCTION_TASK();
    return dynamic_cast<const IContextMT&>(GetPatternBase().GetRenderContextBase());
}

} // namespace Methane::Graphics
