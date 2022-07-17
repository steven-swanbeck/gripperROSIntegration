#ifndef _ROS_ur_dashboard_msgs_ProgramState_h
#define _ROS_ur_dashboard_msgs_ProgramState_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace ur_dashboard_msgs
{

  class ProgramState : public ros::Msg
  {
    public:
      typedef const char* _state_type;
      _state_type state;
      enum { STOPPED = STOPPED };
      enum { PLAYING = PLAYING };
      enum { PAUSED = PAUSED };

    ProgramState():
      state("")
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      uint32_t length_state = strlen(this->state);
      varToArr(outbuffer + offset, length_state);
      offset += 4;
      memcpy(outbuffer + offset, this->state, length_state);
      offset += length_state;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      uint32_t length_state;
      arrToVar(length_state, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_state; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_state-1]=0;
      this->state = (char *)(inbuffer + offset-1);
      offset += length_state;
     return offset;
    }

    virtual const char * getType() override { return "ur_dashboard_msgs/ProgramState"; };
    virtual const char * getMD5() override { return "0cf2a8b9397741f9e4ac8b71fd354966"; };

  };

}
#endif
