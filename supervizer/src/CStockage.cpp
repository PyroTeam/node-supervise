#include "CStockage.h"

CStockage::CStockage(void)
  :m_sName("default"),m_sTopic_In("input"),m_sTopic_Out("output"),m_bState(false)
{
  m_tTps_Reception.data = ros::Time(0);
  m_dDead_line.data= ros::Duration(60);
}
  CStockage::CStockage(string sName, string sTopic_In, string sTopic_Out,
   ros::Time temps = ros::Time::now(), ros::Duration dead_line = ros::Duration(60),ros::NodeHandle* pnh = NULL)
  :m_sName(sName),m_sTopic_In(sTopic_In),m_sTopic_Out(sTopic_Out),m_bState(false),m_nohNode(pnh)
{
  essai = ros::Time(0);
  m_tTps_Reception.data = temps;
  m_dDead_line.data= dead_line;
  this->subscribe();
  m_pubEch = m_nohNode->advertise<std_msgs::Time>(m_sTopic_Out, 1000);
}
CStockage::~CStockage(void)
{
}

void CStockage::callback(const std_msgs::Time &msg)
{
  //TODO Adapter aux echanges complets
 // ROS_INFO_STREAM(endl<<"Reception : " << this->getName() << endl);
  ros::Time tmp = msg.data;

  this->setData(tmp);
  ROS_INFO_STREAM("Temps: "<< this->essai<<" Node :"<< this ->m_sName << " I/O : "<< m_sTopic_In << " "<< m_sTopic_Out);

}
void CStockage::subscribe()
{
  m_subEch = m_nohNode->subscribe(m_sTopic_In, 10,&CStockage::callback,this);
}

void CStockage::repub(void)
{
  //TODO Réecrire pour le message complet
  ROS_INFO_STREAM("J'envoie de "<< this-> m_sName);
  this->m_pubEch.publish(this->m_tTps_Reception);
}

void CStockage::transmission(void)
{
  //TODO adapter au vrai message
 
  
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
