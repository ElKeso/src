#include "ros/ros.h"
#include "geometry_msgs/Twist.h"


int flag=0; int a=4;
geometry_msgs::Twist mover;

int main(int argc, char **argv){
  ros::init(argc, argv, "freno");
  ros::NodeHandle nh;
  ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("RosAria/cmd_vel",1000);
  ros::Rate loop_rate(5);
  while (ros::ok()){
    switch(flag){
        case 0:
            for(int i=0; i=15; i=i+1){
                mover.linear.x=4;
                pub.publish(mover);
                loop_rate.sleep();
            }
            flag=1;
        break;
        case 1:
            for(int i=0; i=5; i=i+1){
                mover.linear.x=-a;
                pub.publish(mover);
                if(i==5){
                    ros::shutdown();
                }
            }
        break;
        default: pub.publish(mover);
    }
  }
  return 0;
}
