#pragma once

#include "pch.h"
#include "SettingsForm.h"
#include "stepImporters.h"
#include "calcBasicInfo.h"
#include "geomAnalysen.h"
#include "GeomInfo.h"
#include "innenStruktur.h"
#include "verfahrensBerechnung.h"
#include "TextBoxStreamBuf.h"
#include <iostream>

#include <Graphic3d_GraphicDriver.hxx>
#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
#include <V3d_View.hxx>
#include <V3d_Viewer.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <Graphic3d_TypeOfShadingModel.hxx>
#include <WNT_Window.hxx>

#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBndLib.hxx>
#include <BRepTools.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopExp_Explorer.hxx>

#include <STEPControl_Reader.hxx>
#include <Interface_Static.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Edge.hxx>
#include <IFSelect_ReturnStatus.hxx>
#include <vector>
#include <streambuf>
#include <ostream>
#include <set>
#include <vcclr.h>
#include <string>




Handle(V3d_View) myView;
Handle(AIS_InteractiveContext) myAISContext;


namespace CppCLRWinFormsProject {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    public ref class Form1 : public System::Windows::Forms::Form
    {
    public:
        Form1(void)
        {
            InitializeComponent();

            myHelperEdges = new std::vector<Handle(AIS_Shape)>();

            std::cout.setf(std::ios::unitbuf);

            textBoxStreamBuf = new TextBoxStreamBuf(gcnew System::Action<System::String^>(this, &Form1::AppendTextToTextBox));
            oldBuf = std::cout.rdbuf(textBoxStreamBuf);
            std::cout << "Umleitung von std::cout erfolgreich.\n";
            std::cout.flush();
        }
    protected:
        ~Form1()
        {
            if (components)
            {
                delete components;
            }

            if (myHelperEdges != nullptr)
            {
                delete myHelperEdges;
                myHelperEdges = nullptr;
            }
        }
    
    private:
        TextBoxStreamBuf* textBoxStreamBuf;
        std::streambuf* oldBuf;
    private: System::Windows::Forms::Button^ button2;
    private: System::Windows::Forms::Button^ button4;
    private: System::Windows::Forms::Button^ button3;
    private: System::Windows::Forms::Button^ button5;
    private: System::Windows::Forms::CheckBox^ checkBox1;
           System::ComponentModel::Container^ components;

    private:
        std::vector<Handle(AIS_Shape)>* myHelperEdges;

    public:
        void AppendTextToTextBox(System::String^ text) {

            if (textBox2->InvokeRequired) {
                textBox2->BeginInvoke(gcnew Action<System::String^>(this, &Form1::AppendTextToTextBox), text);
            }
            else {
                if (!textBox2->IsDisposed) {
                    textBox2->AppendText(text);
                }
            }
        }

