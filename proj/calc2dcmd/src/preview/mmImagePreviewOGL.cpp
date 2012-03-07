//---------------------------------------------------------------------------

#include <preview/mmImagePreviewOGL.h> //gl, glu
#include <algorithm>

#pragma comment ( lib, "opengl32.lib" )

#define GET_X_LPARAM( LPARAM ) static_cast<int>(LOWORD(LPARAM))
#define GET_Y_LPARAM( LPARAM ) static_cast<int>(HIWORD(LPARAM))

//---------------------------------------------------------------------------


GLfloat const mmImages::mmRGBPalette::_m_fBYR[] = 			{0.0f,0.0f,1.0f,0.0f, 1.0f,1.0f,0.0f,0.5f, 1.0f,0.0f,0.0f,1.0f, 1.0f,0.0f,1.0f};
GLfloat const mmImages::mmRGBPalette::_m_fBLUE[] = 			{1.0f,1.0f,1.0f,0.0f, 0.5f,0.7f,1.0f,0.5f, 0.0f,0.42f,1.0f,1.0f, 0.5f,0.5f,0.5f};
GLfloat const mmImages::mmRGBPalette::_m_fGRAYSCALE[] = 	{0.0f,0.0f,0.0f,0.0f, 0.5f,0.5f,0.5f,0.5f, 1.0f,1.0f,1.0f,1.0f, 1.0f,1.0f,0.0f};
GLfloat const mmImages::mmRGBPalette::_m_fTEMP[] = 			{0.0f,0.0f,1.0f,0.0f, 0.0f,1.0f,0.0f,0.25f, 0.5f,0.9f,0.0f,0.38f, 1.0f,0.0f,0.0f,0.5f, 1.0f,1.0f,0.0f,0.75f, 1.0f,1.0f,1.0f,1.0f, 1.0f,0.0f,1.0f};

mmImages::mmRGBPalette::mmRGBPalette( GLfloat const _p_fThresholds[], mmInt const p_iSize ) {
	Set( _p_fThresholds, p_iSize );
}
//---------------------------------------------------------------------------

void mmImages::mmRGBPalette::Set( GLfloat const _p_fThresholds[], mmInt const p_iSize  ) {
	GLfloat v_fDR(0.0), v_fDG(0.0), v_fDB(0.0);
	GLfloat v_fBottom(0.0), v_fTop(0.0);

	for( mmInt v_iI = 0; v_iI < p_iSize-1; ++v_iI ) {
		mmInt v_iBottom(static_cast<mmInt>(_p_fThresholds[v_iI*4+3]*256.0f));
		mmInt v_iTop(static_cast<mmInt>(_p_fThresholds[(v_iI+1)*4+3]*256.0f));
		if( v_iTop > 256 )
			v_iTop = 256;
		_m_fPalette[v_iBottom*3] = _p_fThresholds[v_iI*4];
		_m_fPalette[v_iBottom*3+1] = _p_fThresholds[v_iI*4+1];
		_m_fPalette[v_iBottom*3+2] = _p_fThresholds[v_iI*4+2];
		v_fDR = ( _p_fThresholds[(v_iI+1)*4] - _p_fThresholds[v_iI*4] ) / static_cast<GLfloat>((v_iTop-v_iBottom));
		v_fDG = ( _p_fThresholds[(v_iI+1)*4+1] - _p_fThresholds[v_iI*4+1] ) / static_cast<GLfloat>((v_iTop-v_iBottom));
		v_fDB = ( _p_fThresholds[(v_iI+1)*4+2] - _p_fThresholds[v_iI*4+2] ) / static_cast<GLfloat>((v_iTop-v_iBottom));
		for( mmInt v_iJ = v_iBottom+1; v_iJ < v_iTop; ++v_iJ ) {
			_m_fPalette[v_iJ*3] = _m_fPalette[(v_iJ-1)*3] + v_fDR;
			_m_fPalette[v_iJ*3+1] = _m_fPalette[(v_iJ-1)*3+1] + v_fDG;
			_m_fPalette[v_iJ*3+2] = _m_fPalette[(v_iJ-1)*3+2] + v_fDB;
		}
	}
	m_sDefaultColor.fR = _p_fThresholds[p_iSize*4];
	m_sDefaultColor.fG = _p_fThresholds[p_iSize*4+1];
	m_sDefaultColor.fB = _p_fThresholds[p_iSize*4+2];
}
//---------------------------------------------------------------------------

