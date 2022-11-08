// Termm-Fall 2022

#include "A3.hpp"
#include "scene_lua.hpp"
using namespace std;

#include "cs488-framework/GlErrorCheck.hpp"
#include "cs488-framework/MathUtils.hpp"
#include "GeometryNode.hpp"
#include "JointNode.hpp"

#include <imgui/imgui.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

using namespace glm;

static bool show_gui = true;

const size_t CIRCLE_PTS = 48;

//----------------------------------------------------------------------------------------
// Constructor
A3::A3(const std::string & luaSceneFile)
	: m_luaSceneFile(luaSceneFile),
	  m_positionAttribLocation(0),
	  m_normalAttribLocation(0),
	  m_vao_meshData(0),
	  m_vbo_vertexPositions(0),
	  m_vbo_vertexNormals(0),
	  m_vao_arcCircle(0),
	  m_vbo_arcCircle(0),
	  mode(PO),
	  picking_mode(false),
	  translateAll(mat4(1.0f)),
	  rotateAll(mat4(1.0f)),
	  middle_pressed(false),
	  left_pressed(false),
	  right_pressed(false),
	  picking(false),
	  backculling(false),
	  frontculling(false),
	  Z_buffer(true),
	  trackball(false)
{

}

//----------------------------------------------------------------------------------------
// Destructor
A3::~A3()
{

}

//----------------------------------------------------------------------------------------
/*
 * Called once, at program start.
 */
void A3::init()
{
	// Set the background colour.
	glClearColor(0.4, 0.4, 0.4, 1.0);

	createShaderProgram();

	glGenVertexArrays(1, &m_vao_arcCircle);
	glGenVertexArrays(1, &m_vao_meshData);
	enableVertexShaderInputSlots();

	processLuaSceneFile(m_luaSceneFile);

	// Load and decode all .obj files at once here.  You may add additional .obj files to
	// this list in order to support rendering additional mesh types.  All vertex
	// positions, and normals will be extracted and stored within the MeshConsolidator
	// class.
	unique_ptr<MeshConsolidator> meshConsolidator (new MeshConsolidator{
			getAssetFilePath("cube.obj"),
			getAssetFilePath("sphere.obj"),
			getAssetFilePath("suzanne.obj")
	});


	// Acquire the BatchInfoMap from the MeshConsolidator.
	meshConsolidator->getBatchInfoMap(m_batchInfoMap);

	// Take all vertex data within the MeshConsolidator and upload it to VBOs on the GPU.
	uploadVertexDataToVbos(*meshConsolidator);

	mapVboDataToVertexShaderInputLocations();

	initPerspectiveMatrix();

	initViewMatrix();

	initLightSources();


	// Exiting the current scope calls delete automatically on meshConsolidator freeing
	// all vertex data resources.  This is fine since we already copied this data to
	// VBOs on the GPU.  We have no use for storing vertex data on the CPU side beyond
	// this point.
}

//----------------------------------------------------------------------------------------
void A3::processLuaSceneFile(const std::string & filename) {
	// This version of the code treats the Lua file as an Asset,
	// so that you'd launch the program with just the filename
	// of a puppet in the Assets/ directory.
	// std::string assetFilePath = getAssetFilePath(filename.c_str());
	// m_rootNode = std::shared_ptr<SceneNode>(import_lua(assetFilePath));

	// This version of the code treats the main program argument
	// as a straightforward pathname.
	m_rootNode = std::shared_ptr<SceneNode>(import_lua(filename));
	if (!m_rootNode) {
		std::cerr << "Could Not Open " << filename << std::endl;
	}
}

