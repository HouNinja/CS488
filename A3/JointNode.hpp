// Termm-Fall 2022

#pragma once

#include "SceneNode.hpp"

class JointNode : public SceneNode {
public:
	JointNode(const std::string & name);
	virtual ~JointNode();

	void set_joint_x(double min, double init, double max);
	void set_joint_y(double min, double init, double max);

	struct JointRange {
		double min, init, max;
	};

	float cur_joint_x;
	float cur_joint_y;
	JointRange m_joint_x, m_joint_y;
};
