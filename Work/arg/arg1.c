#include <stdio.h>

int main()
{
	int input, sum=0, i;

	scanf("%d", &input);

	//for(i=1;i<=input;i++)
	//	sum+=i;
	for(i=1;i<=input;i++)
	{
		//if(i==input || i>input) sum+=i;
		//else sum+=i+(input--);
		
		sum += i+(input--);
		printf("SUM : %d\n", sum);
	}
	printf("\nANS : %d\n", sum);
	return 0;
}
