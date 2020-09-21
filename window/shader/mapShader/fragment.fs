R""(
#version 400

out vec4 FragColor;

in float value;

void main() {
    FragColor = vec4(value, 0.0f, (1-value), 1.0f);
}

)""