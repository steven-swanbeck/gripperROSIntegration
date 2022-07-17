#ifndef _ROS_SERVICE_GetRobotMode_h
#define _ROS_SERVICE_GetRobotMode_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "ur_dashboard_msgs/RobotMode.h"

namespace ur_dashboard_msgs
{

static const char GETROBOTMODE[] = "ur_dashboard_msgs/GetRobotMode";

  class GetRobotModeRequest : public ros::Msg
  {
    public:

    GetRobotModeRequest()
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

    virtual const char * getType() override { return GETROBOTMODE; };
    virtual const char * getMD5() override { return "d41d8cd98f00b204e9800998ecf8427e"; };

  };

  class GetRobotModeResponse : public ros::Msg
  {
    public:
      typedef ur_dashboard_msgs::RobotMode _robot_mode_type;
      _robot_mode_type robot_mode;
      typedef const char* _answer_type;
      _answer_type answer;
      typedef bool _success_type;
      _success_type success;

    GetRobotModeResponse():
      robot_mode(),
      answer(""),
      success(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->robot_mode.serialize(outbuffer + offset);
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
      offset += this->robot_mode.deserialize(inbuffer + offset);
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

    virtual const char * getType() override { return GETROBOTMODE; };
    virtual const char * getMD5() override { return "ef94c03da2e53bb8c576236bb003fe8a"; };

  };

  class GetRobotMode {
    public:
    typedef GetRobotModeRequest Request;
    typedef GetRobotModeResponse Response;
  };

}
#endif
