/***************************************************************************
 * Copyright (c) 2003 Volker Christian <voc@users.sourceforge.net>         *
 *                                                                         *
 * Permission is hereby granted, free of charge, to any person obtaining a *
 * copy of this software and associated documentation files (the           *
 * "Software"), to deal in the Software without restriction, including     *
 * without limitation the rights to use, copy, modify, merge, publish,     *
 * distribute, sublicense, and/or sell copies of the Software, and to      *
 * permit persons to whom the Software is furnished to do so, subject to   *
 * the following conditions:                                               *
 *                                                                         *
 * The above copyright notice and this permission notice shall be included *
 * in all copies or substantial portions of the Software.                  *
 *                                                                         *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS *
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF              *
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY    *
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,    *
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE       *
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                  *
 ***************************************************************************/

#include "pdaconfigdialogimpl.h"
#include "synctasklistitem.h"
#include "removepartnershipdialogimpl.h"

#include <librra.h>

#include <qcheckbox.h>
#include <klineedit.h>
#include <kpushbutton.h>
#include <kapplication.h>
#include <kmessagebox.h>
#include <ksimpleconfig.h>
#include <kdebug.h>

#include <unistd.h>
#include <stdlib.h>

#ifdef WITH_DMALLOC
#include <dmalloc.h>
#include <kde_dmalloc.h>
#endif

PdaConfigDialogImpl::PdaConfigDialogImpl(QString pdaName, QWidget* parent,
        const char* name, bool modal, WFlags fl)
        : PdaConfigDialog(parent, name, modal, fl)
{
    this->pdaName = pdaName;
    partnershipCreated.setTime_t(0);
    readConfig();
    updateFields();
    buttonApply->setEnabled(false);
    objectTypeList->setFullWidth(true);
    syncTaskItemList.setAutoDelete(true);
    syncAtConnectCheckbox->setEnabled(false);
}


PdaConfigDialogImpl::~PdaConfigDialogImpl()
{
    writeConfig();
}


void PdaConfigDialogImpl::updateFields()
{
    SyncTaskListItem *item;

    passwordEdit->setText(password);
    masqEnabledCheckbox->setChecked(masqEnabled);
    syncAtConnectCheckbox->setChecked(syncAtConnect);
    for (item = syncTaskItemList.first(); item; item = syncTaskItemList.next()) {
        item->undo();
    }

    buttonApply->setDisabled(true);
}


QString PdaConfigDialogImpl::getPassword()
{
    return password;
}


bool PdaConfigDialogImpl::getMasqueradeEnabled()
{
    return masqEnabled;
}


bool PdaConfigDialogImpl::getSyncAtConnect()
{
    return syncAtConnect;
}


void PdaConfigDialogImpl::writeConfig()
{
    SyncTaskListItem *syncTaskListItem;

    QString allTypeIds;
    ksConfig->setGroup(pdaName);
    ksConfig->writeEntry("Password", password);
    ksConfig->writeEntry("Masquerade", masqEnabled);
    ksConfig->writeEntry("SyncAtConnect", syncAtConnect);
    for (syncTaskListItem = syncTaskItemList.first(); syncTaskListItem; syncTaskListItem = syncTaskItemList.next()) {
        allTypeIds = allTypeIds + QString::number(syncTaskListItem->getObjectType()->id) + ";";
        ksConfig->writeEntry(QString::number(syncTaskListItem->getObjectType()->id), syncTaskListItem->isOn());
        ksConfig->writeEntry(QString::number(syncTaskListItem->getObjectType()->id) + "-PreferedLibrary", syncTaskListItem->getPreferedLibrary());
        ksConfig->writeEntry(QString::number(syncTaskListItem->getObjectType()->id) + "-PreferedOffer", syncTaskListItem->getPreferedOffer());
        ksConfig->writeEntry(QString::number(syncTaskListItem->getObjectType()->id) + "-LastSynchronized", syncTaskListItem->getLastSynchronized());
    }
    ksConfig->writeEntry("SyncObjectIDs", allTypeIds);
    ksConfig->writeEntry("PartnerName", partnerName);
    ksConfig->writeEntry("PartnerId", partnerId);
    ksConfig->writeEntry("PartnershipCreated", partnershipCreated);
    ksConfig->sync();
}


