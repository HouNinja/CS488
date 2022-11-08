// Termm--Fall 2022

#include "A2.hpp"
#include "cs488-framework/GlErrorCheck.hpp"

#include <iostream>
using namespace std;

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
using namespace glm;

const float PI = 3.14159265f;
const float WINDOW_X = 768.0f;
const float WINDOW_Y = 768.0f;
//----------------------------------------------------------------------------------------
// Constructor
VertexData::VertexData()
	: numVertices(0),
	  index(0)
{
	positions.resize(kMaxVertices);
	colours.resize(kMaxVertices);
}


//----------------------------------------------------------------------------------------
// Constructor
A2::A2()
	: m_currentLineColour(vec3(0.0f))
{
	

}

//----------------------------------------------------------------------------------------
// Destructor
A2::~A2()
{

}

//----------------------------------------------------------------------------------------
/*
 * Called once, at program start.
 */
void A2::init()
{
	// Set the background colour.
	glClearColor(0.2, 0.5, 0.3, 1.0);

	createShaderProgram();

	glGenVertexArrays(1, &m_vao);

	enableVertexAttribIndices();

	generateVertexBuffers();

	mapVboDataToVertexAttributeLocation();

	initall();
	reset();
}

//----------------------------------------------------------------------------------------
void A2::createShaderProgram()
{
	m_shader.generateProgramObject();
	m_shader.attachVertexShader( getAssetFilePath("VertexShader.vs").c_str() );
	m_shader.attachFragmentShader( getAssetFilePath("FragmentShader.fs").c_str() );
	m_shader.link();
}

//---------------------------------------------------------------------------------------- Fall 2022
void A2::enableVertexAttribIndices()
{
	glBindVertexArray(m_vao);

	// Enable the attribute index location for "position" when rendering.
	GLint positionAttribLocation = m_shader.getAttribLocation( "position" );
	glEnableVertexAttribArray(positionAttribLocation);

	// Enable the attribute index location for "colour" when rendering.
	GLint colourAttribLocation = m_shader.getAttribLocation( "colour" );
	glEnableVertexAttribArray(colourAttribLocation);

	// Restore defaults
	glBindVertexArray(0);

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
void A2::generateVertexBuffers()
{
	// Generate a vertex buffer to store line vertex positions
	{
		glGenBuffers(1, &m_vbo_positions);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_positions);

		// Set to GL_DYNAMIC_DRAW because the data store will be modified frequently.
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * kMaxVertices, nullptr,
				GL_DYNAMIC_DRAW);


		// Unbind the target GL_ARRAY_BUFFER, now that we are finished using it.
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CHECK_GL_ERRORS;
	}

	// Generate a vertex buffer to store line colors
	{
		glGenBuffers(1, &m_vbo_colours);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_colours);

		// Set to GL_DYNAMIC_DRAW because the data store will be modified frequently.
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * kMaxVertices, nullptr,
				GL_DYNAMIC_DRAW);


		// Unbind the target GL_ARRAY_BUFFER, now that we are finished using it.
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CHECK_GL_ERRORS;
	}
}

