#ifndef calc2dcmd_mmPreview_H_
#define calc2dcmd_mmPreview_H_

#include <commands/mmCommand.h>
#include <preview/mmImagePreviewOGL.h>

namespace mmCommands {
	class mmPreview : public mmCommand { 
	public:
		mmPreview(mmImages::mmImageStructureI * const p_psImageStructure);
		virtual bool Run(std::wstring const & p_sName, std::vector<mmParam> const & p_sParams);
	private:
		std::auto_ptr<mmImages::mmImagePreviewOGL> m_psImagePreview;
		mmImages::mmImageStructureI * const m_psImageStructure;
	};
};

#endif