/*
 * Copyright (C) 2019 ~ %YEAR% Deepin Technology Co., Ltd.
 *
 * Author:     WangXin
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "frame/mainwindow.h"
#include "application.h"
#include "frame/cviewmanagement.h"
#include "frame/ccentralwidget.h"

#include <DGuiApplicationHelper>
#include <DApplicationSettings>

#include <QCommandLineOption>
#include <QObject>
#include <QTranslator>
#include <QMessageBox>
#include <QDebug>
#include <QDBusMetaType>

#include <fcntl.h>
#include "drawinterface.h"

#include <DLog>

QStringList getFilesFromQCommandLineParser(const QCommandLineParser &parser)
{
    QStringList files;
    QStringList pas = parser.positionalArguments();
    for (int  i = 0; i < pas.count(); i++) {
        QString file = pas.at(i);
        files.append(file);
    }
    return files;
}

bool checkOnly()
{
    QString userName = QDir::homePath().section("/", -1, -1);
    std::string path = ("/home/" + userName + "/.cache/deepin/deepin-draw/single").toStdString();
    int fd = open(path.c_str(), O_WRONLY | O_CREAT, 0644);
    int flock = lockf(fd, F_TLOCK, 0);
    if (fd == -1) {
        perror("open lockfile/n");
        return false;
    }
    if (flock == -1) {
        perror("lock file error/n");
        return false;
    }
    return true;
}

bool isRunning(Application &a)
{
    //判断实例是否已经运行
    return (!a.setSingleInstance("deepinDraw") || !checkOnly());
}

int main(int argc, char *argv[])
{
#if defined(STATIC_LIB)
    DWIDGET_INIT_RESOURCE();
#endif

    Application::setAttribute(Qt::AA_UseHighDpiPixmaps);
    Application a(argc, argv);

    QCommandLineOption openImageOption(QStringList() << "o" << "open",
                                       "Specify a path to load an image.", "PATH");
    QCommandLineOption activeWindowOption(QStringList() << "s" << "show",
                                          "Show deepin draw.");
    QCommandLineParser cmdParser;
    cmdParser.setApplicationDescription("deepin-draw");
    cmdParser.addOption(openImageOption);
    cmdParser.addOption(activeWindowOption);
    cmdParser.process(a);

    QStringList paths = getFilesFromQCommandLineParser(cmdParser);

    //判断实例是否已经运行
    if (isRunning(a)) {
        DrawInterface *m_draw = new DrawInterface("com.deepin.Draw",
                                                  "/com/deepin/Draw", QDBusConnection::sessionBus(), &a);
        m_draw->openFiles(paths);
        return 0;
    }
    return a.execDraw(paths);
}
