/*
 * Copyright (c) 2011, Prevas A/S
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Prevas A/S nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * DARC Component class
 *
 * \author Morten Kjaergaard
 */

#ifndef __DARC_COMPONENT_H_INCLUDED__
#define __DARC_COMPONENT_H_INCLUDED__

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <darc/node.h>
#include <darc/owner.h>
#include <darc/enable_weak_from_static.h>
#include <darc/id.h>
#include <darc/log.h>

namespace darc
{

namespace python { class ComponentProxy; }

class Component : public Owner, public EnableWeakFromStatic<Component>
{
  friend class python::ComponentProxy;

public:
  typedef boost::shared_ptr<Component> Ptr;

protected:
  std::string name_;
  boost::shared_ptr<Node> node_;
  bool attached_;
  boost::asio::io_service io_service_;
  boost::scoped_ptr<boost::asio::io_service::work> keep_alive_;
  ID id_;

  boost::posix_time::time_duration statistics_period_;
  boost::asio::deadline_timer statistics_timer_;

protected:
  Component():
    name_(""),
    attached_(false),
    id_(ID::create()),
    statistics_period_(boost::posix_time::seconds(1)),
    statistics_timer_(io_service_, statistics_period_)
  {
  }

  /*
  Component(const std::string& name, boost::shared_ptr<Node> node):
    name_(""),
    attached(false),
    id_(ID::create()),
    statistics_period_(boost::posix_time::seconds(1)),
    statistics_timer_(io_service_, statistics_period_)
  {
  }
  */

  void attachNode(const std::string& instance_name, Node::Ptr node)
  {
    attached_ = true;
    name_ = instance_name;
    node = node;
  }

  void statisticsTimerHandler(const boost::system::error_code& error)
  {
    if(!error)
    {
      statistics_timer_.expires_from_now(statistics_period_);
      statistics_timer_.async_wait(boost::bind( &Component::statisticsTimerHandler, this, boost::asio::placeholders::error ));
      latchStatistics( statistics_period_.total_milliseconds() );
    }
  }


public:
  // impl of darc::Owner
  boost::asio::io_service * getIOService()
  {
    return &io_service_;
  }

  // impl of darc::Owner
  boost::shared_ptr<Node> getNode()
  {
    assert(attached_);
    return node_;
  }

  virtual void onStart()
  {
    statistics_timer_.expires_from_now(statistics_period_);
    statistics_timer_.async_wait(boost::bind( &Component::statisticsTimerHandler, this, boost::asio::placeholders::error ));
  }

  template<typename T>
  static boost::shared_ptr<T> instantiate( const std::string& instance_name, Node::Ptr node )
  {
    boost::shared_ptr<T> instance( new T() );
    instance->attachNode(instance_name, node);
    node->attach(instance);
    return instance;
  }

  const std::string getName() const
  {
    return name_;
  }

  const ID getID() const
  {
    return id_;
  }

  const ID& getComponentID()
  {
    return id_;
  }

  void run()
  {
    assert(attached_);
    node_->runComponent(id_);
  }

  void stop()
  {
    assert(attached_);
    node_->stopComponent(id_);
  }

  void pause()
  {
    pausePrimitives();
  }

  void unpause()
  {
    unpausePrimitives();
  }

  void work()
  {
    DARC_INFO("Running Component: %s", name_.c_str());
    keep_alive_.reset( new boost::asio::io_service::work(io_service_) );
    startPrimitives();
    onStart();
    io_service_.reset();
    io_service_.run();
    DARC_INFO("Stopped Component: %s", name_.c_str());
  }

  void stop_work()
  {
    stopPrimitives();
    keep_alive_.reset();
  }

};

typedef boost::shared_ptr<Component> ComponentPtr;
typedef boost::weak_ptr<Component> ComponentWkPtr;

}

// Include here so its available for the components
#include <darc/registry.h>

#endif
