#pragma once

#include "pch.h"
#include "stepImporters.h"
#include "calcBasicInfo.h"
#include "geomAnalysen.h"
#include "GeomInfo.h"
#include "innenStruktur.h"

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
#include <iostream>
#include <streambuf>
#include <ostream>
#include <string>
#include <set>
#include <vcclr.h>

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
        }

    protected:
        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        ~Form1()
        {
            if (components)
            {
                delete components;
            }
        }
    protected:

    private:
        /// <summary>
        /// Required designer variable.
        /// </summary>
        System::ComponentModel::Container^ components;

    private:
        System::Windows::Forms::Panel^ panel1;
        System::Windows::Forms::TextBox^ textBox1;


    private: System::Windows::Forms::TextBox^ textBox4;
    private: System::Windows::Forms::TextBox^ textBox5;
    private: System::Windows::Forms::TextBox^ textBox3;
    private: System::Windows::Forms::Button^ button1;
           System::Windows::Forms::TextBox^ textBox2;


#pragma region Windows Form Designer generated code
        void InitializeComponent(void)
        {
            this->panel1 = (gcnew System::Windows::Forms::Panel());
            this->button1 = (gcnew System::Windows::Forms::Button());
            this->textBox5 = (gcnew System::Windows::Forms::TextBox());
            this->textBox4 = (gcnew System::Windows::Forms::TextBox());
            this->textBox1 = (gcnew System::Windows::Forms::TextBox());
            this->textBox2 = (gcnew System::Windows::Forms::TextBox());
            this->textBox3 = (gcnew System::Windows::Forms::TextBox());
            this->panel1->SuspendLayout();
            this->SuspendLayout();
            // 
            // panel1
            // 
            this->panel1->Controls->Add(this->button1);
            this->panel1->Controls->Add(this->textBox5);
            this->panel1->Controls->Add(this->textBox4);
            this->panel1->Controls->Add(this->textBox1);
            this->panel1->Location = System::Drawing::Point(12, 12);
            this->panel1->Name = L"panel1";
            this->panel1->Size = System::Drawing::Size(1051, 613);
            this->panel1->TabIndex = 0;
            this->panel1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Form1::panel1_Paint);
            this->panel1->Resize += gcnew System::EventHandler(this, &Form1::panel1_Resize);
            // 
            // button1
            // 
            this->button1->BackColor = System::Drawing::SystemColors::ButtonHighlight;
            this->button1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.875F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->button1->ForeColor = System::Drawing::Color::LimeGreen;
            this->button1->Location = System::Drawing::Point(936, 2);
            this->button1->Margin = System::Windows::Forms::Padding(0);
            this->button1->Name = L"button1";
            this->button1->Size = System::Drawing::Size(111, 23);
            this->button1->TabIndex = 4;
            this->button1->Text = L"Analysieren..";
            this->button1->UseVisualStyleBackColor = false;
            this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
            // 
            // textBox5
            // 
            this->textBox5->BackColor = System::Drawing::SystemColors::WindowFrame;
            this->textBox5->BorderStyle = System::Windows::Forms::BorderStyle::None;
            this->textBox5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.125F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->textBox5->ForeColor = System::Drawing::SystemColors::Window;
            this->textBox5->Location = System::Drawing::Point(931, 546);
            this->textBox5->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
            this->textBox5->Multiline = true;
            this->textBox5->Name = L"textBox5";
            this->textBox5->Size = System::Drawing::Size(115, 63);
            this->textBox5->TabIndex = 3;
            this->textBox5->TabStop = false;
            this->textBox5->Text = L" SP (000,000,000)\r\n HA1 (000/000/000)\r\n HA2 (000/000/000)\r\n HA3 (000/000/000)";
            // 
            // textBox4
            // 
            this->textBox4->BackColor = System::Drawing::SystemColors::WindowFrame;
            this->textBox4->BorderStyle = System::Windows::Forms::BorderStyle::None;
            this->textBox4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.125F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->textBox4->ForeColor = System::Drawing::SystemColors::Window;
            this->textBox4->Location = System::Drawing::Point(961, 26);
            this->textBox4->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
            this->textBox4->Multiline = true;
            this->textBox4->Name = L"textBox4";
            this->textBox4->Size = System::Drawing::Size(86, 77);
            this->textBox4->TabIndex = 2;
            this->textBox4->TabStop = false;
            this->textBox4->Text = L" - Solids: 000\r\n - Shells: 000\r\n - Fl‰chen: 000\r\n - Kanten: 000\r\n - Ecken: 000";
            this->textBox4->TextChanged += gcnew System::EventHandler(this, &Form1::textBox4_TextChanged);
            // 
            // textBox1
            // 
            this->textBox1->BackColor = System::Drawing::SystemColors::ControlDarkDark;
            this->textBox1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            this->textBox1->ForeColor = System::Drawing::SystemColors::Window;
            this->textBox1->Location = System::Drawing::Point(3, 3);
            this->textBox1->Name = L"textBox1";
            this->textBox1->ReadOnly = true;
            this->textBox1->Size = System::Drawing::Size(931, 22);
            this->textBox1->TabIndex = 1;
            this->textBox1->TabStop = false;
            // 
            // textBox2
            // 
            this->textBox2->BackColor = System::Drawing::SystemColors::InfoText;
            this->textBox2->ForeColor = System::Drawing::SystemColors::Window;
            this->textBox2->Location = System::Drawing::Point(12, 631);
            this->textBox2->Multiline = true;
            this->textBox2->Name = L"textBox2";
            this->textBox2->ReadOnly = true;
            this->textBox2->ScrollBars = System::Windows::Forms::ScrollBars::Both;
            this->textBox2->Size = System::Drawing::Size(1051, 167);
            this->textBox2->TabIndex = 1;
            // 
            // textBox3
            // 
            this->textBox3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.875F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->textBox3->Location = System::Drawing::Point(1067, 15);
            this->textBox3->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
            this->textBox3->Multiline = true;
            this->textBox3->Name = L"textBox3";
            this->textBox3->ReadOnly = true;
            this->textBox3->RightToLeft = System::Windows::Forms::RightToLeft::No;
            this->textBox3->Size = System::Drawing::Size(275, 419);
            this->textBox3->TabIndex = 2;
            this->textBox3->TabStop = false;
            // 
            // Form1
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(1349, 808);
            this->Controls->Add(this->textBox3);
            this->Controls->Add(this->textBox2);
            this->Controls->Add(this->panel1);
            this->Name = L"Form1";
            this->Text = L"Form1";
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
     if (AllocConsole()) {
         FILE* pCout;
         freopen_s(&pCout, "CONOUT$", "w", stdout);
     }
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
        myAISContext->UpdateCurrentViewer();

        Standard_CString stepFilePath = "C:\\Users\\Ali Mounir\\Desktop\\test-platte.stp";
        int samplingAmount = 16; // am besten immer eine Zahl teilbar durch 4
        this->textBox1->Text = gcnew System::String(stepFilePath);
        TopoDS_Shape importedShape = ImportSTEP(stepFilePath);
        //BRepTools::Clean(importedShape);
        InspectShape(importedShape);
        TopoDS_Shape centeredShape = CenterShape(importedShape);
        GeomInfo bauteilInfo = getBasicInfo(centeredShape);

        double smallestDimension = std::min({ bauteilInfo.xDimension, bauteilInfo.yDimension, bauteilInfo.zDimension });
        double largestDimension = std::max({ bauteilInfo.xDimension, bauteilInfo.yDimension, bauteilInfo.zDimension });
        gp_Dir mainAxis = GetPrincipalInertiaAxes(centeredShape)[0];
        gp_Dir midAxis = GetPrincipalInertiaAxes(centeredShape)[1];
        gp_Dir minAxis = GetPrincipalInertiaAxes(centeredShape)[2];

        double parallelit‰tGroﬂFl‰chen = FindLargestFacesAndCheckParallel(centeredShape);

        std::vector<gp_Lin> samplingLines;
        std::vector<gp_Lin> samplingLines2;
        std::vector<gp_Lin> samplingLines3;
        std::vector<gp_Lin> samplingLines4;
        std::vector<gp_Lin> samplingLines5;
        std::vector<gp_Lin> samplingLines6;
        double symmetrieMid = CheckLinSymmetry(centeredShape, mainAxis, midAxis, samplingAmount*2, samplingLines2);
        double symmetrieMin = CheckLinSymmetry(centeredShape, mainAxis, minAxis, samplingAmount*2, samplingLines3);
        double gesamtSymmetrie = (symmetrieMin + symmetrieMid) / 2;
        double rotSym = CheckRotSymmetry(centeredShape, mainAxis, midAxis, samplingAmount*4, samplingLines4);

        
        double wandStaerke = FindWandstarke(centeredShape, mainAxis, samplingAmount, samplingLines)[0];
        double wandPercent = FindWandstarke(centeredShape, mainAxis, samplingAmount, samplingLines)[1];
        if (wandStaerke > smallestDimension) {
           samplingLines.clear();
           std::cout << "Berechnete Wandst‰rke ist grˆﬂer als kleinstest Bauteilabmaﬂ, wechsele Berechnung auf ThirdAxisOfInertia()." << std::endl;
           wandStaerke = FindWandstarke(centeredShape, minAxis, samplingAmount, samplingLines)[0];
           wandPercent = FindWandstarke(centeredShape, mainAxis, samplingAmount, samplingLines)[1];
        } else {
            std::cout << "Berechnete Wandst‰rke besteht so!!" << std::endl;
        }

        double profileScore = checkForProfile(centeredShape, mainAxis, samplingAmount*2, samplingLines5);

        double planarAnteil = checkCurvePercentage(centeredShape)[0];
        double einfachKr¸mmungsAnteil = checkCurvePercentage(centeredShape)[1];
        double zweifachKr¸mmungsAnteil = checkCurvePercentage(centeredShape)[2];
        double andererAnteil = checkCurvePercentage(centeredShape)[3];


        std::vector<gp_Pnt> visitedPoints;
        double innenStruktur = checkHohlraum(centeredShape, samplingAmount*2, gp_Pnt(0.0, 0.0, 0.0), visitedPoints, samplingLines6, 0);







        //Ergebnisse Tabelle
        System::Text::StringBuilder^ sb = gcnew System::Text::StringBuilder();
        sb->AppendLine("Abmaﬂe: " + bauteilInfo.xDimension.ToString("F1") + "x" + bauteilInfo.yDimension.ToString("F1") + "x" + bauteilInfo.zDimension.ToString("F1") + "mm");
        sb->AppendLine("Oberfl‰che: " + bauteilInfo.surfaceArea.ToString("F2") + " mm≤");
        sb->AppendLine("Volumen: " + bauteilInfo.volume.ToString("F2") + " mm≥");
        sb->AppendLine("Boxvolumen: " + bauteilInfo.boxVolume.ToString("F2") + " mm≥  (" + (bauteilInfo.volumeRatio*100).ToString("F2") + "%)");
        sb->AppendLine(" ");
        sb->AppendLine("----------------------------------------------------------------");
        sb->AppendLine(" ");
        sb->AppendLine("Parallelit‰t (Hauptfl‰chen): " + (parallelit‰tGroﬂFl‰chen).ToString("F1"));
        sb->AppendLine("x-Symmetrie (2.HTA): " + (symmetrieMid*100).ToString("F2") + "%");
        sb->AppendLine("y-Symmetrie (3.HTA): " + (symmetrieMin * 100).ToString("F2") + "%");
        sb->AppendLine("ÿ Spiegelsymmetrie: " + (gesamtSymmetrie * 100).ToString("F2") + "%");
        sb->AppendLine("Rotationssymmetrie: " + (rotSym*100).ToString("F2") + "%");
        sb->AppendLine("Wandst‰rke: " + wandStaerke.ToString("F2") + "mm  (" + (wandPercent*100).ToString("F2") + "%)");
        sb->AppendLine(" ");
        sb->AppendLine("----------------------------------------------------------------");
        sb->AppendLine(" ");
        sb->AppendLine("Profilerkennung: " + (profileScore*100).ToString("F2") + "%");
        sb->AppendLine("Planare Fl‰chen: " + (planarAnteil*100).ToString("F2") + "%");
        sb->AppendLine("1x-Gekr¸mmte Fl‰chen: " + (einfachKr¸mmungsAnteil*100).ToString("F2") + "%");
        sb->AppendLine("2x-Gekr¸mmte Fl‰chen: " + (zweifachKr¸mmungsAnteil*100).ToString("F2") + "%");
        sb->AppendLine("Andere Fl‰chenanteile: " + (andererAnteil*100).ToString("F2") + "%");
        sb->AppendLine(" ");
        sb->AppendLine("----------------------------------------------------------------");
        sb->AppendLine(" ");
        //sb->AppendLine("Biegungen: " + (profileScore*100).ToString("F2") + "%");
        sb->AppendLine("int. Geschlossenheit: " + (innenStruktur*100).ToString("F2") + "%");
        textBox3->Text = sb->ToString();




        
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

        for (const auto& line : samplingLines2) {
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
        }

        // Redisplay the viewer
        myAISContext->UpdateCurrentViewer();







        //Andere display text boxes infos
        /*System::Text::StringBuilder^ sa = gcnew System::Text::StringBuilder();
        sa->AppendLine("Abmaﬂe: " + bauteilInfo.xDimension.ToString("F1") + "x" + bauteilInfo.yDimension.ToString("F1") + "x" + bauteilInfo.zDimension.ToString("F1") + "mm");
        sa->AppendLine("Oberfl‰che: " + bauteilInfo.surfaceArea.ToString("F2") + " mm≤");
        sa->AppendLine("Volumen: " + bauteilInfo.volume.ToString("F2") + " mm≥");
        sa->AppendLine("Boxvolumen: " + bauteilInfo.boxVolume.ToString("F2") + " mm≥  (" + (bauteilInfo.volumeRatio * 100).ToString("F2") + "%)");
        sa->AppendLine(" ");
        textBox344->Text = sa->ToString();*/






        // ‹berpr¸fen, ob das Shape g¸ltig ist
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
            std::cout << "Das importierte Shape ist ung¸ltig." << std::endl;
        }

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
private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
}
};
}

