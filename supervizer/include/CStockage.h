
#ifndef CSTOCKAGE
#define CSTOCKAGE



#include "ros/ros.h"
#include "supervizer/beacon.h"
#include "std_msgs/Time.h"
#include "std_msgs/Duration.h"
#include <cstdio>


#include <sstream>
using std::string;

class CStockage{
 private:
  string m_sName, m_sTopic_In, m_sTopic_Out;
  bool m_bState;
  std_msgs::Time m_tTps_Reception;
  std_msgs::Duration m_dDead_line;
  ros::Publisher m_pubEch;
  ros::Subscriber m_subEch;
  ros::NodeHandle m_nohNode;
  
 public:
  CStockage(void);
  CStockage(string, string, string, ros::Time, ros::Duration);
  ~CStockage(void);
  void callback(const std_msgs::Time&);
  void subscribe(void);
  void transmission(void);
  void repub(void);

  void setTime(ros::Time temps){m_tTps_Reception.data = temps;};
  void setDuration(ros::Duration dur){m_dDead_line.data = dur;};
  void setState(bool etat){m_bState = etat;};

  ros::Time getTime(void){return m_tTps_Reception.data;};
  ros::Duration getDuration(void){return m_dDead_line.data;};
  std::string getName(void){return m_sName;};

};

#endif