#pragma once

#include "noncopyable.h"
#include "Timestamp.h"

#include <functional>
#include <memory>

class EventLoop;

//Channel里面有Poller监听的fd，有自己感兴趣的事件，当前发生的事件，所以Channel负责回调函数的调用
//EventLoop对应Reactor里面的事件分发器Demultiplex，里面Poller负责监听事件，Channel负责调用回调函数处理
class Channel{
public:
    using EventCallback = std::function<void()>;
    using ReadEventCallback = std::function<void(Timestamp)>;

    Channel(EventLoop* loop, int fd);
    ~Channel(); // 析构函数负责销毁自身

    //poller通知发生的事件之后，调用该函数处理事件
    void handleEvent(Timestamp recviveTime);

    //设置回调函数
    void setReadCallback(ReadEventCallback cb) {readCallback_ = std::move(cb);}
    void setWriteCallback(EventCallback cb) { writeCallback_ = std::move(cb);}
    void setCloseCallback(EventCallback cb) { closeCallback_ = std::move(cb);}
    void setErrorCallback(EventCallback cb) { errorCallback_ = std::move(cb);}

    //防止收到remove channel之后，还在执行回调函数，导致错误
    void tie(const std::shared_ptr<void>&);

    int fd() const{ return fd_; }
    int events() const{ return events_;}
    int set_revents(int revt) { return revents_ = revt; }

    //设置fd感兴趣的事件
    void enableReading(){ events_ |= kReadEvent; update(); }
    void disableReading(){ events_ & ~kReadEvent; update(); }
    void enableWriting(){ events_ |= kWriteEvent; update(); }
    void disableWriting(){ events_ & ~kWriteEvent; update(); }
    void disableAll(){ events_ = kNoneEvent; update(); }

    //返回fd当前感兴趣的事件状态
    bool isNoneEvent()const { return events_ == kNoneEvent; }
    bool isWriting() const { return events_ & kWriteEvent; }
    bool isReading() const { return events_ & kReadEvent; }

    //TODO
    int index(){ return index_; }
    void set_index(int idx) {index_ = idx; }

    //one loop per thread，当前channel属于哪个eventLoop
    EventLoop* ownerLoop(){return loop_; }
    void remove(); // 删除当前通道

private:
    void update(); //更新感兴趣的事件
    void handleEventWithGuard(Timestamp receiveTime); //事件处理函数底层调用

    //事件触发的状态
    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;

    EventLoop* loop_; //事件循环
    const int fd_;    //当前监听的fd
    int events_;      //感兴趣的事件
    int revents_;     //poller返回正在发生的事件
    int index_;       //下标 TODO

    std::weak_ptr<void> tie_;  //检测TcpConnection是否存在，也要防止循环引用，Channel会随TcpConnect销毁而销毁
    bool tied_;

    //回调函数
    ReadEventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback closeCallback_;
    EventCallback errorCallback_;
};