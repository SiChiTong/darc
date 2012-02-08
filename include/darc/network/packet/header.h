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
 * DARC Header class
 *
 * \author Morten Kjaergaard
 */

#ifndef __DARC_PACKET_HEADER_H_INCLUDED__
#define __DARC_PACKET_HEADER_H_INCLUDED__

#include <darc/id.h>

namespace darc
{
namespace network
{
namespace packet
{

struct Header
{
  typedef enum {NONE = 0x00,
		DISCOVER_PACKET = 0x01,
		DISCOVER_REPLY_PACKET = 0x02,
		MSG_PACKET = 0x03,
		PROCEDURE_CALL_PACKET = 0x04,
		PROCEDURE_STATUS_PACKET = 0x05,
		PROCEDURE_RETURN_PACKET = 0x06
  } PayloadType;

  ID sender_node_id;
  PayloadType payload_type; //store as uint8

  Header( const ID& node_id, PayloadType payload_type):
    sender_node_id(node_id),
    payload_type(payload_type)
  {
  }

  Header():
    sender_node_id(), //nullID
    payload_type(NONE)
  {
  }

  size_t read( const uint8_t * data, size_t len )
  {
    size_t idx = Parser::readID(sender_node_id, data, len);
    payload_type = (PayloadType)data[idx];
    return size();
  }

  size_t write( uint8_t * data, size_t len )
  {
    size_t idx = Parser::writeID(sender_node_id, data, len);
    data[idx] = (uint8_t) payload_type;
    return size();
  }

  static size_t size()
  {
    return ID::static_size()+1;
  }

};

} // namespace packet
} // namespace network
} // namespace darc

#endif