mmImages::mmImagePreviewOGLIMPL::mmImagePreviewOGLIMPL( wchar_t const _p_tcWindowTitle[] ):
	m_fPixelZoom(1.0f),
	_m_fPixels(NULL),
	m_iWidth(0), m_iHeight(0), m_iChannels(0),
	m_hWindow(NULL), m_hWindowDC(NULL), m_hWindowContext(NULL),
	m_bInitialized(false), 
	m_bVisible(false),
	m_sPalette(mmRGBPalette::_m_fTEMP,6)
{
	if( ! CreatePreviewWindow( _p_tcWindowTitle ) || ! InitializeDisplay() )
		throw mmError(0);
}
//---------------------------------------------------------------------------

mmImages::mmImagePreviewOGLIMPL::~mmImagePreviewOGLIMPL( void ) {
	DeinitializeDisplay();
	delete[] _m_fPixels;
}
//---------------------------------------------------------------------------

bool mmImages::mmImagePreviewOGLIMPL::InitializeDisplay( void ) {
	if( m_bInitialized )
		DeinitializeDisplay();

	if( m_hWindow == NULL )
		return false;
	if( ( m_hWindowDC = ::GetDC( m_hWindow ) ) == NULL )
		return false;
	if( ! SetPixelFormatDescriptor(  m_hWindowDC ) )
		return false;
	if( ( m_hWindowContext = ::wglCreateContext( m_hWindowDC ) ) == NULL )
		return false;
	if(::wglMakeCurrent( m_hWindowDC, m_hWindowContext ) != TRUE)
		return false;

	::glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
	::glClear( GL_COLOR_BUFFER_BIT );
	::glMatrixMode( GL_PROJECTION );
	::glLoadIdentity();
	::glOrtho( 0.0, 1.0, 0.0, 1.0, -1.0, 1.0 );
	::glMatrixMode( GL_MODELVIEW );
	::glLoadIdentity();
	::glPixelStorei( GL_PACK_ALIGNMENT, 1 );
	::glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

	m_bInitialized = true;

	return true;
}
//---------------------------------------------------------------------------

void mmImages::mmImagePreviewOGLIMPL::DeinitializeDisplay( void ) {
	if( m_bInitialized ) {
		HidePreviewWindow();
		::wglMakeCurrent( NULL, m_hWindowContext );
		::ReleaseDC( m_hWindow, m_hWindowDC );
		m_hWindowDC = NULL;
		::wglDeleteContext( m_hWindowContext );
		m_hWindowContext = NULL;
		m_bInitialized = false;
		::DestroyWindow( m_hWindow );
		::PostQuitMessage(0);
	}
}
//---------------------------------------------------------------------------

bool mmImages::mmImagePreviewOGLIMPL::IsDisplayInitialized( void ) {
	return m_bInitialized;
}
//---------------------------------------------------------------------------

bool mmImages::mmImagePreviewOGLIMPL::CreatePreviewWindow( wchar_t const _p_tcWindowTitle[] ) {

	WNDCLASS v_sWindowClass = {};
	::ZeroMemory( &v_sWindowClass, sizeof(v_sWindowClass) );

	v_sWindowClass.style = CS_HREDRAW | CS_VREDRAW;
	v_sWindowClass.lpfnWndProc = &mmImagePreviewOGLIMPL::MsgRouter;
	v_sWindowClass.hInstance = ::GetModuleHandle(NULL);
	v_sWindowClass.hIcon = ::LoadIcon(NULL,IDI_APPLICATION);
	v_sWindowClass.hCursor = ::LoadCursor(NULL,IDC_ARROW);
	v_sWindowClass.hbrBackground = HBRUSH(COLOR_WINDOW);
	v_sWindowClass.lpszClassName = L"OGX2DPreviewWndClass";

	ATOM v_sRegisteredClass = ::RegisterClass( &v_sWindowClass );

	m_hWindow = NULL;
	::SetLastError(ERROR_SUCCESS);
	if( v_sRegisteredClass != 0 )
		m_hWindow = ::CreateWindow( L"OGX2DPreviewWndClass", _p_tcWindowTitle, WS_POPUPWINDOW, 10, 10, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, v_sWindowClass.hInstance, static_cast<LPVOID>(this) );

	if(!m_hWindow)
	{
		DWORD err = ::GetLastError();
		err = err;
	}
	return true;
}
//---------------------------------------------------------------------------

