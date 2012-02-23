#include "demo.h"


bool Demo::isDemoVersion() const
{
#ifdef DEMO
	return true;
#else
	return false;
#endif
}


int Demo::patientCountLimit() const
{
	return 10;
}


int Demo::examinationCountLimit() const
{
	return 10;
}
