#ifndef LEBLANC_RENDER_PASS_H
#define LEBLANC_RENDER_PASS_H

#include <string>
using namespace std;

namespace Leblanc
{
	class RenderPass
	{
	public:
		RenderPass(const string& pass_name);
		virtual ~RenderPass();

		virtual void Render() = 0;

	protected:
		string m_pass_name;
	};
}
#endif