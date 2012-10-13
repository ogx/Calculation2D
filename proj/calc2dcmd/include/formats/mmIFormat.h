#ifndef calc2dcmd_mmIFormat_H_
#define calc2dcmd_mmIFormat_H_

#include <list>

#include <interfaces\mmIImages.h>

namespace mmFormats {
	class mmFormatI {
	public:
		std::list<mmString> GetSupportedExensions(void) {
			return m_sSupportedExtensions;
		}
		virtual bool Read(mmString const & p_sFileName, mmImages::mmImageStructureI * const p_psImageStructure, mmString const & p_sName) = 0;
		virtual bool Write(mmString const & p_sFileName, mmImages::mmImageI const * const p_psImage) = 0;
		virtual ~mmFormatI(void) {};
	protected:
		std::list<mmString> m_sSupportedExtensions;
	};
};

#endif