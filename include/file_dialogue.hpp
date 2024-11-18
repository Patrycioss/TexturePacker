#pragma once

#include <string>
#include <nfd.h>
#include <iostream>

inline std::string choose_file_dialogue(const nfdopendialogu8args_t& open_args) {
	nfdu8char_t* outPath;
	const nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &open_args);

	if (result == NFD_OKAY) {
		std::string stringPath = std::string(outPath);
		NFD_FreePathU8(outPath);
		return stringPath;
	}

	std::cerr << "Failed to get file from dialogue" << std::endl;
	return "";
}