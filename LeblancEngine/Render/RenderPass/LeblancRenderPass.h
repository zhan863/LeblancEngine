#ifndef LEBLANC_RENDER_PASS_H
#define LEBLANC_RENDER_PASS_H

#include <string>
using namespace std;

namespace Leblanc
{
	class PipelineState;
	class RenderPass
	{
	public:
		RenderPass();
		virtual ~RenderPass();

		virtual void render() = 0;

	protected:
		void clear();

		PipelineState* m_pipeline_state_cache = nullptr;

		string m_pass_name = "invalid_pass";
	};
}
#endif