    private: System::Windows::Forms::Panel^ panel1;
    private: System::Windows::Forms::TextBox^ textBox1;
    private: System::Windows::Forms::TextBox^ textBox4;
    private: System::Windows::Forms::TextBox^ textBox5;
    private: System::Windows::Forms::TextBox^ textBox3;
    private: System::Windows::Forms::Button^ button1;
    private: System::Windows::Forms::TextBox^ textBox7;
    private: System::Windows::Forms::TextBox^ textBox6;
    private: System::Windows::Forms::TextBox^ textBox8;
    private: System::Windows::Forms::TextBox^ textBox9;
    private: System::Windows::Forms::TextBox^ textBox10;
    private: System::Windows::Forms::TextBox^ textBox2;


#pragma region Windows Form Designer generated code
        void InitializeComponent(void)
        {
            this->panel1 = (gcnew System::Windows::Forms::Panel());
            this->button4 = (gcnew System::Windows::Forms::Button());
            this->textBox7 = (gcnew System::Windows::Forms::TextBox());
            this->textBox6 = (gcnew System::Windows::Forms::TextBox());
            this->textBox5 = (gcnew System::Windows::Forms::TextBox());
            this->textBox4 = (gcnew System::Windows::Forms::TextBox());
            this->textBox1 = (gcnew System::Windows::Forms::TextBox());
            this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
            this->button1 = (gcnew System::Windows::Forms::Button());
            this->textBox2 = (gcnew System::Windows::Forms::TextBox());
            this->textBox3 = (gcnew System::Windows::Forms::TextBox());
            this->textBox8 = (gcnew System::Windows::Forms::TextBox());
            this->textBox9 = (gcnew System::Windows::Forms::TextBox());
            this->textBox10 = (gcnew System::Windows::Forms::TextBox());
            this->button2 = (gcnew System::Windows::Forms::Button());
            this->button3 = (gcnew System::Windows::Forms::Button());
            this->button5 = (gcnew System::Windows::Forms::Button());
            this->panel1->SuspendLayout();
            this->SuspendLayout();
            // 
            // panel1
            // 
            this->panel1->Controls->Add(this->checkBox1);
            this->panel1->Controls->Add(this->button4);
            this->panel1->Controls->Add(this->textBox7);
            this->panel1->Controls->Add(this->textBox6);
            this->panel1->Controls->Add(this->textBox5);
            this->panel1->Controls->Add(this->textBox4);
            this->panel1->Controls->Add(this->textBox1);
            this->panel1->Location = System::Drawing::Point(12, 12);
            this->panel1->Name = L"panel1";
            this->panel1->Size = System::Drawing::Size(1051, 539);
            this->panel1->TabIndex = 0;
            this->panel1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Form1::panel1_Paint);
            this->panel1->Resize += gcnew System::EventHandler(this, &Form1::panel1_Resize);
            // 
            // button4
            // 
            this->button4->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
            this->button4->Font = (gcnew System::Drawing::Font(L"Wingdings", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(2)));
            this->button4->ForeColor = System::Drawing::Color::Red;
            this->button4->Location = System::Drawing::Point(3, 1);
            this->button4->Margin = System::Windows::Forms::Padding(0);
            this->button4->Name = L"button4";
            this->button4->Size = System::Drawing::Size(28, 29);
            this->button4->TabIndex = 10;
            this->button4->TabStop = false;
            this->button4->Text = L"1";
            this->button4->UseVisualStyleBackColor = true;
            this->button4->Click += gcnew System::EventHandler(this, &Form1::button4_Click);
            // 
            // textBox7
            // 
            this->textBox7->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            this->textBox7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->textBox7->Location = System::Drawing::Point(1010, 3);
            this->textBox7->Name = L"textBox7";
            this->textBox7->Size = System::Drawing::Size(36, 27);
            this->textBox7->TabIndex = 5;
            this->textBox7->TabStop = false;
            this->textBox7->Text = L"1";
            this->textBox7->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // textBox6
            // 
            this->textBox6->BackColor = System::Drawing::SystemColors::ControlDarkDark;
            this->textBox6->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            this->textBox6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->textBox6->ForeColor = System::Drawing::SystemColors::InactiveBorder;
            this->textBox6->Location = System::Drawing::Point(864, 3);
            this->textBox6->Name = L"textBox6";
            this->textBox6->ReadOnly = true;
            this->textBox6->Size = System::Drawing::Size(140, 27);
            this->textBox6->TabIndex = 4;
            this->textBox6->TabStop = false;
            this->textBox6->Text = L"Toleranzen: 1e-";
            this->textBox6->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // textBox5
            // 
            this->textBox5->BackColor = System::Drawing::SystemColors::WindowFrame;
            this->textBox5->BorderStyle = System::Windows::Forms::BorderStyle::None;
            this->textBox5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.125F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->textBox5->ForeColor = System::Drawing::SystemColors::Window;
            this->textBox5->Location = System::Drawing::Point(918, 474);
            this->textBox5->Margin = System::Windows::Forms::Padding(2);
            this->textBox5->Multiline = true;
            this->textBox5->Name = L"textBox5";
            this->textBox5->ReadOnly = true;
            this->textBox5->Size = System::Drawing::Size(128, 63);
            this->textBox5->TabIndex = 3;
            this->textBox5->TabStop = false;
            this->textBox5->Text = L" MSP (00,00,00)\r\n HA1 (0/0/0)\r\n HA2 (0/0/0)\r\n HA3 (0/0/0)";
            this->textBox5->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
            // 
            // textBox4
            // 
            this->textBox4->BackColor = System::Drawing::SystemColors::WindowFrame;
            this->textBox4->BorderStyle = System::Windows::Forms::BorderStyle::None;
            this->textBox4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.125F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->textBox4->ForeColor = System::Drawing::SystemColors::Window;
            this->textBox4->Location = System::Drawing::Point(3, 35);
            this->textBox4->Margin = System::Windows::Forms::Padding(2);
            this->textBox4->Multiline = true;
            this->textBox4->Name = L"textBox4";
            this->textBox4->ReadOnly = true;
            this->textBox4->Size = System::Drawing::Size(86, 77);
            this->textBox4->TabIndex = 2;
            this->textBox4->TabStop = false;
            this->textBox4->Text = L" - Solids: 000\r\n - Shells: 000\r\n - Flächen: 000\r\n - Kanten: 000\r\n - Ecken: 000";
            this->textBox4->TextChanged += gcnew System::EventHandler(this, &Form1::textBox4_TextChanged);
            // 
            // textBox1
            // 
            this->textBox1->BackColor = System::Drawing::SystemColors::ControlDarkDark;
            this->textBox1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            this->textBox1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->textBox1->ForeColor = System::Drawing::SystemColors::Window;
            this->textBox1->Location = System::Drawing::Point(34, 3);
            this->textBox1->Name = L"textBox1";
            this->textBox1->Size = System::Drawing::Size(824, 27);
            this->textBox1->TabIndex = 1;
            this->textBox1->TabStop = false;
            this->textBox1->Text = L"C:\\Users\\Ali Mounir\\Desktop\\TestCads\\test-platte.stp";
            // 
            // checkBox1
            // 
            this->checkBox1->AutoSize = true;
            this->checkBox1->BackColor = System::Drawing::SystemColors::ControlDarkDark;
            this->checkBox1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->checkBox1->ForeColor = System::Drawing::SystemColors::ControlLightLight;
            this->checkBox1->Location = System::Drawing::Point(965, 450);
            this->checkBox1->Name = L"checkBox1";
            this->checkBox1->Size = System::Drawing::Size(83, 19);
            this->checkBox1->TabIndex = 11;
            this->checkBox1->TabStop = false;
            this->checkBox1->Text = L"Hilfslinien";
            this->checkBox1->UseVisualStyleBackColor = false;
            this->checkBox1->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBox1_CheckedChanged);
            // 
            // button1
            // 
            this->button1->BackColor = System::Drawing::Color::Lime;
            this->button1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->button1->ForeColor = System::Drawing::Color::Ivory;
            this->button1->Location = System::Drawing::Point(1068, 15);
            this->button1->Margin = System::Windows::Forms::Padding(0);
            this->button1->Name = L"button1";
            this->button1->Size = System::Drawing::Size(275, 40);
            this->button1->TabIndex = 4;
            this->button1->Text = L"Analysieren..";
            this->button1->UseVisualStyleBackColor = false;
            this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
            // 
            // textBox2
            // 
            this->textBox2->BackColor = System::Drawing::SystemColors::InfoText;
            this->textBox2->ForeColor = System::Drawing::SystemColors::Window;
            this->textBox2->Location = System::Drawing::Point(12, 557);
            this->textBox2->MaxLength = 150000;
            this->textBox2->Multiline = true;
            this->textBox2->Name = L"textBox2";
            this->textBox2->ReadOnly = true;
            this->textBox2->ScrollBars = System::Windows::Forms::ScrollBars::Both;
            this->textBox2->Size = System::Drawing::Size(1051, 241);
            this->textBox2->TabIndex = 1;
            // 
            // textBox3
            // 
            this->textBox3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.875F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->textBox3->Location = System::Drawing::Point(1068, 68);
            this->textBox3->Margin = System::Windows::Forms::Padding(2);
            this->textBox3->Multiline = true;
            this->textBox3->Name = L"textBox3";
            this->textBox3->ReadOnly = true;
            this->textBox3->RightToLeft = System::Windows::Forms::RightToLeft::No;
            this->textBox3->Size = System::Drawing::Size(275, 467);
            this->textBox3->TabIndex = 2;
            this->textBox3->TabStop = false;
            this->textBox3->Text = L"--\r\n";
            this->textBox3->TextChanged += gcnew System::EventHandler(this, &Form1::textBox3_TextChanged);
            // 
            // textBox8
            // 
            this->textBox8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->textBox8->Location = System::Drawing::Point(1068, 539);
            this->textBox8->Margin = System::Windows::Forms::Padding(2);
            this->textBox8->Multiline = true;
            this->textBox8->Name = L"textBox8";
            this->textBox8->ReadOnly = true;
            this->textBox8->RightToLeft = System::Windows::Forms::RightToLeft::No;
            this->textBox8->Size = System::Drawing::Size(275, 132);
            this->textBox8->TabIndex = 5;
            this->textBox8->TabStop = false;
            this->textBox8->Text = L" CNC-Fräsen: 00,00%\r\n CNC-Drehen: 00,00%\r\n CNC-Blech: 00,00%\r\n Biegeblech: 00,00%"
                L"\r\n Rohrlaser: 00,00%\r\n Additive Fertigung: 00,00%";
            // 
            // textBox9
            // 
            this->textBox9->BackColor = System::Drawing::SystemColors::MenuHighlight;
            this->textBox9->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            this->textBox9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->textBox9->ForeColor = System::Drawing::SystemColors::Window;
            this->textBox9->Location = System::Drawing::Point(1069, 709);
            this->textBox9->Name = L"textBox9";
            this->textBox9->Size = System::Drawing::Size(274, 36);
            this->textBox9->TabIndex = 6;
            this->textBox9->TabStop = false;
            this->textBox9->Text = L"Ergebnis Verfahren";
            this->textBox9->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->textBox9->TextChanged += gcnew System::EventHandler(this, &Form1::textBox9_TextChanged);
            // 
            // textBox10
            // 
            this->textBox10->BorderStyle = System::Windows::Forms::BorderStyle::None;
            this->textBox10->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->textBox10->Location = System::Drawing::Point(1097, 687);
            this->textBox10->Margin = System::Windows::Forms::Padding(2);
            this->textBox10->Multiline = true;
            this->textBox10->Name = L"textBox10";
            this->textBox10->ReadOnly = true;
            this->textBox10->RightToLeft = System::Windows::Forms::RightToLeft::No;
            this->textBox10->Size = System::Drawing::Size(222, 17);
            this->textBox10->TabIndex = 7;
            this->textBox10->TabStop = false;
            this->textBox10->Text = L"am besten für das Bauteil geeignet:";
            this->textBox10->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // button2
            // 
            this->button2->Font = (gcnew System::Drawing::Font(L"Wingdings", 13.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(2)));
            this->button2->ForeColor = System::Drawing::Color::Black;
            this->button2->Location = System::Drawing::Point(1309, 764);
            this->button2->Margin = System::Windows::Forms::Padding(0);
            this->button2->Name = L"button2";
            this->button2->Size = System::Drawing::Size(34, 34);
            this->button2->TabIndex = 8;
            this->button2->Text = L"<";
            this->button2->UseVisualStyleBackColor = true;
            this->button2->Visible = false;
            this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
            // 
            // button3
            // 
            this->button3->Font = (gcnew System::Drawing::Font(L"Wingdings", 18, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(2)));
            this->button3->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
                static_cast<System::Int32>(static_cast<System::Byte>(64)));
            this->button3->Location = System::Drawing::Point(1069, 765);
            this->button3->Margin = System::Windows::Forms::Padding(0);
            this->button3->Name = L"button3";
            this->button3->Size = System::Drawing::Size(34, 34);
            this->button3->TabIndex = 9;
            this->button3->Text = L"]";
            this->button3->UseVisualStyleBackColor = true;
            this->button3->Visible = false;
            this->button3->Click += gcnew System::EventHandler(this, &Form1::button3_Click);
            // 
            // button5
            // 
            this->button5->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
            this->button5->Font = (gcnew System::Drawing::Font(L"Wingdings", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(2)));
            this->button5->ForeColor = System::Drawing::Color::Navy;
            this->button5->Location = System::Drawing::Point(842, 15);
            this->button5->Margin = System::Windows::Forms::Padding(0);
            this->button5->Name = L"button5";
            this->button5->Size = System::Drawing::Size(28, 29);
            this->button5->TabIndex = 11;
            this->button5->TabStop = false;
            this->button5->Text = L"ì";
            this->button5->UseVisualStyleBackColor = true;
            this->button5->Visible = false;
            this->button5->Click += gcnew System::EventHandler(this, &Form1::button5_Click);
            // 
            // Form1
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(1349, 808);
            this->Controls->Add(this->button5);
            this->Controls->Add(this->button3);
            this->Controls->Add(this->button2);
            this->Controls->Add(this->textBox10);
            this->Controls->Add(this->textBox9);
            this->Controls->Add(this->textBox8);
            this->Controls->Add(this->button1);
            this->Controls->Add(this->textBox3);
            this->Controls->Add(this->textBox2);
            this->Controls->Add(this->panel1);
            this->Name = L"Form1";
            this->Text = L"BA - Bauteil Analyse & Verfahrensidentifikation";
            this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &Form1::Form1_FormClosing);
            this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
            this->Shown += gcnew System::EventHandler(this, &Form1::Form1_Shown);
            this->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Form1::Form1_Paint);
            this->panel1->ResumeLayout(false);
            this->panel1->PerformLayout();
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
    private: System::Void Form1_Load(System::Object^ sender, System::EventArgs^ e) {

        // Entfernen des Konsolenfensters
     /*if (AllocConsole()) {
         FILE* pCout;
         freopen_s(&pCout, "CONOUT$", "w", stdout);
     }*/
        // Beispielausgabe
        std::cout << "Anwendung gestartet..." << std::endl;

        Handle(Graphic3d_GraphicDriver) aGraphicDriver2 = new OpenGl_GraphicDriver(Handle(Aspect_DisplayConnection)());
        TCollection_ExtendedString aName("OCC Viewer");
        Handle(V3d_Viewer) myViewer = new V3d_Viewer(aGraphicDriver2);

        myViewer->SetDefaultLights();
        myViewer->SetLightOn();
        Graphic3d_TypeOfShadingModel model = myViewer->DefaultShadingModel();

        Handle(WNT_Window) aWNTWindow;
        IntPtr pointer = this->panel1->Handle;
        HWND hw1 = (HWND)pointer.ToPointer();
        aWNTWindow = new WNT_Window(hw1, Quantity_NOC_BLACK);
        bool result = aWNTWindow->IsMapped();

        myView = myViewer->CreateView();
        myView->SetWindow(aWNTWindow);

        myAISContext = new AIS_InteractiveContext(myViewer);

        myView->FitAll();
        myView->Redraw();
        myView->MustBeResized();

        gp_Pnt ursprung(0, 0, 0);
        TopoDS_Vertex vertex1 = BRepBuilderAPI_MakeVertex(ursprung);
        Handle(AIS_Shape) originPoint = new AIS_Shape(vertex1);
        myAISContext->Display(originPoint, true);

        myAISContext->SetDisplayMode(AIS_Shaded, Standard_True);

        myView->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_WHITE, 0.08, V3d_ZBUFFER);
        gp_Pnt origin(0, 0, 0);
        //Standard_Real axisLength = largestDimension * 1.1;
        Standard_Real axisLength = 20;
        gp_Pnt xAxisEnd(axisLength, 0, 0);
        TopoDS_Edge xAxisEdge = BRepBuilderAPI_MakeEdge(origin, xAxisEnd);
        Handle(AIS_Shape) aisXAxis = new AIS_Shape(xAxisEdge);
        myAISContext->SetColor(aisXAxis, Quantity_NOC_RED, Standard_False);
        myAISContext->Display(aisXAxis, Standard_False);
        gp_Pnt yAxisEnd(0, axisLength, 0);
        TopoDS_Edge yAxisEdge = BRepBuilderAPI_MakeEdge(origin, yAxisEnd);
        Handle(AIS_Shape) aisYAxis = new AIS_Shape(yAxisEdge);
        myAISContext->SetColor(aisYAxis, Quantity_NOC_GREEN, Standard_False);
        myAISContext->Display(aisYAxis, Standard_False);
        gp_Pnt zAxisEnd(0, 0, axisLength);
        TopoDS_Edge zAxisEdge = BRepBuilderAPI_MakeEdge(origin, zAxisEnd);
        Handle(AIS_Shape) aisZAxis = new AIS_Shape(zAxisEdge);
        myAISContext->SetColor(aisZAxis, Quantity_NOC_BLUE1, Standard_False);
        myAISContext->Display(aisZAxis, Standard_False);
        myView->FitAll();
        myView->Redraw();
        myView->MustBeResized();
        myAISContext->UpdateCurrentViewer();

        

    }
    private: System::Void panel1_Resize(System::Object^ sender, System::EventArgs^ e) {

        if (myView.IsNull() == false) {
            myView->MustBeResized();
        }
    }
    private: System::Void Form1_Shown(System::Object^ sender, System::EventArgs^ e) {

        if (myView.IsNull() == false) {
            myView->Redraw();
        }
    }
    private: System::Void Form1_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
        if (myView.IsNull() == false) {
            myView->Redraw();
        }
    }
    private: System::Void panel1_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
        if (myView.IsNull() == false) {
            myView->Redraw();
        }
    }
    private: System::Void textBox1_TextChanged(System::Object^ sender, System::EventArgs^ e) {
    }
    private: System::Windows::Forms::TextBox^ textBoxFilePath;

    

    private: System::Void textBox4_TextChanged(System::Object^ sender, System::EventArgs^ e) {
    }
    private: void ResetForNewAnalysis() {

        std::cout.flush();
        if (textBoxStreamBuf) {
            std::cout.rdbuf(oldBuf);
            delete textBoxStreamBuf;
            textBoxStreamBuf = nullptr;
        }
        textBox2->Clear();

        // Clear old shapes
        std::cout << "Analysis started.. " << std::endl;
        myAISContext->EraseAll(Standard_True);
        myAISContext->UpdateCurrentViewer();
        myView->FitAll();
        myView->Redraw();
        myView->MustBeResized();

        // Redraw coordinate system
        gp_Pnt origin(0, 0, 0);
        TopoDS_Vertex vertex1 = BRepBuilderAPI_MakeVertex(origin);
        Handle(AIS_Shape) originPoint = new AIS_Shape(vertex1);
        myAISContext->Display(originPoint, true);
        Standard_Real axisLength = 20;
        gp_Pnt xAxisEnd(axisLength, 0, 0);
        TopoDS_Edge xAxisEdge = BRepBuilderAPI_MakeEdge(origin, xAxisEnd);
        Handle(AIS_Shape) aisXAxis = new AIS_Shape(xAxisEdge);
        myAISContext->SetColor(aisXAxis, Quantity_NOC_RED, Standard_False);
        myAISContext->Display(aisXAxis, Standard_False);
        gp_Pnt yAxisEnd(0, axisLength, 0);
        TopoDS_Edge yAxisEdge = BRepBuilderAPI_MakeEdge(origin, yAxisEnd);
        Handle(AIS_Shape) aisYAxis = new AIS_Shape(yAxisEdge);
        myAISContext->SetColor(aisYAxis, Quantity_NOC_GREEN, Standard_False);
        myAISContext->Display(aisYAxis, Standard_False);
        gp_Pnt zAxisEnd(0, 0, axisLength);
        TopoDS_Edge zAxisEdge = BRepBuilderAPI_MakeEdge(origin, zAxisEnd);
        Handle(AIS_Shape) aisZAxis = new AIS_Shape(zAxisEdge);
        myAISContext->SetColor(aisZAxis, Quantity_NOC_BLUE1, Standard_False);
        myAISContext->Display(aisZAxis, Standard_False);
        myAISContext->UpdateCurrentViewer();
        myAISContext->UpdateCurrentViewer();

        std::cout.flush();
    }


