#ifndef calc2dcmd_mmImageUtils_H_
#define calc2dcmd_mmImageUtils_H_

#include <commands/mmCommand.h>

#include <interfaces/mmIImages.h>

namespace mmCommands {
	namespace mmImageUtils {
		class mmRename: public mmCommand {
		public:
			mmRename(mmImages::mmImageStructureI * const p_psImageStructure);
			virtual bool Run(std::wstring const & p_sName, std::vector<mmParam> const & p_sParams);
		private:
			mmImages::mmImageStructureI * const m_psImageStructure;
		};

		class mmResize: public mmCommand {
		public:
			mmResize(mmImages::mmImageStructureI * const p_psImageStructure);
			virtual bool Run(std::wstring const & p_sName, std::vector<mmParam> const & p_sParams);
		private:
			mmImages::mmImageStructureI * const m_psImageStructure;
		};

		class mmClean : public mmCommand {
		public:
			mmClean(mmImages::mmImageStructureI * const p_psImageStructure);
			virtual bool Run(std::wstring const & p_sName, std::vector<mmParam> const & p_sParams);
		private:
			mmImages::mmImageStructureI * const m_psImageStructure;
		};
	};
};

#endif