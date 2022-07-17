#ifndef _ROS_SERVICE_SetPayload_h
#define _ROS_SERVICE_SetPayload_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "geometry_msgs/Vector3.h"

namespace ur_msgs
{

static const char SETPAYLOAD[] = "ur_msgs/SetPayload";

  class SetPayloadRequest : public ros::Msg
  {
    public:
      typedef float _mass_type;
      _mass_type mass;
      typedef geometry_msgs::Vector3 _center_of_gravity_type;
      _center_of_gravity_type center_of_gravity;

    SetPayloadRequest():
      mass(0),
      center_of_gravity()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      union {
        float real;
        uint32_t base;
      } u_mass;
      u_mass.real = this->mass;
      *(outbuffer + offset + 0) = (u_mass.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_mass.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_mass.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_mass.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->mass);
      offset += this->center_of_gravity.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      union {
        float real;
        uint32_t base;
      } u_mass;
      u_mass.base = 0;
      u_mass.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_mass.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_mass.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_mass.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->mass = u_mass.real;
      offset += sizeof(this->mass);
      offset += this->center_of_gravity.deserialize(inbuffer + offset);
     return offset;
    }

    virtual const char * getType() override { return SETPAYLOAD; };
    virtual const char * getMD5() override { return "6a2cd594b640cc49946d268b22a837bd"; };

  };

  class SetPayloadResponse : public ros::Msg
  {
    public:
      typedef bool _success_type;
      _success_type success;

    SetPayloadResponse():
      success(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
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

    virtual const char * getType() override { return SETPAYLOAD; };
    virtual const char * getMD5() override { return "358e233cde0c8a8bcfea4ce193f8fc15"; };

  };

  class SetPayload {
    public:
    typedef SetPayloadRequest Request;
    typedef SetPayloadResponse Response;
  };

}
#endif
