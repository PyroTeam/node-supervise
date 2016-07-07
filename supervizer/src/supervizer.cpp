#include "ros/ros.h"
#include "std_msgs/supervizer.h"

#include <sstream>
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
  ROS_INFO("I heard: [%s]", msg->data.c_str());
}
void reception()
{
  ros::Subscriber sub = n.subscribe("beacon", 1000, chatterCallback);
  ros::spin();

  
}
int main(int argc, char **argv)
{

  ros::init(argc, argv, "supervizer");

  ros::NodeHandle n;

 
 


  return 0;
}
