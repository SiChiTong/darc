#if 0

#pragma once

#include <boost/asio.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/signals.hpp>

#include <darc/id.hpp>

namespace darc
{
namespace pubsub
{

typedef IDType darc::ID;

template<typename T>
class PublisherImpl
{
private:
  boost::asio::io_service &io_service_;
  MessageServiceType message_service_;
  IDType topic_id_;

  typename MessageServiceType::template DispatcherType<T> * dispatcher_; // ptr type?

public:
  PublisherImpl(boost::asio::io_service &io_service,
                MessageService &message_service,
                const IDType& topic_id) :
    io_service_(io_service),
    message_service_(message_service),
    topic_id_(topic_id)
  {
  }

  void attach();
  void detach();
  void publish(const boost::shared_ptr<const T&> msg);

};

template<typename T>
class Publisher
{
private:
  boost::scoped_ptr<PublisherImpl<T> > impl_;

public:
  Publisher(boost::asio::io_service &io_service,
            const IDType& topic_id,
            MessageService &message_service) :
    impl_(new PublisherImpl<T, MessageServiceType>(io_service,
                                                   topic_id,
                                                   message_service) )
  {
    impl_->attach();
  }

  void publish(const boost::shared_ptr<const T&> msg)
  {
    impl_->publish(msg);
  }

};

}
}
#endif
