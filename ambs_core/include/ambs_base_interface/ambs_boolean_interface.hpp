#ifndef AMBSBASEBOOLEANINTERFACE_HPP
#define AMBSBASEBOOLEANINTERFACE_HPP

#include "ambs_base_interface/ambs_base_interface.hpp"
#include "ambs_msgs/BoolStamped.h"

namespace ambs_base {

/**
 * @brief A bool specialisation of AMBSTemplatedInterface. Offers two extra functions
 */
class AMBSBooleanInterface : public AMBSTemplatedInterface<ambs_msgs::BoolStamped>
{
public:
  AMBSBooleanInterface() {}
  AMBSBooleanInterface(ros::NodeHandle nh,
                       std::string node_name,
                       std::vector<std::string> extended_bool_inputs,
                       std::vector<std::string> extended_bool_outputs);
  virtual ~AMBSBooleanInterface() {}

  ambs_msgs::BoolStamped contructNewBoolStamped(bool data);
  ambs_msgs::BoolStamped waitForTrueOnPort(std::string port);

private:
  const double wait_loop_rate = 100;
};


// ---------------------------------------------------------------------------------------------------------------------
//                                              IMPLEMENTATION
// ---------------------------------------------------------------------------------------------------------------------


inline AMBSBooleanInterface::AMBSBooleanInterface(ros::NodeHandle nh,
                                           std::string node_name,
                                           std::vector<std::string> extended_bool_inputs,
                                           std::vector<std::string> extended_bool_outputs)
{
  init(extended_bool_inputs, extended_bool_outputs, nh, node_name);
}

/**
 * @brief Create a new BoolStamped type message
 *
 * @param[in] data The data to be held by the new msg
 * @returns msg The message to be returned
 */
inline ambs_msgs::BoolStamped AMBSBooleanInterface::contructNewBoolStamped(bool data)
{
  ambs_msgs::BoolStamped msg;
  msg.data  = data;
  msg.header = std_msgs::Header();
  msg.header.stamp = ros::Time::now();
  return msg;
}

/**
 * @brief Wait for TRUE on a port
 * @param port The port to wait on
 * @returns The msg that set TRUE
 */
inline ambs_msgs::BoolStamped AMBSBooleanInterface::waitForTrueOnPort(std::string port)
{
  ambs_msgs::BoolStamped msg;
  ros::Rate loop(wait_loop_rate);
  while(ros::ok())
  {
    loop.sleep();
    ros::spinOnce();
    msg = getPortMsg(port);
    if(msg.data)
    {
      break;
    }
  }
  resetPort(port);
  return msg;
}


}  // namespace ambs_base
#endif // AMBSBASEBOOLEANINTERFACE_HPP
