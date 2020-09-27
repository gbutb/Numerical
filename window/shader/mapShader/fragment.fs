R""(
#version 400

out vec4 FragColor;

in float value;
in float depth;

void main() {
    FragColor = vec4(vec3(3*value, value, (1-3*value)) / (1.0 + abs(depth)/6.0), 1.0f);
}

)""