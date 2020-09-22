R""(
#version 400

out vec4 FragColor;

in float value;
in float depth;

void main() {
    FragColor = vec4(2*value, value, (1-2*value), 1.0f / (1.0 + abs(depth)));
}

)""