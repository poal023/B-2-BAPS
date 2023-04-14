# B-2-BAPS

This is the Github repository for a NASA-Sponsored EE 4812/4813 project. The project aims to create an automatic alignment routine with an S-band antenna and a gimbal.

The repository contains all the associated source code used for the project.

# Directory Structure
The `GNURadio/` Directory contains the associated .grc file and the generated Python output. Inputs recieved by the SDR are processed through GNU Radio and then fed through relevant Python scripts using ZMQ

`Scripts/`, as the name implies, contains any helper scripts (aside from the generated GNU Radio program) to allow data recieved from ZMQ to be transformed and/or sent to an Arduino.

Finally, `arduino/` contains all code used to prototype and test designs onto an Arudino board.

For all relevant directories, the associated `README.md` contains documentation.

# Issues

If you have any issues, questions, or feedback, please feel free to open up an issue or PR!

