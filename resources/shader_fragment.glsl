#version 330 core

#define SPACE_IND -1
#define CIRCLE_IND -2
#define SUN_IND -3
#define BELT_IND -4
#define COMET_IND -5

#define EARTH_IND 0
#define MOON_IND 1
#define MARS_IND 3
#define MERCURY_IND 5
#define VENUS_IND 6
#define JUPITER_IND 7
#define SATURN_IND 8
#define DEIMOS_IND 9
#define PHOBOS_IND 10
#define URANUS_IND 2
#define NEPTUNE_IND 4
#define IO_IND 11
#define EUROPA_IND 12
#define GANYMEDE_IND 13
#define CALLISTO_IND 14
#define TITAN_IND 15
#define ENCELADUS_IND 16
#define IAPETUS_IND 17

#define MAX_IND 18

//out vec3 color;
out vec4 color;
in vec3 vertex_normal;
in vec3 vertex_pos;
in vec2 vertex_tex;
uniform vec3 campos;
uniform float time;
uniform int isClouds;
uniform int object_index;
uniform float radius; // only needed for earth
uniform vec3 center; // only needed for earth
uniform float radius_factors [MAX_IND]; // for shadowing
uniform float radius_factors_b [MAX_IND]; // for experimental shadowing
uniform float directions [MAX_IND * 3]; // for shadowing
uniform float cone_starts [MAX_IND]; // for shadowing
uniform int solar_levels [MAX_IND]; // for shadowing

uniform sampler2D earth_day;
uniform sampler2D earth_spec;
uniform sampler2D earth_night;
uniform sampler2D earth_clouds;
uniform sampler2D earth_normal;
uniform sampler2D moon;
uniform sampler2D mars;
uniform sampler2D space;
uniform sampler2D mercury;
uniform sampler2D venus_ground;
uniform sampler2D venus_sky;
uniform sampler2D sun;
uniform sampler2D jupiter;
uniform sampler2D asteroid_round;
uniform sampler2D saturn;
uniform sampler2D saturn_rings;
uniform sampler2D uranus;
uniform sampler2D neptune;
uniform sampler2D circle;
uniform sampler2D io;
uniform sampler2D europa;
uniform sampler2D ganymede;
uniform sampler2D callisto;
uniform sampler2D titan;
uniform sampler2D enceladus;
uniform sampler2D iapetus;
uniform sampler2D comet;

float value = 1.0f;

int in_cone(float radius_factor, float radius_factor_b, vec3 direction, float cone_start) // -1 not in cone 0 in cone
{
	if (dot(vertex_pos, direction) <= 0)
		return -1;
	float y = distance(vec3(0,0,0), vertex_pos);
	if (y < cone_start)
		return -1;
	if (y > 2 * cone_start) // for experimental cone
		return -1;
	direction = normalize(direction);
	float x = distance(vec3(0,0,0), direction);
	float z = distance(direction, vertex_pos);
	float s = (y*y - x*x - z*z)/(2 * x);
	//float dist = sqrt(z*z - s*s);
	float distsq = z*z - s*s; // shortest distance from point to line squared
	//float radius = -radius_factor * (x + s); // normal cone
	float radius = radius_factor * (x + s) + radius_factor_b; // experimental cone
	radius *= radius; // radius squared
	if (distsq <= radius && distsq >= -radius)
	{
		value = distsq/radius;
		if (value < 0)
			value = 0; // -1 * value;
		return 0;
	}
	return -1;
}

