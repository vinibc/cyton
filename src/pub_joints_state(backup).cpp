//     Copyright (c) 2009-2012 Energid Technologies. All rights reserved. ////
//
// Filename:    send_joints.cpp
//
// Description: Action client send joint values and joint rates to hardware_node
//
// Contents:    Example code to move cyton to some joints
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
void move_callback(const cyton::cytonActionFeedbackConstPtr& position){
  cyton::cytonFeedback pos=position->feedback;
  for(int i = 0; i<7; i++){
    jstate.position.push_back(pos.position[i]);
  }
  jstate.position[2]= pos.position[3];
  jstate.name.push_back("shoulder_base");
  jstate.name.push_back("shoulder_pitch");
  jstate.name.push_back("shoulder_yaw");
  jstate.name.push_back("elbow_pitch");
  jstate.name.push_back("wrist_roll");
  jstate.name.push_back("wrist_yaw");
  jstate.name.push_back("wrist_pitch");
  
  jstate.header.stamp = ros::Time::now();
  pub.publish(jstate);
  ROS_INFO("pUBLISH");
}

int main(int argc, char **argv)

{
   ros::init(argc,argv,"state_publish"); // ,ros::init_options::NoSigintHandler)
   ros::NodeHandle n;
  
   pub = n.advertise<sensor_msgs::JointState>("/joint_states", 10);
   ros::Subscriber sub = n.subscribe("/cyton/feedback",1000, move_callback);
   

   ROS_INFO("Listening to position");

   ros::spin();
 
return 0;
}



