#include <vector>
#include <iostream>
#include <glad/glad.h>
#include <time.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "GLSL.h"
#include "Program.h"
#include "MatrixStack.h"

#include "WindowManager.h"
#include "Shape.h"
#include "Constants.h"
// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace std;
using namespace glm;
shared_ptr<Shape> sphere;
shared_ptr<Shape> asteroid_round;
shared_ptr<Shape> square;
vector<float> rand_angles;
vector<float> rand_floats;
float radius_factors [MAX_IND]; // for shadowing
float radius_factors_b[MAX_IND]; // for experimental shadowing
float directions [MAX_IND * 3]; // for shadowing
float cone_starts [MAX_IND]; // for shadowing
float speed_scale = 0.001;
double alpha = 0;

//double orbital_period[MAX_IND];
float orbital_speed[MAX_IND];
float orbital_radius[MAX_IND];
float radius[MAX_IND];
float day[MAX_IND];
int parent[MAX_IND];
int solar_order[MAX_IND] = { MERCURY_IND, VENUS_IND, MOON_IND, EARTH_IND, PHOBOS_IND, DEIMOS_IND, MARS_IND, IO_IND, EUROPA_IND, GANYMEDE_IND, 
CALLISTO_IND, JUPITER_IND, ENCELADUS_IND, TITAN_IND, IAPETUS_IND, SATURN_IND, URANUS_IND, NEPTUNE_IND };
int lockable_objects[10] = { VENUS_IND, EARTH_IND, MARS_IND, JUPITER_IND, SATURN_IND, MOON_IND, IO_IND, GANYMEDE_IND, TITAN_IND, COMET_IND };
int solar_levels[MAX_IND];
mat4 parent_M[MAX_IND];

void set_up_constants()
{
	//orbital_period[EARTH_IND] = EARTH_ORBITAL_PERIOD;
	orbital_speed[EARTH_IND] = EARTH_ORBITAL_SPEED;
	orbital_radius[EARTH_IND] = EARTH_ORBITAL_RADIUS;
	radius[EARTH_IND] = EARTH_RADIUS;
	day[EARTH_IND] = EARTH_DAY;
	parent[EARTH_IND] = -1;
	solar_levels[EARTH_IND] = 2;

	orbital_speed[MOON_IND] = MOON_ORBITAL_SPEED;
	orbital_radius[MOON_IND] = MOON_ORBITAL_RADIUS;
	radius[MOON_IND] = MOON_RADIUS;
	day[MOON_IND] = MOON_DAY;
	parent[MOON_IND] = EARTH_IND;
	solar_levels[MOON_IND] = 2;

	orbital_speed[MARS_IND] = MARS_ORBITAL_SPEED;
	orbital_radius[MARS_IND] = MARS_ORBITAL_RADIUS;
	radius[MARS_IND] = MARS_RADIUS;
	day[MARS_IND] = MARS_DAY;
	parent[MARS_IND] = -1;
	solar_levels[MARS_IND] = 3;

	orbital_speed[MERCURY_IND] = MERCURY_ORBITAL_SPEED;
	orbital_radius[MERCURY_IND] = MERCURY_ORBITAL_RADIUS;
	radius[MERCURY_IND] = MERCURY_RADIUS;
	day[MERCURY_IND] = MERCURY_DAY;
	parent[MERCURY_IND] = -1;
	solar_levels[MERCURY_IND] = 0;

	orbital_speed[VENUS_IND] = VENUS_ORBITAL_SPEED;
	orbital_radius[VENUS_IND] = VENUS_ORBITAL_RADIUS;
	radius[VENUS_IND] = VENUS_RADIUS;
	day[VENUS_IND] = VENUS_DAY;
	parent[VENUS_IND] = -1;
	solar_levels[VENUS_IND] = 1;

	orbital_speed[JUPITER_IND] = JUPITER_ORBITAL_SPEED;
	orbital_radius[JUPITER_IND] = JUPITER_ORBITAL_RADIUS;
	radius[JUPITER_IND] = JUPITER_RADIUS;
	day[JUPITER_IND] = JUPITER_DAY;
	parent[JUPITER_IND] = -1;
	solar_levels[JUPITER_IND] = 4;

	orbital_speed[SATURN_IND] = SATURN_ORBITAL_SPEED;
	orbital_radius[SATURN_IND] = SATURN_ORBITAL_RADIUS;
	radius[SATURN_IND] = SATURN_RADIUS;
	day[SATURN_IND] = SATURN_DAY;
	parent[SATURN_IND] = -1;
	solar_levels[SATURN_IND] = 5;

	orbital_speed[DEIMOS_IND] = DEIMOS_ORBITAL_SPEED;
	orbital_radius[DEIMOS_IND] = DEIMOS_ORBITAL_RADIUS;
	radius[DEIMOS_IND] = DEIMOS_RADIUS;
	day[DEIMOS_IND] = DEIMOS_DAY;
	parent[DEIMOS_IND] = MARS_IND;
	solar_levels[DEIMOS_IND] = 3;

	orbital_speed[PHOBOS_IND] = PHOBOS_ORBITAL_SPEED;
	orbital_radius[PHOBOS_IND] = PHOBOS_ORBITAL_RADIUS;
	radius[PHOBOS_IND] = PHOBOS_RADIUS;
	day[PHOBOS_IND] = PHOBOS_DAY;
	parent[PHOBOS_IND] = MARS_IND;
	solar_levels[PHOBOS_IND] = 3;

	orbital_speed[URANUS_IND] = URANUS_ORBITAL_SPEED;
	orbital_radius[URANUS_IND] = URANUS_ORBITAL_RADIUS;
	radius[URANUS_IND] = URANUS_RADIUS;
	day[URANUS_IND] = URANUS_DAY;
	parent[URANUS_IND] = -1;
	solar_levels[URANUS_IND] = 6;

	orbital_speed[NEPTUNE_IND] = NEPTUNE_ORBITAL_SPEED;
	orbital_radius[NEPTUNE_IND] = NEPTUNE_ORBITAL_RADIUS;
	radius[NEPTUNE_IND] = NEPTUNE_RADIUS;
	day[NEPTUNE_IND] = NEPTUNE_DAY;
	parent[NEPTUNE_IND] = -1;
	solar_levels[NEPTUNE_IND] = 7;

	orbital_speed[IO_IND] = IO_ORBITAL_SPEED;
	orbital_radius[IO_IND] = IO_ORBITAL_RADIUS;
	radius[IO_IND] = IO_RADIUS;
	day[IO_IND] = IO_DAY;
	parent[IO_IND] = JUPITER_IND;
	solar_levels[IO_IND] = 4;

	orbital_speed[EUROPA_IND] = EUROPA_ORBITAL_SPEED;
	orbital_radius[EUROPA_IND] = EUROPA_ORBITAL_RADIUS;
	radius[EUROPA_IND] = EUROPA_RADIUS;
	day[EUROPA_IND] = EUROPA_DAY;
	parent[EUROPA_IND] = JUPITER_IND;
	solar_levels[EUROPA_IND] = 4;

	orbital_speed[GANYMEDE_IND] = GANYMEDE_ORBITAL_SPEED;
	orbital_radius[GANYMEDE_IND] = GANYMEDE_ORBITAL_RADIUS;
	radius[GANYMEDE_IND] = GANYMEDE_RADIUS;
	day[GANYMEDE_IND] = GANYMEDE_DAY;
	parent[GANYMEDE_IND] = JUPITER_IND;
	solar_levels[GANYMEDE_IND] = 4;

	orbital_speed[CALLISTO_IND] = CALLISTO_ORBITAL_SPEED;
	orbital_radius[CALLISTO_IND] = CALLISTO_ORBITAL_RADIUS;
	radius[CALLISTO_IND] = CALLISTO_RADIUS;
	day[CALLISTO_IND] = CALLISTO_DAY;
	parent[CALLISTO_IND] = JUPITER_IND;
	solar_levels[CALLISTO_IND] = 4;

	orbital_speed[TITAN_IND] = TITAN_ORBITAL_SPEED;
	orbital_radius[TITAN_IND] = TITAN_ORBITAL_RADIUS;
	radius[TITAN_IND] = TITAN_RADIUS;
	day[TITAN_IND] = TITAN_DAY;
	parent[TITAN_IND] = SATURN_IND;
	solar_levels[TITAN_IND] = 5;

	orbital_speed[ENCELADUS_IND] = ENCELADUS_ORBITAL_SPEED;
	orbital_radius[ENCELADUS_IND] = ENCELADUS_ORBITAL_RADIUS;
	radius[ENCELADUS_IND] = ENCELADUS_RADIUS;
	day[ENCELADUS_IND] = ENCELADUS_DAY;
	parent[ENCELADUS_IND] = SATURN_IND;
	solar_levels[ENCELADUS_IND] = 5;

	orbital_speed[IAPETUS_IND] = IAPETUS_ORBITAL_SPEED;
	orbital_radius[IAPETUS_IND] = IAPETUS_ORBITAL_RADIUS;
	radius[IAPETUS_IND] = IAPETUS_RADIUS;
	day[IAPETUS_IND] = IAPETUS_DAY;
	parent[IAPETUS_IND] = SATURN_IND;
	solar_levels[IAPETUS_IND] = 5;
}

