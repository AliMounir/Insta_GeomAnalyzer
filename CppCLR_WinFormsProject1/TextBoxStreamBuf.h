#pragma once

#include <streambuf>
#include <string>
#include <vcclr.h>
#include <mutex>

using namespace System::Windows::Forms;

class TextBoxStreamBuf : public std::streambuf {
public:
    TextBoxStreamBuf(System::Action<System::String^>^ appendTextAction)
        : m_appendTextAction(appendTextAction), m_active(true) {
        setp(buffer, buffer + sizeof(buffer) - 1); // Reserve space for null terminator
    }

protected:
    virtual int_type overflow(int_type c = traits_type::eof()) override {
        if (c != traits_type::eof()) {
            *pptr() = (char)c;
            pbump(1);
        }
        return flushBuffer() == traits_type::eof() ? traits_type::eof() : c;
    }

    virtual std::streamsize xsputn(const char* s, std::streamsize n) override {
        std::streamsize space = epptr() - pptr();
        if (n <= space) {
            std::memcpy(pptr(), s, n);
            pbump(static_cast<int>(n));
            return n;
        }

        // Flush the current buffer
        std::streamsize flushed = flushBuffer();
        if (flushed == traits_type::eof()) {
            return 0;
        }

        if (n <= sizeof(buffer)) {
            std::memcpy(buffer, s, n);
            setp(buffer, buffer + sizeof(buffer) - 1);
            pbump(static_cast<int>(n));
            return n;
        }

        // If the data is larger than the buffer, write directly
        writeToTextBox(std::string(s, s + n));
        return n;
    }

private:
    int flushBuffer() {
        int num = static_cast<int>(pptr() - pbase());
        if (num > 0) {
            std::string output(pbase(), pbase() + num);
            writeToTextBox(output);
            setp(buffer, buffer + sizeof(buffer) - 1);
        }
        return num;
    }

    void writeToTextBox(const std::string& output) {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (!m_active) {
            return;
        }

        // Konvertieren von '\n' zu '\r\n' für Windows
        std::string processedOutput = output;
        size_t pos = 0;
        while ((pos = processedOutput.find('\n', pos)) != std::string::npos) {
            processedOutput.replace(pos, 1, "\r\n");
            pos += 2;
        }

        // Konvertieren von std::string zu System::String^
        System::String^ text = gcnew System::String(processedOutput.c_str());

        // Aufrufen des Delegaten
        m_appendTextAction->Invoke(text);

        // Zusätzliche Testausgabe
        std::cout << "TextBoxStreamBuf hat Text verarbeitet.\n";
        std::cout.flush();
    }

    gcroot<System::Action<System::String^>^> m_appendTextAction;
    std::mutex m_mutex;
    bool m_active;
    char buffer[1024];
};
