#include "ros/ros.h"
#include "my_srv/Velocity.h"
#include <cstdlib>
#include <ostream>
#include <string>

char choose_velocity;



int main(int argc, char **argv)
{
	ros::init(argc, argv, "interface_server");
	
	while(1){
		ros::NodeHandle n;
		ros::ServiceClient client= n.serviceClient<my_srv::Velocity>("/velocity");
		my_srv::Velocity velocity_srv;
		std::cout<<"Press i to increase the velocity or d to decrease the velocity:/n insert your choice: ";
		std::cin>>choose_velocity;

		velocity_srv.request.v =choose_velocity;
		
		if(client.call(velocity_srv)){		
			std::cout<<"the velocity is: " << velocity_srv.response.x << std::endl;		
		}else {
		ROS_ERROR("Failed to call service speed");
		return 1;
		}
	}
	return 0;
}


