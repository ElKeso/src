#include "ros/ros.h"
#include "std_msgs/String.h"

void nombredecallback(const std_msgs::String &msg)
{
  ROS_INFO("mensaje");
  //se hace algo con los datos de la subcripcion
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "subscriptor_publicador");
  ros::NodeHandle nh;

  ros::Subscriber subcripcion = nh.subscribe("nombre del topico", 1000, nombredecallback);
  ros::spin();
  ros::Publisher publicacion = nh.advertise<geometry_msgs::Twist>("nombre topico",1000);
  ros::Rate rate(10);
  geometry_msgs::Twist nombremensaje;
  chatter_pub.publish(nombredemensaje);
  while (ros::ok()){
      geometry_msgs::Twist nombredemensaje;
      msg.linear.x=1;
      msg.angular.z=2;
      chatter_pub.publish(nombredemensaje);
      ROS_INFO("estoy publicandooo");
      rate.sleep();
      //en este sector se publica
  }

  return 0;
}