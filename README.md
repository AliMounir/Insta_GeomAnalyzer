# GeomAnalyzer – Fertigungsverfahren erkennen (Desktop-GUI)

Desktop-Anwendung (Windows Forms, C++/CLI) zur **automatischen Geometrieanalyse von Maschinenbauteilen** aus STEP-Dateien und zur Empfehlung eines **Fertigungsverfahrens**. Das ist die GUI-Variante des Algorithmus aus meiner Bachelorarbeit (Note 1,0).

![C++](https://img.shields.io/badge/C%2B%2B%2FCLI-00599C?style=flat-square&logo=cplusplus&logoColor=white)
![OpenCASCADE](https://img.shields.io/badge/OpenCASCADE-STEP%20%2F%20BRep-1f2937?style=flat-square)
![Windows Forms](https://img.shields.io/badge/Windows_Forms-0078D4?style=flat-square&logo=windows&logoColor=white)

<!-- Screenshot der Oberfläche einfügen -->

## Funktionen

- STEP-Datei laden und Bauteil analysieren (Abmessungen, Volumen, Oberfläche, Symmetrien, Wandstärken, Krümmungen)
- **Hohlraumerkennung** über Abtaststrahlen
- **Eignungs-Scores** für Fräsen, Drehen, Rohr-/Laserteil, Blech, Biegeblech und additive Fertigung
- **Visualisierung** der Analyse (`visualizers.cpp`) und Ausgabe im Programmfenster
- **Einstellungen** für Schwellwerte und Analyseparameter (`SettingsForm`)

## Aufbau

Die Analysemodule (`stepImporters`, `calcBasicInfo`, `geomAnalysen`, `innenStruktur`, `verfahrensBerechnung`) teilt die GUI mit der Kommandozeilen-Version [BAClassifierCMD01](https://github.com/AliMounir/BAClassifierCMD01).

## Bauen

Visual Studio mit C++/CLI-Unterstützung und installiertem **OpenCASCADE**. Lösung `CppCLR_WinFormsProject1.sln` öffnen, OpenCASCADE-Pfade setzen, bauen.