float frag_earth(vec3 lp, vec3 ld, vec3 cd, vec3 h)
{
	float dist_f = distance(lp, center) + radius;
	float dist_n = distance(lp, center) - radius;
	float dist = distance(lp, vertex_pos);

	dist = (dist - dist_n)/(dist_f - dist_n);

	vec3 col;
	if (isClouds == 0)
	{
		//vec3 n = normalize(texture(earth_normal, vec2(vertex_tex.x, -1*vertex_tex.y)).rgb);// - vec3(0.5) + vec3(0,0,1));
		vec3 n = normalize(vertex_normal);
		float diffuse = dot(n,ld);
		diffuse = clamp(diffuse, 0.5, 10);

		col = (1 - dist) * texture(earth_day, vec2(vertex_tex.x, -1*vertex_tex.y)).rgb;
		col += dist * texture(earth_night, vec2(vertex_tex.x, -1*vertex_tex.y)).rgb;

		col *= diffuse*0.9;

		float spec = dot(n,h);
		spec = clamp(spec,0,1);
		spec = pow(spec,50);
		col += texture(earth_spec, vec2(vertex_tex.x, -1*vertex_tex.y)).rgb * vec3(1,1,1)*spec*0.3;
		col *= 1.6;
		col += vec3(0, 0.02, 0.03);
		color = vec4(col, 1);
	}
	else if (isClouds == 1)
	{
		vec3 n = normalize(vertex_normal);
		float diffuse = dot(n,ld);
		diffuse = clamp(diffuse, 0.5, 10);

		col = texture(earth_clouds, vec2(vertex_tex.x, -1*vertex_tex.y)).rgb;
		//col = texture(earth_clouds, time * vec2(1,1) * 0.003 + vec2(vertex_tex.x, -1*vertex_tex.y)).rgb * 0.7;
		//col += texture(earth_clouds, (time * vec2(1,1) * 0.003 + vec2(-0.8*vertex_tex.x, -0.7*vertex_tex.y)) * 0.7).rgb;

		col *= diffuse*0.9;

		float spec = dot(n,h);
		spec = clamp(spec,0,1);
		spec = pow(spec,50);
		col += vec3(1,1,1)*spec*0.4;
		col *= 1.2;
		color.rgb = col;
		color.a = (col.r + col.g + col.b) / 3 + 0.2;
		//color.a = 0;
	}
	return dist;
}

void frag_moon(vec3 lp, vec3 ld, vec3 cd, vec3 h)
{
	vec3 col;
	vec3 n = normalize(vertex_normal);
	float diffuse = dot(n,ld);

	col = texture(moon, vec2(vertex_tex.x, -1*vertex_tex.y)).rgb;

	col *= diffuse*0.9;

	float spec = dot(n,h);
	spec = clamp(spec,0,1);
	spec = pow(spec,20);
	col += vec3(1,1,1)*spec*0.2;
	color = vec4(col, 1);
}

void frag_sun() // make a cool sun effect
{
	vec3 col;
	col = texture(sun, time * vec2(1,1) * 0.003 + vec2(vertex_tex.x, -1*vertex_tex.y)).rgb * 0.7;
	col += texture(sun, (time * vec2(1,1) * 0.003 + vec2(-0.5*vertex_tex.x, -0.7*vertex_tex.y)) * 0.7).rgb;
	//col += texture(sun, (time * vec2(1,1) * 0.003 + vec2(0.6*vertex_tex.x, 0.2*vertex_tex.y)) * 0.7).rgb;
	//col += texture(sun, (time * vec2(1,1) * 0.003 + vec2(0.9*vertex_tex.x, -0.8*vertex_tex.y)) * 0.7).rgb;
	//col += vec3(1,1,1)*0.2;
	col *= 0.7;
	//col *= 0.3;
	color = vec4(col, 1);
}

void frag_space()
{
	vec3 col;
	col = texture(space, vec2(vertex_tex.x, -1*vertex_tex.y)).rgb;
	color = vec4(col, 1);
}

void frag_solid(vec3 lp, vec3 ld, vec3 cd, vec3 h, sampler2D tex, vec3 col_a)
{
	vec3 col;
	vec3 n = normalize(vertex_normal);
	float diffuse = dot(n,ld);
	col = texture(tex, vec2(vertex_tex.x, -1*vertex_tex.y)).rgb;
	col += col_a;
	col *= diffuse*0.9;
	float spec = dot(n,h);
	spec = clamp(spec,0,1);
	spec = pow(spec,20);
	col += vec3(1,1,1)*spec*0.1;
	color = vec4(col, 1);
}

