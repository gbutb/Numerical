R""(
#version 400

// layout (location = 0) in float value;
layout (location = 0) in vec2 aPos;

uniform mat4 model;
uniform mat4 projection;

uniform int width;
uniform int height;

void main() {
    vec4 position = model * vec4(aPos.y, 0.0, aPos.x, 1.0);
	gl_Position = projection *  position;
}

)""