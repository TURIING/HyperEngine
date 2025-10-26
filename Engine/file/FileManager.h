//
// Created by turiing on 2025/10/1.
//

#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "../common/Common.h"
#include "../engine/Module.hpp"

USING_ENGINE_NAMESPACE_BEGIN

class FileManager final: public Module::Registrar<FileManager> {
public:
    FileManager();
    ~FileManager() override;
    void Update() override;
    void AddSearchPath(const std::string &path);
    void RemoveSearchPath(const std::string &path);
    void ClearSearchPath();
    NODISCARD static bool ExistPath(const std::filesystem::path &path);
    NODISCARD static std::optional<std::string> ReadFile(const std::filesystem::path &path);
    NODISCARD static std::vector<uint8_t> ReadFileInBytes(const std::filesystem::path &path);

private:
    std::vector<std::string> m_searchPaths;
};

USING_ENGINE_NAMESPACE_END

#endif //FILEMANAGER_H
