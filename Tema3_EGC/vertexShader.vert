#version 330 core
struct Wave {
	float A;
	vec2 D;
	vec2 C;
	float w;
	float phi;
};

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float Q;
uniform float E;

// Uniform Waves
uniform Wave w1;
uniform Wave w2;
uniform Wave w3;
// Time
uniform float dt;
// Damping
uniform float damp;
// Amplitude threshold
const float lowerBound = 0.01;
const float upperBound = 0.2;

// Array of waves
const int SIZE = 3;
Wave w[SIZE] = Wave[SIZE](w1, w2, w3);
// New position
vec3 Position = position;

float interpQ(in float x, in float y1, in float y2)
{
	return (x - y1) / (y2 - y1); 
}

float computeQ(in Wave wave)
{
	if (wave.A < lowerBound)
		return 0;
	else if (wave.A >= lowerBound && wave.A < upperBound)
		return interpQ(wave.A, lowerBound, upperBound) / (wave.w * wave.A * SIZE);
	else
		return Q / (wave.w * wave.A * SIZE);
}

float computePosX()
{
	float q, sum = 0.f;
	for (int i = 0; i < SIZE; ++i) {
		q = computeQ(w[i]);
		sum += q * w[i].A * w[i].D.x * cos(dot(w[i].w * w[i].D, position.xz) + w[i].phi * dt);
	}
	return sum;
}

float computePosZ()
{
	float q, sum = 0.f; vec2 s; 
	for (int i = 0; i < SIZE; ++i) {
		q = computeQ(w[i]);
		sum += q * w[i].A * w[i].D.y * cos(dot(w[i].w * w[i].D, position.xz) + w[i].phi * dt);
	}
	return sum;
}

float computePosY()
{
	float sum = 0.f;
	for (int i = 0; i < SIZE; ++i) {
		sum += w[i].A * sin(dot(w[i].w * w[i].D, position.xz) + w[i].phi * dt);
	}
	return sum;
}

float computeNormX()
{
	float sum = 0.f;
	for (int i = 0; i < SIZE; ++i) {
		sum += w[i].D.x * w[i].w * w[i].A * cos(dot(w[i].w * w[i].D, Position.xz) + w[i].phi * dt);
	}
	return -sum;
}

float computeNormZ()
{
	float sum = 0.f;
	for (int i = 0; i < SIZE; ++i) {
		sum += w[i].D.y * w[i].w * w[i].A * cos(dot(w[i].w * w[i].D, Position.xz) + w[i].phi * dt);
	}
	return -sum;
}

float computeNormY()
{
	float q, sum = 0.f;
	for (int i = 0; i < SIZE; ++i) {
		q = computeQ(w[i]);
		sum += q * w[i].w * w[i].A * sin(dot(w[i].w * w[i].D, Position.xz) + w[i].phi * dt);
	}
	return 1 - sum;
}

void computeDamping()
{
	for (int i = 0; i < SIZE; ++i) {
		w[i].A *= pow(E, -damp * dt);
		if (w[i].A <= lowerBound) w[i].A = lowerBound;
	}
}

void main()
{
	Normal = normal;
	// Compute direction for circular wave
	w[1].D = -normalize(position.xz - w[1].C);
	w[2].D = -normalize(position.xz - w[2].C);
	computeDamping();
	
	/** Using Grestner Waves **/
	// Compute new position
	Position.x += computePosX();
	Position.z += computePosZ();
	Position.y += computePosY();
	// Compute new normals
	Normal.x = computeNormX();
	Normal.z = computeNormZ();
	Normal.y = computeNormY();
	
	vec4 worldPos = model * vec4(Position, 1);  
	gl_Position = projection * view * worldPos;
	FragPos = worldPos.xyz;
}