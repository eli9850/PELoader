module;

#include <Windows.h>

#include <vector>
#include <iostream>

import exceptions;


module loader;

Loader::Loader(std::vector<std::byte> pe_data): m_pe_data(std::move(pe_data))
{}

void Loader::init_headers()
{
	m_dos_header = {reinterpret_cast<_IMAGE_DOS_HEADER*>(&m_pe_data[0])};
	m_file_header = reinterpret_cast<_IMAGE_FILE_HEADER*>(&m_pe_data[m_dos_header->e_lfanew + sizeof(IMAGE_NT_SIGNATURE)]);
	m_optional_header = reinterpret_cast<_IMAGE_OPTIONAL_HEADER64*>(&m_pe_data[m_dos_header->e_lfanew + sizeof(
		IMAGE_NT_SIGNATURE) + sizeof(_IMAGE_FILE_HEADER)]);
	m_section_headers = reinterpret_cast<_IMAGE_SECTION_HEADER*>(&m_pe_data[m_dos_header->e_lfanew + sizeof(
		IMAGE_NT_SIGNATURE) + sizeof(_IMAGE_FILE_HEADER) + sizeof(_IMAGE_OPTIONAL_HEADER64)]);
}

void Loader::map_file()
{
	auto allocation_size = m_optional_header->SizeOfImage;

	m_base_address = VirtualAlloc(nullptr, allocation_size, MEM_RESERVE, PAGE_READWRITE);

	if (nullptr == m_base_address)
	{
		throw LoaderException(GetLastError());
	}

	// Write all the data before the sections data
	if (nullptr == VirtualAlloc(m_base_address, m_optional_header[0].SizeOfHeaders, MEM_COMMIT, PAGE_READWRITE))
	{
		throw LoaderException(GetLastError());
	}

	if (nullptr == memcpy(m_base_address, &m_pe_data[0], m_optional_header[0].SizeOfHeaders))
	{
		throw LoaderException(GetLastError());
	}


	// Write the sections data
	for (size_t i = 0; i < m_file_header->NumberOfSections; i++)
	{
		auto section_real_address = reinterpret_cast<LPVOID>(reinterpret_cast<uint64_t>(m_base_address) +
			m_section_headers[i].VirtualAddress);
		if (nullptr == VirtualAlloc(section_real_address, m_section_headers[i].Misc.VirtualSize, MEM_COMMIT,
		                            PAGE_READWRITE))
		{
			throw LoaderException(GetLastError());
		}

		if (nullptr == memcpy(section_real_address, &m_pe_data[m_section_headers[i].PointerToRawData],
		                      m_section_headers[i].SizeOfRawData))
		{
			throw LoaderException(GetLastError());
		}
	}
}

void Loader::fix_permissions()
{
	/*for (size_t i = 0; i < m_file_header->NumberOfSections; i++)
	{
		auto permissions = m_section_headers[i].
		if (!VirtualProtect(section_real_address, m_section_headers[i].Misc.VirtualSize, MEM_COMMIT, PAGE_READWRITE)) {
			throw LoadException(GetLastError());
		}
	}*/
}

void Loader::load_file()
{
	init_headers();
	map_file();

	// fix import
	// fix export
	// fix relocations
	// fix exceptions
	// fix resources?
	// check what else need to fix
	fix_permissions(); // fix this function

	
}

void Loader::execute_dll_main() {
	dll_main main_function = reinterpret_cast<dll_main>(reinterpret_cast<uint64_t>(m_base_address) + m_optional_header->
		AddressOfEntryPoint);
	main_function(NULL, DLL_PROCESS_ATTACH, NULL);
}
