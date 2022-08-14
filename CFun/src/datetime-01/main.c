#include <stdio.h>
#include <time.h>
#include <string.h>
#include <ncurses.h>

#define TIMESTR_MAX 256

int main(void)
{
	int w, h;

	char timestr[TIMESTR_MAX];

	time_t timestamp = 0;
	time_t prevtime = 0;

	initscr();
	
	curs_set(0);

	while (1)
	{
		timestamp = time(NULL);

		if (timestamp != prevtime)
		{
			prevtime = time(NULL);

			struct tm *tm = localtime(&timestamp);

			sprintf(timestr, "%02d/%02d/%4d %2d:%02d:%02d\n",
					tm->tm_mon + 1,
					tm->tm_mday,
					tm->tm_year + 1900,
					tm->tm_hour,
					tm->tm_min,
					tm->tm_sec);

			clear();

			getmaxyx(stdscr, h, w);

			move(h / 2, (w - strlen(timestr)) / 2);

			printw("%s", timestr);

			refresh();
		}
	}

	endwin();

	return 0;
}
