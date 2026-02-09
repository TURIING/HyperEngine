//
// Created by turiing on 2026/2/3.
//

#ifndef HYPERENGINE_RESOURCECACHE_H
#define HYPERENGINE_RESOURCECACHE_H
#include "../../common/Common.h"

USING_ENGINE_NAMESPACE_BEGIN
class Sampler;

template<typename T>
using ResourceCacheMap = std::unordered_map<std::size_t, Share<T>>;

struct SamplerCacheInfo {
    Filter      magFilter;
    Filter      minFilter;
    AddressMode addressModeU;
    AddressMode addressModeV;
    AddressMode addressModeW;
};

class ResourceCache {
public:
    NODISCARD Share<Sampler> RequestSampler(const SamplerCacheInfo& info);

private:
    ResourceCacheMap<Sampler> m_mapSampler;
};

USING_ENGINE_NAMESPACE_END
#endif //HYPERENGINE_RESOURCECACHE_H