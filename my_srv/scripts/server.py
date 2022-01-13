#!/usr/bin/env python

import rospy
import random
from my_srv.srv import Velocity, VelocityResponse

def set_vel(req):
	return VelocityResponse(random.uniform(req.min, req.max),random.uniform(req.min,req.max))

def my_vel_server():
	rospy.init_node('my_vel_server')
	s = rospy.Service('velocity', Velocity, set_vel)
	print("Service ready.")
	rospy.spin()

if __name__ == "__main__":
	my_vel_server()

