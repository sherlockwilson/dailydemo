#pragma once

#include "Global.h"

namespace Base
{
    /** @class 
     *  @brief ��ֹ�����Ϳ�������
     *
     *  ����ʹ�õ�ģ��
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
        // ����������͸�ֵ����������Ϊ˽������
        CNoncopyable(const CNoncopyable&);
        const CNoncopyable& operator= (const CNoncopyable&);
    };

}