double get_last_elapsed_time()
{
	static double lasttime = glfwGetTime();
	double actualtime = glfwGetTime();
	double difference = actualtime - lasttime;
	lasttime = actualtime;
	return difference;
}

vec3 get_location(float orbital_radius, float orbital_speed)
{
	double operand = orbital_speed * (glfwGetTime() * speed_scale + alpha);
	//double operand = glfwGetTime() * speed_factor * speed_scale;
	return vec3(orbital_radius * sin(operand), 0, orbital_radius * cos(operand));
}

vec3 get_rand_location(float orbital_radius, float orbital_speed, int f)
{
	double operand = orbital_speed * (glfwGetTime() * speed_scale + alpha) + rand_angles.at(f);
	return vec3(orbital_radius * sin(operand), 0, orbital_radius * cos(operand));
}

float get_angle(unsigned int object_index)
{
	return glfwGetTime() * day[object_index] * speed_scale + alpha;
}

float get_angle_no_ind(float day_l)
{
	return glfwGetTime() * day_l * speed_scale + alpha;
}

void optimize_belt(int num)
{
	for (int j = 0; j < 9 * num; j += 9)
	{
		rand_floats.at(j) = (rand_floats.at(j) * 2 - 1) * 0.5;
		rand_floats.at(j + 1) = (rand_floats.at(j + 1) * 2 - 1) * 0.5;
		rand_floats.at(j + 2) = (rand_floats.at(j + 2) * 2 - 1) * 0.5;
		rand_floats.at(j + 3) = BELT_ORBITAL_RADIUS + (2 * (double)rand_floats.at(j + 3) - 1) * 0.25 * EARTH_ORBITAL_RADIUS;
		rand_floats.at(j + 4) = SPEED_SCALE / (BELT_ORBITAL_PERIOD + (3 * (double)rand_floats.at(j + 4) - 1.5) * 365);
		rand_floats.at(j + 5) = (2 * (double)rand_floats.at(j + 5) - 1) * 0.1 * EARTH_ORBITAL_RADIUS;
		rand_floats.at(j + 6) = 12.0f * BELT_RADIUS * (0.75 + rand_floats.at(j + 6) * 0.5);
		rand_floats.at(j + 7) = 12.0f * BELT_RADIUS * (0.75 + rand_floats.at(j + 7) * 0.5);
		rand_floats.at(j + 8) = 12.0f * BELT_RADIUS * (0.75 + rand_floats.at(j + 8) * 0.5);
	}
}

