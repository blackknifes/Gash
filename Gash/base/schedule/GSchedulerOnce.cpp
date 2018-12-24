#include "GSchedulerOnce.h"



GSchedulerOnce::GSchedulerOnce(GTaskPtr pTask)
	:GScheduler(pTask)
{

}

void GSchedulerOnce::schedule()
{
	getTask()->execute();
}
