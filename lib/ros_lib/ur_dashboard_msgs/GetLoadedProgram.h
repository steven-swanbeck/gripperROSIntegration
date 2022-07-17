#ifndef _ROS_SERVICE_GetLoadedProgram_h
#define _ROS_SERVICE_GetLoadedProgram_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace ur_dashboard_msgs
{

static const char GETLOADEDPROGRAM[] = "ur_dashboard_msgs/GetLoadedProgram";

  class GetLoadedProgramRequest : public ros::Msg
  {
    public:

    GetLoadedProgramRequest()
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

    virtual const char * getType() override { return GETLOADEDPROGRAM; };
    virtual const char * getMD5() override { return "d41d8cd98f00b204e9800998ecf8427e"; };

  };

  class GetLoadedProgramResponse : public ros::Msg
  {
    public:
      typedef const char* _answer_type;
      _answer_type answer;
      typedef const char* _program_name_type;
      _program_name_type program_name;
      typedef bool _success_type;
      _success_type success;

    GetLoadedProgramResponse():
      answer(""),
      program_name(""),
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
      uint32_t length_program_name = strlen(this->program_name);
      varToArr(outbuffer + offset, length_program_name);
      offset += 4;
      memcpy(outbuffer + offset, this->program_name, length_program_name);
      offset += length_program_name;
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
      uint32_t length_program_name;
      arrToVar(length_program_name, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_program_name; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_program_name-1]=0;
      this->program_name = (char *)(inbuffer + offset-1);
      offset += length_program_name;
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

    virtual const char * getType() override { return GETLOADEDPROGRAM; };
    virtual const char * getMD5() override { return "0193dad3cdb834b9f37e420a88468ebe"; };

  };

  class GetLoadedProgram {
    public:
    typedef GetLoadedProgramRequest Request;
    typedef GetLoadedProgramResponse Response;
  };

}
#endif
