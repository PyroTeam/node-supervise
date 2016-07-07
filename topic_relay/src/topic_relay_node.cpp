#include <ros/ros.h>

#include "topic_tools/shape_shifter.h"
#include "topic_tools/parse.h"

using std::vector;
using namespace topic_tools;

ros::NodeHandle *g_node = nullptr;
bool g_advertised = false;
std::string g_input_topic;
std::string g_output_topic;
ros::Publisher g_pub;
ros::Subscriber* g_sub;
ros::TransportHints g_th;
ros::MessageEvent<ShapeShifter> g_msgEvent;

void conn_cb(const ros::SingleSubscriberPublisher&);
void in_cb(const ros::MessageEvent<ShapeShifter>& msg_event);

void subscribe()
{
	g_sub = new ros::Subscriber(g_node->subscribe(g_input_topic, 10, &in_cb, g_th));
}

void conn_cb(const ros::SingleSubscriberPublisher&)
{
	subscribe();
}

void in_cb(const ros::MessageEvent<ShapeShifter>& msg_event)
{
	boost::shared_ptr<ShapeShifter const> const &msg = msg_event.getConstMessage();
	boost::shared_ptr<const ros::M_string> const& connection_header = msg_event.getConnectionHeaderPtr();
	g_msgEvent = msg_event;

	if (!g_advertised)
	{
		// If the input topic is latched, make the output topic latched,.
		bool latch = false;
		if (connection_header)
		{
			ros::M_string::const_iterator it = connection_header->find("latching");
			if((it != connection_header->end()) && (it->second == "1"))
			{
				ROS_DEBUG("Input topic is latched; latching output topic to match");
				latch = true;
			}
		}
		g_pub = msg->advertise(*g_node, g_output_topic, 10, latch, conn_cb);
		g_advertised = true;
		ROS_INFO("Advertised as %s\n", g_output_topic.c_str());
	}

	g_pub.publish(msg);
}

void repub()
{
	boost::shared_ptr<ShapeShifter const> const &msg = g_msgEvent.getConstMessage();

	if (!g_advertised)
		return;

	g_pub = msg->advertise(*g_node, g_output_topic, 10, true, conn_cb);
	g_advertised = true;

	g_pub.publish(msg);
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "topic_relay");
	ros::NodeHandle nh("~");
	g_node = &nh;

	ros::Rate mainLoopRate(0.5);

	g_output_topic = "output";
	g_input_topic = "input";

	subscribe();

	while (ros::ok())
	{
		repub();
		ros::spinOnce();
		mainLoopRate.sleep();
	}

	return 0;
}