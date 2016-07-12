#include "supervizer.h"

CStockage(void)
  :m_sName(""),m_sTopic_In("input"),m_sTopic_Out,m_bState(false)
{
  m_tTps_Reception.data = ros::Time(0);
  m_dDead_line.data= ros::Duration(0);
}
  
~CStockage(void)
{
}

void callback(const std_msgs::Time &msg)
{
  ROS_INFO_STREAM("Temps :"<<this->m_tTps_Reception);
}
void subscribe()
{
  m_subEch = g_node->subscribe(m_sTopic_In, 10, &CStockage::callback,this);
}

void repub(void)
{
  this->m_pubEch.publish(m_tTps_Reception);
}

void transmission(void)
{
  m_pubEch = g_node->advertise<std_msgs::Time>(m_sTopic_Out, 1000);

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