//----------------------------------------------------------------------------------------
void A2::mapVboDataToVertexAttributeLocation()
{
	// Bind VAO in order to record the data mapping.
	glBindVertexArray(m_vao);

	// Tell GL how to map data from the vertex buffer "m_vbo_positions" into the
	// "position" vertex attribute index for any bound shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_positions);
	GLint positionAttribLocation = m_shader.getAttribLocation( "position" );
	glVertexAttribPointer(positionAttribLocation, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Tell GL how to map data from the vertex buffer "m_vbo_colours" into the
	// "colour" vertex attribute index for any bound shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_colours);
	GLint colorAttribLocation = m_shader.getAttribLocation( "colour" );
	glVertexAttribPointer(colorAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	//-- Unbind target, and restore default values:
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	CHECK_GL_ERRORS;
}

//---------------------------------------------------------------------------------------
void A2::initLineData()
{
	m_vertexData.numVertices = 0;
	m_vertexData.index = 0;
}

//---------------------------------------------------------------------------------------
void A2::setLineColour (
		const glm::vec3 & colour
) {
	m_currentLineColour = colour;
}

//---------------------------------------------------------------------------------------
void A2::drawLine(
		const glm::vec2 & V0,   // Line Start (NDC coordinate)
		const glm::vec2 & V1    // Line End (NDC coordinate)
) {

	m_vertexData.positions[m_vertexData.index] = V0;
	m_vertexData.colours[m_vertexData.index] = m_currentLineColour;
	++m_vertexData.index;
	m_vertexData.positions[m_vertexData.index] = V1;
	m_vertexData.colours[m_vertexData.index] = m_currentLineColour;
	++m_vertexData.index;

	m_vertexData.numVertices += 2;
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, before guiLogic().
 */
void A2::appLogic()
{
	// Place per frame, application logic here ...

	// Call at the beginning of frame, before drawing lines:
	initLineData();

	// Draw outer square:
	/*setLineColour(vec3(1.0f, 0.7f, 0.8f));
	drawLine(vec2(-0.5f, -0.5f), vec2(0.5f, -0.5f));
	drawLine(vec2(0.5f, -0.5f), vec2(0.5f, 0.5f));
	drawLine(vec2(0.5f, 0.5f), vec2(-0.5f, 0.5f));
	drawLine(vec2(-0.5f, 0.5f), vec2(-0.5f, -0.5ft));


	// Draw inner square:
	setLineColour(vec3(0.2f, 1.0f, 1.0f));
	drawLine(vec2(-0.25f, -0.25f), vec2(0.25f, -0.25f));
	drawLine(vec2(0.25f, -0.25f), vec2(0.25f, 0.25f));
	drawLine(vec2(0.25f, 0.25f), vec2(-0.25f, 0.25f));
	drawLine(vec2(-0.25f, 0.25f), vec2(-0.25f, -0.25f));*/

	//Draw the cube:
	setLineColour(glm::vec3(1.0f, 1.0f, 1.0f));
	float center_x = -1.0f + x_min + x_max;
	float center_y = 1.0f - y_min - y_max;
	for (int i = 0; i < vertex_buffer.size(); i += 2) {
		glm::vec4 pointA = Projection * ViewTransformation* ModelTransformation * ScaleTransformation * vertex_buffer[i];
		glm::vec4 pointB = Projection  * ViewTransformation* ModelTransformation * ScaleTransformation * vertex_buffer[i + 1];
		pointA.x += center_x;
		pointA.y += center_y;
		pointB.x += center_x;
		pointB.y += center_y;
		if ( clipping(pointA, pointB) ) {
			drawLine(glm::vec2(pointA.x, pointA.y), glm::vec2(pointB.x, pointB.y));
		}
		if (i == 0) {
			//cout << pointA.x << " " << pointA.y << " " << pointA.z << " " << pointA.w<< endl;
			//cout << fov << endl;
		}
	}
	
	//Draw genormon
	for (int i = 0; i < gnormon.size(); i += 2) {
		if ( i == 0 ) {
			setLineColour(glm::vec3(1.0f, 0.0f, 0.0f));
		} else if ( i == 2 ) {
			setLineColour(glm::vec3(0.0f, 1.0f, 0.0f));
		} else if ( i == 4 ) {
			setLineColour(glm::vec3(0.0f, 0.0f, 1.0f));
		}
		glm::vec4 pointA = Projection * ViewTransformation * ModelTransformation  * gnormon[i];
		glm::vec4 pointB = Projection * ViewTransformation * ModelTransformation * gnormon[i + 1];
		glm::vec4 pointC = Projection * ViewTransformation  * gnormon[i];
		glm::vec4 pointD = Projection * ViewTransformation * gnormon[i + 1];
		pointA.x += center_x;
		pointA.y += center_y;
		pointB.x += center_x;
		pointB.y += center_y;
		pointC.x += center_x;
		pointC.y += center_y;
		pointD.x += center_x;
		pointD.y += center_y;
		if ( clipping(pointA, pointB) ) {
			drawLine(glm::vec2(pointA.x, pointA.y), glm::vec2(pointB.x, pointB.y));
		}
		if ( clipping(pointC, pointD) ) {
			drawLine(glm::vec2(pointC.x, pointC.y), glm::vec2(pointD.x, pointD.y));
		}
		//drawLine(glm::vec2(pointA.x, pointA.y), glm::vec2(pointB.x, pointB.y));
		
	}

	//Draw Viewport
	setLineColour(glm::vec3(0.0f, 0.0f, 0.0f));
	float x1 = -1.0 + x_min * 2;
	float x2 = -1.0 + x_max * 2;
	float y1 = 1.0 - y_min * 2;
	float y2 = 1.0 - y_max * 2;
	drawLine(glm::vec2(x1, y1), glm::vec2(x2, y1));
	drawLine(glm::vec2(x2, y1), glm::vec2(x2, y2));
	drawLine(glm::vec2(x2, y2), glm::vec2(x1, y2));
	drawLine(glm::vec2(x1, y2), glm::vec2(x1, y1));
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after appLogic(), but before the draw() method.
 */
void A2::guiLogic()
{
	static bool firstRun(true);
	if (firstRun) {
		ImGui::SetNextWindowPos(ImVec2(50, 50));
		firstRun = false;
	}

	static bool showDebugWindow(true);
	ImGuiWindowFlags windowFlags(ImGuiWindowFlags_AlwaysAutoResize);
	float opacity(0.5f);

	ImGui::Begin("Properties", &showDebugWindow, ImVec2(100,100), opacity,
			windowFlags);


		// Add more gui elements here here ...


		// Create Button, and check if it was clicked:
		if( ImGui::Button( "Quit Application" ) ) {
			glfwSetWindowShouldClose(m_window, GL_TRUE);
		}
		if( ImGui::Button( "Reset" ) ) {
			reset();
		}
		ImGui::RadioButton("Rotate View(O)", (int*)&mode, RotateV);
		ImGui::RadioButton("Translate View(E)", (int*)&mode, TranslateV);
		ImGui::RadioButton("Perspective(P)", (int *)&mode, Perspective);
		ImGui::RadioButton("Rotate Model(R)", (int *)&mode, RotateM);
		ImGui::RadioButton("Translate Model(T)", (int *)&mode, TranslateM);
		ImGui::RadioButton("Scale Model(S)", (int *)&mode, ScaleM);
		ImGui::RadioButton("Viewport(V)", (int *)&mode, Viewport);
		
		ImGui::Text( "far: %.1f, near: %.1f, fov: %.1f", far, near, fov );
		ImGui::Text( "Framerate: %.1f FPS", ImGui::GetIO().Framerate );

	ImGui::End();
}

//----------------------------------------------------------------------------------------
void A2::uploadVertexDataToVbos() {

	//-- Copy vertex position data into VBO, m_vbo_positions:
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_positions);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec2) * m_vertexData.numVertices,
				m_vertexData.positions.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CHECK_GL_ERRORS;
	}

	//-- Copy vertex colour data into VBO, m_vbo_colours:
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_colours);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3) * m_vertexData.numVertices,
				m_vertexData.colours.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CHECK_GL_ERRORS;
	}
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after guiLogic().
 */
