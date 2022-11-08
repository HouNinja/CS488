// Termm--Fall 2022

#pragma once

#include "cs488-framework/CS488Window.hpp"
#include "cs488-framework/OpenGLImport.hpp"
#include "cs488-framework/ShaderProgram.hpp"

#include <glm/glm.hpp>

#include <vector>

// Set a global maximum number of vertices in order to pre-allocate VBO data
// in one shot, rather than reallocating each frame.
const GLsizei kMaxVertices = 1000;


// Convenience class for storing vertex data in CPU memory.
// Data should be copied over to GPU memory via VBO storage before rendering.
class VertexData {
public:
	VertexData();

	std::vector<glm::vec2> positions;
	std::vector<glm::vec3> colours;
	GLuint index;
	GLsizei numVertices;
};


class A2 : public CS488Window {
public:
	A2();
	virtual ~A2();

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

	void createShaderProgram();
	void enableVertexAttribIndices();
	void generateVertexBuffers();
	void mapVboDataToVertexAttributeLocation();
	void uploadVertexDataToVbos();

	void initLineData();

	void setLineColour(const glm::vec3 & colour);

	void drawLine (
			const glm::vec2 & v0,
			const glm::vec2 & v1
	);

	ShaderProgram m_shader;

	GLuint m_vao;            // Vertex Array Object
	GLuint m_vbo_positions;  // Vertex Buffer Object
	GLuint m_vbo_colours;    // Vertex Buffer Object

	VertexData m_vertexData;

	glm::vec3 m_currentLineColour;

	//my modification:
	enum Mode {
		RotateM,
		TranslateM,
		ScaleM,
		RotateV,
		TranslateV,
		Perspective,
		Viewport
	};
	glm::mat4 ModelTransformation;
	glm::mat4 ScaleTransformation;
	glm::mat4 View;
	glm::mat4 ViewTransformation;
	glm::mat4 Projection;
	Mode mode;

	void initall();
	void reset();
	void RotateModel(float theta);
	void TranslateModel(float delta);
	void ScaleModel(float value);
	void RotateView(float theta);
	void TranslateView(float delta);
	void perspective(float value);

	void viewport(float xPos, float yPos);
	
	void SetProjection();

	bool clipping_oneside(glm::vec4 & A, glm::vec4 & B, glm::vec3 P, glm::vec3 n);
	bool clipping(glm::vec4 & A, glm::vec4 & B);

	float prev_x;
	float prev_y;

	std::vector<glm::vec4> vertex_buffer;
	std::vector<glm::vec4> gnormon;

	
	bool left_pressed = false;
	bool right_pressed = false;
	bool middle_pressed = false;
	float x_min = 0.05f;
	float x_max = 0.95f;
	float y_min = 0.05f;
	float y_max = 0.95f;

	float far = 25.0f;
	float near = 6.0f;
	float fov = 30.0f;;

	float tempx_1;
	float tempx_2;
	float tempy_1;
	float tempy_2;
};
