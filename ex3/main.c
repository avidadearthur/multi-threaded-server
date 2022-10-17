#include <stdio.h>
#include <ctype.h> //toupper()
#include <string.h> //strcmp(), strcasecmp(), strcat()
#include <stdlib.h>

int main() {

	int MAX =  45;
	char first[MAX];
	char second[MAX];

	/*Use scanf to read out your first and
	 second name in first and second, respectively.*/
	printf("Enter your first name: ");
	scanf("%s",first);
        printf("Enter your second name: ");
        scanf("%s",second);

	/*Convert your second name to all upper case chars and store the result in str.
	Apply strcmp on second and str and make sure you understand the result. Is
	there another compare function ignoring the case of chars?
	*/

	char str[MAX];
	char ch;
	int j = 0;

	while (second[j]) {
        	ch = second[j];
		//printf("%c",ch);
        	str[j] = toupper(ch);
        	j++;
    	}

	printf("Nice second nam Mr. %s! \n",str);
	int cmp = strcmp(second, str);
	printf("Is the lowercase string larger? %d \n", cmp);
	int casecomp = strcasecmp(second, str);
	printf("If you use strcasecmp() the comparison result is: %d \n",casecomp);

	/*Concatenate your first and second name into name; be aware that there is an
	unsafe and a safe string copy and concatenation function. Print the result.
	*/
	char *fullName = malloc(strlen(first) + strlen(second) + 1);
	strcat(fullName, first);
	strcat(fullName, second);
	printf("Welcome %s \n",fullName);
	/*Read your birth year in some int year.*/
	int year = 0;
	printf("Enter your birth year: ");
        scanf("%d",&year);
	printf("Welcome %s%d \n",fullName,year);

	return 0;
}

