#include "ros/ros.h"
#include "supervizer/beacon.h"
#include "std_msgs/Time.h"
#include "std_msgs/Duration.h"
#include <cstdio>
#include "topic_tools/shape_shifter.h"
#include "topic_tools/parse.h"

#include <sstream>
using std::string;
using std::vector;
using namespace topic_tools;

ros::NodeHandle *g_node = NULL;
class CSupervizer{
private:
  string m_sName, m_sTopic;
  bool m_bState;
  std_msgs::Time m_tTps_Reception;
  std_msgs::Duration m_dDead_line;
  ros::Publisher m_pubEch;
  ros::Subscriber m_subEch;
public:
  CSupervizer(void)
    :m_sName(""),m_sTopic(""),m_bState(false)
  {
    m_tTps_Reception.data = ros::Time(0);
    m_dDead_line.data= ros::Duration(0);
  }
  //CSupervizer(void);
  ~CSupervizer(void)
  {
  }
  void callback(const std_msgs::Time &msg)
  {
    ROS_INFO_STREAM("Temps :"<<this->m_tTps_Reception);
  }
  void subscribe()
  {
    m_subEch = g_node->subscribe("input", 10, &CSupervizer::callback,this);
  }
  void transmission(void)
  {
   m_pubEch = g_node->advertise<std_msgs::Time>("input", 1000);

    ros::Rate loop_rate(1000);

    while (ros::ok())
      {

	std_msgs::Time msg;

	
	msg.data = m_tTps_Reception.data + ros::Duration(1);

	ROS_INFO_STREAM("Time : " << msg.data);

	/**
	 * The publish() function is how you send messages. The parameter
	 * is the message object. The type of this object must agree with the type
	 * given as a template parameter to the advertise<>() call, as was done
	 * in the constructor above.
	 *
	 */
	this->m_pubEch.publish(msg);
	//TODO looprate
	ros::spinOnce();
	 
      }
  }
};
/*
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
  ROS_INFO("I heard: [%s] [%s] [%d] [%f] [%f]", msg->name.c_str(), msg->data_topic.c_str(), msg->state, msg->r_time.toSec(),msg->dead_line.toSec());
}
void reception(ros::NodeHandle* pN)
{
  ros::Subscriber sub = pN->subscribe("beacon", 10, chatterCallback);
  ros::spin();

  
}
int main(int argc, char **argv)
{
  ros::init(argc, argv, "supervizer");
  
  ros::NodeHandle n;
  g_node = &n;
  reception(&n);
 
 


  return 0;
}
