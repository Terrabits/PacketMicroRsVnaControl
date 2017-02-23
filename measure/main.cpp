

// RsaToolbox
#include <Vna.h>
using namespace RsaToolbox;

// Qt
#include <QApplication>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QVector>

// stdlib
#include <cstdio>

// measure <connection_type> <address> <filename>
//   Runs a measurement on the VNA and saves
//   a touchstone file to <filename>.
//
// Command line arguments
//   connection_type:  gpib, tcpip etc
//   address:          instrument address
//   filename:         path and filename for generated touchstone file
//
// Return value
//   0:             success
//   1:             failure. Check stderr for info.
//
// Successful example:
//   measure tcpip 127.0.0.1 test.s4p
//   => return code: 0
//      stderr:      -empty-
//
// Failed example:
//   measure gpib 20 test.s4p
//   => return code: 1
//      stderr:      "Instrument not connected"

bool           isArgs         (int     argc);
ConnectionType connectionType (QString arg );
bool           isVnaConnection(Vna &vna);

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QStringList args = QApplication::arguments();

    // Check arguments
    if (!isArgs(argc))
        return 1;

    // Check connection type
    ConnectionType type = connectionType(args[1]);
    if (type == ConnectionType::NoConnection)
        return 1;

    // Check vna connection
    const QString address = args[2];
    Vna vna(type, address);
    if (!isVnaConnection(vna))
        return 1;


    if (vna.properties().physicalPorts() < 4) {
        QTextStream err(stderr);
        err << "VNA must have at least 4 ports";
        return 1;
    }

    // Ports
    QVector<uint> ports;
    ports << 1 << 2 << 3 << 4;

    // Measure
    const QString filename = args[3];
    vna.channel(1).linearSweep().measureToSnpLocally(filename, ports);
}

bool isArgs(int argc) {
    QTextStream err(stderr);
    if (argc <= 1) {
        err << "Missing connection type";
        return false;
    }
    if (argc == 2) {
        err << "Missing instrument address";
        return false;
    }
    if (argc == 3) {
        err << "Missing filename";
        return false;
    }

    return true;
}
ConnectionType connectionType(QString arg) {

    arg = arg.toLower();
    if (arg == "tcpip") {
        return ConnectionType::VisaTcpConnection;
    }
    else if (arg == "gpib") {
        return ConnectionType::VisaGpibConnection;
    }
    else {
        QTextStream(stderr) << "Invalid connection type";
        return ConnectionType::NoConnection;
    }
}
bool isVnaConnection(Vna &vna) {
    QTextStream err(stderr);
    if (!vna.isConnected() || vna.idString().isEmpty()) {
        err << "Instrument not connected";
        return false;
    }
    else if (!vna.isRohdeSchwarz()) {
        err << "Did not recognize *IDN? response as R&S VNA";
        return false;
    }

    return true;
}
