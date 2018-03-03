#include "types.h"
#include "user.h"
#include "stat.h"
#include "date.h"

int
main(int argc, char *argv[])
{
	struct rtcdate r;

	//printf(1,"main(): %d\n",&r);
	if (date(&r)) {
		printf(2, "date failed\n");
		exit();
	}
	// At his point r should have the date from the date() sys call
	
  // your code to print the time in any format you like...
	//printf(1, "\n main(): Bot->%d Top->%d\n", &r.second, &r.year);
	printf(1,"Time EST: %d-%d-%d %d:%d:%d\n", r.year, r.month, r.day, r.hour, r.minute, r.second);
	exit();
}