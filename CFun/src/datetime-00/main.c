#include <stdio.h>
#include <time.h>

int main(void) {

	time_t timestamp=time(NULL);
	struct tm *tm=localtime(&timestamp);

	printf("year:  %4d\n",tm->tm_year+1900);
	printf("month:   %2d\n",tm->tm_mon+1);
	printf("day:     %2d\n",tm->tm_mday);

	printf("hour:    %2d\n",tm->tm_hour);
	printf("min:     %2d\n",tm->tm_min);
	printf("sec:     %2d\n",tm->tm_sec);

	return 0;	
}
