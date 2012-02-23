#pragma once

#include "interfaces/demointerface.h"

class Demo : public DemoInterface
{
public:
	bool isDemoVersion() const;
	int patientCountLimit() const;
	int examinationCountLimit() const;

};
