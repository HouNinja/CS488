// Termm-Fall 2022

#include "JointNode.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "cs488-framework/MathUtils.hpp"
#include <iostream>
//---------------------------------------------------------------------------------------
JointNode::JointNode(const std::string& name)
	: SceneNode(name)
{
	m_nodeType = NodeType::JointNode;
}

//---------------------------------------------------------------------------------------
JointNode::~JointNode() {

}
 //---------------------------------------------------------------------------------------
void JointNode::set_joint_x(double min, double init, double max) {
	m_joint_x.min = min;
	m_joint_x.init = init;
	m_joint_x.max = max;
	cur_joint_x = init;
	float radian = init * PI / 180.0f;
	trans = glm::rotate(glm::mat4(1.0f), radian, glm::vec3(1.0f,0.0f,0.0f));
}

//---------------------------------------------------------------------------------------
void JointNode::set_joint_y(double min, double init, double max) {
	m_joint_y.min = min;
	m_joint_y.init = init;
	m_joint_y.max = max;
	cur_joint_y = init;
	float radian = init * PI / 180.0f;
	trans = glm::rotate(trans, radian, glm::vec3(0.0f,1.0f,0.0f));
}
