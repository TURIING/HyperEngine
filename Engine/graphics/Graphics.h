//
// Created by turiing on 2025/9/24.
//

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "../common/Common.h"
#include "../engine/Module.hpp"

USING_ENGINE_NAMESPACE_BEGIN

class ENGINE_EXPORT Graphics: public Module::Registrar<Graphics> {
    inline static const bool Registered = Register(Stage::Render);
public:
    Graphics();
    ~Graphics() override;

    void Update() override;
    NODISCARD HyperGpu::GpuDevice* GetGpuDevice() const { return m_pGpuDevice; }

private:
    HyperGpu::GpuFactory* m_pGpuFactory = nullptr;
    HyperGpu::GpuDevice* m_pGpuDevice = nullptr;
};

USING_ENGINE_NAMESPACE_END

#endif //GRAPHICS_H
