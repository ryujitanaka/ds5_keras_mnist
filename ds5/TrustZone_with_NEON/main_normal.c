/*
==================================================================
 Copyright ARM Ltd 2009-2012. All rights reserved.

 Simple Normal World Application
==================================================================
*/

#include <stdio.h>

__smc(0) void yield(void);
extern void enableBranchPrediction(void);
extern void enableCaches(void);

int main(void)
{
	unsigned int loop;

// MMU was enabled earlier and scatterloading has now finished, so
// it is now safe to enable caches and branch prediction for each core
  enableBranchPrediction();
  enableCaches();

  for (loop = 0; loop < 10; loop++)
  {

    printf("Hello from Normal world\n");

    yield();
  }
  return 0;
}
