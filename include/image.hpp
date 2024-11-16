#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <nfd.h>
#include <filesystem>

struct display_image;

struct image {
	enum image_type {
		rgb,
		rgba,
	} image_type;

	int width, height, channels;
	unsigned char* data;
};

static nfdu8filteritem_t image_filters[1] = {{"Images code", "png,jpg,jpeg"}};
static nfdopendialogu8args_t image_open_args = {image_filters, 1};

static std::string get_file_dialogue() {
	nfdu8char_t* outPath;
	const nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &image_open_args);

	if (result == NFD_OKAY) {
		std::string stringPath = std::string(outPath);
		NFD_FreePathU8(outPath);
		return stringPath;
	}

	std::cerr << "Failed to get file from dialogue" << std::endl;
	return "";
}

static bool load_image(const std::string& path, image* image) {
	const std::filesystem::path extension = std::filesystem::path(path).extension();

	if (extension == ".jpg" || extension == ".jpeg") {
		image->data = stbi_load(path.c_str(), &image->width, &image->height, &image->channels, 3);
		image->image_type = image::rgb;
	} else if (extension == ".png") {
		image->data = stbi_load(path.c_str(), &image->width, &image->height, &image->channels, 4);
		image->image_type = image::rgba;
	} else {
		std::cerr << "Unsupported image type with extension: " << extension << std::endl;
		return false;
	}

	if (!image->data) {
		std::cerr << "Failed to load image with path: " << path << std::endl;
		return false;
	}
	return true;
}

static bool load_image_from_dialogue(image* image) {
	const std::string image_path = get_file_dialogue();
	if (!image_path.empty()) {
		if (load_image(image_path, image)) {
			std::cout << "Loaded image from path: " << image_path << std::endl;
			return true;
		}
	}
	return false;
}
