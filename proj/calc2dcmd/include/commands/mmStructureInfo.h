#ifndef calc2dcmd_mmStructureInfo_H_
#define calc2dcmd_mmStructureInfo_H_

#include <commands/mmCommand.h>
#include <interfaces/mmIImages.h>

namespace mmCommands {
	class mmStructureInfo : public mmCommand {
	public:
		mmStructureInfo(mmImages::mmImageStructureI * const p_psImageStructure);
		virtual bool Run(std::wstring const & p_sName, std::vector<mmParam> const & p_sParams);
	private:
		void DisplayImageInfo(mmImages::mmImageI * const p_psImage);
	private:
		mmImages::mmImageStructureI * const m_psImageStructure;
	};
};

#endif