#ifndef calc2dcmd_mmLoad_H_
#define calc2dcmd_mmLoad_H_

#include <commands/mmCommand.h>
#include <interfaces/mmIImages.h>

namespace mmCommands {
	class mmLoad : public mmCommand {
	public:
		mmLoad(mmImages::mmImageStructureI * const p_psImageStructure);
		virtual bool Run(std::wstring const & p_sName, std::vector<mmParam> const & p_sParams);
	private:
		mmImages::mmImageStructureI * const m_psImageStructure;
	};
};

#endif