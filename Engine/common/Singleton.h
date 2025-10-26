/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/10/25 22:37
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef SINGLETON_H
#define SINGLETON_H

template <typename T>
class Singleton {
    //使用默认构造和析构函数
    Singleton() = default;
    ~Singleton() = default;
public:
    //删除默认的移动、拷贝、赋值、取址
    Singleton(Singleton  &&) = delete;
    Singleton(const Singleton  &) = delete;
    void operator = (const Singleton  &) = delete;
    T *operator &() = delete;
    //获取实例，对此处有异议也可自行查阅资料
    static T* GetInstance()
    {
        static T object;
        return &object;
    }
};


#endif //SINGLETON_H
