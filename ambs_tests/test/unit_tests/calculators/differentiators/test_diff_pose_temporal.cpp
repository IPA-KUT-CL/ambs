#include <gtest/gtest.h>
#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <std_msgs/Float64.h>

#include "ambs_msgs/BoolStamped.h"
#include "ambs_core/ambs_base_interface/ambs_boolean_interface.hpp"
#include "ambs_core/ambs_base_interface/ambs_base_interface.hpp"
#include "ambs_core/ambs_helper/helper.h"

class TestDiffPoseTemporal : public testing::Test
{
public:
  TestDiffPoseTemporal() {}

  const std::string START_="/out_start";
  const std::string STOP_="/out_stop";
  const std::string RESET_="/out_reset";
  const std::string DONE_="/in_done";
  const std::string POSE_="/out_pose";
  const std::string TRANSLATION_="/in_diff_position";
  const std::string ORIENTATION_="/in_diff_orientation";
  const double response_time_ = 0.2;
  const double spawn_time_ = 2;

  std::vector<std::string> bool_inputs_{DONE_};
  std::vector<std::string> bool_outputs_{START_, STOP_, RESET_};
  std::vector<std::string> pose_inputs_;
  std::vector<std::string> pose_outputs_{POSE_};
  std::vector<std::string> float_inputs_{TRANSLATION_, ORIENTATION_};
  std::vector<std::string> float_outputs_;

  ambs_base::AMBSBooleanInterface control_iface;
  ambs_base::AMBSTemplatedInterface<geometry_msgs::PoseStamped> pose_interface_;
  ambs_base::AMBSTemplatedInterface<std_msgs::Float64> float_interface_;
  ros::NodeHandle nh_;

  void init(ros::NodeHandle nh);

};


void TestDiffPoseTemporal::init(ros::NodeHandle nh)
{
  nh_ = nh;

  control_iface.init(bool_inputs_, bool_outputs_, nh_,ros::this_node::getName());
  pose_interface_.init(pose_inputs_, pose_outputs_, nh_,ros::this_node::getName());
  float_interface_.init(float_inputs_, float_outputs_, nh_,ros::this_node::getName());
}

TEST_F(TestDiffPoseTemporal, test_translation_diff_once)
{
  ros::NodeHandle nh;
  init(nh);

  // Waiting for tested nodelet to come up
  ros::Duration(spawn_time_).sleep();

  geometry_msgs::PoseStamped msg;
  msg.pose.position.x = 0;
  msg.pose.position.y = 0;
  msg.pose.position.z = 0;
  msg.pose.orientation.x = 0;
  msg.pose.orientation.y = 0;
  msg.pose.orientation.z = 0;
  msg.pose.orientation.w = 1;

  pose_interface_.publishMsgOnPort(POSE_, msg);
  ros::Duration(response_time_).sleep();
  control_iface.publishMsgOnPort(START_, control_iface.constructNewBoolStamped(true));
  ros::Duration(response_time_).sleep();

  msg.pose.position.x = 1;
  msg.pose.position.y = 1;

  pose_interface_.publishMsgOnPort(POSE_, msg);
  ros::Duration(response_time_).sleep();
  control_iface.publishMsgOnPort(STOP_, control_iface.constructNewBoolStamped(true));
  ros::Duration(response_time_).sleep();

  EXPECT_FLOAT_EQ(float_interface_.getPortMsg(TRANSLATION_).data,1.4142135);
  EXPECT_TRUE(control_iface.getPortMsg(DONE_).data);
}


TEST_F(TestDiffPoseTemporal, test_orientation_diff)
{
  ros::NodeHandle nh;
  init(nh);

  ros::Duration(spawn_time_/2).sleep();
  control_iface.publishMsgOnPort(RESET_, control_iface.constructNewBoolStamped(true));
  ros::Duration(spawn_time_/2).sleep();

  geometry_msgs::PoseStamped msg;
  msg.pose.position.x = 0;
  msg.pose.position.y = 0;
  msg.pose.position.z = 0;
  msg.pose.orientation.x = 0;
  msg.pose.orientation.y = 0;
  msg.pose.orientation.z = 0;
  msg.pose.orientation.w = 1;

  pose_interface_.publishMsgOnPort(POSE_, msg);
  ros::Duration(response_time_).sleep();
  control_iface.publishMsgOnPort(START_, control_iface.constructNewBoolStamped(true));
  ros::Duration(response_time_).sleep();

  msg.pose.orientation.z = 0.7071068; // 90 degrees
  msg.pose.orientation.w = 0.7071068;

  pose_interface_.publishMsgOnPort(POSE_, msg);
  ros::Duration(response_time_).sleep();
  control_iface.publishMsgOnPort(STOP_, control_iface.constructNewBoolStamped(true));
  ros::Duration(response_time_).sleep();

  EXPECT_FLOAT_EQ(float_interface_.getPortMsg(ORIENTATION_).data,90);
  EXPECT_TRUE(control_iface.getPortMsg(DONE_).data);
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    ros::init(argc, argv, "test_POSE_param_comp");

    ros::AsyncSpinner spinner(1);
    spinner.start();
    int ret = RUN_ALL_TESTS();
    spinner.stop();
    return ret;
}

