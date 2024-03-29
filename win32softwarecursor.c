#include "win32softwarecursor.h"
#include <stdio.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

int win32softwarecursor(bool enable)
{
	static int setting = 0;
	static bool enabled = false;

	if(enabled == enable) // if no state change, return 0
		return 0;

	if(enable)
	{
		if(!SystemParametersInfo(SPI_GETMOUSETRAILS, 0, &setting, 0))
		{
			fprintf(stderr, "failed to read windows mouse trails settting\n");
			return -1;
		}

		if((setting == 0) || (setting == 1))
		{
			if(!SystemParametersInfo(SPI_SETMOUSETRAILS, -1, 0, SPIF_SENDCHANGE))
			{
				fprintf(stderr, "failed to set windows mouse trails setting to -1\n");
				return -2;
			}
		}

		enabled = true;
	}
	else
	{
		if((setting == 0) || (setting == 1))
		{
			if(!SystemParametersInfo(SPI_SETMOUSETRAILS, setting, 0, SPIF_SENDCHANGE))
			{
				fprintf(stderr, "failed to set windows mouse trails setting to %i\n", setting);
				return -1;
			}
		}

		enabled = false;
	}

	return 0;
}
