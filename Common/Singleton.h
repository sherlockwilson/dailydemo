#pragma once

#include "Noncopyable.h"
//#include <boost/thread/thread.hpp>
#include <mutex>
#include <memory>
namespace Base
{
    template <class T>
    class CSingleton : private CNoncopyable
    {
    public:
        // 访问单件的唯一方式
        static T& Instance();

    protected:
        CSingleton() {}

        static std::unique_ptr<T> s_instance_;
        static std::mutex mutex_;
    };

    template<class T>
    std::unique_ptr<T> CSingleton<T>::s_instance_;

    template<class T>
    std::mutex CSingleton<T>::mutex_;

    /** @fn	T& CSingleton<T>::Instance()
     *  @brief	获取实例的接口
     *  @param	void
     *  @return	T& 对象的引用
     */
    template <class T>
    inline T& CSingleton<T>::Instance()
    {
        if (s_instance_.get() == NULL)
        {
            std::unique_lock<std::mutex> lock(mutex_); //! 加锁避免同时被创建两个实例
                                                          //! CSingleton<T>作用域下互斥量，不需要担心不同类型之间的互斥
            if (s_instance_.get() == NULL) // 第二次判断的目的在于不被多线程重复创建
            {
                s_instance_ = std::unique_ptr<T>(new T);
            }
        }
        return *s_instance_.get();
    }

}