#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <math.h>

// ��������� �������� �� ���� ������ ������
// ��������� � ������� �����, ����� ����� ���������� ������������ ������
// ����� � ����� 0.1 �������� �� ���� �������� ������� ����� ������� � ������ ������ � ������� ������ � ���� ������ �������
// ����� ����, ����������� ��������� ������ �������� ������ � ������ ����� ������

const float g = 9.8f;

bool AksInputHeight(int &jumpHeight)
{
	printf("%s", "������� �������� ������������ ������ ������: ");

	return (0 != scanf("%d", &jumpHeight) && jumpHeight >= 0);
}

void CalculateCurrHeight(float time, float maxHeightTime)
{
	float currHeight = 0;
	float initialSpeed = g * maxHeightTime;

	currHeight = initialSpeed * time - 0.5f * g * pow(time, 2);
	printf("������� �����: %f, ������� ������: %f\n", time, currHeight);
}

void ProcessJump(float maxHeightTime)
{
	bool maxHeight = false;

	printf("����� ���������� ������������ ������ = %f\n", maxHeightTime);

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
		printf("\n" "��������� ������ ������������� ������������� ��������" "\n");
		return 1;
	}

	float maxHeightTime = sqrt(jumpHeight * 2 / g);

	ProcessJump(maxHeightTime);
	
	return 0;
}