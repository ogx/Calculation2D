#ifndef calc2dcmd_mmMultiFormat_H_
#define calc2dcmd_mmMultiFormat_H_

#include <interfaces/mmIFormat.h>
#include <map>
#include <vector>

namespace mmFormats {
	class mmMultiFormat : public mmFormatI {
	public:
		mmMultiFormat(void);
		virtual bool Read(mmString const & p_sFileName, mmImages::mmImageStructureI * const p_psImageStructure, mmString const & p_sName);
		virtual bool Write(mmString const & p_sFileName, mmImages::mmImageI const * const p_psImage);

	private:
		std::vector<mmFormats::mmFormatI*> m_sFormats;
		std::map<mmString, mmFormats::mmFormatI*> m_sExtensions;
	};
};

#endif