#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/LaserScan.h"
#include "my_srv/Velocity.h"
#include "std_srvs/Empty.h"
#include <string>

float FL,F,FR;   // front left, front, front right
ros::Subscriber sub;
ros::Publisher pub;
ros::ServiceClient client;
float vel=1.0;    // linear velocity
float a_vel=1.5;  // angular velocity


bool drive (my_srv::Velocity::Request &req, my_srv::Velocity::Response &res){
	my_srv::Velocity velocity_srv;
	std_srvs::Empty srv;
	
	geometry_msgs::Twist my_vel;
	my_vel.linear.x = vel;
	my_vel.angular.z = a_vel;
	
	
	if (req.v == "i" ){
		// increase the speed
		res.x = vel + 0.1*vel;		
	}
	else if (req.v == "d" ){
		// decrease the speed
		res.x = vel - 0.1*vel;
	}
	if (res.x > 1.4 || res.x < 0.3){
		// set speed limits to prevent the robot from crashing into the wall (the velocity turns to the initional value)
		res.x = 1.0;
	}
	vel=res.x;
	return res.x;
}




void laserCallback(const sensor_msgs::LaserScan::ConstPtr& msg)
	
	{
	//call the client
	my_srv::Velocity velocity_srv;
	velocity_srv.request.v=1;
	client.call(velocity_srv);
	
	int i;
	
	//decomposing the ranges in 3 sections
	float FR= msg->ranges[288];
	float F= msg->ranges[360];
	float FL= msg->ranges[576];
	
	geometry_msgs::Twist my_vel;
	my_vel.linear.x = vel;
	my_vel.angular.z = a_vel;
	
	if(F>1 &&  FL>1 && FR>1){
		// go straight 
		printf("go straight\n");
		my_vel.linear.x = 1.5*vel;
		my_vel.angular.z = 0;
	}
	
	else if ((FL>1 && F>1 && FR<1)||(FL<1 && F>1  && FR<1)){
		//obstacle on right
		printf("turn left \n");
		my_vel.linear.x = vel;
		my_vel.angular.z = a_vel;
	}

	else if ((FL>1 && F<1 && FR>1)||(FL<1 && F<1 && FR>1)){
		//obstacle on left
		printf("turn right \n");
		my_vel.linear.x = vel;
		my_vel.angular.z = -a_vel;
	}

	else if ((FL<1 && F>1 && FR>1)||(FL>1 && F<1 && FR<1)||(FL<1 && F<1 && FR<1)){
		//check for the next obstacle
		
		if(FL>FR){ //left is free
			printf("turn left \n");
			my_vel.linear.x = vel;
			my_vel.angular.z = a_vel;
		}
		else if(FL<FR){//right is free
			printf("turn right \n");
			my_vel.linear.x = vel;
			my_vel.angular.z = -a_vel;
		}
	}
	
	pub.publish(my_vel);
	ROS_INFO("Turtle subscriber@[%f, %f, %f]", FR, F, FL);
	
}


int main (int argc, char **argv)
{
  	ros::init(argc, argv, "turtlebot_subscriber");  
	ros::NodeHandle nh;
	ros::NodeHandle n;
 
	pub = nh.advertise<geometry_msgs::Twist> ("/cmd_vel", 1);
	ros::Subscriber sub = nh.subscribe("/base_scan", 1,laserCallback);
	
	ros::ServiceServer service= n.advertiseService("/velocity", drive);
	ros::ServiceClient client1 = nh.serviceClient<std_srvs::Empty>("/reset_positions");
	std_srvs::Empty srv;
	
	client.waitForExistence();
	client.call(srv);
	
	ros::spin();
	return 0;
}




