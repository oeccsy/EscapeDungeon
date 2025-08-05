#include "Delegate.h"

void Delegate::Invoke()
{
	if (!instance || !function) return;

	function(instance);
}