void frag_venus(vec3 lp, vec3 ld, vec3 cd, vec3 h)
{
	vec3 col;
	if (isClouds == 0)
	{
		vec3 n = normalize(vertex_normal);
		float diffuse = dot(n,ld);

		col = texture(venus_ground, vec2(vertex_tex.x, -1*vertex_tex.y)).rgb;

		col *= diffuse*0.9;

		float spec = dot(n,h);
		spec = clamp(spec,0,1);
		spec = pow(spec,20);
		col += vec3(1,1,1)*spec*0.1;
		col *= 1.3;
		color = vec4(col, 1);
	}
	else if (isClouds == 1)
	{
		vec3 n = normalize(vertex_normal);
		float diffuse = dot(n,ld);
		// diffuse = clamp(diffuse, 0.7, 1.5);

		col = texture(venus_sky, vec2(vertex_tex.x, -1*vertex_tex.y)).rgb;

		col *= diffuse*0.7;

		float spec = dot(n,h);
		spec = clamp(spec,0,1);
		spec = pow(spec,50);
		col += vec3(1,1,1)*spec*0.4;
		col *= 1.3;
		color.rgb = col;
		//color.a = 0;
		color.a = (col.r + col.g + col.b) / 3 + 0.4;
	}
}

bool frag_gas(vec3 lp, vec3 ld, vec3 cd, vec3 h, sampler2D tex)
{
	vec3 col;
	if (isClouds == 0)
	{
		vec3 n = normalize(vertex_normal);
		float diffuse = dot(n,ld);

		col = texture(tex, vec2(vertex_tex.x, -1*vertex_tex.y)).rgb;

		col *= diffuse*0.9;

		float spec = dot(n,h);
		spec = clamp(spec,0,1);
		spec = pow(spec,20);
		col += vec3(1,1,1)*spec*0.2;
		color = vec4(col, 1);
	}
	else if (isClouds == 1)
	{
		vec3 n = normalize(vertex_normal);
		float diffuse = dot(n,ld);

		col = texture(saturn_rings, vec2(vertex_tex.x, -1*vertex_tex.y)).rgb;
		color = vec4(col, 1);
		color.a = (col.r + col.g + col.b) / 3 + 0.4;
		if (distance(col, vec3(1,1,1)) < 0.3)
		{
			color.a = 0;
		}
		if (distance(col, vec3(0,0,0)) < 0.01)
		{
			color.a = 0;
		}
		color = vec4(color.xyz * 0.4, color.a);
		return true;
	}
	return false;
}

void frag_circle()
{
	vec3 col;
	col = texture(circle, vec2(vertex_tex.x, -1*vertex_tex.y)).rgb;
	color += vec4(col, (col.r + col.g + col.b) / 3);
}

void frag_comet(vec3 lp, vec3 ld, vec3 cd, vec3 h, sampler2D tex)
{
	vec3 col;

	if (isClouds == 0)
	{
		vec3 n = normalize(vertex_normal);
		float diffuse = dot(n,ld);
		col = texture(tex, vec2(vertex_tex.x, -1*vertex_tex.y)).rgb;
		//col *= diffuse*0.9;
		col *= 0.2;
		float spec = dot(n,h);
		spec = clamp(spec,0,1);
		spec = pow(spec,20);
		col += vec3(1,1,1)*spec*0.5;
		color = vec4(col, 1);
	}
	else if (isClouds == 1)
	{
		//int flip = (int(2000 * time)) % 2;
		//if (flip == 0)
			//flip -= 1;
		//col = texture(comet, vec2(vertex_tex.x, flip * vertex_tex.y)).rgb;
		col = texture(comet, vec2(vertex_tex.x, -1 * vertex_tex.y)).rgb;
		// make the comet blue
		float r = col.r;
		col.r = col.b;
		col.b = r;
		color = vec4(col, 1);

		if (distance(center, vertex_pos) > 0.85)
			color.a = 0;

		float avg = (col.r + col.g + col.b) / 3;
		color.a = 1 - avg;
		if (avg > 0.9)
			color.a *= 0.1;
		color *= 2;
		color.b *= 3;
	}
}

