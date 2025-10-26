//
// Created by turiing on 2025/10/18.
//

#include "ObjModel.h"

#include "../../resource/ResourceManager.hpp"
#include "../../file/IFStream.hpp"
#include "../../file/FileManager.h"
#include "tinyobj/tiny_obj_loader.h"

USING_ENGINE_NAMESPACE_BEGIN

class MaterialStreamReader : public tinyobj::MaterialReader {
public:
    explicit MaterialStreamReader(std::filesystem::path folder) :
        folder(std::move(folder)) {
    }

    bool operator()(const std::string &matId, std::vector<tinyobj::material_t> *materials, std::map<std::string, int> *matMap, std::string *warn, std::string *err) override {
        auto filepath = folder / matId;

        if (!FileManager::ExistPath(filepath)) {
            std::stringstream ss;
            ss << "Material stream in error state. \n";

            if (warn) {
                (*warn) += ss.str();
            }

            return false;
        }

        IFStream inStream(filepath);
        tinyobj::LoadMtl(matMap, materials, &inStream, warn, err);
        return true;
    }

private:
    std::filesystem::path folder;
};

std::shared_ptr<ObjModel> ObjModel::Create(const Node &node) {
    if (auto resource = ResourceManager::Instance()->Find<ObjModel>(node)) {
        return resource;
    }

    auto result = std::make_shared<ObjModel>("");
    ResourceManager::Instance()->Add(node, result);
    node >> *result;
    result->load();
    return result;
}

std::shared_ptr<ObjModel> ObjModel::Create(const std::filesystem::path &fileName) {
    ObjModel temp(fileName);
    Node node;
    node << temp;
    return Create(node);
}

ObjModel::ObjModel(std::filesystem::path fileName): m_fileName(std::move(fileName)) {}

void ObjModel::load() {
    LOG_ASSERT(!m_fileName.empty());

#if USING_TIME_CONSUMING_DETECTION
    auto timeStart = Time::Now();
#endif

    auto folder = m_fileName.parent_path();
    IFStream inStream(m_fileName);
    MaterialStreamReader materialReader(folder);

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, &inStream, &materialReader)) {
        LOG_CRITICAL("{}", warn + err);
    }

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::unordered_map<Vertex, size_t> uniqueVertices;

    for (const auto &shape: shapes) {
        for (const auto &index: shape.mesh.indices) {
            Vertex vertex;
            vertex.position = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };
            vertex.uv = {
                attrib.texcoords[2 * index.vertex_index + 0],
                attrib.texcoords[2 * index.vertex_index + 1]
            };
            if (!attrib.normals.empty()) {
                vertex.normal = {
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2]
                };
            }

            if (!uniqueVertices.contains(vertex)) {
                uniqueVertices[vertex] = vertices.size();
                vertices.push_back(vertex);
            }
        }
    }

#if USING_TIME_CONSUMING_DETECTION
    LOG_INFO("Model({}) loaded in {} ms", m_fileName.c_str(), (Time::Now() - timeStart).AsMilliseconds<float>());
#endif

    initialize<Vertex>(vertices, indices);
}

USING_ENGINE_NAMESPACE_END
