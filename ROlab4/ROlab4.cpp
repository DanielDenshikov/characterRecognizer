// ROlab4.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include "CImg.h"
#include "CScaner.h"
#include <locale.h>

using namespace cimg_library;



int main()
{
	CScaner ScanText;
	ScanText.CreateAlphabet();
	//ScanText.DownloadAlphabet();
	ScanText.ScanText("text.bmp");
	ScanText.UploadAlphabet();

    return 0;
}

