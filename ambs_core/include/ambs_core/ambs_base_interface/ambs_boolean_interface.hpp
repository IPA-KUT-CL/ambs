#ifndef AMBSBASEBOOLEANINTERFACE_HPP
#define AMBSBASEBOOLEANINTERFACE_HPP

#include "ambs_core/ambs_base_interface/ambs_base_interface.hpp"
#include "ambs_msgs/BoolStamped.h"
#include <iterator>

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

  ambs_msgs::BoolStamped constructNewBoolStamped(bool data);
  ambs_msgs::BoolStamped waitForTrueOnPort(std::string port);
  std::vector<bool> getAllOutPortMsg();
  std::vector<bool> getAllInPortMsg();
  int inputsSize;
  int outputsSize;

private:
  const double wait_loop_rate = 500;
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
  inputsSize = extended_bool_inputs.size();
  outputsSize = extended_bool_outputs.size();
}

/**
 * @brief Create a new BoolStamped type message
 *
 * @param[in] data The data to be held by the new msg
 * @returns msg The message to be returned
 */
inline ambs_msgs::BoolStamped AMBSBooleanInterface::constructNewBoolStamped(bool data)
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

inline std::vector<bool> AMBSBooleanInterface::getAllInPortMsg()
{
  std::vector<bool> inPortMsg;
  auto iter = interfaces_.begin();
  for(int i = 0; i < inputsSize; i++){
    inPortMsg.push_back(iter->second.msg_.data);
    ++iter;
  }
  return inPortMsg;
}

inline std::vector<bool> AMBSBooleanInterface::getAllOutPortMsg()
{
  std::vector<bool> outPortMsg;
  auto iter = interfaces_.begin();
  std::advance(iter, inputsSize);
  for(int i = 0; i < outputsSize; i++){
    outPortMsg.push_back(iter->second.msg_.data);
    ++iter;
  }
  return outPortMsg;
}


}  // namespace ambs_base
#endif // AMBSBASEBOOLEANINTERFACE_HPP
