//
// Created by turiing on 2025/9/22.
//

#ifndef APP_H
#define APP_H

#include "../common/Common.h"

USING_ENGINE_NAMESPACE_BEGIN

class ENGINE_EXPORT App: public rocket::trackable {
public:
    explicit App(const std::string &appName): m_appName(appName) {};
    virtual ~App() = default;

    virtual void Init() = 0;
    virtual void Update() = 0;

private:
    std::string m_appName;

};

USING_ENGINE_NAMESPACE_END
#endif //APP_H
