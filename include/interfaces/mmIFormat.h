#ifndef calc2dcmd_mmIFormat_H_
#define calc2dcmd_mmIFormat_H_

#include <list>

#include <interfaces\mmIImages.h>

namespace mmFormats {
	class mmFormatI {
	public:
		////////////////////////////////////////////////////////////////////////////////
		/// Returns list of file extensions supported by this format
		///
		/// @return list of supported formats as strings
		////////////////////////////////////////////////////////////////////////////////
		std::list<mmString> GetSupportedExensions(void) {
			return m_sSupportedExtensions;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// Loads image from specified file into images structure
		///
		/// @param[in] p_sFileName path to file
		/// @param[in] p_psImageStructure pointer to images structure which stores loaded data
		/// @param[in] p_sName name for newly loaded image
		/// @return true if success, false otherwise
		////////////////////////////////////////////////////////////////////////////////
		virtual bool Read(mmString const & p_sFileName, mmImages::mmImageStructureI * const p_psImageStructure, mmString const & p_sName) = 0;
		
		////////////////////////////////////////////////////////////////////////////////
		/// Saves specified image into file
		///
		/// @param[in] p_sFileName path to file
		/// @param[in] p_psImage pointer to image
		/// @return true if success, false otherwise
		////////////////////////////////////////////////////////////////////////////////
		virtual bool Write(mmString const & p_sFileName, mmImages::mmImageI const * const p_psImage) = 0;

		////////////////////////////////////////////////////////////////////////////////
		/// Virtual destructor
		////////////////////////////////////////////////////////////////////////////////
		virtual ~mmFormatI(void) {};
	protected:
		std::list<mmString> m_sSupportedExtensions;
	};
};

#endif