// Termm--Fall 2022

#include "A1.hpp"
#include "cs488-framework/GlErrorCheck.hpp"

#include <iostream>
#include <ctime>

#include <sys/types.h>
#include <unistd.h>

#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;
using namespace std;

static const size_t DIM = 16;
const float PI = 3.14159265f;

//----------------------------------------------------------------------------------------
// Constructor
A1::A1()
	: current_col( 0 ), m(DIM)
{
	colour[0] = 0.0f;
	colour[1] = 0.0f;
	colour[2] = 0.0f;
}

//----------------------------------------------------------------------------------------
// Destructor
A1::~A1()
{}

//----------------------------------------------------------------------------------------
/*
 * Called once, at program start.
 */
void A1::init()
{
	// Initialize random number generator
	int rseed=getpid();
	srandom(rseed);
	// Print random number seed in case we want to rerun with
	// same random numbers
	cout << "Random number seed = " << rseed << endl;
	

	m.digMaze();
	m.printMaze();

	for(int i = 0; i < m.getDim();  ++i) {
		if ( m.getValue(i, 0) == 0 ) {
			avatar = vec3(i, 0, 0);
		}
	}

	for(int j = 0; j < m.getDim();  ++j) {
		if ( m.getValue(0, j) == 0 ) {
			avatar = vec3(0, 0, j);
		}
	}
	
	// Set the background colour.
	glClearColor( 0.3, 0.5, 0.7, 1.0 );

	// Build the shader
	m_shader.generateProgramObject();
	m_shader.attachVertexShader(
		getAssetFilePath( "VertexShader.vs" ).c_str() );
	m_shader.attachFragmentShader(
		getAssetFilePath( "FragmentShader.fs" ).c_str() );
	m_shader.link();

	// Set up the uniforms
	P_uni = m_shader.getUniformLocation( "P" );
	V_uni = m_shader.getUniformLocation( "V" );
	M_uni = m_shader.getUniformLocation( "M" );
	col_uni = m_shader.getUniformLocation( "colour" );

	initGrid();
	initCube();
	initAvatar();
	initFloor();

	// Set up initial view and projection matrices (need to do this here,
	// since it depends on the GLFW window being set up correctly).
	view = glm::lookAt( 
		glm::vec3( 0.0f, 2.*float(DIM)*2.0*M_SQRT1_2, float(DIM)*2.0*M_SQRT1_2 ),
		glm::vec3( 0.0f, 0.0f, 0.0f ),
		glm::vec3( 0.0f, 1.0f, 0.0f ) );

	proj = glm::perspective( 
		glm::radians( 30.0f ),
		float( m_framebufferWidth ) / float( m_framebufferHeight ),
		1.0f, 1000.0f );
}

void A1::initGrid()
{
	size_t sz = 3 * 2 * 2 * (DIM+3);

	float *verts = new float[ sz ];
	size_t ct = 0;
	for( int idx = 0; idx < DIM+3; ++idx ) {
		verts[ ct ] = -1;
		verts[ ct+1 ] = 0;
		verts[ ct+2 ] = idx-1;
		verts[ ct+3 ] = DIM+1;
		verts[ ct+4 ] = 0;
		verts[ ct+5 ] = idx-1;
		ct += 6;

		verts[ ct ] = idx-1;
		verts[ ct+1 ] = 0;
		verts[ ct+2 ] = -1;
		verts[ ct+3 ] = idx-1;
		verts[ ct+4 ] = 0;
		verts[ ct+5 ] = DIM+1;
		ct += 6;
	}

	// Create the vertex array to record buffer assignments.
	glGenVertexArrays( 1, &m_grid_vao );
	glBindVertexArray( m_grid_vao );

	// Create the cube vertex buffer
	glGenBuffers( 1, &m_grid_vbo );
	glBindBuffer( GL_ARRAY_BUFFER, m_grid_vbo );
	glBufferData( GL_ARRAY_BUFFER, sz*sizeof(float),
		verts, GL_STATIC_DRAW );

	// Specify the means of extracting the position values properly.
	GLint posAttrib = m_shader.getAttribLocation( "position" );
	glEnableVertexAttribArray( posAttrib );
	glVertexAttribPointer( posAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr );

	// Reset state to prevent rogue code from messing with *my* 
	// stuff!
	glBindVertexArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	// OpenGL has the buffer now, there's no need for us to keep a copy.
	delete [] verts;

	CHECK_GL_ERRORS;
}

