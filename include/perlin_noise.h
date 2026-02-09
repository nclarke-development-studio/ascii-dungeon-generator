#pragma once

#include <ctime>
#include <vector>
#include <random>

class PerlinNoise
{
public:
	explicit PerlinNoise(unsigned int seed = 0);

	// Single-octave Perlin
	double noise(double x, double y, double z = 0.0) const;

	// Fractal Brownian Motion (octaves)
	// layer multiple noises on top of each other at different scales
	/*
		neutral: 6, 2.0, 0.5
		jagged: 6, 2.3, 0.55
	
	*/
	double fbm(
		double x,
		double y,
		int octaves, // number of noise layers added
		double lacunarity = 2.0, // frequency increases per octave
		double gain = 0.5) const; // strength of each layer

private:
	std::vector<int> p;

	static double fade(double t);
	static double lerp(double a, double b, double t);
	static double grad(int hash, double x, double y, double z);
};

// https://www.geeksforgeeks.org/inline-functions-cpp/
inline float *RandomArray(int size, int seed)
{
	float *seedArray = new float[size];

	if (seed < 0)
		srand(time(NULL));
	else
		srand(seed);

	for (int i = 0; i < size; i++)
	{
		seedArray[i] = (float)rand() / (float)RAND_MAX;
	}

	return seedArray;
}