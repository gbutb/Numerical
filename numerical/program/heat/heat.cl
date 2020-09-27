R""(
int get_tid(int x, int y, int width, int height) {
    return x + y * width;
}

bool is_outside_bounds(int x, int y, int width, int height) {
    return (y < 0 || x < 0 || y >= height || x >= width);
}

float get_value_at(int x, int y, int width, int height, __global float* matrix) {
    if (is_outside_bounds(x, y, width, height)) return 0;
    return matrix[get_tid(x, y, width, height)];
}

__kernel void single_heat_step(
        __global float* input_heat_matrix,
        __global float* output_heat_matrix,
        int width, int height, float r_coeff) {
    int x = get_global_id(0);
    int y = get_global_id(1);

    int tid = get_tid(x, y, width, height);
    if (is_outside_bounds(x, y, width, height)) return;

    output_heat_matrix[tid] = (1 - 4*r_coeff) * input_heat_matrix[tid] + r_coeff * (
        get_value_at(x - 1, y, width, height, input_heat_matrix) +
        get_value_at(x, y - 1, width, height, input_heat_matrix) +
        get_value_at(x + 1, y, width, height, input_heat_matrix) +
        get_value_at(x, y + 1, width, height, input_heat_matrix)
    );
}
)""
