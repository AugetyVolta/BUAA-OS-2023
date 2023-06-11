#include <stdio.h>
int main() {
	int n;
	scanf("%d", &n);
	char str[10];
	int i=0;
	do{
		str[i]=n%10;
		n/=10;
		i++;
	}
	while(n);
	int flag=0;
	for(int j=0;j<i/2;j++){
		if(str[j]!=str[i-1-j]){
			flag=1;
			break;
		}		
	}

	if (!flag) {
		printf("Y\n");
	} else {
		printf("N\n");
	}
	return 0;
}
