#include "rawoutboundformmodel.h"

using namespace across;

RawOutboundFormModel::RawOutboundFormModel(QObject *parent) : QObject(parent)
{

}

const QString &RawOutboundFormModel::rawText() const
{
    return m_rawText;
}

void RawOutboundFormModel::setRawText(const QString &newRawText)
{
    if (m_rawText == newRawText)
        return;
    m_rawText = newRawText;
    emit rawTextChanged();
}
