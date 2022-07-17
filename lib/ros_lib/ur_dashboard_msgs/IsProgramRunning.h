#ifndef _ROS_SERVICE_IsProgramRunning_h
#define _ROS_SERVICE_IsProgramRunning_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace ur_dashboard_msgs
{

static const char ISPROGRAMRUNNING[] = "ur_dashboard_msgs/IsProgramRunning";

  class IsProgramRunningRequest : public ros::Msg
  {
    public:

    IsProgramRunningRequest()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
     return offset;
    }

    virtual const char * getType() override { return ISPROGRAMRUNNING; };
    virtual const char * getMD5() override { return "d41d8cd98f00b204e9800998ecf8427e"; };

  };

  class IsProgramRunningResponse : public ros::Msg
  {
    public:
      typedef const char* _answer_type;
      _answer_type answer;
      typedef bool _program_running_type;
      _program_running_type program_running;
      typedef bool _success_type;
      _success_type success;

    IsProgramRunningResponse():
      answer(""),
      program_running(0),
      success(0)
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
      union {
        bool real;
        uint8_t base;
      } u_program_running;
      u_program_running.real = this->program_running;
      *(outbuffer + offset + 0) = (u_program_running.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->program_running);
      union {
        bool real;
        uint8_t base;
      } u_success;
      u_success.real = this->success;
      *(outbuffer + offset + 0) = (u_success.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->success);
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
      union {
        bool real;
        uint8_t base;
      } u_program_running;
      u_program_running.base = 0;
      u_program_running.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->program_running = u_program_running.real;
      offset += sizeof(this->program_running);
      union {
        bool real;
        uint8_t base;
      } u_success;
      u_success.base = 0;
      u_success.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->success = u_success.real;
      offset += sizeof(this->success);
     return offset;
    }

    virtual const char * getType() override { return ISPROGRAMRUNNING; };
    virtual const char * getMD5() override { return "290f0cb02c3a44353957f0a92f954209"; };

  };

  class IsProgramRunning {
    public:
    typedef IsProgramRunningRequest Request;
    typedef IsProgramRunningResponse Response;
  };

}
#endif