void A1::initCube() {

	float verts [] = {
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, cur_height, 0.0f,
		1.0f, cur_height, 0.0f,
		1.0f, cur_height, 1.0f,
		0.0f, cur_height, 1.0f
	};

	unsigned int index[] = {
		2, 3, 7,
		2, 6, 7,
		1, 2, 6,
		1, 5, 6,
		4, 5, 6,
		4, 7, 6,
		0, 1, 5,
		0, 4, 5,
		0, 3, 7,
		0, 4, 7,
		0, 1, 2,
		0, 3, 2
	};

	glGenVertexArrays( 1, &m_cube_vao );
	glBindVertexArray( m_cube_vao );

	glGenBuffers(1, &m_cube_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_cube_vbo);
	glBufferData(GL_ARRAY_BUFFER, 24*sizeof(float), verts, GL_STATIC_DRAW);

	glGenBuffers(1, &m_cube_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_cube_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(unsigned int), index, GL_STATIC_DRAW);

	// Specify the means of extracting the position values properly.
	GLint posAttrib = m_shader.getAttribLocation( "position" );
	glEnableVertexAttribArray( posAttrib );
	glVertexAttribPointer( posAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr );

	// Reset state to prevent rogue code from messing with *my* 
	// stuff!
	glBindVertexArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	CHECK_GL_ERRORS;
}

void A1::initAvatar()
{
	size_t sz = 6480;
	
	float *verts = new float[ sz ];
	for (int i = 0; i < 360; ++i) {
		verts[18 * i] = 0.5f;
		verts[18 * i + 1] = 0.0f;
		verts[18 * i + 2] = 0.5f;

		verts[18 * i + 3] = 0.5f + cos(i * PI / 180) / 2;
		verts[18 * i + 4] = 0.5f;
		verts[18 * i + 5] = 0.5f - sin(i * PI / 180) / 2;
		
		//i + 2 to avoid possible gaps between triangles;
		verts[18 * i + 6] = 0.5f + cos((i + 12)  * PI / 180) / 2;
		verts[18 * i + 7] = 0.5f;
		verts[18 * i + 8] = 0.5f - sin((i + 12)  * PI / 180) / 2;

		verts[18 * i + 9] = 0.5f;
		verts[18 * i + 10] = 1.0f;
		verts[18 * i + 11] = 0.5f;

		verts[18 * i + 12] = 0.5f + cos(i * PI / 180) / 2;
		verts[18 * i + 13] = 0.5f;
		verts[18 * i + 14] = 0.5f - sin(i * PI / 180) / 2;
		
		//i + 2 to avoid possible gaps between triangles;
		verts[18 * i + 15] = 0.5f + cos((i + 12) * PI / 180) / 2;
		verts[18 * i + 16] = 0.5f;
		verts[18 * i + 17] = 0.5f - sin((i + 12) * PI / 180) / 2;
		i += 17;
		
	}

	float verts2[] = {5.0f, 0.0f, 5.0f, 0.0f, 5.0f, 5.0f, 10.0f, 5.0f, 5.0f};

	// Create the vertex array to record buffer assignments.
	glGenVertexArrays( 1, &m_avatar_vao );
	glBindVertexArray( m_avatar_vao );

	// Create the cube vertex buffer
	glGenBuffers( 1, &m_avatar_vbo );
	glBindBuffer( GL_ARRAY_BUFFER, m_avatar_vbo );
	glBufferData( GL_ARRAY_BUFFER, sz*sizeof(float),
		verts, GL_STATIC_DRAW );

	// Specify the means of extracting the position values properly.
	GLint posAttrib = m_shader.getAttribLocation( "position" );
	glEnableVertexAttribArray( posAttrib );
	glVertexAttribPointer( posAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr );

	// Reset state to prevent rogue code from messing with *my* 
	// stuff!
	glBindVertexArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	// OpenGL has the buffer now, there's no need for us to keep a copy.
	delete [] verts;

	CHECK_GL_ERRORS;
}

