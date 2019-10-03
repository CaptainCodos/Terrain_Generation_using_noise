#version 430

//layout (location = 0) in vec3 position;
layout (location = 3) in vec4 myColor;

in vec3 normal;
out vec4 color;

uniform bool lighting;

// light
struct DirectionalLight
{
    vec3 color;
    vec3 direction;
};

vec3 getLightDirection(){
	return vec3(-2.0f, 0.5f, 3.7f);
}

vec3 getLightColour(){
	return vec3(.9f, .9f, .9f);
}


// material
vec4 computeColour(){
	
    // ambient 
    float ambientIntensity = .4f;
    vec3 ambient = getLightColour() * ambientIntensity;

    // diffuse
    float diffuseIntensity = 2.0f;
	
	vec3 c = vec3(myColor.r, myColor.g, myColor.b);
	
	if (lighting)
	{
		vec3 diffuse = max(dot(getLightDirection(), normal),0) * c * diffuseIntensity;
		
		// final colour
		return vec4(ambient + diffuse, 1.0);
	}
	else
	{
		vec3 diffuse = c;
		
		
		// final colour
		return vec4(diffuse, 1.0);
	}
	
	
	//return vec4(diffuse, 1.0);
}

// return fragment colour
void main()
{
    color = computeColour();
}

