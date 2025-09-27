//
// Created by turiing on 2025/8/15.
//

#ifndef REFOBJECT_H
#define REFOBJECT_H

#include <atomic>
#include <cassert>

namespace HyperEngine {
    class RefObject {
    public:
        virtual ~RefObject() = default;

        void AddRef() { m_refCount.fetch_add(1, std::memory_order_relaxed); }

        void SubRef() {
            assert(m_refCount.load() >= 1);
            m_refCount.fetch_sub(1, std::memory_order_relaxed);
            if (m_refCount.load() == 0) {
                delete this;
            }
        }

    private:
        std::atomic<uint32_t> m_refCount = 1;
    };
}
#endif //REFOBJECT_H
