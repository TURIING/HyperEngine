//
// Created by turiing on 2025/10/18.
//

#ifndef OBJMODEL_H
#define OBJMODEL_H

#include "../../common/Common.h"
#include "../ModelFactory.hpp"
#include "../Model.hpp"

USING_ENGINE_NAMESPACE_BEGIN

class ObjModel: public Model::Registrar<ObjModel> {
    inline static const bool Registered = Register("obj", ".obj");
public:
    static std::shared_ptr<ObjModel> Create(const Node &node);
    static std::shared_ptr<ObjModel> Create(const std::filesystem::path &fileName);
    explicit ObjModel(std::filesystem::path fileName);

private:
    void load();

private:
    std::filesystem::path m_fileName;
};

USING_ENGINE_NAMESPACE_END

#endif //OBJMODEL_H
