#include <WProgram.h>

int Marlin_main(void)
{
	//init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}

