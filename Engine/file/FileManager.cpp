//
// Created by turiing on 2025/10/1.
//

#include "FileManager.h"
#include "physfs.h"
#include "../engine/Engine.h"
#include "Config.hpp"
#include <fstream>
#include "IFStream.hpp"

USING_ENGINE_NAMESPACE_BEGIN
FileManager::FileManager() {
    PHYSFS_init(Engine::Instance()->GetArgv0().c_str());

    if (std::filesystem::exists(HYPER_RESOURCE_PATH)) {
        AddSearchPath(std::string(HYPER_RESOURCE_PATH));
    }
    LOG_INFO("FileManager Module Created.");
}

FileManager::~FileManager() {
    PHYSFS_deinit();
}

void FileManager::Update() {}

void FileManager::AddSearchPath(const std::string &path) {
    if (std::find(m_searchPaths.begin(), m_searchPaths.end(), path) != m_searchPaths.end()) {
        return;
    }

    if (PHYSFS_mount(path.c_str(), nullptr, true) == 0) {
        LOG_WARNING("Failed to mount path({}), error in {}", path, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
        return;
    }

    m_searchPaths.push_back(path);
}

void FileManager::RemoveSearchPath(const std::string &path) {
    auto it = std::find(m_searchPaths.begin(), m_searchPaths.end(), path);
    if (it == m_searchPaths.end()) {
        return;
    }
    if (PHYSFS_unmount(path.c_str()) == 0) {
        LOG_WARNING("Failed to unmount path({}), error in {}", path, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
        return;
    }
    m_searchPaths.erase(it);
}

void FileManager::ClearSearchPath() {
    for (const auto &path : m_searchPaths) {
        if (PHYSFS_unmount(path.c_str()) == 0) {
            LOG_WARNING("Failed to unmount path({}), error in {}", path, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
        }
    }
    m_searchPaths.clear();
}

bool FileManager::ExistPath(const std::filesystem::path &path) {
    LOG_ASSERT(PHYSFS_isInit() != 0);
    auto pathStr = path.string();
    std::ranges::replace(pathStr, '\\', '/');
    return PHYSFS_exists(pathStr.c_str()) != 0;
}

std::optional<std::string> FileManager::ReadFile(const std::filesystem::path &path) {
    LOG_ASSERT(PHYSFS_isInit() != 0);
    auto pathStr = path.string();
    std::ranges::replace(pathStr, '\\', '/');

    auto fsFile = PHYSFS_openRead(pathStr.c_str());
    if (!fsFile) {
        if (!std::filesystem::exists(path) || !std::filesystem::is_regular_file(path)) {
            LOG_CRITICAL("Failed to open file {}", pathStr);
        }

        std::ifstream is(path);
        std::stringstream buffer;
        buffer << is.rdbuf();
        return buffer.str();
    }

    auto size = PHYSFS_fileLength(fsFile);
    std::vector<uint8_t> data(size);
    PHYSFS_readBytes(fsFile, data.data(), static_cast<PHYSFS_uint64>(size));

    if (PHYSFS_close(fsFile) == 0) {
        LOG_CRITICAL("Failed to close file {}", pathStr);
    }

    return std::string(data.begin(), data.end());
}

std::vector<uint8_t> FileManager::ReadFileInBytes(const std::filesystem::path &path) {
    IFStream file(path);
    file >> std::noskipws;

    auto fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<unsigned char> bytes;
    bytes.reserve(fileSize);

    std::copy(std::istream_iterator<unsigned char>(file), std::istream_iterator<unsigned char>(), std::back_inserter(bytes));

    return bytes;
}

USING_ENGINE_NAMESPACE_END
