

// RsaToolbox
#include <VisaBus.h>
#include <Vna.h>
using namespace RsaToolbox;

// Qt
#include <QApplication>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QVector>

// stdlib
#include <cstdio>

// calibrate <connection_type> <address>
//   Auto-calibrates a R&S VNA with 4+ ports
//   and a 4+ port cal unit attached via USB.
//
// Command line arguments
//   connection_type:  gpib, tcpip etc
//   address:          instrument address
//
// Return value
//   0:             success
//   1:             failure. Check stderr for info.
//
// Example 1:
//   calibrate gpib 20
//   => return code: 0
//      stderr:      -empty-
//
// Example 2:
//   calibrate tcpip 127.0.0.1
//   => return code: 1
//      stderr:      "VNA port 2 is not connected"

bool           isArgs         (int     argc);
ConnectionType connectionType (QString arg );
bool           isVnaConnection(Vna &vna);
bool           isPorts        (Vna &vna, QVector<uint> ports);
bool           isFreqRange    (Vna &vna);

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

    QTextStream err(stderr);
    if (!VisaBus::isVisaInstalled()) {
        err << "VISA not installed";
        return 1;
    }

    // Check vna connection
    const QString address = args[2];
    Vna vna(type, address);
    if (!isVnaConnection(vna))
        return 1;

    if (vna.properties().physicalPorts() < 4) {
        err << "VNA must have at least 4 ports";
        return 1;
    }

    // Check for cal unit
    if (!vna.isCalUnit()) {
        err << "No cal unit found";
        return 1;
    }

    // Check for port connections
    QVector<uint> ports;
    ports << 1 << 2 << 3 << 4;
    if (!isPorts(vna, ports))
        return 1;

    // Check cal unit freq range
    if (!isFreqRange(vna))
        return 1;

    // Calibrate
    vna.calibrate().autoCalibrate(ports);
}

bool isArgs(int argc) {
    QTextStream err(stderr);
    if (argc <= 1) {
        err << "Missing connection type";
        return 1;
    }
    if (argc == 2) {
        err << "Missing instrument address";
        return 1;
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
        err << "Instrument not found";
        return false;
    }
    else if (!vna.isRohdeSchwarz()) {
        err << "Did not recognize *IDN? response as R&S VNA";
        return false;
    }

    return true;
}
bool isPorts(Vna &vna, QVector<uint> ports) {
    QTextStream err(stderr);

    QString id = vna.calUnits().first();
    QVector<uint> unitPorts = vna.calUnit(id).connectedToPorts();
    if (unitPorts != ports) {
        foreach (uint port, ports) {
            if (!unitPorts.contains(port)) {
                err << QString("Port %1 not connected").arg(port);
                return false;
            }
        }
        err << "Check cal unit port connections";
        return false;
    }

    return true;
}
bool isFreqRange(Vna &vna) {
    QTextStream err(stderr);
    const QString id = vna.calUnits().first();
    VnaCalUnit calUnit = vna.calUnit(id);

    foreach (uint i, vna.channels()) {
        VnaChannel   ch        = vna.channel(i);
        const double chMax_Hz  = ch.linearSweep().frequencies_Hz().last();
        const double chMin_Hz  = ch.linearSweep().frequencies_Hz().first();
        const double calMax_Hz = calUnit.maximumFrequency_Hz();
        const double calMin_Hz = calUnit.minimumFrequency_Hz();
        if (chMin_Hz < calMin_Hz) {
            err << "Cal unit frequency range too low";
            return false;
        }
        if (chMax_Hz > calMax_Hz) {
            err << "Cal unit frequency range too low";
            return false;
        }
    }
    return true;
}