//----------------------------------------------------------------------------------------
void A3::createShaderProgram()
{
	m_shader.generateProgramObject();
	m_shader.attachVertexShader( getAssetFilePath("Phong.vs").c_str() );
	m_shader.attachFragmentShader( getAssetFilePath("Phong.fs").c_str() );
	m_shader.link();

	m_shader_arcCircle.generateProgramObject();
	m_shader_arcCircle.attachVertexShader( getAssetFilePath("arc_VertexShader.vs").c_str() );
	m_shader_arcCircle.attachFragmentShader( getAssetFilePath("arc_FragmentShader.fs").c_str() );
	m_shader_arcCircle.link();
}

//----------------------------------------------------------------------------------------
void A3::enableVertexShaderInputSlots()
{
	//-- Enable input slots for m_vao_meshData:
	{
		glBindVertexArray(m_vao_meshData);

		// Enable the vertex shader attribute location for "position" when rendering.
		m_positionAttribLocation = m_shader.getAttribLocation("position");
		glEnableVertexAttribArray(m_positionAttribLocation);

		// Enable the vertex shader attribute location for "normal" when rendering.
		m_normalAttribLocation = m_shader.getAttribLocation("normal");
		glEnableVertexAttribArray(m_normalAttribLocation);

		CHECK_GL_ERRORS;
	}


	//-- Enable input slots for m_vao_arcCircle:
	{
		glBindVertexArray(m_vao_arcCircle);

		// Enable the vertex shader attribute location for "position" when rendering.
		m_arc_positionAttribLocation = m_shader_arcCircle.getAttribLocation("position");
		glEnableVertexAttribArray(m_arc_positionAttribLocation);

		CHECK_GL_ERRORS;
	}

	// Restore defaults
	glBindVertexArray(0);
}

