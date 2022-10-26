#include "ros/ros.h"
#include "nav_msgs/Odometry.h"
#include "geometry_msgs/Twits"

geometry_msgs::Twist msg;
nav_msgs::Odometry cam;


void posicion(const nav_msgs::Odometry &pos){
  cam=pos;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "subscriptor_publicador");
  ros::NodeHandle nh;
  ros::Subscriber subcripcion = nh.subscribe("/RosAria/pose", 1000, posicion);

  ros::Publisher publicacion = nh.advertise<nav:<nav_msgs::Odometry>("/revision",1000);
  ros::Rate rate(10);
  while (ros::ok()){
      ros::spinOnce();
    
  }

  return 0;
}