void A1::initFloor() {

	float verts [] = {
		-1.0f, -0.1f, -1.0f,
		1.0f * m.getDim() + 1.0f, -0.1f, -1.0f,
		1.0f * m.getDim() + 1.0f, -0.1f, 1.0f * m.getDim() + 1.0f,
		-1.0f, -0.1f, -1.0f,
		-1.0f, -0.1f, 1.0f * m.getDim() + 1.0f,
		1.0f * m.getDim() + 1.0f, -0.1f, 1.0f * m.getDim() + 1.0f,
	};


	glGenVertexArrays( 1, &m_floor_vao );
	glBindVertexArray( m_floor_vao );

	glGenBuffers(1, &m_floor_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_floor_vbo);
	glBufferData(GL_ARRAY_BUFFER, 18*sizeof(float), verts, GL_STATIC_DRAW);

	// Specify the means of extracting the position values properly.
	GLint posAttrib = m_shader.getAttribLocation( "position" );
	glEnableVertexAttribArray( posAttrib );
	glVertexAttribPointer( posAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr );

	// Reset state to prevent rogue code from messing with *my* 
	// stuff!
	glBindVertexArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, before guiLogic().
 */
void A1::appLogic()
{
	// Place per frame, application logic here ...
	if (persistence) {
		m_shape_rotation += rotate_rate;
	}
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after appLogic(), but before the draw() method.
 */
void A1::guiLogic()
{
	// We already know there's only going to be one window, so for 
	// simplicity we'll store button states in static local variables.
	// If there was ever a possibility of having multiple instances of
	// A1 running simultaneously, this would break; you'd want to make
	// this into instance fields of A1.
	static bool showTestWindow(false);
	static bool showDebugWindow(true);

	ImGuiWindowFlags windowFlags(ImGuiWindowFlags_AlwaysAutoResize);
	float opacity(0.5f);

	ImGui::Begin("Debug Window", &showDebugWindow, ImVec2(100,100), opacity, windowFlags);
		if( ImGui::Button( "Quit Application" ) ) {
			glfwSetWindowShouldClose(m_window, GL_TRUE);
		}

		if( ImGui::Button( "Reset" ) ) {
			reset_parameters();
		}

		
		if ( PREV_ID != -1 ) {
			save_color(PREV_ID, colour);
		}
		if (ImGui::RadioButton("Blocks", &RB_ID, 1)) {
			if ( PREV_ID != RB_ID ) {
				load_color(RB_ID, colour);
				PREV_ID = RB_ID;
			}
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("Floors", &RB_ID, 2)) {
			if ( PREV_ID != RB_ID ) {
				load_color(RB_ID, colour);
				PREV_ID = RB_ID;
			}
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("Avatar", &RB_ID, 3)) {
			if ( PREV_ID != RB_ID ) {
				load_color(RB_ID, colour);
				PREV_ID = RB_ID;
			}
		}
		
		
		
		

		// Eventually you'll create multiple colour widgets with
		// radio buttons.  If you use PushID/PopID to give them all
		// unique IDs, then ImGui will be able to keep them separate.
		// This is unnecessary with a single colour selector and
		// radio button, but I'm leaving it in as an example.

		// Prefixing a widget name with "##" keeps it from being
		// displayed.

		ImGui::PushID( 0 );
		ImGui::ColorEdit3( "##Colour", colour );
		ImGui::SameLine();
		if( ImGui::RadioButton( "##Col", &current_col, 0 ) ) {
			// Select this colour.
			
		}
		ImGui::PopID();

		/*if ( PREV_ID != -1 ) {
			save_color(PREV_ID, colour);
			cout << "function called" <<endl;
		}*/
		

		// For convenience, you can uncomment this to show ImGui's massive
		// demonstration window right in your application.  Very handy for
		// browsing around to get the widget you want.  Then look in 
		// shared/imgui/imgui_demo.cpp to see how it's done.
		if( ImGui::Button( "Test Window" ) ) {
			showTestWindow = !showTestWindow;
		}


		ImGui::Text( "Framerate: %.1f FPS", ImGui::GetIO().Framerate );

	ImGui::End();

	if( showTestWindow ) {
		ImGui::ShowTestWindow( &showTestWindow );
	}
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after guiLogic().
 */
void A1::draw()
{
	// Create a global transformation for the model (centre it).
	mat4 W;
	vec3 y_axis(0.0f, 1.0f, 0.0f);
	W = glm::scale( W, vec3(scaling_para));
	W = glm::rotate( W, m_shape_rotation, y_axis);
	W = glm::translate( W, vec3( -float(DIM)/2.0f, 0, -float(DIM)/2.0f ) );
	
	m_shader.enable();
		glEnable( GL_DEPTH_TEST );

		glUniformMatrix4fv( P_uni, 1, GL_FALSE, value_ptr( proj ) );
		glUniformMatrix4fv( V_uni, 1, GL_FALSE, value_ptr( view ) );
		glUniformMatrix4fv( M_uni, 1, GL_FALSE, value_ptr( W ) );

		//draw the floor;
		glBindVertexArray( m_floor_vao );
		glUniform3f( col_uni, floor_color.r, floor_color.g, floor_color.b );
		glDrawArrays( GL_TRIANGLES, 0, 18);
		// Just draw the grid for now.
		glBindVertexArray( m_grid_vao );
		glUniform3f( col_uni, 1, 1, 1 );
		glDrawArrays( GL_LINES, 0, (3+DIM)*4 );

		// Draw the cubes
		mat4 temp = W;
		for (int i = 0; i < m.getDim(); ++i) {
			for (int j = 0; j < m.getDim(); ++j) {
				if ( m.getValue( i, j ) == 1 ) {
					temp = glm::translate(temp, vec3(i, 0, j));
					glUniformMatrix4fv( M_uni, 1, GL_FALSE, value_ptr( temp ) );
					glBindVertexArray( m_cube_vao );
					glUniform3f( col_uni, cube_color.r, cube_color.g, cube_color.b );
					glDrawElements( GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0 );
					temp = W;
				}
			}
		}
		// Draw the avatar
		temp = W;
		temp = glm::translate(temp, avatar);
		glUniformMatrix4fv( M_uni, 1, GL_FALSE, value_ptr( temp ) );
		glBindVertexArray( m_avatar_vao );
		glUniform3f( col_uni, avatar_color.r, avatar_color.g, avatar_color.b );
		glDrawArrays( GL_TRIANGLES, 0, 6480);
		// Highlight the active square.
	m_shader.disable();

	// Restore defaults
	glBindVertexArray( 0 );

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
/*
 * Called once, after program is signaled to terminate.
 */
void A1::cleanup()
{}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles cursor entering the window area events.
 */
bool A1::cursorEnterWindowEvent (
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
bool A1::mouseMoveEvent(double xPos, double yPos) 
{
	bool eventHandled(false);

	if (!ImGui::IsMouseHoveringAnyWindow()) {
		// Put some code here to handle rotations.  Probably need to
		// check whether we're *dragging*, not just moving the mouse.
		// Probably need some instance variables to track the current
		// rotation amount, and maybe the previous X position (so 
		// that you can rotate relative to the *change* in X.
		if ( ImGui::IsMouseDragging(0, 0.0f) && rotating ) {
			m_shape_rotation += (xPos - prev_x) / 64;
			rotate_rate = (xPos - prev_x) / 64;
		}
		prev_x = xPos;
	}
	eventHandled = true;

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse button events.
 */
bool A1::mouseButtonInputEvent(int button, int actions, int mods) {
	bool eventHandled(false);
	
	if (!ImGui::IsMouseHoveringAnyWindow()) {
		// The user clicked in the window.  If it's the left
		// mouse button, initiate a rotation.
		if( actions == GLFW_PRESS ) {
			rotate_rate = 0;
			persistence = false;
			rotating = true;
		}
		if( actions == GLFW_RELEASE ) {
			rotating = false;
			if ( rotate_rate != 0 ) {
				persistence = true;
			}
		}
	}
	eventHandled = true;
	return eventHandled;
}



//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse scroll wheel events.
 */
bool A1::mouseScrollEvent(double xOffSet, double yOffSet) {
	bool eventHandled(false);

	// Zoom in or out.
	if (yOffSet > 0 ) {
		scaling_para += yOffSet / 10;
	} else {
		scaling_para += yOffSet / 10;
	}

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles window resize events.
 */
bool A1::windowResizeEvent(int width, int height) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles key input events.
 */
bool A1::keyInputEvent(int key, int action, int mods) {
	bool eventHandled(false);

	// Fill in with event handling code...
	if( action == GLFW_PRESS ) {
		persistence = false;
		rotate_rate = 0;
		// Respond to some key events.
		if (key == GLFW_KEY_Q) {
			glfwSetWindowShouldClose(m_window, GL_TRUE);
		}
		if (key == GLFW_KEY_R) {
			reset_parameters();
			eventHandled = true;
		}
		if (key == GLFW_KEY_D) {
			m.digMaze();
			m.printMaze();
			for(int i = 0; i < m.getDim();  ++i) {
				if ( m.getValue(i, 0) == 0 ) {
					avatar = vec3(i, 0, 0);
				}
			}

			for(int j = 0; j < m.getDim();  ++j) {
				if ( m.getValue(0, j) == 0 ) {
					avatar = vec3(0, 0, j);
				}
			}
			initCube();
			eventHandled = true;
		}
		if (key == GLFW_KEY_BACKSPACE) {
			if ( cur_height > 0.5f ) {
				cur_height -= 0.1f;
				initCube();
				eventHandled = true;
			}
		}
		if ( key == GLFW_KEY_SPACE ) {
			if ( cur_height < 2.0f ) {
				cur_height += 0.1f;
				initCube();
				eventHandled = true;
			}
		}
		if ( key == GLFW_KEY_UP ) {
			if ( avatar.z >= 0 ) {
				if ( shift_key != -1 ) {
					m.setValue(avatar.x, avatar.z - 1, 0);
				}
				if ( m.getValue(avatar.x, avatar.z - 1) == 0 ) {
					avatar.z--;
				}
			} 
			eventHandled = true;
		}
		if ( key == GLFW_KEY_DOWN ) {
			if ( avatar.z <= m.getDim() ) {
				if ( shift_key != -1 ) {
					m.setValue(avatar.x, avatar.z + 1, 0);
				}
				if ( m.getValue(avatar.x, avatar.z + 1) == 0 ) {
					avatar.z++;
				}
			} 
			eventHandled = true;
		}
		if ( key == GLFW_KEY_LEFT ) {
			if ( avatar.x >= 0 ) {
				if ( shift_key != -1 ) {
					m.setValue(avatar.x - 1, avatar.z, 0);
				}
				if ( m.getValue(avatar.x - 1, avatar.z) == 0 ) {
					avatar.x--;
				}
			} 
			eventHandled = true;
		}

		if ( key == GLFW_KEY_RIGHT ) {
			if ( avatar.x <= m.getDim() ) {
				if ( shift_key != -1 ) {
					m.setValue(avatar.x + 1, avatar.z, 0);
				}
				if ( m.getValue(avatar.x + 1, avatar.z) == 0 ) {
					avatar.x++;
				}
			} 
			eventHandled = true;
		}
		if ( key == GLFW_KEY_LEFT_SHIFT ) {
			shift_key = GLFW_KEY_LEFT_SHIFT;
			eventHandled = true;
		}

		if ( key == GLFW_KEY_RIGHT_SHIFT ) {
			shift_key = GLFW_KEY_LEFT_SHIFT;
			eventHandled = true;
		}
	}

	if (action == GLFW_RELEASE) {
		if ( key == GLFW_KEY_LEFT_SHIFT ) {
			shift_key = -1;
			eventHandled = true;
		}

		if ( key == GLFW_KEY_RIGHT_SHIFT ) {
			shift_key = -1;
			eventHandled = true;
		}
	}
	
	return eventHandled;
}

void A1::load_color(int ID, float *colour) {
	glm::vec3 *temp;
	if ( ID == 1 ) {
		temp = &cube_color;
	} else if ( ID == 2 ) {
		temp = &floor_color;
	} else if ( ID == 3 ) {
		temp = &avatar_color;
	}
	colour[0] = temp->r;
	colour[1] = temp->g;
	colour[2] = temp->b;
	return;
};

void A1::save_color(int ID, float *colour) {
	glm::vec3 *temp;
	if ( ID == 1 ) {
		cube_color.r = colour[0];
		cube_color.g = colour[1];
		cube_color.b = colour[2];
	} else if ( ID == 2 ) {
		floor_color.r = colour[0];
		floor_color.g = colour[1];
		floor_color.b = colour[2];
	} else if ( ID == 3 ) {
		avatar_color.r = colour[0];
		avatar_color.g = colour[1];
		avatar_color.b = colour[2];
	}
	
	return;
};

void A1::reset_parameters() {
	m.reset();
	cur_height = 1.0f;
	avatar = {0.0f, 0.0f, 0.0f};
	cube_color = {1.0f, 0.0f, 1.0f};
	avatar_color= {0.0f, 1.0f, 0.0f};
	floor_color = {0.0f, 0.0f, 0.0f};
	colour[0] = 0.0f;
	colour[1] = 0.0f;
	colour[2] = 0.0f;
	RB_ID = -1;
	PREV_ID = -1;


	m_shape_rotation = 0.0f;
	rotate_rate = 0.0f;
	rotating = false;
	persistence = false;
	scaling_para = 1.0f;
}





