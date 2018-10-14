
/* Program name - calendar
 * Writer	- Aaron
 * Date		- 2018/10/14
 */

/* Program act	- Since 1970, 1970.1.1 - Thursday
 *
 *	$ calendar		- show current month
 *	$ calendar year		- show all month of year
 *	$ calendar year month	- show the month of year
 */

/* Program Functions
 *
 *	sum_of_range (low, high, term func, next func);
 *	leap_year (year);
 *	year_term (cur_year);
 *	year_next (cur_year);
 *	days_of_year (low, high);
 *	month_term (cur_month);
 *	month_next (cur_month);
 *	days_of_month (low, high, year);
 *	pre_fixed (times);
 *	calendar (total_days, year, month);
 */

#include <stdio.h>
#include <time.h>

#define START_YEAR	1970
#define CYCLE		7

const int months[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

int sum_of_range (int low, int high, int (*term) (int x), int (*next) (int x))
{
	int i, result = 0;

	for (i = low; i <= high; i = (*next) (i))
	{
		result += (*term) (i);
	}

	return result;
}

int leap_year (int year)
{
	return (year%4 == 0 && year%100 != 0) || (year%400 == 0);
}

int inc (int x)
{
	return x + 1;
}

int year_term (int year)
{
	return leap_year (year)? 366: 365;
}

int days_of_year (int low, int high)
{
	return sum_of_range (low, high, year_term, inc);
}

int month_term (int month)
{
	return months[month];
}

int days_of_month (int low, int high, int year)
{
	int days = sum_of_range (low, high, month_term, inc);

	return low <= 2 && high >= 2 && leap_year (year)? days+1: days;
}

void pre_fixed (int times)
{
	while (times--) printf ("\t");
}

void calendar (int days, int year, int month)
{
	int i;

	printf ("\n\t\t%d\t\t%d\n\n", year, month);
	printf ("Mon\tTues\tWen\tThur\tFri\tSta\tSun\n");

	pre_fixed ((days+3) % CYCLE);
	for (i = 1; i <= months[month]; i++)
	{
		printf ("%d%c", i, (days+i+3) % CYCLE? '\t': '\n');
	}

	printf ("\n\n");
}

int main (int argc, char **argv)
{
	int year = 0, month = 0;
	int i, days = 0;

	struct tm *time_ptr;
	time_t timeval;

	(void)time (&timeval);
	time_ptr = localtime (&timeval);

	switch (argc)
	{
		case 1:
			year = time_ptr->tm_year + 1900;
			month = time_ptr->tm_mon + 1;
			days += days_of_year (START_YEAR, year-1);
			days += days_of_month (1, month-1, year);

			calendar (days, year, month);
			break;

		case 2:
			sscanf (argv[1], "%d", &year);
			days += days_of_year (START_YEAR, year-1);
			for (i = 1; i <= 12; i++)
			{
				calendar (days, year, i);
				days += months[i];
			}
			break;

		case 3:
			sscanf (argv[1], "%d", &year);
			sscanf (argv[2], "%d", &month);
			days += days_of_year (START_YEAR, year-1);
			days += days_of_month (1, month-1, year);

			calendar (days, year, month);
			break;

		default:
			fprintf (stderr, "Unknown options\n");
	}

	return 0;
}
