//     LCAD - Cyton-Alpha						 ////
//
// Filename:    pub_joint_state.cpp
//
// Description: Node to send joint values and joint rates to joint_states topic
//
/////////////////////////////////////////////////////////////////////////


//ROS Headers
#include "ros/ros.h"
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <cyton/cytonAction.h>
#include <boost/thread.hpp>
#include <sensor_msgs/JointState.h>

//System headers
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>
#include <stdlib.h>



using namespace std;

//global variables
bool finished_before_timeout ;
sensor_msgs::JointState jstate;
ros::Publisher pub;
const double g_multiplicador = (1.57/0.02);
double convertRadToPos(double rad);
void move_callback(const cyton::cytonActionFeedbackConstPtr& position){
  cyton::cytonFeedback pos=position->feedback;
  for(int i = 0; i<7; i++){
    jstate.position.push_back(pos.position[i]);
  }
  jstate.position[7] = (-1)*convertRadToPos(pos.gripper_feed_value);
  jstate.position[8] = convertRadToPos(pos.gripper_feed_value);
  
  jstate.name.push_back("shoulder_base");
  jstate.name.push_back("shoulder_pitch");
  jstate.name.push_back("shoulder_yaw");
  jstate.name.push_back("elbow_pitch");
  jstate.name.push_back("wrist_roll");
  jstate.name.push_back("wrist_yaw");
  jstate.name.push_back("wrist_pitch");
  jstate.name.push_back("claw_left");
  jstate.name.push_back("claw_right");
  
  jstate.header.stamp = ros::Time::now();
  pub.publish(jstate);
  ROS_INFO("PUBLISH");
}
double convertRadToPos(double rad)
{
  double p = 0.0;
  p = rad*g_multiplicador;
  return p;
}

int main(int argc, char **argv)

{
   ros::init(argc,argv,"state_publish"); // ,ros::init_options::NoSigintHandler)
   ros::NodeHandle nh;
  
   pub = nh.advertise<sensor_msgs::JointState>("/joint_states", 10);
   ros::Subscriber sub = nh.subscribe("/cyton/feedback",10, move_callback);
   

   ROS_INFO("Listening to position");

   ros::spin();
 
return 0;
}



