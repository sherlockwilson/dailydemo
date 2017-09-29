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
        // ���ʵ�����Ψһ��ʽ
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
     *  @brief	��ȡʵ���Ľӿ�
     *  @param	void
     *  @return	T& ���������
     */
    template <class T>
    inline T& CSingleton<T>::Instance()
    {
        if (s_instance_.get() == NULL)
        {
            std::unique_lock<std::mutex> lock(mutex_); //! ��������ͬʱ����������ʵ��
                                                          //! CSingleton<T>�������»�����������Ҫ���Ĳ�ͬ����֮��Ļ���
            if (s_instance_.get() == NULL) // �ڶ����жϵ�Ŀ�����ڲ������߳��ظ�����
            {
                s_instance_ = std::unique_ptr<T>(new T);
            }
        }
        return *s_instance_.get();
    }

}