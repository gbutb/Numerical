R""(
int get_tid(int x, int y, int width, int height) {
    return x + y * width;
}

bool is_outside_bounds(int x, int y, int width, int height) {
    return (y < 0 || x < 0 || y >= height || x >= width);
}

bool is_on_boundary(int x, int y, int width, int height) {
    return (y == 0 || x == 0 || y == height - 1 || x == width - 1);
}

float get_value_at(int x, int y, int width, int height, __global float* matrix) {
    if (is_outside_bounds(x, y, width, height)) return 0;
    return matrix[get_tid(x, y, width, height)];
}

__kernel void laplace(
        __global float* input_matrix,
        __global float* output_matrix,
        int width, int height) {
    int x = get_global_id(0);
    int y = get_global_id(1);

    int tid = get_tid(x, y, width, height);
    if (is_outside_bounds(x, y, width, height)) return;

    if (is_on_boundary(x, y, width, height)) {
        output_matrix[tid] = get_value_at(x, y, width, height, input_matrix);
        return;
    }

    output_matrix[tid] = (1.0/4.0) * (
        get_value_at(x + 1, y, width, height, input_matrix) +
        get_value_at(x - 1, y, width, height, input_matrix) +
        get_value_at(x, y + 1, width, height, input_matrix) +
        get_value_at(x, y - 1, width, height, input_matrix));
}
)""
