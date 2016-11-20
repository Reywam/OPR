#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <math.h>

// Программа получает на вход высоту прыжка
// вычисляет и выводит время, когда будет достигнута максимальная высота
// затем с шагом 0.1 проходит по всем моментам времени между началом и концом прыжка и выводит высоту в этот момент времени
// кроме того, обязательно выводится момент середины прыжка и момент конца прыжка

const float g = 9.8f;

bool AksInputHeight(int &jumpHeight)
{
	printf("%s", "Введите значение максимальной высоты прыжка: ");

	return (0 != scanf("%d", &jumpHeight) && jumpHeight >= 0);
}

void CalculateCurrHeight(float time, float maxHeightTime)
{
	float currHeight = 0;
	float initialSpeed = g * maxHeightTime;

	currHeight = initialSpeed * time - 0.5f * g * pow(time, 2);
	printf("Текущее время: %f, Текущая высота: %f\n", time, currHeight);
}

void ProcessJump(float maxHeightTime)
{
	bool maxHeight = false;

	printf("Время достижения максимальной высоты = %f\n", maxHeightTime);

	for (float currTime = 0; currTime < maxHeightTime * 2; currTime += 0.1f)
	{
		if (currTime > maxHeightTime && !maxHeight)
		{
			maxHeight = true;
			CalculateCurrHeight(maxHeightTime, maxHeightTime);
		}

		CalculateCurrHeight(currTime, maxHeightTime);
	}

	CalculateCurrHeight(maxHeightTime * 2, maxHeightTime);
}

int main(int, char *[])
{
	setlocale(LC_ALL, "rus");

	int jumpHeight = 0;

	if (!AksInputHeight(jumpHeight))
	{
		printf("\n" "Требуется ввести целочисленное положительное значение" "\n");
		return 1;
	}

	float maxHeightTime = sqrt(jumpHeight * 2 / g);

	ProcessJump(maxHeightTime);
	
	return 0;
}