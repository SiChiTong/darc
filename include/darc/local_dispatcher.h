#ifndef __DARC_LOCAL_DISPATCHER_H__
#define __DARC_LOCAL_DISPATCHER_H__

#include <vector>

#include <boost/smart_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <darc/subscriber_impl.h>
#include <darc/serialized_message.h>
#include <darc/remote_dispatch_handler.h>

namespace darc
{

class LocalDispatcherAbstract
{
public:
  virtual ~LocalDispatcherAbstract() {}

  void DispatchSerializedMessage()
  {
    // Some Stuff
  }
};

template<typename T>
class LocalDispatcher : public LocalDispatcherAbstract
{
public:
  typedef std::vector< boost::shared_ptr<SubscriberImpl<T> > > SubscriberListType;
  SubscriberListType subscriber_list_;

  //void RegisterSubscriber( SubscriberImpl<T>::WeakPtr sub )
  void RegisterSubscriber( boost::shared_ptr<SubscriberImpl<T> > sub )
  {
    subscriber_list_.push_back( sub );
  }

  // Called by the local publishers  
  void DispatchMessage( boost::shared_ptr<T> &msg )
  {
    for( typename SubscriberListType::iterator it = subscriber_list_.begin();
         it != subscriber_list_.end();
         it++)
    {
      (*it)->Dispatch( msg );
    }
  }
};

}

#endif