void A2::draw()
{
	uploadVertexDataToVbos();

	glBindVertexArray(m_vao);

	m_shader.enable();
		glDrawArrays(GL_LINES, 0, m_vertexData.numVertices);
	m_shader.disable();

	// Restore defaults
	glBindVertexArray(0);

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
/*
 * Called once, after program is signaled to terminate.
 */
void A2::cleanup()
{

}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles cursor entering the window area events.
 */
bool A2::cursorEnterWindowEvent (
		int entered
) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse cursor movement events.
 */
bool A2::mouseMoveEvent (
		double xPos,
		double yPos
) {
	bool eventHandled(false);
	float rf = 0.05f;
	float sf = 0.01f;
	float tf = 0.01f;
	
	

	// Fill in with event handling code...
	if (!ImGui::IsMouseHoveringAnyWindow()) {
		switch(mode) {
			case RotateM:
				RotateModel((xPos - prev_x) * rf);
				break;
			case ScaleM:
				ScaleModel((xPos - prev_x) * sf);
				break;
			case TranslateM:
				TranslateModel((xPos - prev_x) * tf);
				break;
			case RotateV:
				RotateView((xPos - prev_x) * rf);
				break;
			case TranslateV:
				TranslateView((xPos - prev_x) * tf);
				break;
			case Perspective:
				perspective(xPos - prev_x);
				break;
			case Viewport:
				viewport(xPos, yPos);
				break;
			default:
				break;
		}
	}
	
	prev_x = xPos;
	prev_y = yPos;
	eventHandled = true;
	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse button events.
 */
bool A2::mouseButtonInputEvent (
		int button,
		int actions,
		int mods
) {
	bool eventHandled(false);

	// Fill in with event handling code...
	if (!ImGui::IsMouseHoveringAnyWindow()) {
		if( actions == GLFW_PRESS ) {
			if (button == GLFW_MOUSE_BUTTON_LEFT) {
				left_pressed = true;
				if ( mode == Viewport ) {
					tempx_1 = prev_x;
					tempy_1 = prev_y;
				}
			}
			if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
				middle_pressed = true;
			}
			if (button == GLFW_MOUSE_BUTTON_RIGHT) {
				right_pressed = true;
			}
		} else if ( actions == GLFW_RELEASE ){
			if (button == GLFW_MOUSE_BUTTON_LEFT) {
				left_pressed = false;
			}
			if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
				middle_pressed = false;
			}
			if (button == GLFW_MOUSE_BUTTON_RIGHT) {
				right_pressed = false;
			}
		}
	}
	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse scroll wheel events.
 */
bool A2::mouseScrollEvent (
		double xOffSet,
		double yOffSet
) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles window resize events.
 */


bool A2::windowResizeEvent (
		int width,
		int height
) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles key input events.
 */
bool A2::keyInputEvent (
		int key,
		int action,
		int mods
) {
	bool eventHandled(false);

	// Fill in with event handling code...
	if( action == GLFW_PRESS ) {
		if ( key == GLFW_KEY_Q ) {
			glfwSetWindowShouldClose(m_window, GL_TRUE);
			eventHandled = true;
		}
		if ( key == GLFW_KEY_A ) {
			reset();
			eventHandled = true;
		}
		if ( key == GLFW_KEY_T ) {
			mode = TranslateM;
			eventHandled = true;
		}
		if ( key == GLFW_KEY_S ) {
			mode = ScaleM;
			eventHandled = true;
		}
		if ( key == GLFW_KEY_R ) {
			mode = RotateM;
			eventHandled = true;
		}
		if ( key == GLFW_KEY_O ) {
			mode = RotateV;
			eventHandled = true;
		}
		if ( key == GLFW_KEY_E ) {
			mode = TranslateV;
			eventHandled = true;
		}
		if ( key == GLFW_KEY_P ) {
			mode = Perspective;
			eventHandled = true;
		}
		if ( key == GLFW_KEY_V ) {
			mode = Viewport;
			eventHandled = true;
		}
	}
	return eventHandled;
}

void A2::initall() {
	//upper 4 edges
	vertex_buffer.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	vertex_buffer.push_back(glm::vec4(1.0f, 1.0f, -1.0f, 1.0f));
	vertex_buffer.push_back(glm::vec4(1.0f, 1.0f, -1.0f, 1.0f));
	vertex_buffer.push_back(glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f));
	vertex_buffer.push_back(glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f));
	vertex_buffer.push_back(glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f));
	vertex_buffer.push_back(glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f));
	vertex_buffer.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	//4 heights
	vertex_buffer.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	vertex_buffer.push_back(glm::vec4(1.0f, -1.0f, 1.0f, 1.0f));
	vertex_buffer.push_back(glm::vec4(1.0f, 1.0f, -1.0f, 1.0f));
	vertex_buffer.push_back(glm::vec4(1.0f, -1.0f, -1.0f, 1.0f));
	vertex_buffer.push_back(glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f));
	vertex_buffer.push_back(glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f));
	vertex_buffer.push_back(glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f));
	vertex_buffer.push_back(glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f));

	//bottom 4 edges;
	vertex_buffer.push_back(glm::vec4(1.0f, -1.0f, 1.0f, 1.0f));
	vertex_buffer.push_back(glm::vec4(1.0f, -1.0f, -1.0f, 1.0f));
	vertex_buffer.push_back(glm::vec4(1.0f, -1.0f, -1.0f, 1.0f));
	vertex_buffer.push_back(glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f));
	vertex_buffer.push_back(glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f));
	vertex_buffer.push_back(glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f));
	vertex_buffer.push_back(glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f));
	vertex_buffer.push_back(glm::vec4(1.0f, -1.0f, 1.0f, 1.0f));

	//gnormon:
	gnormon.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	gnormon.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	gnormon.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	gnormon.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	gnormon.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	gnormon.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
}