class camera
{
public:
	glm::vec3 pos, rot;
	int w, a, s, d, q, e, r, f, t, g, space, l_shift, l_control;
	int l, p;
	int equal, minus;
	bool v;
	int up, down;
	int num;
	camera()
	{
		w = a = s = d = space = l_shift = 0;
		l_control = 0;
		q = e = r = f = t = g = 0;
		l = p = 0;
		equal = minus = 0;
		up = down = 0;
		v = false;
		num = 0;
		rot = vec3(0, 0, 0);
		pos = vec3(0, 0, -30);
	}
	glm::mat4 process(double ftime)
	{
		static int mode = 0; // 0 = normal, 1 = locked
		static int index = 0;
		float speed = 0;
		float sidespeed = 0;
		float upspeed = 0;
		static float speed_val = 15.0;
		if (mode == 0)
		{
			if (w == 1)
			{
				speed = speed_val * ftime;
			}
			else if (s == 1)
			{
				speed = -speed_val * ftime;
			}
			if (a == 1)
			{
				sidespeed = speed_val * ftime;
			}
			else if (d == 1)
			{
				sidespeed = -speed_val * ftime;
			}
			if (space == 1)
			{
				upspeed = -speed_val * ftime;
			}
			else if (l_shift == 1)
			{
				upspeed = speed_val * ftime;
			}
		}
		else
		{
			static double r = 3;
			static double theta = 0;
			static double phi = PI / 2;
			if (w == 1)
			{
				r -= speed_val * 0.2 * ftime;
				if (r < 0)
					r = 0;
			}
			else if (s == 1)
				r += speed_val * 0.2 * ftime;
			if (a == 1)
				phi -= speed_val * 0.1 * ftime;
			else if (d == 1)
				phi += speed_val * 0.1 * ftime;
			if (space == 1)
				theta -= speed_val * 0.1 * ftime;
			else if (l_shift == 1)
				theta += speed_val * 0.1 * ftime;
			pos.x = r * sin(phi) * cos(theta);
			pos.y = r * sin(phi) * sin(theta);
			pos.z = r * cos(phi);
			if (num == 9)
				pos -= get_location(COMET_ORBITAL_RADIUS, COMET_ORBITAL_SPEED);
			else if (parent[lockable_objects[num]] == -1)
				pos -= get_location(orbital_radius[lockable_objects[num]], orbital_speed[lockable_objects[num]]);
			else
				pos -= get_location(orbital_radius[parent[lockable_objects[num]]], orbital_speed[parent[lockable_objects[num]]])
				+ get_location(orbital_radius[lockable_objects[num]], orbital_speed[lockable_objects[num]]);
		}

		if (equal == 1)
		{
			speed_val += 1;
		}
		else if (minus == 1)
		{
			speed_val -= 1;
			if (speed_val <= 0)
				speed_val = 1;
		}

		if (up == 1)
			speed_scale *= 1.1;
		else if (down == 1)
			speed_scale /= 1.1;

		if (l == 1)
			mode = 1;

		if (p == 1)
			mode = 0;

		float xangle = 0;
		if (r == 1)
			xangle = -2 * ftime;
		else if (f == 1)
			xangle = 2 * ftime;
		rot.x += xangle;
		
		float yangle = 0;
		if (q == 1)
			yangle = -2*ftime;
		else if(e==1)
			yangle = 2*ftime;
		rot.y += yangle;

		float zangle = 0;
		if (t == 1)
			zangle = -2 * ftime;
		else if (g == 1)
			zangle = 2 * ftime;
		rot.z += zangle;
		
		if (l_control == 1)
			rot = vec3(0, rot.y, 0);
		
		mat4 Rx = rotate(mat4(1), rot.x, vec3(1, 0, 0));
		mat4 Ry = rotate(mat4(1), rot.y, vec3(0, 1, 0));
		mat4 Rz = rotate(mat4(1), rot.z, vec3(0, 0, 1));
		vec4 dir = vec4(sidespeed, upspeed, speed,1);
		dir = dir * Rz * Ry * Rx;
		sidespeed = upspeed = speed = 0;
		pos += vec3(dir.x, dir.y, dir.z);
		mat4 T = translate(mat4(1), pos);
		return Rz * Ry * Rx * T;
	}
};

camera mycam;

class Application : public EventCallbacks
{

public:

	WindowManager * windowManager = nullptr;

	// Our shader program
	std::shared_ptr<Program> prog;

	// Contains vertex information for OpenGL
	GLuint VertexArrayID;

	// Data necessary to give our box to OpenGL
	GLuint VertexBufferID, VertexNormDBox, VertexTexBox, IndexBufferIDBox, InstanceBuffer;

	//texture data
	GLuint Textures[32];

	void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
		
		if (key == GLFW_KEY_W && action == GLFW_PRESS)
			mycam.w = 1;
		if (key == GLFW_KEY_W && action == GLFW_RELEASE)
			mycam.w = 0;

		if (key == GLFW_KEY_A && action == GLFW_PRESS)
			mycam.a = 1;
		if (key == GLFW_KEY_A && action == GLFW_RELEASE)
			mycam.a = 0;

		if (key == GLFW_KEY_S && action == GLFW_PRESS)
			mycam.s = 1;
		if (key == GLFW_KEY_S && action == GLFW_RELEASE)
			mycam.s = 0;

		if (key == GLFW_KEY_D && action == GLFW_PRESS)
			mycam.d = 1;
		if (key == GLFW_KEY_D && action == GLFW_RELEASE)
			mycam.d = 0;

