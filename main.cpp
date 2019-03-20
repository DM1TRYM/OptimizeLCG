#include <cstdlib>
#include <cstdio>
#include <string.h>
#include <math.h>

               
typedef unsigned long long Int64;
Int64 x0;

Int64 next (Int64 a, Int64 c, Int64 m)
{
	x0 = x0 * a + c;
	return (Int64)(x0 % m);
}

int getCommandIndex(char* command)
{
	if (strcmp (command, "get_c") == 0)
		return 1;
	if (strcmp (command, "get_a") == 0)
		return 2;
	if (strcmp (command, "lcg") == 0)
		return 3;
	if (strcmp (command, "bits") == 0)
		return 4;
	return 0;
}

char* getNextArgument(FILE* F)
{
	char buf[256];
	char c;
	int i = 0;
	while (c = fgetc(F))
	{
		if (c == '\n' || c == '=')
			break;
		if (c != ' ')
			buf[i++] = c;
	}
	buf[i] = 0;

	return buf;
}

Int64 gcd(Int64 a, Int64 b)
{
	Int64 c = 0;
	while (b)
	{
		c = a % b;
		a = b;
		b = c;
	}
	return fabs((Int64)a);
}

void get_c(FILE* F, Int64 min, Int64 max, Int64 m)
{
	Int64 i;
	fprintf(F, "getc_c\n");
	for (i = min; i < max; i++)
		if (gcd(i, m) == 1 && i < m)
			fprintf(F, "%llu ", i);
	fprintf(F, "\n");
}

int simple(Int64 n)
{
	int i;
	for (i = 2; i <= sqrt(n); i++)
	{
		if (n % i == 0)
			return 0;
	}

	return 1;
}


void geta(FILE* F, Int64 m)
{
	int i, j, ok = 0;
	Int64 A[65536] = { 0 };
	int size = 0;
	fprintf(F, "get_a\n");
	for (i = 2; i < m; i++)
		if (simple(i) == 1 && m % i == 0)
			A[size++] = i;
	for (i = 2; i < m; i++)
	{
		ok = 1;
		for (j = 0; j < size; j++)
		{
			if (((i - 1) % A[j]) != 0)
			{
				ok = 0;
				continue;
			}
		}
		if (ok == 1)
		{
			fprintf(F, "%i\n", i);
			return;
		}
	}
	fprintf(F, "no solution\n");
}

void lcg(FILE* F, Int64 a, Int64 c, Int64 m, Int64 n)
{
	int i, j;
	Int64 period = n;
	Int64* Numbers = (Int64*)malloc(sizeof(Int64) * n);
	fprintf(F, "lcg\n");
	if (n == 0 || a > m || c > m || x0 >= m)
	{
		fprintf(F, "no solution\n");
		return;
	}
	for (i = 0; i < n; i++)
	{
		Numbers[i] = next(a, c, m);
		fprintf(F, "%llu\n", Numbers[i]);
	}
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			if (Numbers[i] == Numbers[j] && i != j)
				period = abs(i - j);
		}
	}
	fprintf(F, "Period: %llu\n", period);
}

void bits(FILE* F, Int64 a, Int64 c, Int64 m, Int64 n)
{
	int i, j;
	Int64 x;
	int b[64] = { 0 };
	for (i = 0; i < n; i++)
	{
		x = next(a, c, m);
		j = 0;
		while (x > 0)
		{
			b[j] += (x % 2);
			j++;
			x /= 2;
		}
	}
	fprintf(F, "bits\n");
	for (i = 64 - 1; i >= 0; i--)
		fprintf(F, "%i\n", b[i]);
}

int main()
{
	FILE* F;
	FILE* FOut = fopen("output.txt", "w");
	char command[256];
	char* arg;
	int i;
	int argi;
	Int64 cmin, cmax, m, c, n, a;

	if (!(F = fopen("input.txt", "r")))
	{
		printf("Can not open file input.txt\n");
	}
	else
	{
		while (!feof(F))
		{
			fscanf(F, "%s", command);
			switch (argi = getCommandIndex(command))
			{
				//get_c
				case 1:
					for (i = 0; i < 3; i++)
					{
						arg = getNextArgument(F);
						if (strcmp(arg, "cmin") == 0)
						{
							if (fscanf(F, "%llu", &cmin) != 1 || cmin < 0)
								break;
						}
						if (strcmp(arg, "cmax") == 0)
						{
							if (fscanf(F, "%llu", &cmax) != 1 || cmax < 0)
								break;
						}
						if (strcmp(arg, "m") == 0)
						{
							if (fscanf(F, "%llu", &m) != 1 || m < 0)
								break;
						}
					}
					if (i != 3)
					{
						fprintf(FOut, "incorrect command\n");
					}
					else
					{
						get_c(FOut, cmin, cmax, m);
					}
					break;
					//get_a
				case 2:
					arg = getNextArgument(F);
					if (fscanf(F, "%llu", &m) != 1 || m < 0)
						fprintf(FOut, "incorrect command\n");
					else
						geta(FOut, m);
					break;
					//lcg
				case 3:
					//bits
				case 4:
					for (i = 0; i < 5; i++)
					{
						arg = getNextArgument(F);
						if (strcmp(arg, "a") == 0)
						{
							if (fscanf(F, "%llu", &a) != 1 || cmin < 0)
								break;
						}
						if (strcmp(arg, "x0") == 0)
						{
							if (fscanf(F, "%llu", &x0) != 1 || cmax < 0)
								break;
						}
						if (strcmp(arg, "c") == 0)
						{
							if (fscanf(F, "%llu", &c) != 1 || m < 0)
								break;
						}
						if (strcmp(arg, "m") == 0)
						{
							if (fscanf(F, "%llu", &m) != 1 || m < 0)
								break;
						}
						if (strcmp(arg, "n") == 0)
						{
							if (fscanf(F, "%llu", &n) != 1 || m < 0)
								break;
						}
					}
					if (i != 5)
					{
						fprintf(FOut, "incorrect command\n");
					}
					else
					{
						if (argi == 3)
						{
							lcg(FOut, a, c, m, n);
						}
						if (argi == 4)
						{
							bits(FOut, a, c, m, n);
						}
					}
					break;
				default:
					fprintf(FOut, "incorrect command\n");
					break;
			}
		}
	}

	printf("Done!");
	fclose(FOut);
	fclose(F);
	getchar();
}