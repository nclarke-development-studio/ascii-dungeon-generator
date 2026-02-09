#include "perlin_noise.h"

#include <numeric>
#include <cmath>
#include <algorithm>

PerlinNoise::PerlinNoise(unsigned int seed)
{
	p.resize(256);

	// Fill with 0..255
	std::iota(p.begin(), p.end(), 0);

	// Shuffle deterministically
	std::mt19937 rng(seed);
	std::shuffle(p.begin(), p.end(), rng);

	// Duplicate
	p.insert(p.end(), p.begin(), p.end());
}

double PerlinNoise::noise(double x, double y, double z) const
{
	// Find unit cube
	int X = static_cast<int>(std::floor(x)) & 255;
	int Y = static_cast<int>(std::floor(y)) & 255;
	int Z = static_cast<int>(std::floor(z)) & 255;

	// Relative position
	x -= std::floor(x);
	y -= std::floor(y);
	z -= std::floor(z);

	double u = fade(x);
	double v = fade(y);
	double w = fade(z);

	int A = p[X] + Y;
	int AA = p[A] + Z;
	int AB = p[A + 1] + Z;
	int B = p[X + 1] + Y;
	int BA = p[B] + Z;
	int BB = p[B + 1] + Z;

	double res = lerp(
		lerp(
			lerp(grad(p[AA], x, y, z),
				 grad(p[BA], x - 1, y, z), u),
			lerp(grad(p[AB], x, y - 1, z),
				 grad(p[BB], x - 1, y - 1, z), u),
			v),
		lerp(
			lerp(grad(p[AA + 1], x, y, z - 1),
				 grad(p[BA + 1], x - 1, y, z - 1), u),
			lerp(grad(p[AB + 1], x, y - 1, z - 1),
				 grad(p[BB + 1], x - 1, y - 1, z - 1), u),
			v),
		w);

	// Normalize to [0,1]
	return (res + 1.0) * 0.5;
}

double PerlinNoise::fbm(
	double x,
	double y,
	int octaves,
	double lacunarity,
	double gain) const
{
	double value = 0.0;
	double amplitude = 1.0;
	double frequency = 1.0;
	double maxValue = 0.0;

	for (int i = 0; i < octaves; ++i)
	{
		value += noise(x * frequency, y * frequency) * amplitude;
		maxValue += amplitude;

		frequency *= lacunarity;
		amplitude *= gain;
	}

	return value / maxValue;
}

double PerlinNoise::fade(double t)
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}

double PerlinNoise::lerp(double a, double b, double t)
{
	return a + t * (b - a);
}

double PerlinNoise::grad(int hash, double x, double y, double z)
{
	int h = hash & 15;
	double u = h < 8 ? x : y;
	double v = h < 4 ? y : (h == 12 || h == 14 ? x : z);
	return ((h & 1) ? -u : u) + ((h & 2) ? -v : v);
}