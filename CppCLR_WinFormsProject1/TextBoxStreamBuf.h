// TextBoxStreamBuf.h
#pragma once

#include <streambuf>
#include <string>
#include <vcclr.h>
#include <mutex>

using namespace System::Windows::Forms;

class TextBoxStreamBuf : public std::streambuf {
public:
    TextBoxStreamBuf(System::Action<System::String^>^ appendTextAction)
        : m_appendTextAction(appendTextAction), m_active(true) {}

protected:
    virtual int_type overflow(int_type c) override {
        if (c != traits_type::eof()) {
            char ch = static_cast<char>(c);
            m_buffer += ch;
            if (ch == '\n') {
                flushBuffer();
            }
        }
        return c;
    }

    virtual std::streamsize xsputn(const char* s, std::streamsize n) override {
        m_buffer.append(s, n);
        size_t pos;
        while ((pos = m_buffer.find('\n')) != std::string::npos) {
            std::string line = m_buffer.substr(0, pos + 1);
            m_buffer.erase(0, pos + 1);
            writeToTextBox(line);
        }
        return n;
    }

    virtual int sync() override {
        flushBuffer();
        return 0;
    }

private:
    void flushBuffer() {
        if (!m_buffer.empty()) {
            writeToTextBox(m_buffer);
            m_buffer.clear();
        }
    }

    void writeToTextBox(const std::string& output) {
        if (!m_active) return;

        // Convert '\n' to '\r\n' for Windows
        std::string processedOutput = output;
        size_t pos = 0;
        while ((pos = processedOutput.find('\n', pos)) != std::string::npos) {
            processedOutput.replace(pos, 1, "\r\n");
            pos += 2;
        }

        // Convert std::string to System::String^
        System::String^ text = gcnew System::String(processedOutput.c_str());

        // Invoke on UI thread
        if (m_appendTextAction) {
            m_appendTextAction->Invoke(text);
        }
    }

    gcroot<System::Action<System::String^>^> m_appendTextAction;
    bool m_active;
    std::string m_buffer;
};
