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
 * DARC ProtocolManagerBase class
 *
 * \author Morten Kjaergaard
 */

#ifndef __DARC_NETWORK_PROTOCOL_MANAGER_BASE_H__
#define __DARC_NETWORK_PROTOCOL_MANAGER_BASE_H__

#include <string>
#include <darc/network/link_manager_callback_if.h>
#include <darc/network/id_types.h>
#include <darc/shared_buffer_array.h>

#include <darc/network/packet/discover.h> 

namespace darc
{
namespace network
{

class ProtocolManagerBase
{
protected:
  ProtocolManagerBase()
  {
  }

public:
  virtual ~ProtocolManagerBase() {}

  virtual const ID& accept(const std::string& protocol, const std::string& url) = 0;
  virtual void connect(const std::string& protocol, const std::string& url) = 0;

  virtual void sendPacket(const ConnectionID& outbound_id,
			  packet::Header::PayloadType type, const NodeID& recv_node_id,
			  SharedBuffer buffer, std::size_t data_len) = 0;
/*
  void sendDiscover(const ID& outbound_id)
  {
    std::size_t data_len = 1024*32;
    SharedBuffer buffer = SharedBufferArray::create(data_len);

    // Create packet
    network::packet::Discover discover(outbound_id);
    std::size_t len = discover.write(buffer.data(), buffer.size());
    sendPacket(outbound_id, network::packet::Header::DISCOVER_PACKET, ID::null(), buffer, len);
  }
*/
};

} // namespace network
} // namespace darc

#endif
