//
// Created by turiing on 2025/9/24.
//

#ifndef MAINAPP_H
#define MAINAPP_H

#include "engine/App.h"

class MainApp : public HyperEngine::App {
public:
    MainApp();
    ~MainApp() override;
    void Init() override;
    void Update() override;
};



#endif //MAINAPP_H
