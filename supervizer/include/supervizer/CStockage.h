#include "ros/ros.h"
#include "supervizer/beacon.h"
#include "std_msgs/Time.h"
#include "std_msgs/Duration.h"
#include <cstdio>


#include <sstream>
using std::string;
using std::vector;
using namespace topic_tools;

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
  CStockage(string, string, );
  ~CStockage(void);
  void callback(const std_msgs::Time);
  void subscribe(void);
  void transmission(void);
  void repub(void);
};
