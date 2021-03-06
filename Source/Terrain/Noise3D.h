#pragma once
//#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#ifndef PERMUTATION_NUMBER 
#define PERMUTATION_NUMBER 512
#endif
#ifndef GRADIENT_NUMBER
#define GRADIENT_NUMBER 16
#endif
#ifndef RGB_BYTES
#define RGB_BYTES 3
#endif
class Noise3D {
public:
	Noise3D() {}
	~Noise3D() {}
	void initialize();
	static inline double get_noise(double x, double y, double z) {
		int i, j, k;
		double u, v, w;
		//Get lattice points
		i = fast_floor(x);
		j = fast_floor(y);
		k = fast_floor(z);
		//Get position in lattice
		u = x - i;
		v = y - j;
		w = z - k;
		//Get permutation index
		i = i & 0xFF;
		j = j & 0xFF;
		k = k & 0xFF;
		//Get hashed gradient indices
		int g000 = _permutations[i + _permutations[j + _permutations[k]]] % GRADIENT_NUMBER;
		int g001 = _permutations[i + _permutations[j + _permutations[k + 1]]] % GRADIENT_NUMBER;
		int g010 = _permutations[i + _permutations[j + 1 + _permutations[k]]] % GRADIENT_NUMBER;
		int g011 = _permutations[i + _permutations[j + 1 + _permutations[k + 1]]] % GRADIENT_NUMBER;
		int g100 = _permutations[i + 1 + _permutations[j + _permutations[k]]] % GRADIENT_NUMBER;
		int g101 = _permutations[i + 1 + _permutations[j + _permutations[k + 1]]] % GRADIENT_NUMBER;
		int g110 = _permutations[i + 1 + _permutations[j + 1 + _permutations[k]]] % GRADIENT_NUMBER;
		int g111 = _permutations[i + 1 + _permutations[j + 1 + _permutations[k + 1]]] % GRADIENT_NUMBER;
		//Get noise contributions from each corner
		double n000 = dot(_gradients[g000], u, v, w);
		double n100 = dot(_gradients[g100], u - 1.0, v, w);
		double n010 = dot(_gradients[g010], u, v - 1.0, w);
		double n110 = dot(_gradients[g110], u - 1.0, v - 1.0, w);
		double n001 = dot(_gradients[g001], u, v, w - 1.0);
		double n101 = dot(_gradients[g101], u - 1.0, v, w - 1.0);
		double n011 = dot(_gradients[g011], u, v - 1.0, w - 1.0);
		double n111 = dot(_gradients[g111], u - 1.0, v - 1.0, w - 1.0);
		//Get quintic curve values
		u = quintic(u);
		v = quintic(v);
		w = quintic(w);
		//Interpolate along x-axis
		double nx00 = mix(n000, n100, u);
		double nx01 = mix(n001, n101, u);
		double nx10 = mix(n010, n110, u);
		double nx11 = mix(n011, n111, u);
		//Interpolate along y-axis
		double nxy0 = mix(nx00, nx10, v);
		double nxy1 = mix(nx01, nx11, v);
		//Interpolate along z-axis
		return mix(nxy0, nxy1, w);
	};
	GLuint get_gradient_texture_id() const { return _g_tex_id; }
	GLuint get_permutation_texture_id() const { return _p_tex_id; }
	//void save_textures_to_disk(std::string permutation_filename, std::string gradient_filename);
private:
	void generate_gradient_texture();
	void generate_permutation_texture();
	static const unsigned int _permutations[PERMUTATION_NUMBER];
	static const int _gradients[GRADIENT_NUMBER][RGB_BYTES];
	static const unsigned int _gradients_rgb[RGB_BYTES * GRADIENT_NUMBER];
	//Texture id's for GPU noise
	GLuint _p_tex_id, _g_tex_id;
	//Helper functions for CPU noise
	static inline int fast_floor(double f) { return f > 0 ? (int)f : (int)f - 1; }
	static inline double dot(const int g[], double x, double y, double z) { return g[0] * x + g[1] * y + g[2] * z; }
	static inline double mix(double a, double b, double t) { return a * (1.0 - t) + b * t; }
	static inline double quintic(double x) { return x * x * x * (x * (x * 6.0 - 15.0) + 10.0); }
};