private: void PerformAnalysis() {
    try {
        std::cout << "Start der Analyse..." << std::endl;
        std::cout.flush();

        // Read the STEP file path from textBox1
        System::String^ managedFilePath = textBox1->Text;
        pin_ptr<const wchar_t> wch = PtrToStringChars(managedFilePath);
        int len = managedFilePath->Length;
        std::string filePath;
        filePath.reserve(len);
        for (int i = 0; i < len; i++) {
            filePath.push_back(static_cast<char>(wch[i]));
        }
        Standard_CString stepFilePath = filePath.c_str();

        if (filePath.empty()) {
            MessageBox::Show("No STEP file path provided!");
            return;
        }

        int tolerance;
        if (System::Int32::TryParse(textBox7->Text, tolerance)) {
            std::cout << "Tolerance inputted is: " << tolerance << std::endl;
        }
        else {
            MessageBox::Show("INVALID TOLERANCE ENTERED");
            return;
        }

        //Standard_CString stepFilePath = "C:\\Users\\Ali Mounir\\Desktop\\TestCads\\test-platte.stp";
        int samplingAmount = 16; // am besten immer eine Zahl teilbar durch 4
        //this->textBox1->Text = gcnew System::String(stepFilePath);
        TopoDS_Shape importedShape = ImportSTEP(stepFilePath);
        if (importedShape.IsNull()) {
            std::cout << "Failed to import the STEP file!" << std::endl;
            return;
        }
        std::cout << "Analyzing STEP file: " << stepFilePath << std::endl;
        //BRepTools::Clean(importedShape);
        STEPInfo stepInfo = InspectShape(importedShape);
        TopoDS_Shape centeredShape = CenterShape(importedShape);
        GeomInfo bauteilInfo = getBasicInfo(centeredShape);

        gp_Pnt centerMass = bauteilInfo.centerMass;
        gp_Pnt centerBox = bauteilInfo.centerGeom;
        double smallestDimension = std::min({ bauteilInfo.xDimension, bauteilInfo.yDimension, bauteilInfo.zDimension });
        double largestDimension = std::max({ bauteilInfo.xDimension, bauteilInfo.yDimension, bauteilInfo.zDimension });
        std::cout << "smallest dimension: " << smallestDimension << std::endl;


        //BESTIMMUNG DER ORIENTIERUNGSACHSE DURCH DIE HAUPTTRÄGHEITSAHCSEN
        gp_Dir mainAxis;
        gp_Dir midAxis;
        gp_Dir minAxis;
        double länge;
        double durchmesser1;
        double durchmesser2;
        double eps = 2.0;
        gp_Dir firstAxis = GetPrincipalInertiaAxes(centeredShape)[0];
        gp_Dir secondAxis = GetPrincipalInertiaAxes(centeredShape)[1];
        gp_Dir thirdAxis = GetPrincipalInertiaAxes(centeredShape)[2];
        double firstLength = bauteilInfo.xDimension;
        double secondLength = bauteilInfo.yDimension;
        double thirdLength = bauteilInfo.zDimension;
        std::cout << "Länge1: " << firstLength << "    Länge2: " << secondLength << "    Länge3: " << thirdLength << std::endl;

        bool allSimilar = (std::fabs(firstLength - secondLength) < eps) && (std::fabs(secondLength - thirdLength) < eps);
        bool twoSimilarAndThirdSmaller = (((std::fabs(firstLength - secondLength) < eps) && (thirdLength < firstLength)) ||
                                         ((std::fabs(firstLength - thirdLength) < eps) && (secondLength < firstLength)) ||
                                         ((std::fabs(secondLength - thirdLength) < eps) && (firstLength < secondLength)));
        bool twoSimilarAndThirdLarger = (((std::fabs(firstLength - secondLength) < eps) && (thirdLength > firstLength)) ||
                                        ((std::fabs(firstLength - thirdLength) < eps) && (secondLength > firstLength)) ||
                                        ((std::fabs(secondLength - thirdLength) < eps) && (firstLength > secondLength)));
        std::cout << "allSimilar: " << allSimilar << "  twoSimilarThirdSmaller: " << twoSimilarAndThirdSmaller << "  twoSimilarThirdLarger: " << twoSimilarAndThirdLarger << std::endl;
        if (allSimilar || twoSimilarAndThirdSmaller) {
            mainAxis = firstAxis;
            midAxis = secondAxis;
            minAxis = thirdAxis;
            std::cout << "Szenario A: Alle drei Dimensionen sind ähnlich oder zwei sind gleich und die dritte ist kleiner." << std::endl;
            länge = firstLength;
            durchmesser1 = secondLength;
            durchmesser2 = thirdLength;
        }
        else if (twoSimilarAndThirdLarger) {
            mainAxis = thirdAxis;
            midAxis = secondAxis;
            minAxis = firstAxis;
            std::cout << "Szenario B: Zwei Dimensionen sind ähnlich und die dritte ist größer." << std::endl;
            länge = thirdLength;
            durchmesser1 = secondLength;
            durchmesser2 = firstLength;
        }
        else {
            mainAxis = firstAxis;
            midAxis = secondAxis;
            minAxis = thirdAxis;
            std::cout << "Szenario C: undefined/else" << std::endl;
            länge = firstLength;
            durchmesser1 = secondLength;
            durchmesser2 = thirdLength;
        }
        std::cout << "Länge: " <<länge << "    durchmesser 1: " << durchmesser1 << "    durchmesser 2: " << durchmesser2 << std::endl;


        gp_Pnt anfPunkt = centerBox.Translated(mainAxis.XYZ() * ((länge * 0.5) - 0.1));
        gp_Pnt endPunkt = centerBox.Translated(mainAxis.XYZ() * ((-länge * 0.5) + 0.1));

        double parallelitätGroßFlächen = FindLargestFacesAndCheckParallel(centeredShape).first;
        double parallelitätDistance = FindLargestFacesAndCheckParallel(centeredShape).second;
        
        std::vector<gp_Lin> samplingLines0;//un-used
        std::vector<gp_Lin> samplingLines1;
        std::vector<gp_Lin> samplingLines2;
        std::vector<gp_Lin> samplingLines3;
        std::vector<gp_Lin> samplingLines4;
        std::vector<gp_Lin> samplingLines5;
        std::vector<gp_Lin> samplingLines6;
        double symmetrieMid = CheckLinSymmetry(centeredShape, mainAxis, midAxis, samplingAmount * 2, samplingLines2);
        double symmetrieMin = CheckLinSymmetry(centeredShape, mainAxis, minAxis, samplingAmount * 2, samplingLines3);
        double gesamtSymmetrie = (symmetrieMin + symmetrieMid) / 2;
        double rotSymA = CheckRotSymmetry(centeredShape, mainAxis, midAxis, anfPunkt, samplingAmount, samplingLines4);
        double rotSymB = CheckRotSymmetry(centeredShape, mainAxis, midAxis, centerBox, samplingAmount, samplingLines4);
        double rotSymC = CheckRotSymmetry(centeredShape, mainAxis, midAxis, endPunkt, samplingAmount, samplingLines4);
        double rotSymGes = (rotSymA + rotSymB + rotSymC) / 3;
        
        

        double wandStaerke = FindWandstarke(centeredShape, mainAxis, midAxis, minAxis, samplingAmount, samplingLines1)[0];
        double wandPercent = FindWandstarke(centeredShape, mainAxis, midAxis, minAxis, samplingAmount, samplingLines1)[1];
        if (wandStaerke > smallestDimension) {
            samplingLines0.clear();
            samplingLines1.clear();
            std::cout << "Berechnete Wandstärke ist größer als kleinstest Bauteilabmaß, wechsele Berechnung auf ThirdAxisOfInertia()." << std::endl;
            wandStaerke = FindWandstarke(centeredShape, mainAxis, minAxis, midAxis, samplingAmount, samplingLines1)[0];
            wandPercent = FindWandstarke(centeredShape, mainAxis, minAxis, midAxis, samplingAmount, samplingLines1)[1];
        } else {
            std::cout << "Berechnete Wandstärke besteht so!! (1/2)" << std::endl;
        }

        if(wandStaerke > smallestDimension) {
            samplingLines0.clear();
            samplingLines1.clear();
            std::cout << "Berechnete Wandstärke ist größer als kleinstest Bauteilabmaß, wechsele Berechnung auf SecondAxisOfInertia()." << std::endl;
            wandStaerke = FindWandstarke(centeredShape, minAxis, midAxis, mainAxis, samplingAmount, samplingLines1)[0];
            wandPercent = FindWandstarke(centeredShape, minAxis, midAxis, mainAxis, samplingAmount, samplingLines1)[1];
        }
        else {
            std::cout << "Berechnete Wandstärke besteht so!! (2/2)" << std::endl;
        }
        
        double umfang = calcUmfang(centeredShape, wandStaerke);


        double profileScore = checkForProfile(centeredShape, mainAxis, samplingAmount * 2, samplingLines5);
        
        KurvenInfo curvInfo = checkCurvePercentage(centeredShape);


        BiegeInfo biegInfo = checkBiegung(centeredShape);


        if (biegInfo.BiegeProbability > 0.97 || wandStaerke < 0 || wandPercent < 0.2) {
            std::cout << "Biegewahrscheinlichkeit sehr hoch, Wandstärke wird als distanz zwischen zwei größten flächen gesetzt" << std::endl;
            std::cout << "ODER wandstärke konnte nicht gefunden werden modevalue -1 BZW. wandPercent confidence nicht hoch genug" << std::endl;
            wandStaerke = parallelitätDistance;
            wandPercent = 0.6;
        }

        

        std::vector<gp_Pnt> visitedPoints;
        int numRecursion = 0;
        double innenStruktur = checkHohlraum(centeredShape, mainAxis, samplingAmount*2, gp_Pnt(0.0, 0.0, 0.0), visitedPoints, samplingLines6, numRecursion);


        //Verfahrensberechnungen
        double fraesen = calcFraesen(tolerance, wandStaerke, innenStruktur, bauteilInfo.volumeRatio, profileScore);
        double drehen = calcDrehen(durchmesser1, durchmesser2, tolerance, rotSymGes, profileScore, bauteilInfo.volumeRatio, innenStruktur, curvInfo.zwfKurvAnteil, curvInfo.einfKurvAnteil, curvInfo.andAnteil, curvInfo.planarAnteil);
        double rohrlaser = calcRohrLaserTeil(durchmesser1, durchmesser2, tolerance, gesamtSymmetrie, rotSymGes, profileScore, wandStaerke, wandPercent, bauteilInfo.volumeRatio, innenStruktur);
        double blech2d = calcBlech(tolerance, parallelitätGroßFlächen, wandStaerke, wandPercent, symmetrieMid, symmetrieMin, innenStruktur, bauteilInfo.volumeRatio, biegInfo.BiegeProbability, curvInfo.planarAnteil, bauteilInfo.xDimension, bauteilInfo.yDimension, bauteilInfo.zDimension);
        double blech3d = calcBiegeBlech(tolerance, parallelitätGroßFlächen, wandStaerke, wandPercent, symmetrieMid, symmetrieMin, innenStruktur, bauteilInfo.volumeRatio, biegInfo.BiegeProbability, curvInfo.planarAnteil, biegInfo.Laenge, bauteilInfo.xDimension, bauteilInfo.yDimension, bauteilInfo.zDimension);
        double additivesV = calcAdditiv(bauteilInfo.xDimension, bauteilInfo.yDimension, bauteilInfo.zDimension, tolerance, innenStruktur);
        //System::String^ prefVerfahren = "Präferrenz Verfahren";
        std::string prefVerfahren = calcPrefVerfahren(blech2d, blech3d, rohrlaser, drehen, fraesen, additivesV, 0.72);
        



        //Ergebnisse Tabelle
        System::Text::StringBuilder^ sb = gcnew System::Text::StringBuilder();
        sb->AppendLine(" Abmaße: " + bauteilInfo.xDimension.ToString("F1") + "x" + bauteilInfo.yDimension.ToString("F1") + "x" + bauteilInfo.zDimension.ToString("F1") + "mm");
        sb->AppendLine(" Oberfläche: " + bauteilInfo.surfaceArea.ToString("F2") + " mm²");
        sb->AppendLine(" Volumen: " + bauteilInfo.volume.ToString("F2") + " mm³");
        sb->AppendLine(" Boxvolumen: " + bauteilInfo.boxVolume.ToString("F2") + " mm³  (" + (bauteilInfo.volumeRatio * 100).ToString("F2") + "%)");
        sb->AppendLine(" Konturumfang: " + umfang.ToString("F2") + " mm");
        sb->AppendLine("----------------------------------------------------------------");
        sb->AppendLine(" Parallelität (Hauptflächen): " + (parallelitätGroßFlächen).ToString("F1"));
        sb->AppendLine(" x-Symmetrie (2.HTA): " + (symmetrieMid * 100).ToString("F2") + "%");
        sb->AppendLine(" y-Symmetrie (3.HTA): " + (symmetrieMin * 100).ToString("F2") + "%");
        sb->AppendLine(" Ø Spiegelsymmetrie: " + (gesamtSymmetrie * 100).ToString("F2") + "%");
        sb->AppendLine(" Rotationssymmetrie: " + (rotSymGes * 100).ToString("F2") + "%");
        sb->AppendLine(" Wandstärke: " + wandStaerke.ToString("F2") + "mm  (" + (wandPercent * 100).ToString("F2") + "%)");
        sb->AppendLine("----------------------------------------------------------------");
        sb->AppendLine(" Profilerkennung: " + (profileScore * 100).ToString("F2") + "%");
        sb->AppendLine(" Planare Flächen: " + (curvInfo.planarAnteil * 100).ToString("F2") + "%");
        sb->AppendLine(" 1x-Gekrümmte Flächen: " + (curvInfo.einfKurvAnteil * 100).ToString("F2") + "%");
        sb->AppendLine(" 2x-Gekrümmte Flächen: " + (curvInfo.zwfKurvAnteil * 100).ToString("F2") + "%");
        sb->AppendLine(" Andere Flächenanteile: " + (curvInfo.andAnteil * 100).ToString("F2") + "%");
        sb->AppendLine("----------------------------------------------------------------");
        sb->AppendLine(" Anzahl pot. Biegungen: " + biegInfo.potentialBiegungen.ToString());
        sb->AppendLine(" Biegewahrscheinlichkeit: " + (biegInfo.BiegeProbability * 100).ToString("F2") + "%");
        sb->AppendLine(" Biegeradius R: " + biegInfo.Radius.ToString("F2") + " mm");
        sb->AppendLine(" Biegewinkel v: " + biegInfo.Winkel.ToString("F1") + "°");
        sb->AppendLine(" Biegelänge L: " + biegInfo.Laenge.ToString("F2") + " mm");
        sb->AppendLine("----------------------------------------------------------------");
        sb->AppendLine(" int. Geschlossenheit: " + (innenStruktur * 100).ToString("F2") + "%");
        textBox3->Text = sb->ToString();
        


        System::Text::StringBuilder^ sd = gcnew System::Text::StringBuilder();
        sd->AppendLine(" CNC-Fräsen: " + (fraesen * 100).ToString("F2") + "%");
        sd->AppendLine(" CNC-Drehen: " + (drehen * 100).ToString("F2") + "%");
        sd->AppendLine(" CNC-Blech: " + (blech2d * 100).ToString("F2") + "%");
        sd->AppendLine(" Biegeblech: " + (blech3d * 100).ToString("F2") + "%");
        sd->AppendLine(" Rohrlaser: " + (rohrlaser * 100).ToString("F2") + "%");
        sd->AppendLine(" Additive: " + (additivesV * 100).ToString("F2") + "%");
        textBox8->Text = sd->ToString();
        

        System::String^ mPrefVerf = gcnew System::String(prefVerfahren.c_str());
        System::Text::StringBuilder^ sP = gcnew System::Text::StringBuilder();
        sP->AppendLine(mPrefVerf);
        textBox9->Text = sP->ToString();
        



        //Beispiel Visualisierungen
        Bnd_Box boundingBox;
        BRepBndLib::Add(centeredShape, boundingBox);

        Standard_Real xmin, ymin, zmin, xmax, ymax, zmax;
        boundingBox.Get(xmin, ymin, zmin, xmax, ymax, zmax);

        double dx = xmax - xmin;
        double dy = ymax - ymin;
        double dz = zmax - zmin;
        double boundingBoxLength = sqrt(dx * dx + dy * dy + dz * dz);

        // Use half of the bounding box length for the lines
        double length = boundingBoxLength / 2.0;

        for (const auto& line : samplingLines1) {
            // Define the start and end points of the line segment
            gp_Pnt P1 = line.Location().Translated(line.Direction().XYZ() * (-length));
            gp_Pnt P2 = line.Location().Translated(line.Direction().XYZ() * length);

            // Create an edge from the line segment
            TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(P1, P2);

            // Create an AIS_Shape for visualization
            Handle(AIS_Shape) aisEdge = new AIS_Shape(edge);
            aisEdge->SetColor(Quantity_NOC_WHITE); // Set the color of the line
            aisEdge->SetWidth(1.0); // Optional: set the width of the line

            // Display the line in the viewer
            myAISContext->Display(aisEdge, Standard_True);
            //myHelperEdges.push_back(aisEdge);
        }
        

        // Redisplay the viewer
        myAISContext->UpdateCurrentViewer();

        





        //Andere display text boxes infos
        System::Text::StringBuilder^ sa = gcnew System::Text::StringBuilder();
        sa->AppendLine(" - Solids: " + stepInfo.numSolids.ToString("D3"));
        sa->AppendLine(" - Shells: " + stepInfo.numShells.ToString("D3"));
        sa->AppendLine(" - Flächen: " + stepInfo.numFaces.ToString("D3"));
        sa->AppendLine(" - Kanten: " + stepInfo.numEdges.ToString("D3"));
        sa->AppendLine(" - Ecken: " + stepInfo.numVertices.ToString("D3"));
        textBox4->Text = sa->ToString();

        System::Text::StringBuilder^ sc = gcnew System::Text::StringBuilder();
        sc->AppendLine(" MSP (" + bauteilInfo.centerMass.X().ToString("F1") + "/" + bauteilInfo.centerMass.Y().ToString("F1") + "/" + bauteilInfo.centerMass.Z().ToString("F1") + ")  ");
        sc->AppendLine(" 1.HTA (" + mainAxis.X().ToString("F1") + "/" + mainAxis.Y().ToString("F1") + "/" + mainAxis.Z().ToString("F1") + ")  ");
        sc->AppendLine(" 2.HTA (" + midAxis.X().ToString("F1") + "/" + midAxis.Y().ToString("F1") + "/" + midAxis.Z().ToString("F1") + ")  ");
        sc->AppendLine(" 3.HTA (" + minAxis.X().ToString("F1") + "/" + minAxis.Y().ToString("F1") + "/" + minAxis.Z().ToString("F1") + ")  ");
        textBox5->Text = sc->ToString();



        


        // Überprüfen, ob das Shape gültig ist
        if (!importedShape.IsNull()) {
            // Erstellen eines AIS_Shape aus dem importierten Shape
            Handle(AIS_Shape) aisShape = new AIS_Shape(centeredShape);

            // Anzeigen des Shapes im Viewer
            myAISContext->Display(aisShape, Standard_True);

            // Die Ansicht anpassen, um das gesamte Shape anzuzeigen
            myView->FitAll();
            myView->Redraw();
        }
        else {
            std::cout << "Das importierte Shape ist ungültig." << std::endl;
        }

        std::cout << "Analyse abgeschlossen.\n";
        std::cout.flush();
    }
    catch (const std::exception& ex) {
        std::cout << "Fehler während der Analyse: " << ex.what() << std::endl;
        std::cout.flush();
    }
    catch (...) {
        std::cout << "Unbekannter Fehler während der Analyse." << std::endl;
        std::cout.flush();
    }
    
}

