

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
//      stderr:      "Instrument not found"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QStringList args = QApplication::arguments();


    // Log
    QString logFilename = Core::getLogFilename(Core::applicationName(), "R&S VNA measure log.txt");
    Log log(logFilename, Core::applicationName(), Core::version());
    log.printHeader();

    // VNA
    Vna vna;
    if (!Core::connect(args, vna, log)) {
        return 1;
    }

    QTextStream err(stderr);

    // filename arg
    if (args.size() < 4) {
        QString msg = "No touchstone filename given";
        err << msg;
        log.print("*");
        log.printLine(msg);
        return 1;
    }

    // Ports
    QVector<uint> ports;
    ports << 1 << 2 << 3 << 4;

    // Measure
    const QString filename = args[3];
    vna.settings().errorDisplayOff(); // RsaToolbox bug (disabled ports)
    if (!vna.channel(1).linearSweep().measureToSnpLocally(filename, ports)) {
        QString msg = "Could not generate touchstone file";
        err << msg;
        log.print("*");
        log.printLine(msg);
        return 1;
    }
    vna.isError();
    vna.clearStatus();
    vna.settings().errorDisplayOn();
    return 0;
}