void A2::reset() {
	ModelTransformation = mat4(1.0f);
	ViewTransformation = mat4(1.0f);
	x_min = 0.05f;
	x_max = 0.95f;
	y_min = 0.05f;
	y_max = 0.95f;
	fov = 30.0f;
	far = 25.0f;
	near = 6.0f;
	SetProjection();
	mode = RotateM;
	left_pressed = false;
	right_pressed = false;
	middle_pressed = false;
}

void A2::RotateModel(float theta) {
	mat4 Rx = mat4(1.0f);
	mat4 Ry = mat4(1.0f);
	mat4 Rz = mat4(1.0f);
	if ( left_pressed ) {
		Rx[1][1] = cos(theta);
		Rx[2][1] = -sin(theta);
		Rx[1][2] = sin(theta);
		Rx[2][2] = cos(theta);
	}
	if ( middle_pressed ) {
		Ry[0][0] = cos(theta);
		Ry[0][2] = -sin(theta);
		Ry[2][0] = sin(theta);
		Ry[2][2] = cos(theta);
	}
	if ( right_pressed ) {
		Rz[0][0] = cos(theta);
		Rz[0][1] = sin(theta);
		Rz[1][0] = -sin(theta);
		Rz[1][1] = cos(theta);
	}
	ModelTransformation = Rz * Ry * Rx * ModelTransformation;
}

