#include "Image.h"
#include "ImageIO_png.h"
#include "ImageIO_dds.h"

namespace SimpleGameEngine {

void Image::clear() {
	_info = ImageInfo();
	_pixelData.clear();
}

void Image::loadFile(StrView filename) {
	auto ext = FilePath::extension(filename);
	if (StringUtil::ignoreCaseCompare(ext, "png") == 0) {
		return loadPngFile(filename);
	}

	if (StringUtil::ignoreCaseCompare(ext, "dds") == 0) {
		return loadDdsFile(filename);
	}

	throw SGE_ERROR("Unsupported image file extension {}", ext);
}

void Image::loadPngFile(StrView filename) {
	MemMapFile mm;
	mm.open(filename);
	loadPngMem(mm);
}

void Image::loadPngMem(ByteSpan data) {
	ImageIO_png::Reader r;
	r.load(*this, data);
}

void Image::loadDdsFile(StrView filename) {
	MemMapFile mm;
	mm.open(filename);
	loadDdsMem(mm);
}

void Image::loadDdsMem(ByteSpan data) {
	ImageIO_dds::Reader r;
	r.load(*this, data);
}

void Image::create(ColorType colorType, int width, int height) {
	create(colorType, width, height, width * ColorUtil::pixelSizeInBytes(colorType));
}

void Image::create(ColorType colorType, int width, int height, int strideInBytes) {
	_create(colorType, width, height, strideInBytes, 1, strideInBytes * height);
}

void Image::create(ColorType colorType, int width, int height, int strideInBytes, int mipmapCount, size_t dataSizeInBytes) {
	_create(colorType, width, height, strideInBytes, mipmapCount, dataSizeInBytes);
}

void Image::_create(ColorType colorType, int width, int height, int strideInBytes, int mipmapCount, size_t dataSizeInBytes) {
	_info.colorType = colorType;
	_info.size.set(width, height);
	_info.strideInBytes = strideInBytes;
	_info.mipmapCount = mipmapCount;

	try {
		_pixelData.clear();
		_pixelData.resize(dataSizeInBytes);
	} catch (...) {
		clear();
		throw;
	}
}

} // namespace