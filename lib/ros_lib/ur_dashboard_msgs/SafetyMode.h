#ifndef _ROS_ur_dashboard_msgs_SafetyMode_h
#define _ROS_ur_dashboard_msgs_SafetyMode_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace ur_dashboard_msgs
{

  class SafetyMode : public ros::Msg
  {
    public:
      typedef uint8_t _mode_type;
      _mode_type mode;
      enum { NORMAL = 1 };
      enum { REDUCED = 2 };
      enum { PROTECTIVE_STOP = 3 };
      enum { RECOVERY = 4 };
      enum { SAFEGUARD_STOP = 5 };
      enum { SYSTEM_EMERGENCY_STOP = 6 };
      enum { ROBOT_EMERGENCY_STOP = 7 };
      enum { VIOLATION = 8 };
      enum { FAULT = 9 };
      enum { VALIDATE_JOINT_ID = 10 };
      enum { UNDEFINED_SAFETY_MODE = 11 };
      enum { AUTOMATIC_MODE_SAFEGUARD_STOP = 12 };
      enum { SYSTEM_THREE_POSITION_ENABLING_STOP = 13 };

    SafetyMode():
      mode(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->mode >> (8 * 0)) & 0xFF;
      offset += sizeof(this->mode);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      this->mode =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->mode);
     return offset;
    }

    virtual const char * getType() override { return "ur_dashboard_msgs/SafetyMode"; };
    virtual const char * getMD5() override { return "5da08725a63d4237bad689481131a84b"; };

  };

}
#endif
