#ifndef _ROS_SERVICE_GetProgramState_h
#define _ROS_SERVICE_GetProgramState_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "ur_dashboard_msgs/ProgramState.h"

namespace ur_dashboard_msgs
{

static const char GETPROGRAMSTATE[] = "ur_dashboard_msgs/GetProgramState";

  class GetProgramStateRequest : public ros::Msg
  {
    public:

    GetProgramStateRequest()
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

    virtual const char * getType() override { return GETPROGRAMSTATE; };
    virtual const char * getMD5() override { return "d41d8cd98f00b204e9800998ecf8427e"; };

  };

  class GetProgramStateResponse : public ros::Msg
  {
    public:
      typedef ur_dashboard_msgs::ProgramState _state_type;
      _state_type state;
      typedef const char* _program_name_type;
      _program_name_type program_name;
      typedef const char* _answer_type;
      _answer_type answer;
      typedef bool _success_type;
      _success_type success;

    GetProgramStateResponse():
      state(),
      program_name(""),
      answer(""),
      success(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->state.serialize(outbuffer + offset);
      uint32_t length_program_name = strlen(this->program_name);
      varToArr(outbuffer + offset, length_program_name);
      offset += 4;
      memcpy(outbuffer + offset, this->program_name, length_program_name);
      offset += length_program_name;
      uint32_t length_answer = strlen(this->answer);
      varToArr(outbuffer + offset, length_answer);
      offset += 4;
      memcpy(outbuffer + offset, this->answer, length_answer);
      offset += length_answer;
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
      offset += this->state.deserialize(inbuffer + offset);
      uint32_t length_program_name;
      arrToVar(length_program_name, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_program_name; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_program_name-1]=0;
      this->program_name = (char *)(inbuffer + offset-1);
      offset += length_program_name;
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
      } u_success;
      u_success.base = 0;
      u_success.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->success = u_success.real;
      offset += sizeof(this->success);
     return offset;
    }

    virtual const char * getType() override { return GETPROGRAMSTATE; };
    virtual const char * getMD5() override { return "522d35a6f2ebd9702cb1e33489e1aa96"; };

  };

  class GetProgramState {
    public:
    typedef GetProgramStateRequest Request;
    typedef GetProgramStateResponse Response;
  };

}
#endif
