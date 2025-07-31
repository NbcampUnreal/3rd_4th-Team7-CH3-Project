#include "Enemy/LSEnemy.h"

ALSEnemy::ALSEnemy()
{
	Damage=30.0f;
}

void ALSEnemy::Attack()
{
	
}

void ALSEnemy::Death()
{
	//EnemyTodo : characterBase의 health<=0 ? EnemyDelete()
	if (true)
	{
		EnemyDelete();
	}
}

void ALSEnemy::EnemyDelete()
{
	Destroy();
}
