#ifndef calc2dcmd_mmBitmap_H_
#define calc2dcmd_mmBitmap_H_

#include <interfaces/mmIFormat.h>

namespace mmFormats {
	class mmBitmap : public mmFormatI {
	public:
		mmBitmap(void);
		virtual bool Read(mmString const & p_sFileName, mmImages::mmImageStructureI * const p_psImageStructure, mmString const & p_sName);
		virtual bool Write(mmString const & p_sFileName, mmImages::mmImageI const * const p_psImage);
	};
};

#endif