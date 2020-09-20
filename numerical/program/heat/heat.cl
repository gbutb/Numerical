R""(

int get_x(int tid, int width, int height) {
    return (tid - tid % width) / width;
}

int get_y(int tid, int width, int height) {
    return tid % width;
}

int get_tid(int x, int y, int width, int height) {
    return y + x * width;
}

int get_value_at(int x, int y, int width, int height, __global float* matrix) {
    if (x < 0 || y < 0 || x >= width || y >= height) return 0;
    return matrix[get_tid(x, y, width, height)];
}

__kernel void single_heat_step(
        __global float* input_heat_matrix,
        __global float* output_heat_matrix,
        int width, int height, float r_coeff) {
    int tid = get_global_id(0);

    int x = get_x(tid, width, height);
    int y = get_y(tid, width, height);

    output_heat_matrix[tid] = (1 - 4*r_coeff) * input_heat_matrix[tid] + r_coeff * (
        get_value_at(x, y - 1, width, height, input_heat_matrix) +
        get_value_at(x - 1, y, width, height, input_heat_matrix) +
        get_value_at(x, y + 1, width, height, input_heat_matrix) +
        get_value_at(x + 1, y, width, height, input_heat_matrix)
    );
}
)""
