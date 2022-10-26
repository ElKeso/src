#include "ros/ros.h"
#include "nav_msgs/Odometry.h"
#include "geometry_msgs/Twits"


nav_msgs::Odometry cam;
float rad; float v_c;


void posicion(const nav_msgs::Odometry &pos){
  cam=pos;
}

float conv(float z, float w){ 
  float r_min; float r_max; float rad_min; float rad_max; float m;
    if(z*w>0){
        if(w<=1 && w>=0.7){
            r_min=1; r_max=0.7; rad_min=0; rad_max=PI/2;
            m=(rad_max-rad_min)/(r_max-r_min);
            v_c=m*(w-r_min)-rad_min;
        }
        if(w<0.7 && w>=0){
            r_min=0.7; r_max=0; rad_min=PI/2; rad_max=PI;
            m=(rad_max-rad_min)/(r_max-r_min);
            v_c=m*(w-r_min)-rad_min;  
        }
    }
    else{
        if(abs(w)<=1 && abs(w)>=0.7){
            r_min=1; r_max=0.7; rad_min=0; rad_max=-PI/2;
            m=(rad_max-rad_min)/(r_max-r_min);
            v_c=m*(w-r_min)-rad_min;
        }  
        if(abs(w)<0.7 && abs(w)>=0){
            r_min=0.7; r_max=0; rad_min=-PI/2; rad_max=-PI;
            m=(rad_max-rad_min)/(r_max-r_min);
            v_c=m*(w-r_min)-rad_min;
        }  
    }   
  return v_c;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "revision");
  ros::NodeHandle nh;
  ros::Subscriber subcripcion = nh.subscribe("/RosAria/pose", 1000, posicion);
  ros::Rate rate(10);
  while (ros::ok()){
      ros::spinOnce();
      rad=conv(cam.pose.pose.orientation.z, cam.pose.pose.orientation.w);
      ROS_INFO("la conversion en radianes es: %f", rad);
      rate.sleep();
  }

  return 0;
}