void A2::TranslateModel(float delta) {
	mat4 R = mat4(1.0f);
	if ( left_pressed ) {
		R[3][0] = delta;
	}
	if ( middle_pressed ) {
		R[3][1] = delta;
	}
	if ( right_pressed ) {
		R[3][2] = delta;
	}
	
	ModelTransformation = R * ModelTransformation;
}

void A2::ScaleModel(float value) {
	mat4 R = mat4(1.0f);
	if ( left_pressed ) {
		R[0][0] = 1.0f + value;
	}
	if ( middle_pressed ) {
		R[1][1] = 1.0f + value;
	}
	if ( right_pressed ) {
		R[2][2] = 1.0f + value;
	}
	ScaleTransformation = R * ScaleTransformation;
}

void A2::RotateView(float theta) {
	mat4 Rx = mat4(1.0f);
	mat4 Ry = mat4(1.0f);
	mat4 Rz = mat4(1.0f);
	if ( left_pressed ) {
		Rx[1][1] = cos(theta);
		Rx[2][1] = -sin(theta);
		Rx[1][2] = sin(theta);
		Rx[2][2] = cos(theta);
	}
	if ( middle_pressed ) {
		Ry[0][0] = cos(theta);
		Ry[0][2] = -sin(theta);
		Ry[2][0] = sin(theta);
		Ry[2][2] = cos(theta);
	}
	if ( right_pressed ) {
		Rz[0][0] = cos(theta);
		Rz[0][1] = sin(theta);
		Rz[1][0] = -sin(theta);
		Rz[1][1] = cos(theta);
	}
	Rx = mat4(1.0f) / Rx;
	Ry = mat4(1.0f) / Ry;
	Rz = mat4(1.0f) / Rz;
	ViewTransformation = Rz * Ry * Rx * ViewTransformation;
}

