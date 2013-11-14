
#include <QDebug>

#include "common/errors.hpp"

void Errors::emitError(const QString &text, const QString &fileName, int line, int column) {
    if (line != -1) {
        if (column != -1) {
            qDebug().nospace() << qPrintable(fileName) << ":" << line << ":" << column << ": "
                               << qPrintable(text);
            return;
        }
        qDebug().nospace() << qPrintable(fileName) << ":" << line << ": " << qPrintable(text);
        return;
    }
    qDebug().nospace() << qPrintable(fileName) << ": " << qPrintable(text);
}
