#include <map>
#include <string>
#include "ambs_base/ambs_base.hpp"

template<class T>
void templatedCB(const T msg)
{
  ROS_INFO_STREAM("Val: " << std::to_string(msg->data));
}


int main(int argc, char **argv)
{
  ros::init(argc, argv, "test_base");
  ros::NodeHandle nh;

  std::map<std::string, std::string> control_inputs;
  control_inputs["start"] = "/start";
  control_inputs["stop"] = "/stop";
  control_inputs["reset"] = "/reset";

  std::map<std::string, std::string> control_outputs;
  control_outputs["done"] = "/done";
  control_outputs["result"] = "/result";


  ambs_base::AmbsBase test_obj = ambs_base::AmbsBase(control_inputs, control_outputs, nh);

  ros::Subscriber test_sub =
      nh.subscribe<ambs_msgs::BoolStamped::ConstPtr>("/test_sub", 10,
                                                      templatedCB<ambs_msgs::BoolStamped::ConstPtr>);

  ROS_INFO("Hello World");
  ros::spin();
}
