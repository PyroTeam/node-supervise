#include "CStockage.h"

CStockage::CStockage(void)
  :m_sName("default"),m_sTopic_In("input"),m_sTopic_Out("output"),m_bState(false)
{
  m_tTps_Reception.data = ros::Time(0);
  m_dDead_line.data= ros::Duration(60);
}
  CStockage::CStockage(string sName, string sTopic_In, string sTopic_Out)
  :m_sName(sName),m_sTopic_In(sTopic_In),m_sTopic_Out(sTopic_Out),m_bState(false)
{
  m_tTps_Reception  .data = ros::Time::now();
  m_dDead_line.data= ros::Duration(60);
}
CStockage::~CStockage(void)
{
}

void CStockage::callback(const std_msgs::Time &msg)
{
  //TODO Adapter aux echanges complets
  ROS_INFO_STREAM("Temps :"<<this->m_tTps_Reception);
}
void CStockage::subscribe()
{
  m_subEch = m_nohNode.subscribe(m_sTopic_In, 10, &CStockage::callback,this);
}

void CStockage::repub(void)
{
  //TODO Réecrire pour le message complet
  this->m_pubEch.publish(m_tTps_Reception);
}

void CStockage::transmission(void)
{
  //TODO adapter au vrai message
  m_pubEch = m_nohNode.advertise<std_msgs::Time>(m_sTopic_Out, 1000);
  
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