void A2::TranslateView(float delta) {
	mat4 R = mat4(1.0f);
	if ( left_pressed ) {
		R[3][0] = delta;
	}
	if ( middle_pressed ) {
		R[3][1] = delta;
	}
	if ( right_pressed ) {
		R[3][2] = delta;
	}
	R = mat4(1.0f) / R;
	ViewTransformation = R * ViewTransformation;
	
}


void A2::SetProjection() {
	float aspect = (y_max - y_min) / (x_max - x_min);
	float trig = cos(fov / 2 / 180 * PI) / sin(fov / 2 / 180 * PI);
	mat4 R = mat4(1.0f);
	R[0][0] = trig / aspect;
	R[1][1] = trig;
	R[2][2] = ( far + near ) / ( far - near );
	R[2][3] = -2.0 * far * near / (far - near);
	R[3][2] = 1;
	R[3][3] = 0;
	Projection = R;
}


void A2::perspective(float value) {
	float coefficient = 0.5f * value;
	if ( left_pressed ) {
		if ( 5.0f <= (fov + value) && (fov + value) <= 160.f) {
			fov += value;
		}
	}
	if ( middle_pressed ) {
		if ( 1.0f <= (near + coefficient) && (near + coefficient) < far) {
			near += value;
		}
	}
	if ( right_pressed ) {
		if (  far + coefficient >= near) {
			far += coefficient;
		}
	}
	SetProjection();
}


bool A2::clipping_oneside(glm::vec4 & A, glm::vec4 & B, glm::vec3 P, glm::vec3 n) {
	glm::vec3 Ai = vec3(A.x, A.y, 1.0f);
	glm::vec3 Bi = vec3(B.x, B.y, 1.0f);
	float wecA = glm::dot(Ai - P, n);
	float wecB = glm::dot(Bi - P, n);
	if ( wecA < 0 && wecB < 0 ) {
		return false;
	}
	if ( wecA >= 0 && wecB >= 0 ) {
		return true;
	}
	float t = wecA / (wecA - wecB);
	if ( wecA < 0 ) {
		Ai = Ai + t * (Bi - Ai);
		A.x = Ai.x;
		A.y = Ai.y;
	} else {
		Bi = Ai + t * (Bi - Ai);
		B.x = Bi.x;
		B.y = Bi.y;
	}
	return true;
};

bool A2::clipping(glm::vec4 & A, glm::vec4 & B) {
	float x1 = -1.0 + x_min * 2;
	float x2 = -1.0 + x_max * 2;
	float y1 = 1.0 - y_min * 2;
	float y2 = 1.0 - y_max * 2;
	if ( !clipping_oneside(A, B, vec3(x1, y1, 1.0f), vec3(0.0f, -1.0f, 0.0f)) ) {
		return false;
	}
	if ( !clipping_oneside(A, B, vec3(x2, y1, 1.0f), vec3(-1.0f, 0.0f, 0.0f)) ) {
		return false;
	}
	if ( !clipping_oneside(A, B, vec3(x2, y2, 1.0f), vec3(0.0f, 1.0f, 0.0f)) ) {
		return false;
	}
	if ( !clipping_oneside(A, B, vec3(x1, y2, 1.0f), vec3(1.0f, 0.0f, 0.0f)) ) {
		return false;
	}
	return true;
}



void A2::viewport(float xPos, float yPos) {
	if ( left_pressed ) {
		tempx_2 = xPos;
		tempy_2 = yPos;
		
		x_min = std::min(tempx_1, tempx_2) / WINDOW_X;
		x_max = std::max(tempx_1, tempx_2) / WINDOW_X;
		y_min = std::min(tempy_1, tempy_2) / WINDOW_Y;
		y_max = std::max(tempy_1, tempy_2) / WINDOW_Y;
		//cout << y_max << " " << tempy_2 << endl;
		SetProjection();
	}
}








