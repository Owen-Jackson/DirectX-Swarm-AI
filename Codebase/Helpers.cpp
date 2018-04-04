#define NOMINMAX
#include "Helpers.h"
namespace Helpers
{
	float Clip(float num, float lower, float upper)
	{
		return std::max(lower, std::min(num, upper));
	}
}