//
// Created by turiing on 2026/1/27.
//

#ifndef HYPERENGINE_SAMPLER_H
#define HYPERENGINE_SAMPLER_H
#include "../../../common/Common.h"
#include "../../VulObject.h"

USING_ENGINE_NAMESPACE_BEGIN
class LogicDevice;

struct SamplerCreateInfo {
    VkFilter             magFilter;
    VkFilter             minFilter;
    VkSamplerAddressMode addressModeU;
    VkSamplerAddressMode addressModeV;
    VkSamplerAddressMode addressModeW;
};

class Sampler: public VulObject<VkSampler> {
public:
    explicit Sampler(const SamplerCreateInfo &info);
    ~Sampler() override;
};

USING_ENGINE_NAMESPACE_END
#endif //HYPERENGINE_SAMPLER_H