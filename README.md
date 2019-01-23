# Geco
VCV plugin Maker

BUILD: 

- Add these files into a folder named "qnodeseditor"

- Use Qt 5.6 to build the project:

Set Qt to add ther release folder into GecoBuilds:

![alt text](https://github.com/HoRaMusic/Geco/blob/master/QtRunSetting.png)

- Build


TEST:

- Open Geco app press the load button to open an examples project in ->GecoBuilds->GecoExamples.
- Press the export button or ctrl + g to export the module. Select the folder where you wish to export the plugin.
- Open the other exmaples and export each, check the box on the dialog box that ask you if you wish to add it to the existing plugin.
- Compile the created plugin and test it in VCV (Hora->waveosc, buffer, miniseq, LPVCF).

