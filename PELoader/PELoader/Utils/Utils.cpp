module;

#include <filesystem>
#include <vector>
#include <fstream>

module utils;



std::vector<std::byte> read_file(const std::filesystem::path& file_path)
{
	auto file_size = std::filesystem::file_size(file_path);
	std::vector<std::byte> file_data(file_size);

	std::ifstream out(file_path, std::ios::binary);

	out.read(reinterpret_cast<char*>(file_data.data()), file_size);

	return file_data;
}
