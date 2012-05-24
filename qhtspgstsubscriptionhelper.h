#ifndef QHTSPGSTSUBSCRIPTIONHELPER_H
#define QHTSPGSTSUBSCRIPTIONHELPER_H

#include <QObject>
#include <QHash>

#include "gsthtspsrc.h"
#include "qhtspsubscription.h"
#include "qhtspstream.h"

class QHtspGstSubscriptionHelper : public QObject
{
    Q_OBJECT
public:
    explicit QHtspGstSubscriptionHelper(QHtspSubscription *subscription);

    GstHtspSrc *gstHtspSrc();

    void start();

signals:
    void padAdded(GstPad *pad);

private:
    GstHtspSrc *m_gstHtspSrc;
    QHash<quint8, GstPad*> m_pads;
    QHtspSubscription *m_subscription;

private slots:
    void _initPads();
    void _pushData(QHtspStream *stream);

};

#endif // QHTSPGSTSUBSCRIPTIONHELPER_H
