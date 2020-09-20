R""(
__kernel void single_heat_step(
        __global float* input_heat_matrix,
        __global float* output_heat_matrix,
        __constant int* width, __constant int* height, __constant double* r_coeff) {
    int tid = get_global_id(0);


    output_heat_matrix[5] = *height;
}
)""