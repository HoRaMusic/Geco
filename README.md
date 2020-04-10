# Geco
VCV Plugin Maker



IMPORTANT:

Geco is a work on progress, some blocks are obsolete:

- wavetable (use too much CPU, don't work on mac), the wave table object will be completly change to offer antialiasing and use   
  "waveedit" files http://synthtech.com/waveedit/.
- stringMux, stringLeft and stringPack are not tested for V1.0 yet and has to be modified to use less CPU.
- Geco can only create plugins code for rack V1.x
- Currently Geco do not generate the manifest so you have to write it manually.
- The "block creator" mentioned in the manual is available on this repository https://github.com/HoRaMusic/GecoBlockCreator



BUILD:

- Add these files into a folder named "qnodeseditor"

- Add the Geco folder to your documents location (win : "C:/Users/<USER>/Documents" | mac : "~/Documents")

- Use Qt 5.6 to build the project:

Set Qt to add the release folder into GecoBuilds:

![alt text](https://github.com/HoRaMusic/Geco/blob/master/QtRunSetting.png)

- Build


TEST:

- Open Geco app press the load button to open an examples project in ->GecoBuilds->GecoExamples.
- Press the export button or ctrl + g to export the module. Select the folder where you wish to export the plugin.
- Open the other exmaples and export each, check the box on the dialog box that ask you if you wish to add it to the existing plugin.
- Compile the created plugin and test it in VCV (Hora->waveosc, buffer, miniseq, LPVCF).
- Here is a video showing a module creation.
- Don't forget to look at the instalation instructions and the manual

RELEASE/DEPLOY:

 win:
 1. Open project folder in terminal  
 2. Run ```BuildScripts/build-win.bat```  

 mac:  
 1. Open project folder in terminal  
 2. Run ```BuildScripts/build-osx.sh```  
 get .dmg files in GecoBuilds/osx/  


DEPENDENCIES:

 The graphical nodes editor use a STANISLAW ADASZEWSKI software as a work base (widely modified) for the nodes editor UI.  
 https://adared.ch/qnodeseditor-qt-nodesports-based-data-processing-flow-editor/
