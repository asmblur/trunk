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
#ifndef AGSYNC_H
#define AGSYNC_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef WITH_AGSYNC

#include <qstring.h>
#include <AGReader.h>
#include <AGWriter.h>
#include <AGNet.h>
#include <AGServerConfig.h>
#include <AGLocationConfig.h>

#include <rakisyncplugin.h>


class Rra;
class SyncTaskListItem;
class AGSyncConfigImpl;


/**
@author Volker Christian,,,
*/
class AGSync : public RakiSyncPlugin
{
Q_OBJECT
public:
    AGSync();
    ~AGSync();
    void createConfigureObject(KConfig *ksConfig);
    void configure();
    bool preSync(bool firstSynchronize, uint32_t partnerId);
    int syncContext();

private:
    bool sync();
    void doSync(AGReader *r, AGWriter *w, AGNetCtx *ctx);
    void configAGSync();
    void doServerSync(AGReader *r, AGWriter *w, AGServerConfig *s, AGNetCtx *ctx);
    AGLocationConfig *locConfig;
    AGSyncConfigImpl *configDialog;
};

#endif

#endif
