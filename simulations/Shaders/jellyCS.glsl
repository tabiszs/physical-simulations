#version 430 core

layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout (packed, binding = 0) uniform corners_positions
{
	int cc_pos[64];
};

void main() {
	int i=0, sum=0;
	for(i; i<64; ++i) {
		sum += cc_pos[i];
	}
	int p=sum;
}