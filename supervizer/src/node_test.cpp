#include "ros/ros.h"
#include "supervizer/beacon.h"
#include "std_msgs/Time.h"
#include "std_msgs/Duration.h"
#include <cstdio>


#include <sstream>
using std::string;
using std::vector;
using namespace std;

ros::Publisher beacon_pub ;
ros::Publisher stockage_pub ;
 
std::string g_sNomNode, g_sNomIn, g_sNomOut;
ros::NodeHandle* g_nhNode;
void transmission_stockage(void)
{

  
  std_msgs::Time msg;
  msg.data = ros::Time::now();
  stockage_pub.publish(msg);
  ROS_INFO_STREAM("Send time : " <<msg.data);

}     

void transmission_beacon(void)
{
  supervizer::beacon msg;
  msg.name = g_sNomNode;
  msg.r_time = ros::Time::now();
  msg.state = true;
  msg.In_topic = g_sNomIn;
  msg.Out_topic = g_sNomOut;
  msg.dead_line = ros::Duration(60);
  /*
      Pour l'instant usage de deux string pour les noms de topics avant normalisations
      string name
      time r_time
      int8 state
      string In_topic
      string Out_topic
      duration dead_line
  */
  ros::Time now = ros ::Time::now();
  ROS_INFO_STREAM("Send beacon :"<<msg.name <<endl<< "Time: " <<ros::Time::now() << endl
   <<    "Etat :"<< (msg.state?"Actif!":"Idle!") << endl << "Topic d'entrée : "<< msg.In_topic << endl
    <<"Topic de sortie : " << msg.Out_topic <<endl << "Latence : "<< msg.dead_line);
  beacon_pub.publish(msg);
}     

  void chatterCallback(const std_msgs::Time &msg)
{
  //TODO Adapter aux echanges complets et penser a enregistrer les données
  ROS_INFO_STREAM("Temps :"<<msg.data);
}


int main(int argc, char **argv)
  {
    g_sNomNode = "node_test";
    g_sNomOut = "Out_nt";
    g_sNomIn ="In_nt";
    ros::init(argc, argv, g_sNomNode);
    //TODO envoie et receptions de données
    
    ros::NodeHandle n;
    g_nhNode = &n;
    beacon_pub = g_nhNode->advertise<supervizer::beacon>("beacon", 1000);
    stockage_pub = g_nhNode->advertise<std_msgs::Time>(g_sNomOut, 1000);
 
     ros::Subscriber sub = g_nhNode->subscribe(g_sNomIn, 10, chatterCallback);
    
    ros::Rate loop_rate(1);

    while (ros::ok())
      {
        transmission_beacon();
        transmission_stockage();
        ros::spinOnce();
        loop_rate.sleep();
      }
  
  return 0;
  }