//
// Created by turiing on 2026/2/3.
//

#include "ResourceCache.h"
#include "image/Sampler.h"

template <>
struct std::hash<HyperEngine::SamplerCacheInfo> {
    std::size_t operator()(const HyperEngine::SamplerCacheInfo &info) const noexcept {
        std::size_t result = 0;
        HASH_COMBINE(result, info.minFilter);
        HASH_COMBINE(result, info.magFilter);
        HASH_COMBINE(result, info.addressModeU);
        HASH_COMBINE(result, info.addressModeV);
        HASH_COMBINE(result, info.addressModeW);
        return result;
    }
};

template <typename T>
inline void hashParam(size_t &seed, const T &value) {
    HASH_COMBINE(seed, value);
}

USING_ENGINE_NAMESPACE_BEGIN

Share<Sampler> ResourceCache::RequestSampler(const SamplerCacheInfo &info) {
    size_t hash = 0;
    hashParam(hash, info);

    if(const auto it =  m_mapSampler.find(hash); it != m_mapSampler.end()) {
        return it->second;
    }

    m_mapSampler[hash] = std::make_shared<Sampler>(SamplerCreateInfo {
        .magFilter = static_cast<VkFilter>(info.magFilter),
        .minFilter = static_cast<VkFilter>(info.minFilter),
        .addressModeU = static_cast<VkSamplerAddressMode>(info.addressModeU),
        .addressModeV = static_cast<VkSamplerAddressMode>(info.addressModeV),
        .addressModeW = static_cast<VkSamplerAddressMode>(info.addressModeW),
    });

    return m_mapSampler[hash];
}

USING_ENGINE_NAMESPACE_END
