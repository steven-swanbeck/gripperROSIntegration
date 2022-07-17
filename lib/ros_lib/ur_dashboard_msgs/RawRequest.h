#ifndef _ROS_SERVICE_RawRequest_h
#define _ROS_SERVICE_RawRequest_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace ur_dashboard_msgs
{

static const char RAWREQUEST[] = "ur_dashboard_msgs/RawRequest";

  class RawRequestRequest : public ros::Msg
  {
    public:
      typedef const char* _query_type;
      _query_type query;

    RawRequestRequest():
      query("")
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      uint32_t length_query = strlen(this->query);
      varToArr(outbuffer + offset, length_query);
      offset += 4;
      memcpy(outbuffer + offset, this->query, length_query);
      offset += length_query;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      uint32_t length_query;
      arrToVar(length_query, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_query; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_query-1]=0;
      this->query = (char *)(inbuffer + offset-1);
      offset += length_query;
     return offset;
    }

    virtual const char * getType() override { return RAWREQUEST; };
    virtual const char * getMD5() override { return "6490a46152f373285fe18f491ed93702"; };

  };

  class RawRequestResponse : public ros::Msg
  {
    public:
      typedef const char* _answer_type;
      _answer_type answer;

    RawRequestResponse():
      answer("")
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      uint32_t length_answer = strlen(this->answer);
      varToArr(outbuffer + offset, length_answer);
      offset += 4;
      memcpy(outbuffer + offset, this->answer, length_answer);
      offset += length_answer;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      uint32_t length_answer;
      arrToVar(length_answer, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_answer; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_answer-1]=0;
      this->answer = (char *)(inbuffer + offset-1);
      offset += length_answer;
     return offset;
    }

    virtual const char * getType() override { return RAWREQUEST; };
    virtual const char * getMD5() override { return "d7e708f879c94bb931716d8f4f130f30"; };

  };

  class RawRequest {
    public:
    typedef RawRequestRequest Request;
    typedef RawRequestResponse Response;
  };

}
#endif
