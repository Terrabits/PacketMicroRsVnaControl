#include "core.h"


// RsaToolbox
#include <VisaBus.h>
using namespace RsaToolbox;

// Qt
#include <QDesktopServices>


Core::Core()
{
}

QString Core::applicationName() {
    return "Advanced Interconnect Test Tool";
}
QString Core::version() {
    return "0.1.1";
}

QDir Core::getDataDir(const QString &applicationFolder) {
    QDir dataDir;
#ifdef Q_OS_WIN32
    QDir dir = QDir::root();
    if (dir.cd("ProgramData") || dir.cd("Documents and Settings/All Users/Application Data"))
        dataDir = dir;
    else
        dataDir.setPath(QDesktopServices::storageLocation(QDesktopServices::DataLocation));
#else
    dataDir.setPath(QDesktopServices::storageLocation(QDesktopServices::DataLocation));
#endif
    dataDir.mkpath(applicationFolder);
    dataDir.cd(applicationFolder);
    return dataDir;
}
QString Core::getLogFilename(const QString &applicationName, const QString &filename) {
    return getDataDir(applicationName).filePath(filename);
}

bool Core::connect(QStringList args, Vna &vna, Log &log) {
    QTextStream err(stderr);
    // Connection args
    const int argc = args.size();
    if (argc <= 1) {
        QString msg = "Missing connection type";
        err << msg;
        log.print("*");
        log.printLine(msg);
        return false;
    }
    if (argc == 2) {
        QString msg = "Missing instrument address";
        err << msg;
        log.print("*");
        log.printLine(msg);
        return false;
    }

    // Connection type
    const QString arg1 = args[1].toLower();
    ConnectionType type = ConnectionType::NoConnection;
    if (arg1 == "tcpip") {
        type = ConnectionType::VisaTcpConnection;
    }
    else if (arg1 == "gpib") {
        type = ConnectionType::VisaGpibConnection;
    }
    else {
        QString msg = "Invalid connection type";
        err << msg;
        log.print("*");
        log.printLine(msg);
        return false;
    }

    // Check for VISA
    if (!VisaBus::isVisaInstalled()) {
        QString msg = "VISA not installed";
        err << msg;
        log.print("*");
        log.printLine(msg);
        return false;
    }

    // Connect
    vna.resetBus(type, args[2]);
    vna.useLog(&log);
    vna.printInfo();
    if (!vna.isConnected() || vna.idString().isEmpty()) {
        QString msg = "Instrument not found";
        err << msg;
        log.print("*");
        log.printLine(msg);
        return false;
    }
    else if (!vna.isRohdeSchwarz()) {
        QString msg = "Did not recognize id string as R&S VNA";
        err << msg;
        log.print("*");
        log.printLine(msg);
        return false;
    }

    // 4+ ports?
    if (vna.properties().physicalPorts() < 4) {
        QString msg = "VNA must have at least 4 ports";
        err << msg;
        log.print("*");
        log.printLine(msg);
        return false;
    }

    // Previous errors?
    vna.isError();
    vna.clearStatus();
    return true;
}
