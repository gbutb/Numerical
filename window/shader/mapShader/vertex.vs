R""(
#version 400

// layout (location = 0) in float value;
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 projection;

uniform int width;
uniform int height;

out float value;
out float depth;

void main() {
    vec4 position = model * vec4(aPos.y, aPos.z / 3.0, aPos.x, 1.0);
    value = aPos.z;
    depth = position.z;
	gl_Position = projection *  position;
}

)""