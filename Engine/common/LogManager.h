/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/10/27 12:56
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <memory>
#include <stdexcept>
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/async.h>
#include "Singleton.h"

class LogManager {
public:
    void Init();
    [[nodiscard]] std::shared_ptr<spdlog::logger> GetLogger() { return m_pLogger; }

private:
    std::shared_ptr<spdlog::logger> m_pLogger;
};

#ifdef NDEBUG
#define LOG_DEBUG(...)
#define LOG_INFO(...) SPDLOG_LOGGER_INFO(Singleton<LogManager>::GetInstance()->GetLogger(), fmt::format(__VA_ARGS__))
#define LOG_WARNING(...)
#define LOG_CRITICAL(...) \
SPDLOG_LOGGER_CRITICAL(Singleton<LogManager>::GetInstance()->GetLogger(), fmt::format(__VA_ARGS__)); \
std::exit(EXIT_FAILURE);
#define LOG_ASSERT(CONDITION)
#define LOG_ASSERT_INFO(CONDITION, INFO)
#else
#define LOG_DEBUG(...) SPDLOG_LOGGER_DEBUG(Singleton<LogManager>::GetInstance()->GetLogger(), fmt::format(__VA_ARGS__))
#define LOG_INFO(...) SPDLOG_LOGGER_INFO(Singleton<LogManager>::GetInstance()->GetLogger(), fmt::format(__VA_ARGS__))
#define LOG_WARNING(...) SPDLOG_LOGGER_WARN(Singleton<LogManager>::GetInstance()->GetLogger(), fmt::format(__VA_ARGS__))
#define LOG_CRITICAL(...) \
SPDLOG_LOGGER_CRITICAL(Singleton<LogManager>::GetInstance()->GetLogger(), fmt::format(__VA_ARGS__)); \
throw std::runtime_error(fmt::format(__VA_ARGS__));

#define LOG_ASSERT(CONDITION) if(!(CONDITION)){ LOG_CRITICAL("Assert failed({0})", #CONDITION); }
#define LOG_ASSERT_INFO(CONDITION, INFO) if(!(CONDITION)){ LOG_CRITICAL("Assert failed({0}): {1}", #CONDITION, INFO); }

#endif

#endif //LOGMANAGER_H
