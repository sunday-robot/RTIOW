#include "libBmp.h"

#include <iostream>
#include <fstream>

using namespace std;

static bool loadBitmapFileHeader(
	istream* is) {
	is->seekg(2, ios_base::cur);	// bfType "BM"
	is->seekg(4, ios_base::cur);	// ファイルサイズ
	is->seekg(2, ios_base::cur);	// bfReserved1
	is->seekg(2, ios_base::cur);	// bfReserved2
	is->seekg(4, ios_base::cur);	// ファイル先頭から画像データまでのオフセット
	return true;
}

static bool loadBitmapInfoHeader(
	istream* is,
	unsigned __int32* biWidth,
	unsigned __int32* biHeight,
	unsigned __int16* biBitCount) {
	is->seekg(4, ios_base::cur);	// biSize 40
	is->read((char*)biWidth, 4);
	is->read((char*)biHeight, 4);
	is->seekg(2, ios_base::cur);	// biPlanes 1
	is->read((char*)biBitCount, 2);	// 色ビット数
	is->seekg(4, ios_base::cur);	// biCompression
	is->seekg(4, ios_base::cur);	// biSizeImage
	is->seekg(4, ios_base::cur);	// biXPixPerMeter
	is->seekg(4, ios_base::cur);	// biYPixPerMeter
	is->seekg(4, ios_base::cur);	// biClrUsed
	is->seekg(4, ios_base::cur);	// biCirImportant
	return true;
}

void load24(istream* is, unsigned char* data, int width, int height) {
	auto gap = (4 - width * 3 % 4) % 4;
	for (auto i = 0; i < height; i++) {
		is->read((char*)data + (height - 1 - i) * width * 3, width * 3);
		is->seekg(gap, ios::cur);
	}
}

extern "C" bool bmpLoad(const char* filePath, unsigned char** data, int* width, int* height)
{
	ifstream ifs(filePath, ios::binary);
	if (!ifs) {
		cerr << "ファイル file.txt が開けません" << endl;
		return false;
	}

	loadBitmapFileHeader(&ifs);
	unsigned __int32 biWidth;
	unsigned __int32 biHeight;
	unsigned __int16 biBitCount;
	loadBitmapInfoHeader(&ifs, &biWidth, &biHeight, &biBitCount);

	void (*loadData)(istream*, unsigned char*, int, int);
	switch (biBitCount) {
	case 24:
		loadData = load24;
		break;
	default:
		cerr << "unsupported bit count : " << biBitCount << endl;
		goto l_err;
	}
	*data = new unsigned char[biWidth * biHeight * 3];
	*width = biWidth;
	*height = biHeight;
	loadData(&ifs, *data, *width, *height);

	ifs.close();
	return true;

l_err:
	ifs.close();
	return false;
}
