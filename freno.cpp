#include "ros/ros.h"
#include "geometry_msgs/Twist.h"


int flag=0; int a=4;
geometry_msgs::Twist mover;

int main(int argc, char **argv){
  ros::init(argc, argv, "freno");
  ros::NodeHandle nh;
  ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("RosAria/cmd_vel",1000);
  ros::Rate loop_rate(10);
  while (ros::ok()){
    switch(flag){
        case 0:
            for(int i=0; i<80; i=i+1){
                ROS_INFO("voy a detenerme en 50 y voy en %d" ,i);
                loop_rate.sleep();
            }
            flag=1;
        break;
        case 1:
            for(int i=0; i<10 i=i+1){
                mover.linear.x=-a;
                pub.publish(mover);
                }
            flag = 2;
        break;
        case 2:
                for(int i=0; i<5 i=i+1){
                mover.linear.x=0;
                pub.publish(mover);
                }
              flag = 3;
        break;  
      case 3:
        ros::shutdown();  
      break;
    }
  }
  return 0;
}
