// Termm--Fall 2022

#pragma once

#include <glm/glm.hpp>

#include "cs488-framework/CS488Window.hpp"
#include "cs488-framework/OpenGLImport.hpp"
#include "cs488-framework/ShaderProgram.hpp"

#include "maze.hpp"

class A1 : public CS488Window {
public:
	A1();
	virtual ~A1();

protected:
	virtual void init() override;
	virtual void appLogic() override;
	virtual void guiLogic() override;
	virtual void draw() override;
	virtual void cleanup() override;

	virtual bool cursorEnterWindowEvent(int entered) override;
	virtual bool mouseMoveEvent(double xPos, double yPos) override;
	virtual bool mouseButtonInputEvent(int button, int actions, int mods) override;
	virtual bool mouseScrollEvent(double xOffSet, double yOffSet) override;
	virtual bool windowResizeEvent(int width, int height) override;
	virtual bool keyInputEvent(int key, int action, int mods) override;

private:
	Maze m;
	glm::vec3 avatar;
	int shift_key = -1;
	void initGrid();
	void initCube();
	void initAvatar();
	void initFloor();

	void load_color(int ID, float * colour);
	void save_color(int ID, float * colour);

	void reset_parameters();
	//void save_color(int ID, glm::vec3 newcolor);
	// Fields related to the shader and uniforms.
	ShaderProgram m_shader;
	GLint P_uni; // Uniform location for Projection matrix.
	GLint V_uni; // Uniform location for View matrix.
	GLint M_uni; // Uniform location for Model matrix.
	GLint col_uni;   // Uniform location for cube colour.

	// Fields related to grid geometry.
	GLuint m_grid_vao; // Vertex Array Object
	GLuint m_grid_vbo; // Vertex Buffer Object

	GLuint m_cube_vao;
	GLuint m_cube_vbo;
	GLuint m_cube_ebo;
	glm::vec3 cube_color = {1.0f, 0.0f, 1.0f};

	GLuint m_avatar_vao;
	GLuint m_avatar_vbo;
	glm::vec3 avatar_color= {0.0f, 1.0f, 0.0f};

	GLuint m_floor_vao;
	GLuint m_floor_vbo;
	glm::vec3 floor_color = {0.0f, 0.0f, 0.0f};
	// Matrices controlling the camera and projection.
	glm::mat4 proj;
	glm::mat4 view;

	float colour[3];
	float cur_height = 1.0f;
	int current_col;

	int RB_ID = -1;
	int PREV_ID = -1;


	float m_shape_rotation = 0.0f;
	float rotate_rate = 0.0f;
	float prev_x;
	bool rotating = false;
	bool persistence = false;



	float scaling_para = 1.0f;
};