void mmImages::mmImagePreviewOGLIMPL::ShowPreviewWindow( void ) {
	if( m_bInitialized ) {
		::ShowWindow( m_hWindow, SW_SHOW );
		m_bVisible = true;
	}
}
//---------------------------------------------------------------------------

void mmImages::mmImagePreviewOGLIMPL::HidePreviewWindow( void ) {
	if( m_bInitialized ) {
		::ShowWindow( m_hWindow, SW_HIDE );
		m_bVisible = false;
	}
}
//---------------------------------------------------------------------------

bool mmImages::mmImagePreviewOGLIMPL::IsPreviewWindowVisible( void ) {
	if( m_bInitialized )
		return m_bVisible;
	else
		return false;
}
//---------------------------------------------------------------------------

bool mmImages::mmImagePreviewOGLIMPL::ZoomIn( void ) {
	if( m_bInitialized && m_fPixelZoom > 0.1f ) {
		m_fPixelZoom *= 0.9f;
		::glPixelZoom( m_fPixelZoom, m_fPixelZoom );
		ResizeWindow( static_cast<int>(static_cast<GLfloat>(m_iWidth)*m_fPixelZoom), static_cast<int>(static_cast<GLfloat>(m_iHeight)*m_fPixelZoom) );
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------

bool mmImages::mmImagePreviewOGLIMPL::ZoomOut( void ) {
	if( m_bInitialized && m_fPixelZoom < 10.0f ) {
		m_fPixelZoom /= 0.9f;
		::glPixelZoom( m_fPixelZoom, m_fPixelZoom );
		ResizeWindow( static_cast<int>(static_cast<GLfloat>(m_iWidth)*m_fPixelZoom), static_cast<int>(static_cast<GLfloat>(m_iHeight)*m_fPixelZoom) );
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------

void mmImages::mmImagePreviewOGLIMPL::ResizeWindow( int p_iWidth, int p_iHeight ) {
	RECT v_sClientRect, v_sWindowRect;
	POINT v_sDiff;
	::GetClientRect( m_hWindow, &v_sClientRect );
	::GetWindowRect( m_hWindow, &v_sWindowRect );
	v_sDiff.x = (v_sWindowRect.right - v_sWindowRect.left) - v_sClientRect.right;
	v_sDiff.y = (v_sWindowRect.bottom - v_sWindowRect.top) - v_sClientRect.bottom;
	::MoveWindow( m_hWindow, v_sWindowRect.left, v_sWindowRect.top, p_iWidth + v_sDiff.x, p_iHeight + v_sDiff.y, TRUE );
}
//---------------------------------------------------------------------------

void mmImages::mmImagePreviewOGLIMPL::ResetView( mmInt p_iWidth, mmInt p_iHeight ) {
	if( m_bInitialized ) {
		::glViewport( 0, 0, static_cast<GLint>(p_iWidth), static_cast<GLint>(p_iHeight) );
		m_fPixelZoom = 1.0f;
		::glPixelZoom( 1.0f, 1.0f );
		::glRasterPos2i( 0, 0 );
		ResizeWindow( p_iWidth, p_iHeight );
	}
}
//---------------------------------------------------------------------------

void mmImages::mmImagePreviewOGLIMPL::RepaintDisplay( void ) {
	if( m_bInitialized && _m_fPixels != NULL && m_hWindowDC != NULL ) {
		::glClear( GL_COLOR_BUFFER_BIT );
		::glDrawPixels( m_iWidth, m_iHeight, m_eDisplayMode, GL_FLOAT, _m_fPixels );
		::glFlush();
		::SwapBuffers( m_hWindowDC );
	}
}
//---------------------------------------------------------------------------

void mmImages::mmImagePreviewOGLIMPL::DisplayImage( mmImages::mmImageI * const p_psImage, bool p_bResetView ) {
	PrepareImageData(p_psImage);
	if( p_bResetView )
		ResetView(m_iWidth, m_iHeight);
	RepaintDisplay();
}
//---------------------------------------------------------------------------

void mmImages::mmImagePreviewOGLIMPL::PrepareImageData( mmImages::mmImageI * const p_psImage) {
	m_iWidth = p_psImage->GetWidth();
	m_iHeight = p_psImage->GetHeight();
	m_iChannels = static_cast<mmInt>(p_psImage->GetPixelType());

	if(m_iChannels == 1) {
		mmPixel8 * _v_sPixels = new mmPixel8[ m_iWidth * m_iHeight];
		p_psImage->GetRows(0, m_iHeight, _v_sPixels);

		delete[] _m_fPixels;
		_m_fPixels = new GLfloat[m_iWidth * m_iHeight * 3];
		for( mmInt v_iY = 0; v_iY < m_iHeight; ++v_iY )
			for( mmInt v_iX = 0; v_iX < m_iWidth; ++v_iX )
				_m_fPixels[(m_iHeight - v_iY - 1) * m_iWidth + v_iX] =		static_cast<GLfloat>(_v_sPixels[v_iY * m_iWidth + v_iX].rI / 255.0);

		delete[] _v_sPixels;

		m_eDisplayMode = GL_LUMINANCE;
	} else {
		mmPixel24 * _v_sPixels = new mmPixel24[ m_iWidth * m_iHeight];
		p_psImage->GetRows(0, m_iHeight, _v_sPixels);

		delete[] _m_fPixels;
		_m_fPixels = new GLfloat[m_iWidth * m_iHeight * 3];
		for( mmInt v_iY = 0; v_iY < m_iHeight; ++v_iY )
			for( mmInt v_iX = 0; v_iX < m_iWidth; ++v_iX ) {
				_m_fPixels[((m_iHeight-v_iY-1)*m_iWidth+v_iX) * 3] =		static_cast<GLfloat>(_v_sPixels[v_iY * m_iWidth + v_iX].rR / 255.0);
				_m_fPixels[((m_iHeight-v_iY-1)*m_iWidth+v_iX) * 3 + 1] =	static_cast<GLfloat>(_v_sPixels[v_iY * m_iWidth + v_iX].rG / 255.0);
				_m_fPixels[((m_iHeight-v_iY-1)*m_iWidth+v_iX) * 3 + 2] =	static_cast<GLfloat>(_v_sPixels[v_iY * m_iWidth + v_iX].rB / 255.0);
			}

		delete[] _v_sPixels;

		m_eDisplayMode = GL_RGB;
	}
}
//---------------------------------------------------------------------------

void mmImages::mmImagePreviewOGLIMPL::DisplayDataLayer( mmImages::mmLayerI * const p_psLayer, bool p_bResetView ) {
	PrepareDataLayerData( p_psLayer );
	if( p_bResetView )
		ResetView( m_iWidth, m_iHeight );
	RepaintDisplay();
};
//---------------------------------------------------------------------------

void mmImages::mmImagePreviewOGLIMPL::PrepareDataLayerData( mmImages::mmLayerI * const p_psLayer ) {
	m_iWidth = p_psLayer->GetWidth();
	m_iHeight = p_psLayer->GetHeight();
	mmReal * _v_rPixels = new mmReal[ m_iWidth*m_iHeight ];
	p_psLayer->GetRows(0, m_iHeight, _v_rPixels);
	mmReal const v_rDefaultValue = p_psLayer->GetDefaultValue();
	mmImages::mmStats const v_sStats = p_psLayer->GetStats();
	mmReal v_rDLScale = (( v_rDLScale = v_sStats.rMax - v_sStats.rMin ) != 0.0) ? v_rDLScale : 1.0;

	delete[] _m_fPixels;
	_m_fPixels = new GLfloat[ m_iWidth*m_iHeight*3 ];
	mmReal v_rValue;
	mmInt v_iIndex;
	for( mmInt v_iY = 0; v_iY < m_iHeight; ++v_iY )
		for( mmInt v_iX = 0; v_iX < m_iWidth; ++v_iX ) {
			v_rValue = _v_rPixels[m_iWidth*v_iY+v_iX];
			if( ::fabs(v_rValue - v_rDefaultValue) < 1e-20 ) {
				_m_fPixels[((m_iHeight-v_iY-1)*m_iWidth+v_iX)*3] = m_sPalette.m_sDefaultColor.fR;
				_m_fPixels[((m_iHeight-v_iY-1)*m_iWidth+v_iX)*3+1] = m_sPalette.m_sDefaultColor.fG;
				_m_fPixels[((m_iHeight-v_iY-1)*m_iWidth+v_iX)*3+2] = m_sPalette.m_sDefaultColor.fB;
			} else {
				v_iIndex = static_cast<mmInt>(static_cast<GLfloat>((v_rValue-v_sStats.rMin)/v_rDLScale)*255.0f);
				_m_fPixels[((m_iHeight-v_iY-1)*m_iWidth+v_iX)*3] = m_sPalette._m_fPalette[v_iIndex*3];
				_m_fPixels[((m_iHeight-v_iY-1)*m_iWidth+v_iX)*3+1] = m_sPalette._m_fPalette[v_iIndex*3+1];
				_m_fPixels[((m_iHeight-v_iY-1)*m_iWidth+v_iX)*3+2] = m_sPalette._m_fPalette[v_iIndex*3+2];
			}
		}

	delete[] _v_rPixels;

	m_eDisplayMode = GL_RGB;
};
//---------------------------------------------------------------------------

bool mmImages::mmImagePreviewOGLIMPL::SetPixelFormatDescriptor( HDC p_hDeviceContext ) {
	int v_iPixelFormat;
	PIXELFORMATDESCRIPTOR v_sPSD = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1, // version of this data structure - should be set to 1
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		24,
		0,0,0,0,0,0,
		0,0,
		0,0,0,0,0,
		24, // depth of the Z-buffer
		0,0,
		PFD_MAIN_PLANE, // PFD_MAIN_PLANE PFD_OVERLAY_PLANE PFD_UNDERLAY_PLANE
		0,0,0,0
	};
	return ( v_iPixelFormat = ::ChoosePixelFormat( p_hDeviceContext, &v_sPSD ) ) != 0 && ::SetPixelFormat( p_hDeviceContext, v_iPixelFormat, &v_sPSD ) != 0;
}
//---------------------------------------------------------------------------
// DONE: figure out 
// http://msdn.microsoft.com/en-us/library/ms644898
//#define GWL_USERDATA        (-21)

LRESULT CALLBACK mmImages::mmImagePreviewOGLIMPL::MsgRouter( HWND p_hWindow, UINT p_uiMessage, WPARAM p_sWParam, LPARAM p_sLParam ) {
  mmImagePreviewOGLIMPL * _v_sPreview(NULL);

  if( p_uiMessage == WM_NCCREATE ) {
	_v_sPreview = reinterpret_cast<mmImagePreviewOGLIMPL*>( ((LPCREATESTRUCT) p_sLParam)->lpCreateParams );
	::SetWindowLong( p_hWindow, GWLP_USERDATA, reinterpret_cast<long>(_v_sPreview) );
  } else
	_v_sPreview = reinterpret_cast<mmImagePreviewOGLIMPL*>( ::GetWindowLong( p_hWindow, GWLP_USERDATA ) );

	_v_sPreview->m_hWindow = p_hWindow;
  if( _v_sPreview != NULL )
	return _v_sPreview->WndProc( p_uiMessage, p_sWParam, p_sLParam );
  else
	return ::DefWindowProc( p_hWindow, p_uiMessage, p_sWParam, p_sLParam );
}

LRESULT CALLBACK mmImages::mmImagePreviewOGLIMPL::WndProc( UINT p_uiMessage, WPARAM p_sWParam, LPARAM p_sLParam ) {
	PAINTSTRUCT v_sPaintStruct;
	switch( p_uiMessage ) {
		case WM_C2DP_DISPLAY_IMAGE: {
			DisplayImage(reinterpret_cast<mmImageI*>(p_sWParam), p_sLParam != 0);
			return 0;
		}
		case WM_C2DP_DISPLAY_LAYER: {
			DisplayDataLayer(reinterpret_cast<mmLayerI*>(p_sWParam), p_sLParam != 0);
			return 0;
		}
    	case WM_NCHITTEST: {
			LRESULT v_sHitTestResult;
			if( ( v_sHitTestResult = ::DefWindowProc( m_hWindow, p_uiMessage, p_sWParam, p_sLParam ) ) == HTCLIENT )
				return HTCAPTION;
			else
            	return v_sHitTestResult;
        }
		case WM_PAINT: {
			::BeginPaint( m_hWindow, &v_sPaintStruct );
			RepaintDisplay();
			::EndPaint( m_hWindow, &v_sPaintStruct );
			return 0;
        }
		case WM_MOUSEWHEEL: {
			if( GET_WHEEL_DELTA_WPARAM(p_sWParam) < 0 )
				ZoomOut();
			else
				ZoomIn();
			return 0;
        }
		case WM_LBUTTONDBLCLK: {
			ResetView( m_iWidth, m_iHeight );
			RepaintDisplay();
			return 0;
		}
		case WM_NCMBUTTONUP: {
			HidePreviewWindow();
			return 0;
        }
		case WM_CLOSE: {
			HidePreviewWindow();
            return 0;
        }
	}
	return ::DefWindowProc( m_hWindow, p_uiMessage, p_sWParam, p_sLParam );
}

// --------------- WRAPPER --------------

mmImages::mmImagePreviewOGL::mmImagePreviewOGL( wchar_t const _p_tcWindowTitle[] ) : m_sTitle(_p_tcWindowTitle), m_bExit(false) {
	m_hPreviewThread = ::CreateThread(NULL, 0, &mmImagePreviewOGL::PreviewFunc, reinterpret_cast<LPVOID>(this), 0, NULL);
}

DWORD WINAPI mmImages::mmImagePreviewOGL::PreviewFunc(LPVOID p_psParam) {
	mmImagePreviewOGL * v_psInstance = reinterpret_cast<mmImagePreviewOGL*>(p_psParam);

	v_psInstance->m_psPreview = new mmImagePreviewOGLIMPL(v_psInstance->m_sTitle.c_str());

	MSG v_sMessage;
	while(! v_psInstance->m_bExit) {
		while(::PeekMessageW(&v_sMessage, v_psInstance->m_psPreview->m_hWindow, 0, 0, PM_REMOVE)) {
			::TranslateMessage(&v_sMessage);
			::DispatchMessageW(&v_sMessage);
		}
		::Sleep(10);
	}

	delete v_psInstance->m_psPreview;

	return 0;
}

mmImages::mmImagePreviewOGL::~mmImagePreviewOGL( void ) {
	m_bExit = true;
	::WaitForSingleObject(m_hPreviewThread, INFINITE);
}

void mmImages::mmImagePreviewOGL::DisplayImage( mmImages::mmImageI * const p_psImage, bool p_bResetView) {
	::SendMessage(m_psPreview->m_hWindow, WM_C2DP_DISPLAY_IMAGE, reinterpret_cast<WPARAM>(p_psImage), static_cast<LPARAM>(p_bResetView));
}

void mmImages::mmImagePreviewOGL::DisplayDataLayer( mmImages::mmLayerI * const p_psLayer, bool p_bResetView) {
	::SendMessage(m_psPreview->m_hWindow, WM_C2DP_DISPLAY_LAYER, reinterpret_cast<WPARAM>(p_psLayer), static_cast<LPARAM>(p_bResetView));
}

void mmImages::mmImagePreviewOGL::ShowPreviewWindow( void ) {
	m_psPreview->ShowPreviewWindow();
}

void mmImages::mmImagePreviewOGL::HidePreviewWindow( void ) {
	m_psPreview->HidePreviewWindow();
}

bool mmImages::mmImagePreviewOGL::IsPreviewWindowVisible( void ) {
	return m_psPreview->IsPreviewWindowVisible();
}

bool mmImages::mmImagePreviewOGL::ZoomIn( void ) {
	return m_psPreview->ZoomIn();
}

bool mmImages::mmImagePreviewOGL::ZoomOut( void ) {
	return m_psPreview->ZoomOut();
}
