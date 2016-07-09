#include <cstdio>
#include "topic_tools/shape_shifter.h"
#include "topic_tools/parse.h"

using std::string;
using std::vector;
using namespace topic_tools;

ros::NodeHandle *g_node = NULL;
bool g_advertised = false;
string g_input_topic;
string g_output_topic;
ros::Publisher g_pub;
ros::Subscriber* g_sub;
bool g_lazy;
ros::TransportHints g_th;

boost::shared_ptr<ShapeShifter const> g_msg;

void conn_cb(const ros::SingleSubscriberPublisher&);
void in_cb(const ros::MessageEvent<ShapeShifter>& msg_event);

void subscribe()
{
	g_sub = new ros::Subscriber(g_node->subscribe(g_input_topic, 10, &in_cb, g_th));
}

void conn_cb(const ros::SingleSubscriberPublisher&)
{
	// If we're in lazy subscribe mode, and the first subscriber just
	// connected, then subscribe, #3389.
	if(g_lazy && !g_sub)
	{
		ROS_DEBUG("lazy mode; resubscribing");
		subscribe();
	}
}

void in_cb(const ros::MessageEvent<ShapeShifter>& msg_event)
{
	try
	{
		ROS_INFO_STREAM("(in_cb) param use_count : " << msg_event.getConstMessage().use_count());
		ROS_INFO_STREAM("(in_cb) param ptr : " << msg_event.getConstMessage().get());
		ROS_INFO_STREAM("(in_cb) global use_count : " << g_msg.use_count());
		ROS_INFO_STREAM("(in_cb) global ptr : " << g_msg.get());

		boost::shared_ptr<ShapeShifter const> const &msg = msg_event.getConstMessage();
		g_msg = msg_event.getConstMessage();
		ROS_INFO_STREAM("(in_cb) global after use_count : " << g_msg.use_count());
		ROS_INFO_STREAM("(in_cb) global after ptr : " << g_msg.get());
		boost::shared_ptr<const ros::M_string> const& connection_header = msg_event.getConnectionHeaderPtr();

		if (!g_advertised)
		{
			// If the input topic is latched, make the output topic latched, #3385.
			bool latch = false;
			if (connection_header)
			{
				ros::M_string::const_iterator it = connection_header->find("latching");
				if((it != connection_header->end()) && (it->second == "1"))
				{
					ROS_DEBUG("input topic is latched; latching output topic to match");
					latch = true;
				}
			}
			g_pub = msg->advertise(*g_node, g_output_topic, 10, latch, conn_cb);
			g_advertised = true;
			ROS_INFO("advertised as %s\n", g_output_topic.c_str());
		}
		// If we're in lazy subscribe mode, and nobody's listening,
		// then unsubscribe, #3389.
		if(g_lazy && !g_pub.getNumSubscribers())
		{
			ROS_DEBUG("lazy mode; unsubscribing");
			delete g_sub;
			g_sub = NULL;
		}
		else
			g_pub.publish(msg);
	}
	catch(std::exception const& e)
	{
		ROS_ERROR("(in_cb) exception : %s", e.what());
	}
}

void repub()
{
	try
	{
		ROS_INFO_STREAM("(repub) global use_count : " << g_msg.use_count());
		ROS_INFO_STREAM("(repub) global ptr : " << g_msg.get());

		if (!g_advertised)
		{
			return;
		}

		g_pub.publish(g_msg);
	}
	catch(std::exception const& e)
	{
		ROS_ERROR("(repub) exception : %s", e.what());
	}
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "topic_relay");
	g_output_topic = string("output");
	g_input_topic = string("input");

	ros::NodeHandle pnh("~");
	g_node = &pnh;
	bool unreliable = false;
	pnh.getParam("unreliable", unreliable);
	pnh.getParam("lazy", g_lazy);
	if (unreliable)
		g_th.unreliable().reliable(); // Prefers unreliable, but will accept reliable.

	ros::Rate mainLoopRate(0.5);

	subscribe();
	while (ros::ok())
	{
		repub();
		ros::spinOnce();
		mainLoopRate.sleep();
	}
return 0;
}