void PdaConfigDialogImpl::readConfig()
{
    ksConfig=kapp->config();
    if (ksConfig->hasGroup(pdaName)) {
        ksConfig->setGroup(pdaName);
        masqEnabled = ksConfig->readBoolEntry("Masquerade");
        password = ksConfig->readEntry("Password");
        syncAtConnect = ksConfig->readBoolEntry("SyncAtConnect");
        partnerName = ksConfig->readEntry("PartnerName", "");
        partnerId = ksConfig->readUnsignedLongNumEntry("PartnerId", 0);
        partnershipCreated = ksConfig->readDateTimeEntry("PartnershipCreated", &partnershipCreated);
        newPda = false;
    } else {
        newPda = true;
    }
}


void PdaConfigDialogImpl::applySlot()
{
    SyncTaskListItem *syncTaskListItem;

    if (buttonApply->isEnabled()) {
        this->password = passwordEdit->text();
        masqEnabled = masqEnabledCheckbox->isChecked();
        syncAtConnect = syncAtConnectCheckbox->isChecked();
        for (syncTaskListItem = syncTaskItemList.first(); syncTaskListItem; syncTaskListItem = syncTaskItemList.next()) {
            syncTaskListItem->makePersistent();
        }
        writeConfig();
        buttonApply->setDisabled(true);
    }
}


void PdaConfigDialogImpl::masqChangedSlot()
{
    buttonApply->setEnabled(true);
}


void PdaConfigDialogImpl::disableApply()
{
    updateFields();
}


void PdaConfigDialogImpl::changedSlot()
{
    buttonApply->setEnabled(true);
}


QString PdaConfigDialogImpl::getDeviceIp()
{
    char *path = NULL;
    synce::synce_get_directory(&path);
    QString synceDir = QString(path);

    if (path)
        free(path);

    KSimpleConfig activeConnection(synceDir + "/" + pdaName, true);
    activeConnection.setGroup("device");
    QString deviceIp = activeConnection.readEntry("ip");

    return deviceIp;
}


void PdaConfigDialogImpl::objectTypeList_rightButtonClicked( QListViewItem *item, const QPoint &, int )
{
    ((SyncTaskListItem *) item)->openPopup();
}


bool PdaConfigDialogImpl::isNewPda()
{
    return newPda;
}


void PdaConfigDialogImpl::setPartner(QString partnerName, uint32_t partnerId)
{
    this->partnerName = partnerName;
    this->partnerId = partnerId;

    if (partnerId != 0) {
        syncAtConnectCheckbox->setEnabled(true);
    }
}


void PdaConfigDialogImpl::setNewPartner(QString partnerName, uint32_t partnerId)
{
    setPartner(partnerName, partnerId);
    partnershipCreated = QDateTime(QDate::currentDate(), QTime::currentTime());
    kdDebug(2120) << "Partnership created: " << partnershipCreated.toString();
}


QString PdaConfigDialogImpl::getPartnerName()
{
    return partnerName;
}


uint32_t PdaConfigDialogImpl::getPartnerId()
{
    return partnerId;
}


void PdaConfigDialogImpl::addSyncTask(ObjectType *objectType, uint32_t partnerId)
{
    SyncTaskListItem *item;
    QDateTime lastSynchronized;

    ksConfig->setGroup(pdaName);
    item = new SyncTaskListItem(objectType, objectTypeList, partnerId);
    item->setOn(ksConfig->readBoolEntry(QString::number(objectType->id)));
    item->setPreferedLibrary(ksConfig->readEntry(QString::number(objectType->id) + "-PreferedLibrary"));
    item->setPreferedOffer(ksConfig->readEntry(QString::number(objectType->id) + "-PreferedOffer"));
    lastSynchronized = item->getLastSynchronized();
    item->setLastSynchronized(ksConfig->readDateTimeEntry(QString::number(objectType->id) + "-LastSynchronized", &lastSynchronized));
    item->setFirstSynchronization((partnershipCreated > item->getLastSynchronized()));
    connect((const QObject *) item, SIGNAL(stateChanged(bool)), this, SLOT(changedSlot()));
    connect((const QObject *) item, SIGNAL(serviceChanged()), this, SLOT(changedSlot()));
    objectTypeList->insertItem(item);
    syncTaskItemList.append(item);
    item->makePersistent();
}


QPtrList<SyncTaskListItem>& PdaConfigDialogImpl::getSyncTaskItemList()
{
    return syncTaskItemList;
}
