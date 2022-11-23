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
    float t_l=0.15; float t_a=0.1; 
    int i=0; int c; int c_p; int stop=0;
    float x; float y; float z; float w;



void retorno(const retorno_autonomo::ret &r){
    c=r.datos;
    c=c-2;
    c_p=2;
    f=r.ret;
    stop=6;
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
//

//transformar coordenadas rotatorias del robot a radianes
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
    if(abs(v_c)==PI){
        v_c=PI;
    }
  return v_c;
}
//
//funciones secuenciales//
//void angulo(){//funcion para rotar
//  if(abs(eu_angular(x, y)-conv(z, w))>t_a){
//    mover.linear.x=0;
//    if(eu_angular(x, y)-conv(cam.pose.pose.orientation.z, cam.pose.pose.orientation.w)>0){
//      mover.angular.z=0.2;
//    }
//    else {
//      mover.angular.z=-0.2;
//    } 
//  }
//  else{
//    //frenar...
//    stop=2;
//  }
//  pub.publish(mover);
//  robot.sleep();
//}

////void linear(){//funcion para avanzar
//  if((eu_lineal(x, y))>=t_l){
//    mover.linear.x=0.2; 
//    mover.angular.z=0;   
//  }
//  else{
//    //frenamos...
//    stop=3;
//  }
//pub.publish(mover);
//  robot.sleep();
//}

//void nextcoord(){//funcion para recorrer matriz de coordenadas
//  c=c-c_p;
//  if(c>5){
//    stop=1;
//  }
// else{
//    if(c<=5){
//      if(c==1){
//        c=0;
//        stop=4;
//      }
//    else{
//      c=1;
//      stop=1;
//    }
//    }
 // }
  //pub.publish(mover);
//}

//void fin(){//funcion para rotar robot a su posicion original
//  if(c==0){
//    mover.angular.z=0.1;
//    mover.linear.x=0;
//    if(abs(cam.pose.pose.orientation.w)-1<t_a){
//      //frenamos...  
//      mover.linear.x=0;
//      mover.angular.z=0;
//    }
//    else{
//      stop=5;
//    }
//  }
//  pub.publish(mover);
//  robot.sleep();
//}
//

//funcion para frenar el robot
//void frenos(){
//  for(int i=0; i<1000; i=i+1){
//    mover.linear.x=0;
//    pub.publish(mover);
//  }
//}
//

int main(int argc, char **argv){
  ros::init(argc, argv, "retorno");
  ros::NodeHandle nh;
  ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("RosAria/cmd_vel",1000);
  ros::Subscriber sub_cam = nh.subscribe("/camino", 1000, trayectoria);
  ros::Subscriber sub_pos = nh.subscribe("/RosAria/pose", 1000, posicion);
  ros::Subscriber sub_ret = nh.subscribe("/retornar",1000, retorno);
  ros::Rate loop_rate(1000);//rate del while
  ros::Rate robot(10);// publicacion para moverse
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
    //empieza secuencia de retorno
    switch(stop){
      case 1://rotamos para llegar al angulo correcto
        //funcion para rotar
        if(abs(eu_angular(x, y)-conv(z, w))>t_a){
          mover.linear.x=0;
          mover.angular.z=0.2;
          //if(eu_angular(x, y)-conv(cam.pose.pose.orientation.z, cam.pose.pose.orientation.w)>0){
          //  mover.angular.z=0.2;
          //}
          //else {
          //  mover.angular.z=-0.2;
          //} 
        }
        else{
          //frenar...
          mover.linear.x=0;
          mover.angular.z=0;
          stop=2;
        }
        pub.publish(mover);
        robot.sleep();
      break;
         
      case 2://avanzamos hasta avanzar la distancia determinada
      //funcion para avanzar
        if((eu_lineal(x, y))>=t_l){
          mover.linear.x=0.2; 
          mover.angular.z=0;   
        }
        else{
          //frenamos...
          mover.linear.x=0;
          mover.angular.z=0;
          stop=3;
        }
        pub.publish(mover);
        robot.sleep();
      break;

      case 3://cambiamos coordenadas de trayectoria
        //funcion para recorrer matriz de coordenadas
          c=c-c_p;
          if(c>5){
            stop=1;
          }
          else{
            if(c<=5){
              if(c<=1){
                c=0;
                stop=4;
              }
              else{
                c=1;
                stop=1;
              }
            }
          }
  //pub.publish(mover);
      break;

      case 4://rotamos al angulo de origen
        //funcion para rotar robot a su posicion original

          if(abs(conv(z, w))>0.01){
            mover.linear.x=0;
            mover.angular.z=0.2;
          }
          else{
             //frenamos...
            mover.linear.x=0;
            mover.angular.z=0;            
            stop=5;
          }
        pub.publish(mover);
        robot.sleep();
      break;

      case 5://apagamos
        ros::shutdown();
      break;

      case 6://frenar primero!
        //frenar...
        for(int i=0; i<1000; i=i+1){
          mover.linear.x=0;
          pub.publish(mover);
        }
        stop=1;
      break;
     //hacer default para no hacer nada y esperar denuevo otro retorno
    }
//
    ROS_INFO("Estoy en la secuencia %d, voy en %d y coordenadas son x=%f y=%f y el angulo es %f" , stop, c, x, y, eu_angular(x, y));
    loop_rate.sleep();
    }
  }
return 0; 
}
