#include"CGameFrame.h"

CGameFrame::CGameFrame(HWND hWnd, HDC hBackbuffer) {
	this->hWnd = hWnd;
	this->hBackbuffer = hBackbuffer;

	dwAlphaTime = 0;

	hFadeDC = CreateBmpDC("Resource\\Fade.bmp");

	BlendFunction.BlendOp = AC_SRC_OVER;
	BlendFunction.BlendFlags = 0;
	BlendFunction.AlphaFormat = 0;
	BlendFunction.SourceConstantAlpha = nAlpha;

	bFadeIn = false;
	bFadeOut = false;
}

CGameFrame::~CGameFrame() {
	DeleteDC(hFadeDC);
}

HDC CGameFrame::CreateBmpDC(const char* filename) {
	HBITMAP hBitmap;
	HDC hdc = GetDC(hWnd);
	HDC hMemDC = CreateCompatibleDC(hdc);
	hBitmap = (HBITMAP)LoadImage(NULL, TEXT(filename), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

	SelectObject(hMemDC, hBitmap);
	DeleteObject(hBitmap);
	ReleaseDC(hWnd, hdc);
	return hMemDC;
}

void CGameFrame::FadeOut() { // °Ë°Ô
	if (nAlpha < 255) {
		nAlpha += 15;
		BlendFunction.SourceConstantAlpha = nAlpha;
	}
	else {
		bFadeOut = false;
	}
}

void CGameFrame::FadeIn() { // ¹à°Ô
	if (nAlpha > 0) {
		nAlpha -= 15;
		BlendFunction.SourceConstantAlpha = nAlpha;
	}
	else {
		bFadeIn = false;
	}
}