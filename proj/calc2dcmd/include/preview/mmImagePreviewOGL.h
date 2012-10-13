//---------------------------------------------------------------------------

#ifndef calc2dcmd_mmImagePreviewOGL_H_
#define calc2dcmd_mmImagePreviewOGL_H_

#include <interfaces/mmIImages.h>

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
//---------------------------------------------------------------------------

namespace mmImages {
	class mmRGBPalette {
		public:
			struct { GLfloat fR,fG,fB; } m_sDefaultColor;
			GLfloat _m_fPalette[256*3];
			mmRGBPalette( GLfloat const _p_rThresholds[], const mmInt p_iSize );
			void Set( GLfloat const _p_rThresholds[], const mmInt p_iSize );
			static GLfloat const _m_fBYR[];
			static GLfloat const _m_fTEMP[];
			static GLfloat const _m_fGRAYSCALE[];
			static GLfloat const _m_fBLUE[];
	};

	class mmImagePreviewOGLIMPL {
		public:
			GLfloat m_fPixelZoom;
			mmInt m_iWidth, m_iHeight, m_iChannels;
			GLfloat * _m_fPixels;
			GLenum m_eDisplayMode;
			HGLRC m_hWindowContext;
			HDC m_hWindowDC;
			HWND m_hWindow;
			bool m_bInitialized;
			bool m_bVisible;
			mmRGBPalette m_sPalette;

            bool CreatePreviewWindow( void );
			bool InitializeDisplay( void );
			void DeinitializeDisplay( void );
			bool SetPixelFormatDescriptor( HDC p_hDeviceContext );
			void ResetView( mmInt p_iWidth, mmInt p_iHeight );
			void PrepareImageData( mmImages::mmImageI * const p_psImage );
			void PrepareDataLayerData( mmImages::mmLayerI * const p_psLayer);
			static LRESULT CALLBACK MsgRouter( HWND p_hWindow, UINT p_uiMessage, WPARAM p_sWParam, LPARAM p_sLParam );
			LRESULT CALLBACK WndProc( UINT p_uiMessage, WPARAM p_sWParam, LPARAM p_sLParam );
			void ResizeWindow( int p_iWidth, int p_iHeight );
		public:
			mmImagePreviewOGLIMPL( wchar_t const _p_tcWindowTitle[] );
			~mmImagePreviewOGLIMPL( void );
			void DisplayImage( mmImages::mmImageI * const p_psImage, bool p_bResetView=true );
			void DisplayDataLayer( mmImages::mmLayerI * const p_psLayer, bool p_bResetView=true );
			void RepaintDisplay( void );
			bool IsDisplayInitialized( void );
			bool CreatePreviewWindow( wchar_t const _p_tcWindowTitle[] );
			void ShowPreviewWindow( void );
			void HidePreviewWindow( void );
			bool IsPreviewWindowVisible( void );
			bool ZoomIn( void );
			bool ZoomOut( void );
	};

#define WM_C2DP_DISPLAY_IMAGE (WM_APP + 1)
#define WM_C2DP_DISPLAY_LAYER (WM_APP + 2)

	class mmImagePreviewOGL {
	public:
		mmImagePreviewOGL( wchar_t const _p_tcWindowTitle[] );
		~mmImagePreviewOGL( void );
		void DisplayImage( mmImages::mmImageI * const p_psImage, bool p_bResetView=true);
		void DisplayDataLayer( mmImages::mmLayerI * const p_psLayer, bool p_bResetView=true);
		void ShowPreviewWindow( void );
		void HidePreviewWindow( void );
		bool IsPreviewWindowVisible( void );
		bool ZoomIn( void );
		bool ZoomOut( void );
	private:
		std::wstring m_sTitle;
		bool m_bExit;
		mmImagePreviewOGLIMPL * m_psPreview;
		HANDLE m_hPreviewThread;
		static DWORD WINAPI PreviewFunc(LPVOID p_psParam);
	};
};

#endif
