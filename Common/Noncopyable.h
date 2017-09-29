#pragma once

#include "Global.h"

namespace Base
{
    /** @class 
     *  @brief 禁止拷贝和拷贝构造
     *
     *  子类使用的模型
     *  class Foo : private CNoncopyable
     *  {
     *      ...
     *  };
     */
    //!! PCLint [error 1509: (Warning -- base class destructor for class 'CNoncopyable' is not virtual)]
    //lint -esym(1509, CNoncopyable)
    class BASE_LIB_EXPORT CNoncopyable
    {
    protected:
        CNoncopyable() {}
        ~CNoncopyable() {}

    private:
        // 将拷贝构造和赋值操作符声明为私有类型
        CNoncopyable(const CNoncopyable&);
        const CNoncopyable& operator= (const CNoncopyable&);
    };

}