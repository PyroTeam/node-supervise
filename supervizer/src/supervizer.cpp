#include "ros/ros.h"
#include "supervizer/beacon.h"
#include "std_msgs/Time.h"
#include "std_msgs/Duration.h"
#include <cstdio>
#include "topic_tools/shape_shifter.h"
#include "topic_tools/parse.h"
#include "CStockage.h"

#include "stdio.h"
#include <sstream>
using std::string;
using std::vector;
using namespace topic_tools;
using namespace std;
std::vector<CStockage*> g_tab_nodes;
ros::NodeHandle* g_node;

/*
***** Le superviseur ne publie rien de lui meme ******
void transmission(void)
{
   ros::Publisher chatter_pub = n.advertise<supervizer::beacon>("chatter", 1000);

  ros::Rate loop_rate(10);

  int count = 0;
  while (ros::ok())
    {

      std_msgs::String msg;

      std::stringstream ss;
      ss << "hello world " << count;
      msg.data = ss.str();

      ROS_INFO("%s", msg.data.c_str());

      /**
       * The publish() function is how you send messages. The parameter
       * is the message object. The type of this object must agree with the type
       * given as a template parameter to the advertise<>() call, as was done
       * in the constructor above.
       *
      chatter_pub.publish(msg);

      ros::spinOnce();
std_msgs::String
      loop_rate.sleep();
      ++count;
    }
}
*/
  void beaconCallback(const supervizer::beacon::ConstPtr& msg)
{
  ROS_INFO_STREAM("I heard: "<< msg->name.c_str() <<" " << msg->In_topic.c_str() << endl
   << msg->Out_topic.c_str()<< " " << (msg->state?"Actif!":"Idle!")<< " "<<msg->r_time.toSec()<< " " << msg->dead_line.toSec()<<endl);
  int taille = g_tab_nodes.size(),i=0;
  bool research = false;
  while(research == false)
  {
    if(g_tab_nodes.size() != 0 && g_tab_nodes[i]->getName() == msg->name.c_str())
    {
      ROS_INFO("Modification");
      g_tab_nodes[i]->setTime(msg->r_time);
      ROS_INFO_STREAM("Temps enregistré " << g_tab_nodes[i]->getTime());
      g_tab_nodes[i]->setDuration(msg->dead_line);
      g_tab_nodes[i]->setState(msg->state);

      ROS_INFO_STREAM("Ici on gere" << g_tab_nodes[i]->getName());
      research = true;
    }
    else if(g_tab_nodes.size()!= 0 && g_tab_nodes[i]->getName() != msg->name.c_str())
    {
      ROS_INFO("Test ++");
      i++;
    }
    else if (g_tab_nodes.size() == 0  || i>=taille)
    {
      ROS_INFO("Ajout");
      CStockage* S2 = new CStockage(msg->name.c_str(),msg->Out_topic.c_str(), msg->In_topic.c_str(),
      msg->r_time, msg->dead_line, g_node);
      g_tab_nodes.push_back(S2);
      research = true;

    }
    else 
    {
      ROS_INFO("Error IF beacon callBack out of range");
    }
  }
   

}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "supervizer");
  
  ros::NodeHandle n;
  g_node = &n;
  ros::Subscriber sub = g_node->subscribe("beacon", 10, beaconCallback);
 
  while(ros::ok())
  {
    ros::Rate loop_rate(1);
    if(g_tab_nodes.size() != 0)
    {
      int taille = g_tab_nodes.size(),i=0;
      ROS_INFO("Taille vecteur : %d", taille);
      {
        g_tab_nodes[i]->repub();
        i++;
      }
    }
    loop_rate.sleep();
    ros::spinOnce();
  }
  while(g_tab_nodes.size()!=0)
  {
    delete(g_tab_nodes[g_tab_nodes.size()-1]);
    g_tab_nodes.pop_back();

  }
  return 0;
}
