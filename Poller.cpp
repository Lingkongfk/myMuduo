#include "Poller.h"
#include "Channel.h"

Poller::Poller(EventLoop* loop)
    :ownerLoop_(loop){

}

bool Poller::hasChannel(Channel* channel)const{
    auto it = channels_.find(channel->fd());
    return it != channels_.end() && it->second == channel;
}

//为什么不把Poller* newDefaultPoller写在这里
/*
如果这样写，肯定要
#include "PollPoller.h"
#include "EpollPoller.h"
这样才能返回具体Poller
基类怎么能引用派生类的头文件呢

那我们怎么实现呢
单独找一个文件进行实现，并且包含这些头文件
*/