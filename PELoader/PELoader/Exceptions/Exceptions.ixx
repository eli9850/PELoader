module;

import <exception>;
import  <sstream>;
#include <Windows.h>

export module exceptions;

export class LoaderException : public std::exception
{
public:
    LoaderException(int error_number) {
        std::stringstream s;
        s << "Failed with error: ";

        LPSTR message_buffer = nullptr;

        size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, error_number, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&message_buffer, 0, NULL);

        s << message_buffer;

        //Free the Win32's string's buffer.
        //TODO: make this raii
        LocalFree(message_buffer);

        m_last_error_message = s.str();
    }

    char const* what() const override {
        return m_last_error_message.c_str();
    }

private:
    std::string m_last_error_message;
};