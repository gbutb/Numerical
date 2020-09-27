R""(
int get_tid(int x, int y, int width, int height) {
    return x + y * width;
}

bool is_outside_bounds(int x, int y, int width, int height) {
    return (y < 0 || x < 0 || y >= height || x >= width);
}

float get_value_at(int x, int y, int width, int height, __global float2* matrix, int channel) {
    if (is_outside_bounds(x, y, width, height)) return 0;
    float2 val = matrix[get_tid(x, y, width, height)];
    return (channel == 0) ? val.x : val.y;
}

__kernel void single_wave_step(
        __global float2* input_wave_matrix,
        __global float2* output_wave_matrix,
        int width, int height, float ratio) {
    int x = get_global_id(0);
    int y = get_global_id(1);

    int tid = get_tid(x, y, width, height);
    if (is_outside_bounds(x, y, width, height)) return;

    output_wave_matrix[tid].x = get_value_at(x, y, width, height, input_wave_matrix, 1);
    output_wave_matrix[tid].y = (2 - 4 * ratio*ratio) * get_value_at(x, y, width, height, input_wave_matrix, 1) +
        ratio*ratio * (
            get_value_at(x - 1, y, width, height, input_wave_matrix, 1) +
            get_value_at(x, y - 1, width, height, input_wave_matrix, 1) +
            get_value_at(x + 1, y, width, height, input_wave_matrix, 1) +
            get_value_at(x, y + 1, width, height, input_wave_matrix, 1)) - get_value_at(x, y, width, height, input_wave_matrix, 0);
}
)""
