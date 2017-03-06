#ifndef CORE_H
#define CORE_H

// RsaToolbox
#include <Log.h>
#include <Vna.h>

// Qt
#include <QDir>
#include <QString>
#include <QStringList>



class Core
{

public:
    Core();

    static QString applicationName();
    static QString version();

    static QDir    getDataDir    (const QString &applicationFolder);
    static QString getLogFilename(const QString &applicationName, const QString &filename);

    static bool    connect(QStringList args, RsaToolbox::Vna &vna, RsaToolbox::Log &log);

};

#endif // CORE_H
