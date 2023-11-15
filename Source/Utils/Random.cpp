#include "Random.h"

/* init random device */
void Random::Init(){
    std::random_device rd;
    Random::Seed(rd());
}
void Random::Seed(unsigned int seed){
    generator.seed(seed);
}
/* random float between [0,1] */
float Random::GetFloat(){ 
    return GetFloatRange(0.0f, 1.0f);
}
/* random float [min,max] */
float Random::GetFloatRange(float min, float max){
    std::uniform_real_distribution<float> dist(min,max);
    return dist(generator);
}
/* random unsigned int between [min,max] */
uint Random::GetUIntRange(int min, int max){
    std::uniform_int_distribution<uint> dist(min,max);
    return dist(generator);
}

std::mt19937 Random::generator;