# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'synce-kpm-mainwindow.ui'
#
# Created: Sat Jan 19 17:29:44 2008
#      by: PyQt4 UI code generator 4.3.3
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

class Ui_synce_kpm_mainwindow(object):
    def setupUi(self, synce_kpm_mainwindow):
        synce_kpm_mainwindow.setObjectName("synce_kpm_mainwindow")
        synce_kpm_mainwindow.resize(QtCore.QSize(QtCore.QRect(0,0,582,494).size()).expandedTo(synce_kpm_mainwindow.minimumSizeHint()))

        self.groupBox = QtGui.QGroupBox(synce_kpm_mainwindow)
        self.groupBox.setGeometry(QtCore.QRect(30,10,521,51))
        self.groupBox.setObjectName("groupBox")

        self.label = QtGui.QLabel(self.groupBox)
        self.label.setGeometry(QtCore.QRect(10,20,71,16))
        self.label.setObjectName("label")

        self.label_devicename = QtGui.QLabel(self.groupBox)
        self.label_devicename.setGeometry(QtCore.QRect(100,20,44,14))
        self.label_devicename.setObjectName("label_devicename")

        self.labelDeviceName = QtGui.QLabel(self.groupBox)
        self.labelDeviceName.setGeometry(QtCore.QRect(100,20,161,18))
        self.labelDeviceName.setObjectName("labelDeviceName")

        self.toolButtonDeviceIsLocked = QtGui.QToolButton(self.groupBox)
        self.toolButtonDeviceIsLocked.setEnabled(True)
        self.toolButtonDeviceIsLocked.setGeometry(QtCore.QRect(270,20,25,23))
        self.toolButtonDeviceIsLocked.setObjectName("toolButtonDeviceIsLocked")

        self.labelDeviceIsLocked = QtGui.QLabel(self.groupBox)
        self.labelDeviceIsLocked.setGeometry(QtCore.QRect(300,20,221,18))

        font = QtGui.QFont()
        font.setWeight(75)
        font.setBold(True)
        self.labelDeviceIsLocked.setFont(font)
        self.labelDeviceIsLocked.setScaledContents(True)
        self.labelDeviceIsLocked.setObjectName("labelDeviceIsLocked")

        self.pushButton_Quit = QtGui.QPushButton(synce_kpm_mainwindow)
        self.pushButton_Quit.setGeometry(QtCore.QRect(470,440,75,24))
        self.pushButton_Quit.setObjectName("pushButton_Quit")

        self.tabWidget = QtGui.QTabWidget(synce_kpm_mainwindow)
        self.tabWidget.setEnabled(False)
        self.tabWidget.setGeometry(QtCore.QRect(30,80,521,351))
        self.tabWidget.setObjectName("tabWidget")

        self.tab = QtGui.QWidget()
        self.tab.setObjectName("tab")

        self.label_3 = QtGui.QLabel(self.tab)
        self.label_3.setGeometry(QtCore.QRect(10,20,131,18))
        self.label_3.setObjectName("label_3")

        self.labelDeviceOwner = QtGui.QLabel(self.tab)
        self.labelDeviceOwner.setGeometry(QtCore.QRect(150,20,180,16))
        self.labelDeviceOwner.setObjectName("labelDeviceOwner")

        self.label_5 = QtGui.QLabel(self.tab)
        self.label_5.setGeometry(QtCore.QRect(10,50,131,18))
        self.label_5.setObjectName("label_5")

        self.labelModelName = QtGui.QLabel(self.tab)
        self.labelModelName.setGeometry(QtCore.QRect(150,50,180,16))
        self.labelModelName.setObjectName("labelModelName")

        self.label_7 = QtGui.QLabel(self.tab)
        self.label_7.setGeometry(QtCore.QRect(310,110,91,16))
        self.label_7.setObjectName("label_7")

        self.labelStorageTotal = QtGui.QLabel(self.tab)
        self.labelStorageTotal.setGeometry(QtCore.QRect(370,110,91,16))
        self.labelStorageTotal.setLayoutDirection(QtCore.Qt.RightToLeft)
        self.labelStorageTotal.setObjectName("labelStorageTotal")

        self.storageSelector = QtGui.QComboBox(self.tab)
        self.storageSelector.setGeometry(QtCore.QRect(150,110,141,20))
        self.storageSelector.setObjectName("storageSelector")

        self.label_9 = QtGui.QLabel(self.tab)
        self.label_9.setGeometry(QtCore.QRect(10,110,131,18))
        self.label_9.setObjectName("label_9")

        self.label_11 = QtGui.QLabel(self.tab)
        self.label_11.setGeometry(QtCore.QRect(310,140,91,16))
        self.label_11.setObjectName("label_11")

        self.labelStorageUsed = QtGui.QLabel(self.tab)
        self.labelStorageUsed.setGeometry(QtCore.QRect(370,140,91,16))
        self.labelStorageUsed.setLayoutDirection(QtCore.Qt.RightToLeft)
        self.labelStorageUsed.setObjectName("labelStorageUsed")

        self.label_13 = QtGui.QLabel(self.tab)
        self.label_13.setGeometry(QtCore.QRect(310,170,91,16))
        self.label_13.setObjectName("label_13")

        self.labelStorageFree = QtGui.QLabel(self.tab)
        self.labelStorageFree.setGeometry(QtCore.QRect(370,170,91,16))
        self.labelStorageFree.setLayoutDirection(QtCore.Qt.RightToLeft)
        self.labelStorageFree.setObjectName("labelStorageFree")

        self.batteryStatus = QtGui.QProgressBar(self.tab)
        self.batteryStatus.setGeometry(QtCore.QRect(150,80,141,16))

        font = QtGui.QFont()
        font.setPointSize(9)
        self.batteryStatus.setFont(font)
        self.batteryStatus.setProperty("value",QtCore.QVariant(0))
        self.batteryStatus.setInvertedAppearance(False)
        self.batteryStatus.setObjectName("batteryStatus")

        self.label_2 = QtGui.QLabel(self.tab)
        self.label_2.setGeometry(QtCore.QRect(10,80,131,18))
        self.label_2.setObjectName("label_2")
        self.tabWidget.addTab(self.tab,"")

        self.tab_2 = QtGui.QWidget()
        self.tab_2.setObjectName("tab_2")

        self.listInstalledPrograms = QtGui.QListWidget(self.tab_2)
        self.listInstalledPrograms.setGeometry(QtCore.QRect(10,10,411,251))
        self.listInstalledPrograms.setObjectName("listInstalledPrograms")

        self.pushButton_Refresh = QtGui.QPushButton(self.tab_2)
        self.pushButton_Refresh.setGeometry(QtCore.QRect(430,10,75,24))
        self.pushButton_Refresh.setObjectName("pushButton_Refresh")

        self.pushButton_Uninstall = QtGui.QPushButton(self.tab_2)
        self.pushButton_Uninstall.setGeometry(QtCore.QRect(430,40,75,24))
        self.pushButton_Uninstall.setObjectName("pushButton_Uninstall")

        self.pushButton_InstallCAB = QtGui.QPushButton(self.tab_2)
        self.pushButton_InstallCAB.setGeometry(QtCore.QRect(10,270,75,24))
        self.pushButton_InstallCAB.setObjectName("pushButton_InstallCAB")
        self.tabWidget.addTab(self.tab_2,"")

        self.tab_3 = QtGui.QWidget()
        self.tab_3.setObjectName("tab_3")

        self.viewPartnerships = QtGui.QTreeView(self.tab_3)
        self.viewPartnerships.setGeometry(QtCore.QRect(10,10,411,251))
        self.viewPartnerships.setItemsExpandable(False)
        self.viewPartnerships.setObjectName("viewPartnerships")

        self.labelSyncEngineNotRunning = QtGui.QLabel(self.tab_3)
        self.labelSyncEngineNotRunning.setGeometry(QtCore.QRect(90,110,281,18))

        font = QtGui.QFont()
        font.setWeight(75)
        font.setBold(True)
        self.labelSyncEngineNotRunning.setFont(font)
        self.labelSyncEngineNotRunning.setScaledContents(True)
        self.labelSyncEngineNotRunning.setObjectName("labelSyncEngineNotRunning")

        self.button_add_pship = QtGui.QPushButton(self.tab_3)
        self.button_add_pship.setGeometry(QtCore.QRect(430,10,75,24))
        self.button_add_pship.setObjectName("button_add_pship")

        self.button_delete_pship = QtGui.QPushButton(self.tab_3)
        self.button_delete_pship.setGeometry(QtCore.QRect(430,40,75,24))
        self.button_delete_pship.setObjectName("button_delete_pship")
        self.tabWidget.addTab(self.tab_3,"")

        self.retranslateUi(synce_kpm_mainwindow)
        self.tabWidget.setCurrentIndex(0)
        QtCore.QMetaObject.connectSlotsByName(synce_kpm_mainwindow)

    def retranslateUi(self, synce_kpm_mainwindow):
        synce_kpm_mainwindow.setWindowTitle(QtGui.QApplication.translate("synce_kpm_mainwindow", "SynCE KDE PDA Manager", None, QtGui.QApplication.UnicodeUTF8))
        self.groupBox.setTitle(QtGui.QApplication.translate("synce_kpm_mainwindow", "Device Status", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setText(QtGui.QApplication.translate("synce_kpm_mainwindow", "Device:", None, QtGui.QApplication.UnicodeUTF8))
        self.toolButtonDeviceIsLocked.setText(QtGui.QApplication.translate("synce_kpm_mainwindow", "...", None, QtGui.QApplication.UnicodeUTF8))
        self.labelDeviceIsLocked.setText(QtGui.QApplication.translate("synce_kpm_mainwindow", "Device is locked, please unlock", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton_Quit.setText(QtGui.QApplication.translate("synce_kpm_mainwindow", "&Quit", None, QtGui.QApplication.UnicodeUTF8))
        self.label_3.setText(QtGui.QApplication.translate("synce_kpm_mainwindow", "Owner:", None, QtGui.QApplication.UnicodeUTF8))
        self.label_5.setText(QtGui.QApplication.translate("synce_kpm_mainwindow", "Model name:", None, QtGui.QApplication.UnicodeUTF8))
        self.label_7.setText(QtGui.QApplication.translate("synce_kpm_mainwindow", "Total", None, QtGui.QApplication.UnicodeUTF8))
        self.label_9.setText(QtGui.QApplication.translate("synce_kpm_mainwindow", "Storage space", None, QtGui.QApplication.UnicodeUTF8))
        self.label_11.setText(QtGui.QApplication.translate("synce_kpm_mainwindow", "Used:", None, QtGui.QApplication.UnicodeUTF8))
        self.label_13.setText(QtGui.QApplication.translate("synce_kpm_mainwindow", "Free:", None, QtGui.QApplication.UnicodeUTF8))
        self.label_2.setText(QtGui.QApplication.translate("synce_kpm_mainwindow", "Battery status:", None, QtGui.QApplication.UnicodeUTF8))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab), QtGui.QApplication.translate("synce_kpm_mainwindow", "System Information", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton_Refresh.setText(QtGui.QApplication.translate("synce_kpm_mainwindow", "&Refresh", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton_Uninstall.setText(QtGui.QApplication.translate("synce_kpm_mainwindow", "&Uninstall", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton_InstallCAB.setText(QtGui.QApplication.translate("synce_kpm_mainwindow", "&Install CAB", None, QtGui.QApplication.UnicodeUTF8))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab_2), QtGui.QApplication.translate("synce_kpm_mainwindow", "Software Manager", None, QtGui.QApplication.UnicodeUTF8))
        self.labelSyncEngineNotRunning.setText(QtGui.QApplication.translate("synce_kpm_mainwindow", "Make sure Sync-Engine is running...", None, QtGui.QApplication.UnicodeUTF8))
        self.button_add_pship.setText(QtGui.QApplication.translate("synce_kpm_mainwindow", "Add", None, QtGui.QApplication.UnicodeUTF8))
        self.button_delete_pship.setText(QtGui.QApplication.translate("synce_kpm_mainwindow", "Delete", None, QtGui.QApplication.UnicodeUTF8))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab_3), QtGui.QApplication.translate("synce_kpm_mainwindow", "Partnership manager", None, QtGui.QApplication.UnicodeUTF8))
