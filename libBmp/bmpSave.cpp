#include "libBmp.h"

#include <fstream>

using namespace std;

static void saveBitmapFileHeader(
	ostream* os,
	int width,
	int height) {
	__int32 bfSize = 54 + (width * 3 + 3) / 4 * 4 * height;
	__int16 bfReserved = 0;
	__int32 bfOffBits = 54;
	os->write("BM", 2);					// bfType "BM"
	os->write((char*)&bfSize, 4);		// ファイルサイズ
	os->write((char*)&bfReserved, 2);	// bfReserved1
	os->write((char*)&bfReserved, 2);	// bfReserved2
	os->write((char*)&bfOffBits, 4);	// ファイル先頭から画像データまでのオフセット
}

static void saveBitmapInfoHeader(
	ostream* os,
	unsigned __int32 biWidth,
	unsigned __int32 biHeight) {
	__int32 biSize = 40;
	__int16 biPlanes = 1;
	__int16 biBitCount = 24;
	__int32 biCompression = 0;
	__int32 biSizeImage = (biWidth * 3 + 3) / 4 * 4 * biHeight;
	__int32 bi_PixPerMeter = 3780;
	__int32 biClrUsed = 0;
	__int32 biClrImportant = 0;
	os->write((char*)&biSize, 4);
	os->write((char*)&biWidth, 4);
	os->write((char*)&biHeight, 4);
	os->write((char*)&biPlanes, 2);
	os->write((char*)&biBitCount, 2);
	os->write((char*)&biCompression, 4);
	os->write((char*)&biSizeImage, 4);
	os->write((char*)&bi_PixPerMeter, 4);
	os->write((char*)&bi_PixPerMeter, 4);
	os->write((char*)&biClrUsed, 4);
	os->write((char*)&biClrImportant, 4);
}

static void save24(ostream* os, unsigned char* data, int width, int height) {
	auto gap = (4 - width * 3 % 4) % 4;
	__int32 zero = 0;
	for (auto i = 0; i < height; i++) {
		os->write((char*)data + (height - 1 - i) * width * 3, width * 3);
		os->write((char*)&zero, gap);
	}
}

extern "C" void bmpSave(const char* filePath, unsigned char* data, int width, int height) {
	ofstream ofs(filePath, ios::binary);
	saveBitmapFileHeader(&ofs, width, height);
	saveBitmapInfoHeader(&ofs, width, height);
	save24(&ofs, data, width, height);
}
