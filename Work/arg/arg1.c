#include <stdio.h>

int main()
{
	int input, sum=0, i, j;
	
	printf("입력 : ");
	scanf("%d", &input);

	//for(i=1;i<=input;i++)
	//	sum+=i;

	for(i=1, j=input ; i<=(input/2) + (input%2) && i!=j ; i++, j--)
	{
		//if(i==input || i>input) sum+=i;
		//else sum+=i+(input--);

		sum += i+j;
		printf("i : %d,  j : %d, sum : %d \n", i , j, sum);
	}
	sum += (input%2) * i;

	printf("\nANS : %d\n", sum);
	return 0;
}
