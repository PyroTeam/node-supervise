#include "ros/ros.h"
#include "supervizer/beacon.h"
#include "std_msgs/Time.h"
#include "std_msgs/Duration.h"
#include <cstdio>
#include "topic_tools/shape_shifter.h"
#include "topic_tools/parse.h"
#include "CStockage.h"

#include <sstream>
using std::string;
using std::vector;
using namespace topic_tools;

std::vector<CStockage> g_tab_nodes;
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
  void chatterCallback(const supervizer::beacon::ConstPtr& msg)
{
  ROS_INFO("I heard: [%s] [%s]  [%s] [%d] [%f] [%f]", msg->name.c_str(), msg->In_topic.c_str(),
   msg->Out_topic.c_str, msg->state, msg->r_time.toSec(),msg->dead_line.toSec());

  if(g_tab_nodes.size() != 0)
  {
    int taille = g_tab_nodes.size(),i=0;
    while(i<taille-1 && g_tab_nodes[taille].name != msg->name)
    {
      i++;
    }


    if(g_tab_nodes[taille].name == msg->name)
    {
      g_tab_nodes[taille].setTime(msg->r_time);
      g_tab_nodes[taille].setDuration(msg->dead_line);
      g_tab_nodes[taille].setState(msg->state);
    }
    else
    {
      g_tab_nodes.push_back(CStockage(msg->name.c_str,msg->Out_topic.c_str, msg->In_topic.c_str,
      msg->r_time, msg->dead_line));
    } 
  }
  else
  {
    g_tab_nodes.push_back(CStockage(msg->name.c_str,msg->Out_topic.c_str, msg->In_topic.c_str,
    msg->r_time, msg->dead_line));
  }  

}
void reception(ros::NodeHandle* pN)
{
  ros::Subscriber sub = pN->subscribe("beacon", 10, chatterCallback);
 

  
}
int main(int argc, char **argv)
{
  ros::init(argc, argv, "supervizer");
  
  ros::NodeHandle n;
  g_node = &n;
  while(ros::ok())
  {
    reception(&n);
    ros::spinOnce();
  }
  return 0;
}