		if (key == GLFW_KEY_Q && action == GLFW_PRESS)
			mycam.q = 1;
		if (key == GLFW_KEY_Q && action == GLFW_RELEASE)
			mycam.q = 0;

		if (key == GLFW_KEY_E && action == GLFW_PRESS)
			mycam.e = 1;
		if (key == GLFW_KEY_E && action == GLFW_RELEASE)
			mycam.e = 0;

		if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
			mycam.space = 1;
		if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
			mycam.space = 0;

		if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
			mycam.l_shift = 1;
		if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
			mycam.l_shift = 0;

		if (key == GLFW_KEY_R && action == GLFW_PRESS)
			mycam.r = 1;
		if (key == GLFW_KEY_R && action == GLFW_RELEASE)
			mycam.r = 0;

		if (key == GLFW_KEY_F && action == GLFW_PRESS)
			mycam.f = 1;
		if (key == GLFW_KEY_F && action == GLFW_RELEASE)
			mycam.f = 0;

		if (key == GLFW_KEY_T && action == GLFW_PRESS)
			mycam.t = 1;
		if (key == GLFW_KEY_T && action == GLFW_RELEASE)
			mycam.t = 0;

		if (key == GLFW_KEY_G && action == GLFW_PRESS)
			mycam.g = 1;
		if (key == GLFW_KEY_G && action == GLFW_RELEASE)
			mycam.g = 0;

