#ifndef calc2dcmd_mmImageXML_H_
#define calc2dcmd_mmImageXML_H_

#include <formats/mmIFormat.h>

namespace mmFormats {
	class mmImageXML : public mmFormatI {
	public:
		mmImageXML(void);
		virtual bool Read(mmString const & p_sFileName, mmImages::mmImageStructureI * const p_psImageStructure, mmString const & p_sName);
		virtual bool Write(mmString const & p_sFileName, mmImages::mmImageI const * const p_psImage);
	};
};

#endif