#ifndef NON_COPY_ABLE_H
#define NON_COPY_ABLE_H

namespace shadow_muduo
{
class noncopyable
{
public:
    noncopyable(const noncopyable&) = delete;
    void operator=(noncopyable&) = delete;

protected:
    noncopyable() = default;
    ~noncopyable() = default;
};
}

#endif