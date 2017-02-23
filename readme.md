PacketMicro AITT R&S VNA Control
================================

This Qt project contains three subprojects, each designed for command line use with the PacketMicro AITT tool for the purposes of controlling a R&S VNA.

Dependencies
------------

This project uses the Qt4.8 `core`, `gui`, `widgets` and `network` libraries.

This project also requires the RsaToolbox for Qt4.8 project. Source code and usage instructions can be found here:

[RsaToolbox, qt4.8 branch](https://github.com/Terrabits/RsaToolbox/tree/qt4.8)

Command-line utilities
----------------------

Each of the subprojects corresponds to a command line utility.

### Setup

`setup` applies a set file to the VNA.

Usage:

`setup <connection_type> <address> <filename>`

`filename` may contain path information.

Returns:

`0` if successful.

`1` and error message on stderr if an error occurs.

### calibrate

Uses an attached cal unit to calibrate ports 1-4.

Usage:

`calibrate <connection_type> <address>`

Returns:

`0` if successful.

`1` and error message on stderr if an error occurs.

### measure

Performs a measurement and saves the results into touchstone file `filename`

`filename` may contain path information.

Usage:

`measure <connection_type> <address> <filename>`

Returns:

`0` if successful.

`1` and error message on stderr if an error occurs.
