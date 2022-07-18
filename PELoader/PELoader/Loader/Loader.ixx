module;

import <vector>;

#include <Windows.h>

export module loader;


using dll_main = BOOL(*)(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved);


export class Loader
{
public:
	Loader(std::vector<std::byte> pe_data);
	void load_file();
	void execute_dll_main();

private:
	void init_headers();
	void map_file();
	void fix_permissions();


private:
	void* m_base_address;
	std::vector<std::byte> m_pe_data;
	_IMAGE_DOS_HEADER* m_dos_header;
	_IMAGE_FILE_HEADER* m_file_header;
	_IMAGE_OPTIONAL_HEADER64* m_optional_header;
	_IMAGE_SECTION_HEADER* m_section_headers;
};