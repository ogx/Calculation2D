#ifndef calc2dcmd_mmQuit_H_
#define calc2dcmd_mmQuit_H_

#include <commands/mmCommand.h>

namespace mmCommands {
	class mmQuit : public mmCommand {
	public:
		mmQuit(void);
		virtual bool Run(std::wstring const & p_sName, std::vector<mmParam> const & p_sParams);
	};
};

#endif