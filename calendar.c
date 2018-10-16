
/* Program name - calendar
 * Writer	- Aaron
 * Date		- 2018/10/16
 */

/* Program act	- Since 1970, 1900.1.1 - Monday
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
 *	check (argc, argv, check_argc, check_args);
 *	calendar (total_days, year, month);
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define START_YEAR	1900
#define CYCLE		7

static int months[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/*
 * sum_of_range - Get sum of range [a, b]
 */
int sum_of_range (int low, int high, int (*term) (int), int (*next) (int))
{
	int i, result = 0;

	for (i = low; i <= high; i = (*next) (i))
	{
		result += (*term) (i);
	}

	return result;
}


/*
 * leap_year - Return the year is a leap year or not
 */
int leap_year (int year)
{
	return (year%4 == 0 && year%100 != 0) || (year%400 == 0);
}

/*
 * inc - x --> x+1
 */
int inc (int x)
{
	return x + 1;
}

/*
 * year_term - Return the days of year
 */
int year_term (int year)
{
	return leap_year (year)? 366: 365;
}

/*
 * days_of_year - Return the days of years between [low, high]
 */
int days_of_year (int low, int high)
{
	return sum_of_range (low, high, year_term, inc);
}

/*
 * month_term - Return the days of month
 */
int month_term (int month)
{
	return months[month];
}

/*
 * days_of_month - Return the days of months between [low, high]
 */
int days_of_month (int low, int high, int year)
{
	int days = sum_of_range (low, high, month_term, inc);

	return low <= 2 && high >= 2 && leap_year (year)? days+1: days;
}

/*
 * pre_fixed - Print n times tab
 */
void pre_fixed (int times)
{
	while (times--) printf ("\t");
}

/*
 * calendar - Show the calendar of year/month
 */
void calendar (int days, int year, int month)
{
	int i, month_day;

	printf ("\n\t\t%d\t\t%d\n\n", year, month);
	printf ("Mon\tTues\tWen\tThur\tFri\tSta\tSun\n");

	if (month == 2)
		months[month] = leap_year (year)? 29: 28;

	pre_fixed ((days) % CYCLE);
	for (i = 1; i <= months[month]; i++)
	{
		printf ("%d%c", i, (days+i) % CYCLE? '\t': '\n');
	}

	printf ("\n\n");
}

/*
 * chech_argc - Return the arg count is invalid or not
 */
int check_argc (int argc)
{
	return argc > 2;
}

/*
 * is_number - Return the target is number or not
 */
int is_number (char *target)
{
	while (*target != '\0' && *target >= '0' && *target <= '9')
		target++;

	return *target == '\0';
}

/*
 * check_arg1 - Return the arg1 (year) is invalid or not
 */
int check_arg1 (char *arg)
{

	if (!is_number (arg)) return 1;

	return atoi (arg) < 0;
}

/*
 * check_arg2 - Return the arg2 (month) is invalid or not
 */
int check_arg2 (char *arg)
{
	if (!is_number (arg)) return 1;

	return atoi (arg) < 1 || atoi (arg) > 12;
}

/*
 * check - Check the arg count and args by check_argc and check_args[]
 */
int check (int argc, char **argv, int (*check_argc) (int), int (*check_args[]) (char *))
{
	int i;

	if ((*check_argc) (argc))	return 1;

	if (argc > 0)
	{
		for (i = 0; i < argc && !(*check_args) (argv[i]); i++, check_args++)
			;
	}

	return i != argc;
}

/*
 * usage - Show the help massege
 */
void usage ()
{
	printf ("Usage: calendar [year] [month]\n");
	printf ("\tyear >= 1900\n");
	printf ("\tmonth >= 1 and month <= 12\n");
}

/*
 * main - The main function of program
 */
int main (int argc, char **argv)
{
	int year = 0, month = 0;
	int i, days = 0;

	struct tm *time_ptr;
	time_t timeval;

	/* Function pointer array for check_args */
	int (*func_ptrs[]) (char *) = {check_arg1, check_arg2, NULL};

	if (check (argc-1, &argv[1], check_argc, func_ptrs))
	{
		usage ();
		return 1;
	}

	switch (argc)
	{
		/* Type 1 - $ calender */
		case 1:
			(void)time (&timeval);
			time_ptr = localtime (&timeval);

			year = time_ptr->tm_year + 1900;
			month = time_ptr->tm_mon + 1;
			days += days_of_year (START_YEAR, year-1);
			days += days_of_month (1, month-1, year);

			calendar (days, year, month);
			break;

		/* Type 2 - $ calender year */
		case 2:
			sscanf (argv[1], "%d", &year);
			days += days_of_year (START_YEAR, year-1);
			for (i = 1; i <= 12; i++)
			{
				calendar (days, year, i);
				days += months[i];
			}
			break;

		/* Type 3 - $ calender year month */
		case 3:
			sscanf (argv[1], "%d", &year);
			sscanf (argv[2], "%d", &month);
			days += days_of_year (START_YEAR, year-1);
			days += days_of_month (1, month-1, year);

			calendar (days, year, month);
			break;

		/* Unknown type */
		default:
			fprintf (stderr, "Unknown options\n");
			usage ();
	}

	return 0;
}
