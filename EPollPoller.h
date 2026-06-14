#pragma once

#include <vector>
#include <sys/epoll.h>
#include "Timestamp.h"
#include "Poller.h"


//继承Poller，有对应的成员
class EPollPoller: public Poller{
public:
    EPollPoller(EventLoop* loop);
    ~EPollPoller() override;

    Timestamp poll(int timeoutMs, ChannelList* activeChannels)override;
    void updateChannel(Channel* channel)override;
    void removeChannel(Channel* channel)override;


private:
    static const int kInitEventListSize = 16;//就绪队列初始长度

    void fillActiveChannels(int numEvents, ChannelList*activeChannels)const;
    void update(int operation, Channel* channel);

    using EventList = std::vector<epoll_event>;
    int epollfd_; //epoll底层文件系统的fd
    EventList events_; //epoll_wait的第二个参数，存储就绪的fd

};