		if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS)
			mycam.l_control = 1;
		if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_RELEASE)
			mycam.l_control = 0;

		if (key == GLFW_KEY_L && action == GLFW_PRESS)
		{
			mycam.l = 1;
			mycam.p = 0;
		}
		if (key == GLFW_KEY_P && action == GLFW_PRESS)
		{
			mycam.p = 1;
			mycam.l = 0;
		}

		if (key == GLFW_KEY_MINUS && action == GLFW_PRESS)
			mycam.minus = 1;
		if (key == GLFW_KEY_MINUS && action == GLFW_RELEASE)
			mycam.minus = 0;

		if (key == GLFW_KEY_EQUAL && action == GLFW_PRESS)
			mycam.equal = 1;
		if (key == GLFW_KEY_EQUAL && action == GLFW_RELEASE)
			mycam.equal = 0;

		if (key == GLFW_KEY_UP && action == GLFW_PRESS)
			mycam.up = 1;
		if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
			mycam.up = 0;

		if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
			mycam.down = 1;
		if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
			mycam.down = 0;

		if (key == GLFW_KEY_V && action == GLFW_PRESS)
			mycam.v = !mycam.v;

		if (key == GLFW_KEY_1 && action == GLFW_PRESS)
			mycam.num = 1;
		if (key == GLFW_KEY_2 && action == GLFW_PRESS)
			mycam.num = 2;
		if (key == GLFW_KEY_3 && action == GLFW_PRESS)
			mycam.num = 3;
		if (key == GLFW_KEY_4 && action == GLFW_PRESS)
			mycam.num = 4;
		if (key == GLFW_KEY_5 && action == GLFW_PRESS)
			mycam.num = 5;
		if (key == GLFW_KEY_6 && action == GLFW_PRESS)
			mycam.num = 6;
		if (key == GLFW_KEY_7 && action == GLFW_PRESS)
			mycam.num = 7;
		if (key == GLFW_KEY_8 && action == GLFW_PRESS)
			mycam.num = 8;
		if (key == GLFW_KEY_9 && action == GLFW_PRESS)
			mycam.num = 9;
		if (key == GLFW_KEY_0 && action == GLFW_PRESS)
			mycam.num = 0;
	}

	// callback for the mouse when clicked move the triangle when helper functions
	// written
	void mouseCallback(GLFWwindow* window, int button, int action, int mods)
	{
		double posX, posY;
		float newPt[2];
		if (action == GLFW_PRESS)
		{
			glfwGetCursorPos(window, &posX, &posY);
			std::cout << "Pos X " << posX << " Pos Y " << posY << std::endl;

			//change this to be the points converted to WORLD
			//THIS IS BROKEN< YOU GET TO FIX IT - yay!
			newPt[0] = 0;
			newPt[1] = 0;

			std::cout << "converted:" << newPt[0] << " " << newPt[1] << std::endl;
			glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
			//update the vertex array with the updated points
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * 6, sizeof(float) * 2, newPt);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	}

	//if the window is resized, capture the new size and reset the viewport
	void resizeCallback(GLFWwindow *window, int in_width, int in_height)
	{
		//get the window size - may be different then pixels for retina
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
	}

	void setup_texture(string resourceDirectory, string resolution_repo, string filename, GLuint texture, uint gl_tex_num, string text_name, int location)
	{
		int width, height, channels;
		char filepath[1000];
		string str = resourceDirectory + resolution_repo + filename;
		strcpy(filepath, str.c_str());
		unsigned char* data = stbi_load(filepath, &width, &height, &channels, 4);
		glGenTextures(1, &texture);
		glActiveTexture(gl_tex_num);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glUniform1i(glGetUniformLocation(prog->pid, text_name.c_str()), location);
	}

	/*Note that any gl calls must always happen after a GL state is initialized */
	void initGeom()
	{
		string resourceDirectory = "../resources";
		string resolution_repo = "/low_res"; // low_res or high_res
		// Initialize mesh.
		sphere = make_shared<Shape>();
		sphere->loadMesh(resourceDirectory + "/sphere.obj");
		sphere->resize();
		sphere->init();

		asteroid_round = make_shared<Shape>();
		asteroid_round->loadMesh(resourceDirectory + "/asteroid_round.obj");
		asteroid_round->resize();
		asteroid_round->init();

		square = make_shared<Shape>();
		square->loadMesh(resourceDirectory + "/square.obj");
		square->resize();
		square->init();

		glUseProgram(prog->pid);
		vector<string> filenames = { "/8k_stars_milky_way.jpg", "/2k_earth_daymap.jpg", "/2k_earth_specular_map.jpg", "/2k_earth_nightmap.jpg", 
			"/2k_earth_clouds.jpg", "/2k_earth_normal_map.jpg", "/2k_moon.jpg", "/2k_mars.jpg", "/2k_mercury.jpg", "/2k_venus_surface.jpg", 
			"/2k_venus_atmosphere.jpg", "/2k_sun.jpg", "/2k_jupiter.jpg", "/2k_saturn.jpg", "/saturnring1.jpg", "/asteroid_round.jpg", 
			"/2k_uranus.jpg", "/2k_neptune.jpg", "/red_circle.png", "/io.jpg", "/europa.jpg", "/ganymede.jpg", "/callisto.png", "/titan.png", 
			"/enceladus.jpg", "/iapetus.jpg", "/comet.png" };
		vector<string> tex_names = { "space", "earth_day", "earth_spec", "earth_night", "earth_clouds", "earth_normal", "moon", "mars",
			"mercury", "venus_ground", "venus_sky", "sun", "jupiter", "saturn", "saturn_rings", "asteroid_round", "uranus", "neptune", "circle", 
			"io", "europa", "ganymede", "callisto", "titan", "enceladus", "iapetus", "comet" };
		setup_texture(resourceDirectory, "/high_res", filenames.at(0), Textures[0], GL_TEXTURE0, tex_names.at(0), 0);
		for (unsigned int i = 1; i < filenames.size(); i++)
		{
			setup_texture(resourceDirectory, resolution_repo, filenames.at(i), Textures[i], GL_TEXTURE0 + i, tex_names.at(i), i);
		}
	}

	//General OGL initialization - set OGL state here
	void init(const std::string& resourceDirectory)
	{
		GLSL::checkVersion();

		// Set background color.
		glClearColor(0.7f, 0.1f, 0.1f, 1.0f);
		// Enable z-buffer test.
		glEnable(GL_DEPTH_TEST);

		// Initialize the GLSL program.
		prog = std::make_shared<Program>();
		prog->setVerbose(true);
		prog->setShaderNames(resourceDirectory + "/shader_vertex.glsl", resourceDirectory + "/shader_fragment.glsl");
		if (!prog->init())
		{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
			exit(1);
		}
		prog->addUniform("P");
		prog->addUniform("V");
		prog->addUniform("M");
		prog->addUniform("campos");
		prog->addUniform("time");
		prog->addUniform("isClouds");
		prog->addUniform("object_index");
		prog->addUniform("radius"); // only needed for earth
		prog->addUniform("center"); // only needed for earth
		prog->addUniform("radius_factors"); // for shadowing
		prog->addUniform("radius_factors_b"); // for experimental shadowing
		prog->addUniform("directions"); // for shadowing
		prog->addUniform("cone_starts"); // for shadowing
		prog->addUniform("solar_levels"); // for shadowing
		prog->addAttribute("vertPos");
		prog->addAttribute("vertNor");
		prog->addAttribute("vertTex");

		srand(time(0));
		for (int i = 0; i < 9000; i++) // make sure there are enough random values to use
			rand_angles.push_back(2 * PI * rand() / RAND_MAX);
		for (int i = 0; i < 10000; i++) // make sure there are enough random values to use
			rand_floats.push_back(((float)(rand())) / ((float)RAND_MAX));

		// shadowing
		vec3 loc;
		double o_r;
		for (int i = 0; i < MAX_IND; i++)
		{
			if (parent[i] == -1)
			{
				loc = normalize(get_location(orbital_radius[i], orbital_speed[i]));
				cone_starts[i] = orbital_radius[i] + radius[i] * 1.01625;
			}
			else
			{
				loc = normalize(get_location(orbital_radius[parent[i]], orbital_speed[parent[i]]) 
					+ get_location(orbital_radius[i], orbital_speed[i]));
				cone_starts[i] = distance(vec3(0, 0, 0), get_location(orbital_radius[parent[i]], orbital_speed[parent[i]]) 
					+ get_location(orbital_radius[i], orbital_speed[i])) + radius[i];
			}
			directions[i * 3 + 0] = loc.x;
			directions[i * 3 + 1] = loc.y;
			directions[i * 3 + 2] = loc.z;

			if (parent[i] != -1)
			{
				loc = vec3(0, 0, 1);
				o_r = distance(vec3(0, 0, 0), get_location(orbital_radius[parent[i]], orbital_speed[parent[i]]) + get_location(orbital_radius[i], orbital_speed[i]));
			}
			else
			{
				loc = vec3(0, 0, 1);
				o_r = orbital_radius[i];
			}
			double r = radius[i];
			double val = o_r * o_r - r * r;
			double tangent_distance = sqrt(val);
			double phi = acos(val / (tangent_distance * o_r));
			double z1 = tangent_distance * cos(phi);
			double x1 = tangent_distance * sin(phi);

			double x = 1.0f;
			double y = tangent_distance;
			double z = distance(loc, vec3(x1, 0, z1));
			double s = (y * y - x * x - z * z) / (2 * x);
			double dist = sqrt(z * z - s * s); // shortest distance from point to line - this is the radius of the cone!!!

			double lambda = dist / z1;
			radius_factors[i] = -lambda;
			radius_factors_b[i] = x1 + lambda * z1;
		}
	}

	void send_common_uniforms(mat4 P, mat4 V)
	{
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, &P[0][0]);
		glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, &V[0][0]);
		glUniform3fv(prog->getUniform("campos"), 1, &mycam.pos[0]);
		glUniform1f(prog->getUniform("time"), glfwGetTime());
		glUniform1iv(prog->getUniform("solar_levels"), MAX_IND, &solar_levels[0]);
		// shadowing
		vec3 loc;
		double o_r;
		for (int i = 0; i < MAX_IND; i++)
		{
			if (parent[i] == -1)
			{
				loc = normalize(get_location(orbital_radius[i], orbital_speed[i]));
			}
			else
			{
				loc = normalize(get_location(orbital_radius[parent[i]], orbital_speed[parent[i]])
					+ get_location(orbital_radius[i], orbital_speed[i]));
				cone_starts[i] = distance(vec3(0, 0, 0), get_location(orbital_radius[parent[i]], orbital_speed[parent[i]])
					+ get_location(orbital_radius[i], orbital_speed[i])) + radius[i];
			}
			directions[i * 3 + 0] = loc.x;
			directions[i * 3 + 1] = loc.y;
			directions[i * 3 + 2] = loc.z;

			if (parent[i] != -1)
			{
				loc = vec3(0, 0, 1);
				o_r = distance(vec3(0, 0, 0), get_location(orbital_radius[parent[i]], orbital_speed[parent[i]]) 
					+ get_location(orbital_radius[i], orbital_speed[i]));
				//o_r = 2 * orbital_radius[i];
				double r = radius[i];
				double val = o_r * o_r - r * r;
				double tangent_distance = sqrt(val);
				double phi = acos(val / (tangent_distance * o_r));
				double z1 = tangent_distance * cos(phi);
				double x1 = tangent_distance * sin(phi);

				double x = 1.0f;
				double y = tangent_distance;
				double z = distance(loc, vec3(x1, 0, z1));
				double s = (y * y - x * x - z * z) / (2 * x);
				double dist = sqrt(z * z - s * s); // shortest distance from point to line - this is the radius of the cone!!!

				double lambda = dist / z1;
				radius_factors[i] = -lambda;
				radius_factors_b[i] = x1 + lambda * z1;
			}
		}
		glUniform1fv(prog->getUniform("radius_factors"), MAX_IND, &radius_factors[0]);
		glUniform1fv(prog->getUniform("radius_factors_b"), MAX_IND, &radius_factors_b[0]);
		glUniform1fv(prog->getUniform("directions"), MAX_IND * 3, &directions[0]);
		glUniform1fv(prog->getUniform("cone_starts"), MAX_IND, &cone_starts[0]);
	}

	mat4 render_solid(int atmos, unsigned int object_index)
	{
		float w = get_angle(object_index);
		float scale_factor = radius[object_index];
		mat4 Mp;
		if (parent[object_index] == -1)
			Mp = mat4(1);
		else
			Mp = parent_M[parent[object_index]];
		vec3 location = get_location(orbital_radius[object_index], orbital_speed[object_index]);
		mat4 S = scale(mat4(1), scale_factor * vec3(1, 1, 1));
		mat4 Ry = rotate(mat4(1), w, vec3(0, 1, 0));
		mat4 Rx = rotate(mat4(1), -PI / 2.0f, vec3(1, 0, 0));
		//mat4 Rt = rotate(mat4(1), 0.13f * PI, vec3(1, 0, 0)); // axial tilt
		mat4 T = translate(mat4(1), location);
		//mat4 Mm = Mp * T * Rt * Ry * Rx; // axial tilt
		mat4 Mm = Mp * T * Ry * Rx;
		mat4 M = Mm * S;
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
		glUniform1i(prog->getUniform("object_index"), object_index);
		if (atmos == 1)
		{
			glUniform1i(prog->getUniform("isClouds"), 0);
			if (object_index == EARTH_IND)
			{
				glUniform1f(prog->getUniform("radius"), scale_factor);
				glUniform3fv(prog->getUniform("center"), 1, &location[0]);
			}
			sphere->draw(prog, false);

			scale_factor *= 1.01625;
			mat4 S = scale(mat4(1), scale_factor * vec3(1, 1, 1));
			M = Mm * S;
			glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
			glUniform1i(prog->getUniform("isClouds"), 1);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			sphere->draw(prog, false);
		}
		else
			sphere->draw(prog, false);
		return T; // axial tilt ruins the way shadows are made by moons
	}

	mat4 render_gas(int rings, unsigned int object_index)
	{
		float w = get_angle(object_index);
		float scale_factor = radius[object_index];
		mat4 Mp;
		if (parent[object_index] == -1)
			Mp = mat4(1);
		else
			Mp = parent_M[parent[object_index]];
		vec3 location = get_location(orbital_radius[object_index], orbital_speed[object_index]);
		mat4 S = scale(mat4(1), scale_factor * vec3(1, 1, 1));
		mat4 Ry = rotate(mat4(1), w, vec3(0, 1, 0));
		mat4 Rx = rotate(mat4(1), -PI / 2.0f, vec3(1, 0, 0));
		mat4 T = translate(mat4(1), location);
		mat4 M = Mp * T * Ry * Rx * S;
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
		glUniform1i(prog->getUniform("object_index"), object_index);
		glUniform1i(prog->getUniform("isClouds"), 0); // for rings
		if (rings == 1)
		{
			sphere->draw(prog, false);
			scale_factor *= SATURN_RING_SCALE_FACTOR * 2;
			mat4 S = scale(mat4(1), scale_factor * vec3(1, 1, 1));
			M = Mp * T * Rx * S;
			glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
			glUniform1i(prog->getUniform("isClouds"), 1); // for rings
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			square->draw(prog, false);
		}
		else
			sphere->draw(prog, false);
		return T;
	}

	mat4 render_asteroid(unsigned int object_index, int f)
	{
		vec3 rand_rot = rand_gen(f);
		float w = get_angle(object_index);
		mat4 Mp;
		if (parent[object_index] == -1)
			Mp = mat4(1);
		else
			Mp = parent_M[parent[object_index]];
		vec3 location = get_location(orbital_radius[object_index], orbital_speed[object_index]);
		mat4 S = scale(mat4(1), radius[object_index] * vec3(1, 1, 1));
		mat4 Rr = rotate(mat4(1), w, vec3(0, 1, 0));
		mat4 Rx = rotate(mat4(1), rand_rot.x, vec3(1, 0, 0));
		mat4 Ry = rotate(mat4(1), rand_rot.y, vec3(0, 1, 0));
		mat4 Rz = rotate(mat4(1), rand_rot.z, vec3(0, 0, 1));
		mat4 T = translate(mat4(1), location);
		mat4 M = Mp * T * Rr * Rx * Ry * Rz * S;
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
		glUniform1i(prog->getUniform("object_index"), object_index);
		asteroid_round->draw(prog, false);
		return T;
	}

	void render_sun()
	{
		float w = glfwGetTime() * SUN_DAY * speed_scale + alpha;
		float scale_factor = SUN_RADIUS;
		mat4 Ry = rotate(mat4(1), w, vec3(0, 1, 0));
		mat4 Rx = rotate(mat4(1), 3.1415926f / 2.0f, vec3(1, 0, 0));
		mat4 S = scale(mat4(1), scale_factor * vec3(1, 1, 1));
		mat4 M = Ry * Rx * S;
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
		glUniform1i(prog->getUniform("object_index"), SUN_IND);
		sphere->draw(prog, false);
	}

	void render_space() // sky sphere
	{
		glDisable(GL_DEPTH_TEST);
		mat4 T = translate(mat4(1), vec3(-mycam.pos.x, -mycam.pos.y, -mycam.pos.z));
		mat4 Rx = rotate(mat4(1), 3.1415926f / 2.0f, vec3(1, 0, 0));
		mat4 S = scale(mat4(1), 1.0f * vec3(1, 1, 1));
		mat4 M = T * Rx * S;
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
		glUniform1i(prog->getUniform("object_index"), SPACE_IND);
		sphere->draw(prog, false);
		glEnable(GL_DEPTH_TEST);
	}

	void render_orbit_vis(unsigned int object_index, mat4 Mp)
	{
		glUniform1i(prog->getUniform("object_index"), CIRCLE_IND);
		mat4 S = scale(mat4(1), (orbital_radius[object_index] + radius[object_index] / 2) * vec3(1, 1, 1));
		mat4 Rx = rotate(mat4(1), -PI / 2.0f, vec3(1, 0, 0));
		mat4 M = Mp * Rx * S;
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		square->draw(prog, false);
	}

	int render_optimized_belt(int num)
	{
		glUniform1i(prog->getUniform("object_index"), BELT_IND);
		int f = 0; // for angles
		int j = 0; // for floats
		int k = 0; // for ints
		for (int i = 0; i < num; i++)
		{
			vec3 rand_rot = rand_gen(f += 3);
			float calc = glfwGetTime() * BELT_DAY * speed_scale + alpha;
			float wx = calc * rand_floats.at(j);
			float wy = calc * rand_floats.at(j + 1);
			float wz = calc * rand_floats.at(j + 2);
			vec3 location = get_rand_location(rand_floats.at(j + 3), rand_floats.at(j + 4), f ++); // noise
			location += vec3(0, rand_floats.at(j + 5), 0);
			vec3 sc;
			sc.x = rand_floats.at(j + 6);
			sc.y = rand_floats.at(j + 7);
			sc.z = rand_floats.at(j + 8);
			mat4 S = scale(mat4(1), sc); // noise
			mat4 Rrx = rotate(mat4(1), wx, vec3(1, 0, 0));
			mat4 Rry = rotate(mat4(1), wy, vec3(0, 1, 0));
			mat4 Rrz = rotate(mat4(1), wz, vec3(0, 0, 1));
			mat4 Rx = rotate(mat4(1), rand_rot.x, vec3(1, 0, 0));
			mat4 Ry = rotate(mat4(1), rand_rot.y, vec3(0, 1, 0));
			mat4 Rz = rotate(mat4(1), rand_rot.z, vec3(0, 0, 1));
			mat4 T = translate(mat4(1), location);
			mat4 M = T * Rrx * Rry * Rrz * Rx * Ry * Rz * S;
			glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
			asteroid_round->draw(prog, false);
			j += 9;
		}
		return f;
	}

	void render_comet(int f)
	{
		glUniform1i(prog->getUniform("object_index"), COMET_IND);
		vec3 rand_rot = rand_gen(f);
		float w = get_angle_no_ind(COMET_DAY);
		vec3 location = get_location(COMET_ORBITAL_RADIUS, COMET_ORBITAL_SPEED);
		float scale_factor = COMET_RADIUS;
		mat4 S = scale(mat4(1), scale_factor * vec3(1, 1, 1));
		mat4 Rr = rotate(mat4(1), w, vec3(0, 1, 0));
		mat4 Rx = rotate(mat4(1), rand_rot.x, vec3(1, 0, 0));
		mat4 Ry = rotate(mat4(1), rand_rot.y, vec3(0, 1, 0));
		mat4 Rz = rotate(mat4(1), rand_rot.z, vec3(0, 0, 1));
		mat4 T = translate(mat4(1), location);
		mat4 M = T * Rr * Rx * Ry * Rz * S;
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
		glUniform1i(prog->getUniform("isClouds"), 0); // for shape
		asteroid_round->draw(prog, false);
		S = scale(mat4(1), scale_factor * 15 * vec3(1, 1, 1));
		float angle = COMET_ORBITAL_SPEED * (glfwGetTime() * speed_scale + alpha);
		Rr = rotate(mat4(1), angle, vec3(0, 1, 0));
		M = T * Rr * S;
		glUniform1i(prog->getUniform("isClouds"), 1); // for trail
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
		glUniform3fv(prog->getUniform("center"), 1, &location[0]);
		square->draw(prog, false);

		Rx = rotate(mat4(1), PI / 2, vec3(1, 0, 0));
		M = T * Rr * Rx * S;
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
		square->draw(prog, false);

		Rx = rotate(mat4(1), PI / 4, vec3(1, 0, 0));
		M = T * Rr * Rx * S;
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
		square->draw(prog, false);

		Rx = rotate(mat4(1), -PI / 4, vec3(1, 0, 0));
		M = T * Rr * Rx * S;
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
		square->draw(prog, false);
	}

	vec3 rand_gen(int i)
	{
		return vec3(rand_angles.at(i), rand_angles.at(i + 1), rand_angles.at(i + 2));
	}

	void render()
	{
		int f = 0;
		alpha += glfwGetTime() * speed_scale; // this is bugged out long term

		// Get current frame buffer size.
		int width, height;
		glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
		float aspect = width/(float)height;
		glViewport(0, 0, width, height);

		// Clear framebuffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glm::mat4 V, P; //View, Model and Perspective matrix
		V = mycam.process(get_last_elapsed_time());
		// Apply orthographic projection....
		P = glm::perspective((float)(3.14159 / 4.), (float)((float)width/ (float)height), 0.1f, 1000.0f); //so much type casting... GLM methods are quite funny ones

		// Draw the sphere using GLSL.
		prog->bind();
		send_common_uniforms(P, V);
		render_space();
		render_sun();
		f = render_optimized_belt(NUM_ASTEROIDS);

		parent_M[MERCURY_IND] = render_solid(0, MERCURY_IND);
		parent_M[VENUS_IND] = render_solid(1, VENUS_IND);
		parent_M[EARTH_IND] = render_solid(1, EARTH_IND);
			parent_M[MOON_IND] = render_solid(0, MOON_IND);
		parent_M[MARS_IND] = render_solid(0, MARS_IND);
			parent_M[DEIMOS_IND] = render_asteroid(DEIMOS_IND, f += 3);
			parent_M[PHOBOS_IND] = render_asteroid(PHOBOS_IND, f += 3);
		parent_M[JUPITER_IND] = render_gas(0, JUPITER_IND);
			parent_M[IO_IND] = render_solid(0, IO_IND);
			parent_M[EUROPA_IND] = render_solid(0, EUROPA_IND);
			parent_M[GANYMEDE_IND] = render_solid(0, GANYMEDE_IND);
			parent_M[CALLISTO_IND] = render_solid(0, CALLISTO_IND);
		parent_M[SATURN_IND] = render_gas(1, SATURN_IND);
			parent_M[TITAN_IND] = render_solid(0, TITAN_IND);
			parent_M[ENCELADUS_IND] = render_solid(0, ENCELADUS_IND);
			parent_M[IAPETUS_IND] = render_solid(0, IAPETUS_IND);
		parent_M[URANUS_IND] = render_gas(0, URANUS_IND);
		parent_M[NEPTUNE_IND] = render_gas(0, NEPTUNE_IND);

		// orbit visualization
		if (mycam.v == true)
		{
			for (int i = 0; i < MAX_IND; i++)
			{
				unsigned int obj_ind = solar_order[i];
				if (parent[obj_ind] == -1)
					render_orbit_vis(obj_ind, mat4(1));
				else
					render_orbit_vis(obj_ind, parent_M[parent[obj_ind]]);
			}
		}

		render_comet(f += 3);

		prog->unbind();
	}

};
//******************************************************************************************
int main(int argc, char **argv)
{
	set_up_constants();

	std::string resourceDir = "../resources"; // Where the resources are loaded from
	if (argc >= 2)
	{
		resourceDir = argv[1];
	}

	Application *application = new Application();

	/* your main will always include a similar set up to establish your window
		and GL context, etc. */
	WindowManager * windowManager = new WindowManager();
	windowManager->init(1920, 1080);
	windowManager->setEventCallbacks(application);
	application->windowManager = windowManager;

	/* This is the code that will likely change program to program as you
		may need to initialize or set up different data and state */
	// Initialize scene.
	application->init(resourceDir);
	application->initGeom();

	optimize_belt(NUM_ASTEROIDS);

	// Loop until the user closes the window.
	while(! glfwWindowShouldClose(windowManager->getHandle()))
	{
		// Render scene.
		application->render();

		// Swap front and back buffers.
		glfwSwapBuffers(windowManager->getHandle());
		// Poll for and process events.
		glfwPollEvents();
	}

	// Quit program.
	windowManager->shutdown();
	return 0;
}

// DONE
	// Shadowing for objects behind other objects
	// make planets rotate different directions - done
	// toggle visualization of rotations - this is done unless I make orbits elliptical
	// camera speed control
	// add new planets - no more to add
	// time speed controls
	// asteroids
	// advanced camera controls - camera object tracking
	// optimize asteroid random value calculations
	// optimize shadow calculations: make it so only moon shadows must be recalculated; make it so objects only check if they are in the shadows of things that come before them in solar order. This will save time and computational power.
	// comets

// TO DO
	// add more moons
	// dwarf planets

// WON'T DO BEFORE DEADLINE
	// make all orbits elliptical
	// put in music... if super ambitious put in music from the planets when you are close to a planet
	// add the normals for earth
	// kuiper belt
	// rework EVERYTHING so all planets and objects (sun included) orbit around the center of mass of the solar system... 
		// this involves changing how shadowing works as well
	// make it so ANY object can be locked on to... will need some new method
	// make clouds dynamic for earth and venus?
	// make another cloud layer for earth?
	// Alpha is bugged out in the long term
	// axial tilts
	// clean up fragment shader
	// fix shadows
	// do rings of jupiter, uranus, and neptune
	// comets seem to hide pixels in a wide radius around them
