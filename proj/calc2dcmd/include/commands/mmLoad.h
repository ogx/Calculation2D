#ifndef calc2dcmd_mmLoad_H_
#define calc2dcmd_mmLoad_H_

#include <map>

#include <commands/mmCommand.h>
#include <interfaces/mmIImages.h>
#include <formats/mmIFormat.h>

namespace mmCommands {
	class mmLoad : public mmCommand {
	public:
		mmLoad(mmImages::mmImageStructureI * const p_psImageStructure);
		virtual bool Run(std::wstring const & p_sName, std::vector<mmParam> const & p_sParams);
	private:
		mmImages::mmImageStructureI * const m_psImageStructure;
		std::vector<mmFormats::mmFormatI*> m_sFormats;
		std::map<mmString, mmFormats::mmFormatI*> m_sExtensions;
	};

	class mmSave : public mmCommand {
	public:
		mmSave(mmImages::mmImageStructureI * const p_psImageStructure);
		virtual bool Run(std::wstring const & p_sName, std::vector<mmParam> const & p_sParams);
	private:
		mmImages::mmImageStructureI * const m_psImageStructure;
		std::vector<mmFormats::mmFormatI*> m_sFormats;
		std::map<mmString, mmFormats::mmFormatI*> m_sExtensions;
	};
};

#endif