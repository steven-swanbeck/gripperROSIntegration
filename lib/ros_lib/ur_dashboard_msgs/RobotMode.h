#ifndef _ROS_ur_dashboard_msgs_RobotMode_h
#define _ROS_ur_dashboard_msgs_RobotMode_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace ur_dashboard_msgs
{

  class RobotMode : public ros::Msg
  {
    public:
      typedef int8_t _mode_type;
      _mode_type mode;
      enum { NO_CONTROLLER = -1 };
      enum { DISCONNECTED = 0 };
      enum { CONFIRM_SAFETY = 1 };
      enum { BOOTING = 2 };
      enum { POWER_OFF = 3 };
      enum { POWER_ON = 4 };
      enum { IDLE = 5 };
      enum { BACKDRIVE = 6 };
      enum { RUNNING = 7 };
      enum { UPDATING_FIRMWARE = 8 };

    RobotMode():
      mode(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      union {
        int8_t real;
        uint8_t base;
      } u_mode;
      u_mode.real = this->mode;
      *(outbuffer + offset + 0) = (u_mode.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->mode);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      union {
        int8_t real;
        uint8_t base;
      } u_mode;
      u_mode.base = 0;
      u_mode.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->mode = u_mode.real;
      offset += sizeof(this->mode);
     return offset;
    }

    virtual const char * getType() override { return "ur_dashboard_msgs/RobotMode"; };
    virtual const char * getMD5() override { return "73b72d58742c4889c47118411b03a3b4"; };

  };

}
#endif
