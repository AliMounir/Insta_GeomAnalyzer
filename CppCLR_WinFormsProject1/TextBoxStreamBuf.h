#pragma once

#include <streambuf>
#include <string>
#include <vcclr.h>

// Forward Declaration deiner Form-Klasse oder inkludieren der entsprechenden Header
// assuming your form is in namespace CppCLRWinFormsProject
namespace CppCLRWinFormsProject {
    ref class Form1;
}

class TextBoxStreamBuf : public std::streambuf {
public:
    TextBoxStreamBuf(CppCLRWinFormsProject::Form1^ form, System::Windows::Forms::TextBox^ textBox)
        : m_form(form), m_textBox(textBox) {
        // Set an initial put area
        setp(buffer, buffer + sizeof(buffer));
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
        std::streamsize space = (std::streamsize)(epptr() - pptr());
        if (n < space) {
            memcpy(pptr(), s, n);
            pbump((int)n);
            return n;
        }

        // else flush
        memcpy(pptr(), s, space);
        pbump((int)space);
        if (flushBuffer() == traits_type::eof()) {
            return (std::streamsize)0;
        }

        std::streamsize left = n - space;
        if (left < (std::streamsize)sizeof(buffer)) {
            memcpy(buffer, s + space, left);
            setp(buffer, buffer + sizeof(buffer));
            pbump((int)left);
            return n;
        }

        // If it's still too large, write directly without buffering.
        writeToTextBox(std::string(s + space, s + space + left));
        return n;
    }

private:
    int flushBuffer() {
        int num = (int)(pptr() - pbase());
        if (num > 0) {
            std::string output(pbase(), pbase() + num);
            writeToTextBox(output);
            setp(buffer, buffer + sizeof(buffer));
        }
        return num;
    }

    void writeToTextBox(const std::string& output) {
        if (!m_textBox || m_textBox->IsDisposed) {
            return; // Avoid writing to a disposed TextBox
        }

        std::string processedOutput = output;
        size_t pos = 0;
        while ((pos = processedOutput.find('\n', pos)) != std::string::npos) {
            processedOutput.replace(pos, 1, "\r\n");
            pos += 2;
        }

        System::String^ text = gcnew System::String(processedOutput.c_str());

        // Check TextBox length and truncate if necessary
        if (m_textBox->Text->Length + text->Length > 65534) { // Example max size
            m_textBox->Text = m_textBox->Text->Substring(text->Length);
        }

        m_textBox->AppendText(text);
    }

    // If you need a safe append via Invoke (if called from another thread):
    // void AppendTextSafe(System::String^ text) {
    //     m_textBox->AppendText(text);
    // }

    gcroot<CppCLRWinFormsProject::Form1^> m_form;
    gcroot<System::Windows::Forms::TextBox^> m_textBox;

    char buffer[1024];
};
