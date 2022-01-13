ROS assignment
================================

This is the second assignment of the Research Track 1 course. 
It's a C++ code to move a robot in the environment (a reproduction of the Monza's circuit) and allows the user to increase or decrease the velocity of the robot, according to the given input, or even   reset its position by calling a built in service 'reset_position'.


Installing and running
----------------------
First of all is necessary to execute the `roscore & ` command in one shell.

You have to download the repository from http://CarmineD8/second_assignment (github.com) and created your environment.

To run the simulation use:

```bash
$ rosrun stage_ros stageros $(rospack find second_assignment)/world/my_world.world
```

To run the controller node (in a second terminal) use:

```bash
$  rosrun assignment_2 controller_node
```

To run the client node (in a third terminal) use:

```bash
$  rosrun my_srv client
```

Now you can see the robot starts to move in the cicuit.

Code
---------

In this code there are three nodes communicating between them: the controller_node contained in ' controller.cpp ', the stage_ros node contained in 'my_world.world' and the client node contained in the 'client.cpp'. 

### Controller ###

The controller node is the the most important part of the code. It allows to increase or decrease the velocity of the robot thank's to this function:

```bash
bool drive (my_srv::Velocity::Request &req, my_srv::Velocity::Response &res)
```
This function contains the input:

```python
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
```
Where the last condition sets the speed limit to the robot to prevent that, the high speed value, get him into the wall.

Then there is a function which check the obstacles and let the robot move within the circuit:

```bash
void laserCallback(const sensor_msgs::LaserScan::ConstPtr& msg)
```
