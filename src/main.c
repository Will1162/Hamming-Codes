#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "main.h"

int main(int argc, char* argv[])
{
	char inputText[] = "0000";

	int lengthBits = 16;

	int inputSize =  8 * (sizeof(inputText) / sizeof(inputText[0]) - sizeof(inputText[0]));

	int nextPow2 = NextPowerOf2(inputSize);
	int bitsFree = nextPow2 - (LogBaseX(2, nextPow2) + 1 + lengthBits);

	int count = 0;
	while (bitsFree <= inputSize)
	{
		nextPow2 = NextPowerOf2(inputSize + count);
		bitsFree = nextPow2 - (LogBaseX(2, nextPow2) + 1 + lengthBits);

		count++;
	}

	printf("Input length: %i\n", inputSize);
	printf("Next power of 2: %i\n", nextPow2);
	printf("Bits free: %i\n", bitsFree);
	printf("Bits spare: %i\n", bitsFree - inputSize);
	printf("ECC bits: %i\n\n", LogBaseX(2, nextPow2) + 1);

	unsigned char* inputBits = malloc(bitsFree);

	for (int i = 0; i < inputSize; i++)
	{
		unsigned char bitMask = (128 >> (i % 8));
		inputBits[i] = (inputText[i / 8] & bitMask) == bitMask;
	}

	for (int i = inputSize; i < bitsFree; i++)
	{
		inputBits[i] = 0;
	}

	printf("Data bits\n");
	for (int i = 0; i < bitsFree; i++)
	{
		printf("%i", inputBits[i]);
		if (i % 8 == 7)
		{
			printf(" ");
		}
		if (i == inputSize - 1)
		{
			printf("\n\nFiller bits\n");
		}
	}
	printf("\n\n");

	printf("Length Bits\n");
	for (int i = nextPow2 - lengthBits; i < nextPow2; i++)
	{
		int bitMask = ((int)pow(2, lengthBits - 1) >> (i % lengthBits));
		printf("%i",(inputSize & bitMask) / bitMask);
		if (i % 8 == 7)
		{
			printf(" ");
		}
	}
	printf("\n\n");


	unsigned char* output = malloc(nextPow2);

	count = 0;
	for (int i = 0; i < nextPow2; i++)
	{
		if ((i != 0) && ((i & (i - 1)) != 0))
		{
			output[i] = inputBits[count];
			//printf("Other: %i, %i, %i\n", i, output[i], inputBits[count]);
			count++;
		}
		else if ((i != 0) && ((i & (i - 1)) == 0))
		{
			output[i] = 2;
			//printf(" Pow2: %i, %i\n", i, output[i]);
		}
		else if (i == 0)
		{
			output[i] = 3;
			//printf(" Zero: %i, %i\n", i, output[i]);
		}
	}
	for (int i = nextPow2 - lengthBits; i < nextPow2; i++)
	{
		int bitMask = ((int)pow(2, lengthBits - 1) >> (i % lengthBits));
		output[i] = (inputSize & bitMask) / bitMask;
	}

	printf("Output step 1:\n");

	for (int i = 0; i < nextPow2; i++)
	{
		printf("%i", output[i]);
		if (i % 32 == 31)
		{
			printf("\n");
		}
		else if (i % 8 == 7)
		{
			printf(" ");
		}
	}




	printf("\nTemp step\n");

	int logThing = 0;
	count = 0;
	for (int i = 0; i < nextPow2; i++)
	{
		if (output[i] == 1 && count == 0 && (i != 0) && ((i & (i - 1)) != 0))
		{
			logThing = i;
			count++;
		}
		else if (output[i] == 1 && (i != 0) && ((i & (i - 1)) != 0))
		{
			logThing = logThing ^ output[i];
		}
	}

	printf("AAA: %i\n", logThing);


	// ! fix funky ECC calculations












	printf("\nOutput step 2:\n");
	for (int i = 0; i < nextPow2; i++)
	{
		if ((i != 0) && ((i & (i - 1)) == 0))
		{
			count = 0;
			for (int j = 0; j < nextPow2; j++)
			{
				if (((j & i) == i) && j != i)
				{
					if (output[j] == 1)
					{
						count++;
					}
				}
			}

			output[i]  = count % 2;
		}
	}

	count = 0;
	for (int i = 1; i < nextPow2; i++)
	{
		if (output[i] == 1)
		{
			count++;
		}
	}
	output[0] = (count % 2);

	for (int i = 0; i < nextPow2; i++)
	{
		printf("%i", output[i]);
		if (i % 32 == 31)
		{
			printf("\n");
		}
		else if (i % 8 == 7)
		{
			printf(" ");
		}
	}
	

	
	printf("\nFinal Output:\n");
	for (int i = 0; i < nextPow2; i++)
	{
		printf("%i", output[i]);
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