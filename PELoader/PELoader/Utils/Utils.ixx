export module utils;

import <filesystem>;
import <vector>;


export std::vector<std::byte> read_file(const std::filesystem::path& file_path);