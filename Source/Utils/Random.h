#pragma  once
#include <random>

class Random{
public:
    /* init random device */
	static void Init();
    static void Seed(unsigned int seed);
    /* random float between [0,1] */
	static float GetFloat();
    /* random float [min,max] */
	static float GetFloatRange(float min, float max);
    /* random unsigned int between [min,max] */
	static uint GetUIntRange(int min, int max);
private:
	static std::mt19937 generator;
};