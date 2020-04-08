#include "SparkEffect.h"
#include "Define.h"
SparkEffect::SparkEffect(float x, float y) : Effect()
{
	this->x = x;
	this->y = y;
	type = Effect_Type::SPARK_EFFECT;
	Reset();
}

SparkEffect::~SparkEffect()
{
}
