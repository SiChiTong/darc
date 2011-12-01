#include <iostream>
#include <darc/node.h>
#include <darc/component_register.h>
#include <darc/udp_link.h>

int main(int argc, const char* argv[])
{
  // Create Node
  darc::Node::Ptr node( new darc::Node() );

  // Create and run Component1
  darc::Component::Ptr c1 = darc::ComponentRegister::InstantiateComponent( "Component2", node );
  boost::shared_ptr<boost::thread> c1_thread(new boost::thread( boost::bind(&darc::Component::run, c1)));

  // Create and run Component2
  //  darc::Component::Ptr c2 = darc::ComponentRegister::InstantiateComponent( "Component2", node );
  //  boost::shared_ptr<boost::thread> c2_thread(new boost::thread( boost::bind(&darc::Component::run, c2)));

  // You can also manually construct a component and call the run() method if you want.
  // But using the register allows for other cool stuff. E.g. starting remotely.

  darc::UDPLink::Ptr link1( new darc::UDPLink(&(node->io_service_), 7002) );
  link1->addRemoteNode(1, "127.0.0.1", "7001");
  node->remote_dispatch_handler_.addRemoteLink(1, link1);

  // Run Node in main thread
  node->run();
  return 0;
}
