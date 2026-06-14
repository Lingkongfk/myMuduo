#pragma once
#include <vector>
#include <unordered_map>
#include "noncopyable.h"
#include "Timestamp.h"

class Channel;
class EventLoop;

//muduo库中的多路事件分发器的核心模块
class Poller: noncopyable{
public:
    using ChannelList = std::vector<Channel*>;

    Poller(EventLoop* loop);
    virtual ~Poller() = default;

    //保留统一的所有IO复用的接口
    virtual Timestamp poll(int timeoutMs, ChannelList* activeChannels) = 0;
    virtual void updateChannel(Channel* channel) = 0;//对应Channel里面函数
    virtual void removeChannel(Channel* channel) = 0;;//对应Channel里面函数
    
    //判断参数channel是否在当前Poller里面
    bool hasChannel(Channel* channel)const;

    //EventLoop事件循环通过该接口获取默认的IO复用
    static Poller* newDefaultPoller(EventLoop* loop);

protected:
    //key是fd， value是对应的channel
    using ChannelMap = std::unordered_map<int, Channel*>;
    ChannelMap channels_;

private:
    EventLoop* ownerLoop_;//定义事件所属的事件循环
};