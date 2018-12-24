#include "GConditionCancelable.h"



GConditionCancelable::GConditionCancelable()
	:GCondition(GCondition::Cancelable), m_checkflag(true)
{

}

void GConditionCancelable::cancel()
{
	m_checkflag = false;
}

bool GConditionCancelable::check()
{
	return m_checkflag;
}
