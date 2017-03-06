

// Project
#include "core.h"

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
bool isPorts         (Vna &vna, QVector<uint> ports);
bool isFreqRange     (Vna &vna);
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QStringList args = QApplication::arguments();

    // Log
    QString logFilename = Core::getLogFilename(Core::applicationName(), "R&S VNA calibrate log.txt");
    Log log(logFilename, Core::applicationName(), Core::version());
    log.printHeader();

    // VNA
    Vna vna;
    if (!Core::connect(args, vna, log)) {
        return 1;
    }

    // Error stream
    QTextStream err(stderr);

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

    // Errors?
    vna.isError();
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
        const double chMax_Hz  = std::round(ch.linearSweep().stop_Hz());
        const double chMin_Hz  = std::round(ch.linearSweep().start_Hz());
        const double calMax_Hz = std::round(calUnit.maximumFrequency_Hz());
        const double calMin_Hz = std::round(calUnit.minimumFrequency_Hz());
        vna.printLine("Cal unit info:");
        vna.printLine(QString("chMax:  %1").arg(chMax_Hz));
        vna.printLine(QString("chMin:  %1").arg(chMin_Hz));
        vna.printLine(QString("calMax: %1").arg(calMax_Hz));
        vna.printLine(QString("calMin: %1").arg(calMin_Hz));
        vna.printLine();
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
