
#include <sys/epoll.h>
#include "Channel.h"
#include "EventLoop.h"
#include "logger.h"


const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = EPOLLIN  | EPOLLPRI;
const int Channel::kWriteEvent = EPOLLOUT;

//构造函数
Channel::Channel(EventLoop* loop, int fd)
    : loop_(loop)
    , fd_(fd)
    , events_(0)
    , revents_(0)
    , index_(-1)
    , tied_(false){

}

Channel::~Channel(){

}

//TODO
//channel方法什么时候调用tie?
void Channel::tie(const std::shared_ptr<void>& obj){
    tie_ = obj;
    tied_ = true;
}

//TODO
//在epoll上面更新当前fd感兴趣的事件，需要epoll_ctl()
void Channel::update(){
    //通过EventLoop调用poller的相关方法才能注册fd的events事件 ，先注释
    //loop_->updateChannel(this);
}

//TODO
//EventLoop把当前的channel删除，对话结束的时候删除，目前未知
void Channel::remove(){
    //loop_->removeChannel(this);
}


//fd在得到epoll的通知之后要进行处理
void Channel::handleEvent(Timestamp receiveTime){

    //tied_情况，现在已经开始对话了
    if(tied_){
        //想要执行对话的处理，先判断当前对话是否已经关闭
        std::shared_ptr<void> guard = tie_.lock();
        if(guard){
            handleEventWithGuard(receiveTime);
        }
    }else{
        handleEventWithGuard(receiveTime);
    }
}


//根据fd的触发事件，调用相对于的回调函数
void Channel::handleEventWithGuard(Timestamp receiveTime){
    LOG_INFO("channel handleEvent revents:%d", revents_);

    //对话关闭了
    if((revents_ & EPOLLHUP) && !(revents_ & EPOLLIN)){
        if(closeCallback_){
            closeCallback_();
        }
    }

    //如果出错了
    if(revents_ & EPOLLERR){
        if(closeCallback_){
            closeCallback_();
        }
    }

    //如果可读
    if(revents_ & (EPOLLIN | EPOLLPRI)){
        if(readCallback_){
            readCallback_(receiveTime);
        }
    }

    //如果可写
    if(revents_ & EPOLLOUT){
        if(writeCallback_){
            writeCallback_();
        }
    }
}
