#include "Epr100Validator.h"

QValidator::State Epr100DoubleValidator::validate(QString& input, int& pos) const
{
    if (input.isEmpty()) return QValidator::Intermediate;
    bool isOK = false;
    if (input.length() == 1 && input.at(0) == '-' && bottom() < 0) return QValidator::Acceptable;
    double value = input.toDouble(&isOK);
    if (!isOK) return QValidator::Invalid;
    int dotPos = input.indexOf(".");
    if (dotPos > 0) {
        if (input.right(input.length() - (dotPos + 1)).length() > decimals()) {
            return QValidator::Invalid;
        }
    }
    if (value >= bottom() && value <= top()) return QValidator::Acceptable;

    return QValidator::Intermediate;
}

void Epr100DoubleValidator::fixup(QString& input) const
{
    QString sFormat = "%." + QString::number(m_decimals) + "f";
    if (input.toDouble() > top()) {
        input = input.asprintf(sFormat.toLatin1().data(), top());
    }
    if (input.toDouble() < bottom() || input.trimmed().isEmpty()) {
        input = input.asprintf(sFormat.toLatin1().data(), bottom());
    }
}

// QValidator::State Epr100IntValidator::validate(QString &input, int &pos) const {
//	return QValidator::validate(input,pos);
// }

void Epr100IntValidator::fixup(QString& input) const
{
    if (input.toInt() > top()) {
        input = QString::number(top());
    }
    else if (input.toInt() < bottom() || input.trimmed().isEmpty()) {
        input = QString::number(bottom());
    }
}
