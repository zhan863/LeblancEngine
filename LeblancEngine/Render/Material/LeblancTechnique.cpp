#include "LeblancEngine/Render/Material/LeblancTechnique.h"

#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"

namespace Leblanc
{
	ITechnique::ITechnique(IDevice* device, IDeviceContext* device_context) :
		m_device(device), m_device_context(device_context)
	{

	}

	ITechnique::~ITechnique()
	{
		release();
	}

	void ITechnique::release()
	{
	}
}