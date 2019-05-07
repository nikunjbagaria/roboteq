// This code subscribes  "vel_manipulated" geometry_msgs/Twist with vel_l = vel_msg.linear.y and vel_r = vel_msg.linear.x
// sucribes to "estop" (for wireless estop) and "estop_joy" (joy estop) 

#include "ros/ros.h"
#include "std_msgs/Float64.h"
#include "eklavya4_roboteq/SetSpeed.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/Int64.h"
#include "eklavya4_roboteq/control.h"
#define mult_factor  1  // define mult_factor as multipling factor for vel_r and vel_l coming from joy



ros::ServiceClient *client;
//ros::ServiceClient *ems_client;
int a1 = 0;   // for joy estop
int a2 = 0;   // for wireless estop
int a =0;

/*void estopcallbackjoy(const std_msgs::Int64::ConstPtr& msg)
{	
	a1 = msg->data; 
		
}

void estopcallbackwireless(const std_msgs::Int64::ConstPtr& msg)
{	
	a2 = msg->data; 
		
}*/

void commandVelCallback(eklavya4_roboteq::control vel_msg) {

	eklavya4_roboteq::SetSpeed srv;
	
	double vel_l=vel_msg.vl;
	ROS_INFO("Received right velocity : %d", vel_l);

	double vel_r=vel_msg.vr;
	ROS_INFO("Received left velocity : %d", vel_r);
	
	srv.request.v_r = vel_r;
	srv.request.v_l = vel_l;	

	srv.request.estop = vel_msg.estop; 

	if (client->call(srv))
	{
		ROS_INFO("Velocity target set to : %ld", (long int)srv.request.v_r);
		ROS_INFO("Response : %ld", (long int)srv.response.code);
	}
	else
	{
		ROS_ERROR("Failed to call service set speed");
		ROS_INFO("Response : %ld", (long int)srv.response.code);
		return;
	}
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "command_subscriber_motor_controller_client");
	
	ros::NodeHandle n1;
	
	ros::ServiceClient serviceClient = n1.serviceClient<eklavya4_roboteq::SetSpeed>("motor_controller");
	
	client = &serviceClient;
	ros::Subscriber sub = n1.subscribe("control", 10, commandVelCallback);
	//ros::Subscriber ar = n->subscribe("estop_joy", 100, estopcallbackjoy);
	//ross::Subscriber subscriber_mode = n->subscribe("mode_state",1000, modeCallBack);
        //ros::Subscriber subscriber_estop = n1.subscribe("estop", 1000, estopcallbackwireless);
    	ros::spin();

    	return 0;
}
