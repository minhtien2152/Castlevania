#include "SparkEffect.h"
#include "Define.h"
SparkEffect::SparkEffect(float x, float y) : Effect(x,y)
{

	animation_set = CAnimationSets::GetInstance()->Get(Effect_Type::SPARK_EFFECT);
	Reset();
}

SparkEffect::~SparkEffect()
{
}
