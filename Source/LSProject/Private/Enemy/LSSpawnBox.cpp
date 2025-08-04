#include "Enemy/LSSpawnBox.h"

ULSSpawnBox::ULSSpawnBox()
{
	SpqwnZombieNum=10;
}

void ULSSpawnBox::ZombieSqwan()
{
	while(--SpqwnZombieNum)
	{
		//Enemy Todo : Random 위치에서 SpqwnZombieNum 개수의 좀비 발생 >  DataTable 이용
	}
}