//
// Created by turiing on 2026/1/27.
//

#include "Sampler.h"
#include "../../device/LogicDevice.h"
#include "../../device/PhysicalDevice.h"
#include "../../Graphics.h"

USING_ENGINE_NAMESPACE_BEGIN
Sampler::Sampler(const SamplerCreateInfo &info) {
    const auto logicDevice = Graphics::Get()->GetLogicDevice();
    VkSamplerCreateInfo samplerInfo = {
        .sType					 = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        .magFilter				 = info.magFilter,
        .minFilter				 = info.minFilter,
        .mipmapMode				 = VK_SAMPLER_MIPMAP_MODE_LINEAR,
        .addressModeU 			 = info.addressModeU,
        .addressModeV 			 = info.addressModeV,
        .addressModeW 			 = info.addressModeW,
        .mipLodBias				 = 0.0f,
        .anisotropyEnable		 = VK_TRUE,
        .maxAnisotropy			 = logicDevice->GetPhysicalDevice()->GetGpuInfo().limits.maxSamplerAnisotropy,
        .compareEnable			 = VK_FALSE,
        .compareOp				 = VK_COMPARE_OP_ALWAYS,
        .minLod					 = 0.0f,
        .maxLod					 = 11.0f,
        .borderColor			 = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
        .unnormalizedCoordinates = VK_FALSE,
    };
    CALL_VK(vkCreateSampler(logicDevice->GetHandle(), &samplerInfo, nullptr, &m_pHandle));
}

Sampler::~Sampler() {
    const auto logicDevice = Graphics::Get()->GetLogicDevice();
	vkDestroySampler(logicDevice->GetHandle(), m_pHandle, nullptr);
}

USING_ENGINE_NAMESPACE_END