//----------------------------------------------------------------------------------------
void A3::uploadVertexDataToVbos (
		const MeshConsolidator & meshConsolidator
) {
	// Generate VBO to store all vertex position data
	{
		glGenBuffers(1, &m_vbo_vertexPositions);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexPositions);

		glBufferData(GL_ARRAY_BUFFER, meshConsolidator.getNumVertexPositionBytes(),
				meshConsolidator.getVertexPositionDataPtr(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		CHECK_GL_ERRORS;
	}

	// Generate VBO to store all vertex normal data
	{
		glGenBuffers(1, &m_vbo_vertexNormals);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexNormals);

		glBufferData(GL_ARRAY_BUFFER, meshConsolidator.getNumVertexNormalBytes(),
				meshConsolidator.getVertexNormalDataPtr(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		CHECK_GL_ERRORS;
	}

	// Generate VBO to store the trackball circle.
	{
		glGenBuffers( 1, &m_vbo_arcCircle );
		glBindBuffer( GL_ARRAY_BUFFER, m_vbo_arcCircle );

		float *pts = new float[ 2 * CIRCLE_PTS ];
		for( size_t idx = 0; idx < CIRCLE_PTS; ++idx ) {
			float ang = 2.0 * M_PI * float(idx) / CIRCLE_PTS;
			pts[2*idx] = cos( ang );
			pts[2*idx+1] = sin( ang );
		}

		glBufferData(GL_ARRAY_BUFFER, 2*CIRCLE_PTS*sizeof(float), pts, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		CHECK_GL_ERRORS;
	}
}

//----------------------------------------------------------------------------------------
void A3::mapVboDataToVertexShaderInputLocations()
{
	// Bind VAO in order to record the data mapping.
	glBindVertexArray(m_vao_meshData);

	// Tell GL how to map data from the vertex buffer "m_vbo_vertexPositions" into the
	// "position" vertex attribute location for any bound vertex shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexPositions);
	glVertexAttribPointer(m_positionAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Tell GL how to map data from the vertex buffer "m_vbo_vertexNormals" into the
	// "normal" vertex attribute location for any bound vertex shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexNormals);
	glVertexAttribPointer(m_normalAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	//-- Unbind target, and restore default values:
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	CHECK_GL_ERRORS;

	// Bind VAO in order to record the data mapping.
	glBindVertexArray(m_vao_arcCircle);

	// Tell GL how to map data from the vertex buffer "m_vbo_arcCircle" into the
	// "position" vertex attribute location for any bound vertex shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_arcCircle);
	glVertexAttribPointer(m_arc_positionAttribLocation, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	//-- Unbind target, and restore default values:
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
void A3::initPerspectiveMatrix()
{
	float aspect = ((float)m_windowWidth) / m_windowHeight;
	m_perpsective = glm::perspective(degreesToRadians(60.0f), aspect, 0.1f, 100.0f);
}


//----------------------------------------------------------------------------------------
void A3::initViewMatrix() {
	m_view = glm::lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f),
			vec3(0.0f, 1.0f, 0.0f));
}

//----------------------------------------------------------------------------------------
void A3::initLightSources() {
	// World-space position
	m_light.position = vec3(-2.0f, 7.0f, 5.0f);
	m_light.rgbIntensity = vec3(0.5f); // light
}

//----------------------------------------------------------------------------------------
void A3::uploadCommonSceneUniforms() {
	m_shader.enable();
	{
		//-- Set Perpsective matrix uniform for the scene:
		GLint location = m_shader.getUniformLocation("Perspective");
		glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(m_perpsective));
		CHECK_GL_ERRORS;


		//-- Set LightSource uniform for the scene:
		{
			location = m_shader.getUniformLocation("light.position");
			glUniform3fv(location, 1, value_ptr(m_light.position));
			location = m_shader.getUniformLocation("light.rgbIntensity");
			glUniform3fv(location, 1, value_ptr(m_light.rgbIntensity));
			CHECK_GL_ERRORS;
		}

		//-- Set background light ambient intensity
		{
			location = m_shader.getUniformLocation("ambientIntensity");
			vec3 ambientIntensity(0.3f);
			glUniform3fv(location, 1, value_ptr(ambientIntensity));
			CHECK_GL_ERRORS;
		}
	}
	m_shader.disable();
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, before guiLogic().
 */
void A3::appLogic()
{
	// Place per frame, application logic here ...

	uploadCommonSceneUniforms();
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after appLogic(), but before the draw() method.
 */
void A3::guiLogic()
{
	if( !show_gui ) {
		return;
	}

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
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("Application")) {
				if (ImGui::MenuItem("Reset Position (I)")) {
					resetPosition();
				}
				if (ImGui::MenuItem("Reset Orientation (O)")) {
					resetOrientation();
				}
				if (ImGui::MenuItem("Reset Joint (S)")) {
					resetJoints(*m_rootNode);
				}
				if (ImGui::MenuItem("Reset All (A)")) {
					resetPosition();
					resetOrientation();
					resetJoints(*m_rootNode);
				}
				if( ImGui::MenuItem( "Quit (Q)" ) ) {
					glfwSetWindowShouldClose(m_window, GL_TRUE);
				}
				 ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Options")) {
				ImGui::Checkbox("Circle (C)", &trackball);
				ImGui::Checkbox("Z-buffer (Z)", &Z_buffer);
				ImGui::Checkbox("Backface culling (B)", &backculling);
				ImGui::Checkbox("Frontface culling (F)", &frontculling);
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		// Create Button, and check if it was clicked:
		if( ImGui::Button( "Quit Application" ) ) {
			glfwSetWindowShouldClose(m_window, GL_TRUE);
		}
		ImGui::RadioButton("Position/Orientation", (int*)&mode, PO);
		ImGui::RadioButton("Joints", (int*)&mode, Joints);
		ImGui::Text( "Framerate: %.1f FPS", ImGui::GetIO().Framerate );

	ImGui::End();
}

//----------------------------------------------------------------------------------------
// Update mesh specific shader uniforms: Remember to add transformation of curnode to Model!!!!
static void updateShaderUniforms(
		const ShaderProgram & shader,
		const GeometryNode & node,
		const glm::mat4 & viewMatrix,
		const glm::mat4 & Model,
		bool picking
) {

	shader.enable();
	{	
		//-- Set picking:
		GLint location = shader.getUniformLocation("picking");
		glUniform1f(location, picking);
		//-- Set ModelView matrix:
		location = shader.getUniformLocation("ModelView");
		mat4 modelView = viewMatrix * Model;
		glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(modelView));
		CHECK_GL_ERRORS;

		//-- Set NormMatrix:
		location = shader.getUniformLocation("NormalMatrix");
		mat3 normalMatrix = glm::transpose(glm::inverse(mat3(modelView)));
		glUniformMatrix3fv(location, 1, GL_FALSE, value_ptr(normalMatrix));
		CHECK_GL_ERRORS;

		//-- Set Material values:
		if ( picking ) {
			float red = node.m_nodeId / 255.0f;
			float green = node.m_nodeId / 255.0f;
			float blue = 0;
			
			location = shader.getUniformLocation("material.kd");
			vec3 kd = vec3(red, green, blue);
			glUniform3fv(location, 1, value_ptr(kd));
			CHECK_GL_ERRORS;
		} else {
			location = shader.getUniformLocation("material.kd");
			vec3 kd = node.material.kd;
			if (node.isSelected) {
				kd = vec3(1.0f, 1.0f, 0.0f);
			}
			glUniform3fv(location, 1, value_ptr(kd));
			CHECK_GL_ERRORS;
		}

		location = shader.getUniformLocation("material.ks");
		vec3 ks = node.material.ks;
		glUniform3fv(location, 1, value_ptr(ks));
		CHECK_GL_ERRORS;

		location = shader.getUniformLocation("material.shininess");
		glUniform1f(location, node.material.shininess);
		CHECK_GL_ERRORS;
			
	}
	shader.disable();

}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after guiLogic().
 */
void A3::draw() {
	if ( Z_buffer ) {
		glEnable( GL_DEPTH_TEST );
	}
	if (frontculling && backculling) {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT_AND_BACK);
	} else if (frontculling) {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
	} else if (backculling) {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	renderSceneGraph(*m_rootNode);

	if (Z_buffer) {
		glDisable( GL_DEPTH_TEST );
	}

	if (frontculling || backculling) {
		glDisable( GL_CULL_FACE );
	}

	
	glDisable( GL_DEPTH_TEST );
	if ( trackball ) {
		renderArcCircle();
	}
}

//----------------------------------------------------------------------------------------


void A3::renderSceneGraph(const SceneNode & root) {

	// Bind the VAO once here, and reuse for all GeometryNode rendering below.
	glBindVertexArray(m_vao_meshData);

	// This is emphatically *not* how you should be drawing the scene graph in
	// your final implementation.  This is a non-hierarchical demonstration
	// in which we assume that there is a list of GeometryNodes living directly
	// underneath the root node, and that we can draw them in a loop.  It's
	// just enough to demonstrate how to get geometry and materials out of
	// a GeometryNode and onto the screen.

	// You'll want to turn this into recursive code that walks over the tree.
	// You can do that by putting a method in SceneNode, overridden in its
	// subclasses, that renders the subtree rooted at every node.  Or you
	// could put a set of mutually recursive functions in this class, which
	// walk down the tree from nodes of different types.
	
	//when we are rotate entire puppet, it might get out of view since we rotate after applying the translating on root node.
	mat4 root_trans = root.get_transform();
	renderNode(m_shader, &root, translateAll * root_trans * rotateAll * inverse(root_trans));
	
	glBindVertexArray(0);
	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
// Draw the trackball circle.
void A3::renderArcCircle() {
	glBindVertexArray(m_vao_arcCircle);

	m_shader_arcCircle.enable();
		GLint m_location = m_shader_arcCircle.getUniformLocation( "M" );
		float aspect = float(m_framebufferWidth)/float(m_framebufferHeight);
		glm::mat4 M;
		if( aspect > 1.0 ) {
			M = glm::scale( glm::mat4(), glm::vec3( 0.5/aspect, 0.5, 1.0 ) );
		} else {
			M = glm::scale( glm::mat4(), glm::vec3( 0.5, 0.5*aspect, 1.0 ) );
		}
		glUniformMatrix4fv( m_location, 1, GL_FALSE, value_ptr( M ) );
		glDrawArrays( GL_LINE_LOOP, 0, CIRCLE_PTS );
	m_shader_arcCircle.disable();

	glBindVertexArray(0);
	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
/*
 * Called once, after program is signaled to terminate.
 */
void A3::cleanup()
{

}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles cursor entering the window area events.
 */
bool A3::cursorEnterWindowEvent (
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
bool A3::mouseMoveEvent (
		double xPos,
		double yPos
) {
	bool eventHandled(false);

	// Fill in with event handling code...
	if (!ImGui::IsMouseHoveringAnyWindow()) {
		switch(mode) {
			case PO:
				PositionChange(xPos, yPos);
				break;
			case Joints:
				JointChange(xPos, yPos);
				break;
			default:
				break;
		}
	}
	//cout << "real: " << xPos << " " << yPos << endl;
	prev_x = xPos;
	prev_y = yPos;

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse button events.
 */
bool A3::mouseButtonInputEvent (
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
				if ( mode == Joints ) {
					selecting();
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
bool A3::mouseScrollEvent (
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
bool A3::windowResizeEvent (
		int width,
		int height
) {
	bool eventHandled(false);
	initPerspectiveMatrix();
	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles key input events.
 */
bool A3::keyInputEvent (
		int key,
		int action,
		int mods
) {
	bool eventHandled(false);

	if( action == GLFW_PRESS ) {
		if( key == GLFW_KEY_M ) {
			show_gui = !show_gui;
			eventHandled = true;
		}
		if (key == GLFW_KEY_P) {
			mode = PO;
			eventHandled = true;
		}
		if (key == GLFW_KEY_J) {
			mode = Joints;
			eventHandled = true;
		}
		if( key == GLFW_KEY_I) {
			resetPosition();
			eventHandled = true;
		}
		if( key == GLFW_KEY_O) {
			resetOrientation();
			eventHandled = true;
		}
		if( key == GLFW_KEY_S) {
			resetJoints(*m_rootNode);
			eventHandled = true;
		}
		
		if( key == GLFW_KEY_A) {
			resetPosition();
			resetOrientation();
			resetJoints(*m_rootNode);
			eventHandled = true;
		}
		if( key == GLFW_KEY_C ) {
			trackball = !trackball;
			eventHandled = true;
		}
		if( key == GLFW_KEY_Z ) {
			Z_buffer = !Z_buffer;
			eventHandled = true;
		}
		if( key == GLFW_KEY_B ) {
			backculling = !backculling;
			eventHandled = true;
		}
		if( key == GLFW_KEY_F ) {
			frontculling = !frontculling;
			eventHandled = true;
		}
		
		if (key == GLFW_KEY_Q) {
			glfwSetWindowShouldClose(m_window, GL_TRUE);
			eventHandled = true;
		}
	}
	// Fill in with event handling code...

	return eventHandled;
}

void A3::test(const GeometryNode & node) {
	glm::vec3 position = vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 normal = vec3(0.0f, 0.0f, -1.0f);
	glm::vec4 pos4 = vec4(position, 1.0);
	float aspect = ((float)m_windowWidth) / m_windowHeight;
	mat4 Perspective = glm::perspective(degreesToRadians(60.0f), aspect, 0.1f, 100.0f);
	mat4 view = glm::lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f),
			vec3(0.0f, 1.0f, 0.0f));
	mat4 ModelView = m_view * node.trans;
	//-- Convert position and normal to Eye-Space:
	vec3 position_ES = vec3(ModelView * pos4);
	mat3 NormalMatrix = glm::transpose(glm::inverse(mat3(ModelView)));
	vec3 normal_ES = normalize(NormalMatrix * normal);

	vec3 light_pos = vec3(-0.5f, 0.0f, -0.88f);
	vec3 rgb = vec3(1.0f);
	//cout << Perspective * ModelView * vec4(position, 1.0) << endl;



	vec3 l = normalize(light_pos - position_ES);

    // Direction from fragment to viewer (origin - fragPosition).
    vec3 v = normalize(-vec3(position_ES));

    float n_dot_l = 0.0;
	if ( dot(normal_ES, l) > 0 ) {
		n_dot_l = dot(normal_ES, l);
	}

	vec3 diffuse;
	diffuse = node.material.kd * n_dot_l;

    vec3 specular = vec3(0.0);

    if (n_dot_l > 0.0) {
		// Halfway vector.
		vec3 h = normalize(v + l);
	
        float n_dot_h = 0.0;
	if ( dot(normal_ES, h) >= 0.0 ) {
		n_dot_h = dot(normal_ES, h);
	}

        specular = node.material.ks * pow(n_dot_h, node.material.shininess);
    }
	vec3 ambientIntensity = vec3(0.25);
	//cout << diffuse << endl;
    cout << ambientIntensity + rgb * (diffuse + specular) << endl;

}




void A3::renderNode(const ShaderProgram & shader,
		const SceneNode * node,
		const glm::mat4 & prevModel) {
	if (node == nullptr) return;
	if ( node->isSelected ) { //cout << node->get_transform() << endl; }
	}
	//Note: When we are transforming a node, we do bottom-up transformation, since all transformation are in terms of origin
	mat4 curModel = prevModel * node->get_transform() ;
	if (node->m_nodeType == NodeType::GeometryNode) {
		const GeometryNode * geometryNode = static_cast<const GeometryNode *>(node);
		updateShaderUniforms(m_shader, *geometryNode, m_view, curModel, picking);
		//test(*geometryNode);
		// Get the BatchInfo corresponding to the GeometryNode's unique MeshId.
		BatchInfo batchInfo = m_batchInfoMap[geometryNode->meshId];
		//-- Now render the mesh:
		m_shader.enable();
		glDrawArrays(GL_TRIANGLES, batchInfo.startIndex, batchInfo.numIndices);
		m_shader.disable();
		
	}
	for (const SceneNode * child : node->children) {
		renderNode(m_shader, child, curModel);
	}
}

float get_min (float x, float y) {
	if ( x <= y ) {
		return x;
	}
	return y;
}

void A3::PositionChange(float xpos, float ypos) {
	float deltax = xpos - prev_x;
	float deltay = ypos - prev_y;
	float radius = get_min(m_framebufferWidth , m_framebufferHeight) / 4.0f;
	float center_x = m_framebufferWidth / 2.0f;
	float center_y =  m_framebufferHeight / 2.0f;
	if ( left_pressed ) {
		translateAll = translate(translateAll, vec3(deltax * 0.1f, deltay * -0.01f, 0.0f));
	}
	if ( middle_pressed ) {
		translateAll = translate(translateAll, vec3(0.0f, 0.0f, deltax * 0.01f));
	}
	if ( right_pressed ) {
		if ( (xpos - center_x) * (xpos - center_x) + (ypos - center_y) * (ypos - center_y) <= radius * radius) {
			rotateAll = rotate(rotateAll,deltax / 50, vec3(0.0f, 1.0f, 0.0f));
			rotateAll = rotate(rotateAll,deltay / 50, vec3(1.0f, 0.0f, 0.0f));
			//cout << "222222222222222222" << endl;
		} else {
			//cout << xpos << " " << ypos << radius << endl;
			rotateAll = rotate(rotateAll,deltax / 50, vec3(0.0f, 0.0f, 1.0f));
			//cout << "1111111111111111" << endl;
		}
	}
}

void range(float & x, float max, float min) {
	if (x > max) {x = max; }
	if (x < min) {x = min; }
}

void A3::JointChange(float xpos, float ypos) {
	float deltax = (xpos - prev_x) / 10.0f;
	float deltay = (ypos - prev_y) / 10.0f;
	for (JointNode * child : SelectedJoints) {
		if ( middle_pressed ) {
			//cout << child->cur_joint_y <<endl;
			child->trans = mat4(1.0f);
			child->cur_joint_x += deltax;
			range(child->cur_joint_x, child->m_joint_x.max, child->m_joint_x.min);
			float rotate_x = child->cur_joint_x * PI / 180;
			child->trans = glm::rotate(child->trans, rotate_x, vec3(1.0f,0.0f,0.0f));
			child->cur_joint_y += deltay;
			range(child->cur_joint_y, child->m_joint_y.max, child->m_joint_y.min);
			float rotate_y = child->cur_joint_y * PI / 180;
			child->trans = glm::rotate(child->trans, rotate_y, vec3(0.0f,1.0f,0.0f));
			
		}
		if ( right_pressed && child->m_name == "neck_head_joint") {
			float prev_z = rotate_z;
			rotate_z += deltax;
			range(rotate_z, 40.0f, -40.0f);
			//cout << rotate_z << " " << prev_z << endl;
			float radian = (rotate_z - prev_z) * PI / 180.0f;
			child->trans = glm::rotate(child->trans, radian, vec3(0.0f,0.0f,1.0f));
		}
	}
}


void A3::selecting() {
	double xpos;
	double ypos;
	glfwGetCursorPos(m_window, &xpos, &ypos);
	//cout << "scanned:" << xpos << " " << ypos << endl;
	picking = true;
	float pixel[4] = {0.0, 0.0, 0.0, 0.0};
	renderSceneGraph(*m_rootNode);
	//I think y-axis direction of window and frame buffer is opposite
	ypos = m_windowHeight - ypos;
	glReadPixels( int(xpos), int(ypos), 1, 1, GL_RGBA, GL_FLOAT, pixel );
	CHECK_GL_ERRORS;

	//click background
	if (pixel[0] == pixel[2]) {
		picking = false;
		return;
	}

	int selected = round(pixel[0] * 255.0f);
	JointNode * joint_selected = nullptr;
	makeSelection(*m_rootNode, selected, joint_selected);

	bool add = true;
	for (auto a = SelectedJoints.begin(); a != SelectedJoints.end(); ++a) {
		if ( *a == joint_selected ) {
			SelectedJoints.erase(a);
			add = false;
			break;
		}
	}
	if ( add && joint_selected != nullptr) {
		SelectedJoints.push_back(joint_selected);
	} 
	
	picking = false;
}


bool A3::makeSelection(SceneNode & root, int id, JointNode *& last_joint) {
	if (root.m_nodeType == NodeType::JointNode) {
		JointNode * jointnode = static_cast<JointNode *>(&root);
		last_joint = jointnode; 
	}
	if (root.m_nodeId == id) { root.isSelected = !root.isSelected; return true;}
	for (SceneNode * child : root.children) {
		if (makeSelection(*child, id, last_joint)) return true;
	}
	return false;
}

void A3::resetPosition() {
	translateAll = mat4(1.0f);
}

void A3::resetOrientation() {
	rotateAll = mat4(1.0f);
}

void A3::resetJoints(SceneNode & node) {
	if (node.m_nodeType == NodeType::JointNode) {
		JointNode * jnode = static_cast<JointNode *>(&node);
		mat4 initial_trans = mat4(1.0f);
		jnode->trans = glm::rotate(initial_trans, (float)jnode->m_joint_x.init, vec3(1.0f, 0.0f, 0.0f));
		jnode->trans = glm::rotate(jnode->trans, (float)jnode->m_joint_y.init, vec3(0.0f, 1.0f, 0.0f));
	}
	for (SceneNode * child : node.children) {
		resetJoints(*child);
	}
}






