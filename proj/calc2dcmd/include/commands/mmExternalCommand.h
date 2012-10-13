#ifndef calc2dcmd_mmExternalCommand_H_
#define calc2dcmd_mmExternalCommand_H_

#include <commands/mmCommand.h>
#include <interfaces/mmICalculationMethodContainer.h>
#include <interfaces/mmIImages.h>
#include <interfaces/mmInterfaceInitializers.h>
#include <images/mmImagesCalculationManagement.h>

namespace mmCommands {
	class mmExternalCommand : public mmCommand {
	public:
		mmExternalCommand(std::wstring const & p_sID, std::wstring const & p_sName, std::wstring const & p_sInputParams, mmImages::mmImagesCalculationMethodContainerI * const p_psMethodsContainer, mmImages::mmImagesCalculationManagement * const p_psCalculationManager, mmImages::mmImageStructureI * const p_psImageStructure);
		virtual bool Run(std::wstring const & p_sName, std::vector<mmParam> const & p_sParams);
	private:
		std::wstring const m_sID;
		mmImages::mmImagesCalculationMethodContainerI * const m_psMethodsContainer;
		mmImages::mmImageStructureI * const m_psImageStructure;
		mmImages::mmImagesCalculationManagement * const m_psCalculationManager;
	};
};

#endif