void main()
{
	if (object_index == SPACE_IND)
	{
		frag_space();
		return;
	}
	if (object_index == SUN_IND)
	{
		frag_sun();
		return;
	}
	if (object_index == CIRCLE_IND)
	{
		frag_circle();
		return;
	}
	bool rings = false;
	vec3 lp = vec3(0,0,0);
	vec3 ld = -1 * normalize(vertex_pos - lp);
	vec3 cd = -1 * normalize(vertex_pos - campos);
	vec3 h = normalize(cd+ld);
	if (object_index == COMET_IND)
	{
		frag_comet(lp, ld, cd, h, asteroid_round);
		return;
	}
	if (object_index == EARTH_IND)
		frag_earth(lp, ld, cd, h);
	if (object_index == MOON_IND)
		frag_moon(lp, ld, cd, h);
	if (object_index == MARS_IND)
		frag_solid(lp, ld, cd, h, mars, vec3(0,0,0));
	if (object_index == MERCURY_IND)
		frag_solid(lp, ld, cd, h, mercury, vec3(0,0,0));
	if (object_index == VENUS_IND)
		frag_venus(lp, ld, cd, h);
	if (object_index == JUPITER_IND)
		frag_gas(lp, ld, cd, h, jupiter);
	if (object_index == DEIMOS_IND)
		frag_solid(lp, ld, cd, h, asteroid_round, vec3(0.988, 0.976, 0.741) * 0.3);
	if (object_index == PHOBOS_IND)
		frag_solid(lp, ld, cd, h, asteroid_round, vec3(0.651, 0.067, 0.059) * 0.3);
	if (object_index == SATURN_IND)
		rings = frag_gas(lp, ld, cd, h, saturn);
	if (object_index == URANUS_IND)
		frag_gas(lp, ld, cd, h, uranus);
	if (object_index == NEPTUNE_IND)
		frag_gas(lp, ld, cd, h, neptune);
	if (object_index == IO_IND)
		frag_solid(lp, ld, cd, h, io, vec3(0,0,0));
	if (object_index == EUROPA_IND)
		frag_solid(lp, ld, cd, h, europa, vec3(0,0,0));
	if (object_index == GANYMEDE_IND)
		frag_solid(lp, ld, cd, h, ganymede, vec3(0,0,0));
	if (object_index == CALLISTO_IND)
		frag_solid(lp, ld, cd, h, callisto, vec3(0,0,0));
	if (object_index == TITAN_IND)
		frag_solid(lp, ld, cd, h, titan, vec3(0,0,0));
	if (object_index == ENCELADUS_IND)
		frag_solid(lp, ld, cd, h, enceladus, vec3(0,0,0));
	if (object_index == IAPETUS_IND)
		frag_solid(lp, ld, cd, h, iapetus, vec3(0,0,0));
	if (object_index == BELT_IND)
	{
		frag_solid(lp, ld, cd, h, asteroid_round, vec3(0,0,0));
		return;
	}
	for (int i = 0; i < MAX_IND; i ++) // small moons create particularly ugly shadows
	{
		if (solar_levels[i] <= solar_levels[object_index]) // prevent doing pointless calculations
		{
			if (in_cone(radius_factors[i], radius_factors_b[i], vec3(directions[i * 3 + 0], directions[i * 3 + 1], directions[i * 3 + 2]), 
			cone_starts[i]) == 0)
			{
				if (rings == false)
				{
					ld = -1 * normalize(vertex_pos);
					vec3 n = normalize(vertex_normal);
					float dotp = dot(ld, n);
					if (dotp < 0)
						dotp = 0;
					color.rgb *= (1 - ((1 - value) * dotp));
				}
				else
					color.rgb *= value;
				value = 1.0f;
			}
		}
	}
}

// maybe make a hardcoded list of sampler2Ds and put everything in like the lists at the top of main for easy access
