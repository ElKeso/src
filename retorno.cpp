#include "ros/ros.h"
#include "retorno_autonomo/ret.h"
#include "retorno_autonomo/tray.h"
#include "retorno_autonomo/trayArray.h"
#include "nav_msgs/Odometry.h"
#include "geometry_msgs/Twist.h"
#include "math.h"

    retorno_autonomo::trayArray camino;
    retorno_autonomo::tray datos;
    nav_msgs::Odometry cam;
    geometry_msgs::Twist mover;
    int f=0; float cte=1.5;
    float PI = 3.14159265;
    float v_l; float v_a; float v_c;
    float t_l=0.15; float t_a=0.015; 
    int i=0; int c; int c_p;
    float x; float y; float z; float w;

void retorno(const retorno_autonomo::ret &r){
    c=r.datos;
    c=c-2;
    c_p=c/2;
    f=r.ret;
}

void trayectoria(const retorno_autonomo::trayArray &t){
    camino=t;
}

void posicion(const nav_msgs::Odometry &pos){
    cam=pos;
}

//funciones Euclides
float eu_lineal(float x, float y){
  v_l=sqrt(pow((x-cam.pose.pose.position.x),2)+pow((y-cam.pose.pose.position.y),2));
  return v_l;
}

float eu_angular(float x, float y){
  v_a=atan2(y-cam.pose.pose.position.y,x-cam.pose.pose.position.x);
  return v_a;
}

float conv(float z, float w){ 
  float r_min; float r_max; float rad_min; float rad_max; float m;
    if(z*w>0){
        if(w<=1 && w>=0.7){
            r_min=1; r_max=0.7; rad_min=0; rad_max=PI/2;
            m=(rad_max-rad_min)/(r_max-r_min);
            v_c=m*(w-r_min)+rad_min;
        }
        if(w<0.7 && w>=0){
            r_min=0.7; r_max=0; rad_min=PI/2; rad_max=PI;
            m=(rad_max-rad_min)/(r_max-r_min);
            v_c=m*(w-r_min)+rad_min;  
        }
    }
    else{
        if(abs(w)<=1 && abs(w)>=0.7){
            r_min=1; r_max=0.7; rad_min=0; rad_max=-PI/2;
            m=(rad_max-rad_min)/(r_max-r_min);
            v_c=m*(w-r_min)+rad_min;
        }  
        if(abs(w)<0.7 && abs(w)>=0){
            r_min=0.7; r_max=0; rad_min=-PI/2; rad_max=-PI;
            m=(rad_max-rad_min)/(r_max-r_min);
            v_c=m*(w-r_min)+rad_min;
        }  
    }   
  return v_c;
}

int main(int argc, char **argv){
  ros::init(argc, argv, "retorno");
  ros::NodeHandle nh;
  ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("RosAria/cmd_vel",1000);
  ros::Subscriber sub_cam = nh.subscribe("/camino", 1000, trayectoria);
  ros::Subscriber sub_pos = nh.subscribe("/RosAria/pose", 1000, posicion);
  ros::Subscriber sub_ret = nh.subscribe("/retornar",1000, retorno);
  ros::Rate loop_rate(100);
  ros::Rate rate(1);
  ros::Rate robot(1);
  c=0;
  datos.x=c;
  datos.y=c;
  datos.theta=c;
  camino.trayectoria.push_back(datos);
  while(ros::ok()){
    ros::spinOnce();  
    //moverse a las cordenadas...
    if(f==1){//
    x=camino.trayectoria[c].x;
    y=camino.trayectoria[c].y;
    z=cam.pose.pose.orientation.z;
    w=cam.pose.pose.orientation.w;
      if(abs(eu_angular(x, y)-conv(z, w))>t_a){
        //mover.linear.x=0;
        //if(eu_angular(x, y)-conv(cam.pose.pose.orientation.z, cam.pose.pose.orientation.w)>0){
          mover.angular.z=0.2;
        //}
        //else {
        //  mover.angular.z=-0.2;
        //} 
      }
      else {
        robot.sleep();  
        mover.angular.z=0;
        robot.sleep();
        pub.publish(mover);
        if((eu_lineal(x, y))>=t_l){
         mover.linear.x=0.1; 
        }
        else {
          robot.sleep();    
          mover.linear.x=0;
          robot.sleep();
          pub.publish(mover);  
          c=c-c_p;
          if(c<=5){
            if(c==1){
              c=0;
            }
            else{
              c=1;
            }
          }
        if(c==0){
             mover.angular.z=0.2;
            if(abs(cam.pose.pose.orientation.w)-1<t_a){
                  robot.sleep();    
                  mover.linear.x=0;
                  mover.angular.z=0;
                  robot.sleep();  
                  pub.publish(mover);
                  ros::shutdown();
            }
          }
       }
    }
    pub.publish(mover);
    }//
    else{
    }
    ROS_INFO("voy en %d y coordenadas son x=%f y=%f",c, x, y);
    loop_rate.sleep();
  }
return 0; 
}