private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {

    ResetForNewAnalysis();

    textBoxStreamBuf = new TextBoxStreamBuf(gcnew System::Action<System::String^>(this, &Form1::AppendTextToTextBox));
    oldBuf = std::cout.rdbuf(textBoxStreamBuf);

    PerformAnalysis();
    //System::Threading::Tasks::Task::Run(gcnew Action(this, &Form1::PerformAnalysis));
    
}
private: System::Void textBox3_TextChanged(System::Object^ sender, System::EventArgs^ e) {
}



    System::Void Form1_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e) {
    std::cout.flush();

    if (textBoxStreamBuf) {
        std::cout.rdbuf(oldBuf); // Restore original buffer
        delete textBoxStreamBuf; // Delete the custom stream buffer
        textBoxStreamBuf = nullptr;
    }

    std::cout.flush();
}

private: System::Void textBox9_TextChanged(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void button2_Click(System::Object^ sender, System::EventArgs^ e) {

    //settingsForm->nSampl = this->samplingAmount;

    CppCLR_WinFormsProject1::SettingsForm^ settingsForm = gcnew CppCLR_WinFormsProject1::SettingsForm();
    settingsForm->ShowDialog();

    delete settingsForm;
}
private: System::Void button3_Click(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void button4_Click(System::Object^ sender, System::EventArgs^ e) {
    OpenFileDialog^ openFileDialog = gcnew OpenFileDialog();
    openFileDialog->InitialDirectory = "C:\\Users\\Ali Mounir\\Desktop\\TestCads\\";
    openFileDialog->Filter = "Step files (*.step;*.stp)|*.step;*.stp|All files (*.*)|*.*";
    openFileDialog->FilterIndex = 1;
    openFileDialog->RestoreDirectory = true;

    if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
    {
        String^ filePath = openFileDialog->FileName;
        this->textBox1->Text = filePath;
    }
}
private: System::Void button5_Click(System::Object^ sender, System::EventArgs^ e) {

}
private: System::Void checkBox1_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {

    if (checkBox1->Checked == true) {
        for (auto& edge : *myHelperEdges) {
            myAISContext->Display(edge, Standard_True);
        }
    }
    else {
        for (auto& edge : *myHelperEdges) {
            myAISContext->Erase(edge, Standard_True);
        }
    }
    myAISContext->UpdateCurrentViewer();
}
};
}

