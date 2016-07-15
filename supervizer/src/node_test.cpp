#include "ros/ros.h"
#include "supervizer/beacon.h"
#include "std_msgs/Time.h"
#include "std_msgs/Duration.h"
#include <cstdio>


#include <sstream>
using std::string;
using std::vector;

std::string g_sNomNode, g_sNomIn, g_sNomOut;
ros::NodeHandle* g_nhNode;
void transmission_stockage(void)
{

  ros::Publisher chatter_pub = g_nhNode->advertise<std_msgs::Time>(g_sNomOut, 1000);
  ROS_INFO_STREAM("Send time : " <<ros::Time::now());
  chatter_pub.publish(ros::Time::now());
}     

void transmission_beacon(void)
{
  supervizer::beacon::
  ros::Publisher chatter_pub = g_nhNode->advertise<std_msgs::Time>("beacon", 1000);
  ROS_INFO_STREAM("Send beacon : " <<ros::Time::now());
  chatter_pub.publish(ros::Time::now());
}     

  void chatterCallback(const std_msgs::Time &msg)
{
  //TODO Adapter aux echanges complets et penser a enregistrer les données
  ROS_INFO_STREAM("Temps :"<<msg.data);
}

void reception(ros::NodeHandle* pN)
{
  ros::Subscriber sub = pN->subscribe(g_sNomIn, 10, chatterCallback);
  ros::spin();

  
}
int main(int argc, char **argv)
  {
    g_sNomNode = "node_test"
    g_sNomOut = "Out_nt";
    g_sNomIn ="In_nt";
    ros::init(argc, argv, g_sNomNode_);
    //TODO envoie et receptions de données

    ros::NodeHandle n;
    g_nhNode = &n;
    reception(&n);

    ros::Rate loop_rate(10);

    while (ros::ok())
      {
        transmission_stockage();
        ros::spinOnce();
        loop_rate.sleep();
      }
  }
  return 0;
}