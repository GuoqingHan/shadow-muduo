#ifndef MUTEX_H
#define MUTEX_H
#include <pthread.h>
#include <sys/syscall.h>

namespace shadow_muduo
{
class MutexLock
{
private:
    pid_t holder_;
    pthread_mutex_t mutex_;
public:
    MutexLock() : holder_(0)
    {
        pthread_mutex_init(&mutex_, NULL);
    }

    ~MutexLock()
    {
        assert(holder_ == 0);
        pthread_mutex_destroy(&mutex_);
    }

    bool isLockedByThisThread() const
    {
        return holder_ == CurrentThread::tid();
    }

    void assertLocked()
    {
        assert(isLockedByThisThread());
    }

    void lock()
    {
        pthread_mutex_lock(&mutex_);
        assignHolder();
    }

    void unlock()
    {
        unassignHolder();
        pthread_mutex_unlock(&mutex_);
    }

    void assignHolder()
    {
        holder_ = CurrentThread::tid();
    }

    void unassignHolder()
    {
        holder_ = 0;
    }

    pthread_mutex_t* getPthreadMutex()
    {
        return &mutex_;
    }
};

class MutexLockGuard
{
private:
    // 作为普通的栈变量，声明引用变量时必须进行初始化 int& b = a;
    // 作为类的成员，必须要在初始化列表中进行初始化
    MutexLock& mutex_;
public:
    explicit MutexLockGuard(MutexLock& mutex)
            : mutex_(mutex)
    {
        mutex_.lock();
    }

    ~MutexLockGuard()
    {
        mutex_.unlock();
    }
};


}

#endif