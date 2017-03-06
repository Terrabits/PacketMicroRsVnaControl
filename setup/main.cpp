

// Project
#include "core.h"

// RsaToolbox
#include <General.h>
#include <Log.h>
#include <VisaBus.h>
#include <Vna.h>
using namespace RsaToolbox;

// Qt
#include <QApplication>
#include <QFileInfo>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QVector>

// stdlib
#include <cstdio>

// setup <connection_type> <address> <filename>
//   Applies set file 'filename' to VNA
//
// Command line arguments
//   connection_type:  gpib, tcpip etc
//   address:          instrument address
//   filename:         Valid .znx set file
//
// Return value
//   0:             success
//   1:             failure. Check stderr for info.
//
// Successful example:
//   setup tcpip 127.0.0.1 test.s4p
//   => return code: 0
//      stderr:      -empty-
//
// Failed example:
//   setup gpib 20 usbc_data.znx
//   => return code: 1
//      stderr:      "Instrument not found"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QStringList args = QApplication::arguments();

    // Log
    QString logFilename = Core::getLogFilename(Core::applicationName(), "R&S VNA setup log.txt");
    Log log(logFilename, Core::applicationName(), Core::version());
    log.printHeader();

    // VNA
    Vna vna;
    if (!Core::connect(args, vna, log)) {
        return 1;
    }

    QTextStream err(stderr);
    // Check arguments
    if (argc == 3) {
        QString msg = "Missing filename";
        err << msg;
        log.print("*");
        log.printLine(msg);
        return 1;
    }

    // Check for existence of filename
    const QString filename = args[3];
    if (!QFileInfo(filename).exists()) {
        QString msg = "File \'%1\' not found";
        msg         = msg.arg(filename);
        err << msg;
        log.print("*");
        log.printLine(msg);
        return 1;
    }

    // Upload set file to VNA
    QString vna_filename = QFileInfo(filename).fileName();
    VnaFileSystem file = vna.fileSystem();
    file.changeDirectory(VnaFileSystem::Directory::RECALL_SETS_DIRECTORY);
    file.uploadFile(filename, vna_filename);

    // Apply set file
    vna.closeSets();
    vna.openSet(vna_filename);

    // Delete set file from VNA
    file.deleteFile(vna_filename);
    file.changeDirectory(VnaFileSystem::Directory::DEFAULT_DIRECTORY);

    // Error?
    vna.isError();
}
