#include "string.h"

void int_to_ascii(int n, char str[]){
	int sign, i;
	if((sign = n) < 0) n = -n;
	i = 0;
	while((n /= 10) > 0){
		str[i++] = n % 10 + '0';
	}
	if(sign < 0) str[i++] = '-';
	str[i] = '\0';

	reverse(str);
}

void reverse(char str[]){
	int c, i ,j;
	for(i = 0, j = strlen(str)-1; i < j; i++, j--){
		c = str[i];
		str[i] = str[j];
		str[j] = c;
	}
}
int strlen(char str[]){
	int i = 0;
	while(str[i] != '\0'){i++;}
	return i;
}

void backspace(char str[]){
	str[strlen(str) - 1] = '\0';
}

void appendchar(char str[], char s){
	int x = strlen(str);
	str[x] = s;
	str[x + 1] = '\0';
}

int strcmp(char str[], char str2[]){
	int n1 = strlen(str);
	int n2 = strlen(str2);
	int is = 0;
	int i;
	if(n1 == n2){
		for(i = 0; i <= n1; i++){
			if(str[i] != str2[i]){
				is = 1;
			}
		}
	} else {
		is = 1;
	}
	return is;

}
