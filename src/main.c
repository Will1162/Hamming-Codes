#include <stdio.h>
#include <math.h>
#include "main.h"

// this is a git test

int main(int argc, char* argv[])
{
	char inputText[] = "Will Bur";

	int inputSize =  8 * (sizeof(inputText) / sizeof(inputText[0]) - 1);

	int nextPow2 = NextPowerOf2(inputSize);
	int bitsFree = nextPow2 - (LogBaseX(2, nextPow2) + 1);

	int count = 0;
	while (bitsFree <= inputSize)
	{
		nextPow2 = NextPowerOf2(inputSize + count);
		bitsFree = nextPow2 - (LogBaseX(2, nextPow2) + 1);

		count++;
	}

	printf("Input length: %i\n", inputSize);
	printf("Next power of 2: %i\n", nextPow2);
	printf("Bits free: %i\n", bitsFree);

	unsigned char inputBits[bitsFree];

	for (int i = 0; i < inputSize; i++)
	{
		inputBits[i] = (inputText[i / 8] & (1 << (i % 8))) == (1 << (i % 8));
	}

	for (int i = 0; i < inputSize; i++)
	{
		printf("%i,", inputBits[i]);
		if (i % 8 == 7)
		{
			printf("\n");
		}
	}

	printf("\n");

	return 0;
}

int NextPowerOf2(int input)
{
	int power = 1;
	while(power < input)
	{
		power *= 2;
	}
	
	return power;
}

int LogBaseX(int base, int input)
{
	return log10(input) / log10(base);
}