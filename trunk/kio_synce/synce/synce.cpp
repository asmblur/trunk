/*
   kio_synce - an ioslave for KDE2
   Copyright (C) 2001  David Eriksson

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with this library; if not, write to the Free
   Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
   */
#include <sys/types.h>
#include <sys/stat.h>

#include <qcstring.h>
#include <qsocket.h>
#include <qdatetime.h>
#include <qbitarray.h>

#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <kapp.h>
#include <kdebug.h>
#include <kmessagebox.h>
#include <kinstance.h>
#include <kglobal.h>
#include <kstddirs.h>
#include <klocale.h>
#include <kurl.h>
#include <ksock.h>

#include "synce.h"

// kdDebug() and friends allow filtering of messages by 'area'
// The file $KDEDIR/share/config/kdebug.area connects the name to the number
// 7139 except from being in the correct range is semirandom, it needs to be
// properly registered/changed in the long run

#define KIO_SYNCE_KDDEBUG_AREA 7139

//
// The kdemain function was generated by KDevelop, how nice
//
using namespace KIO;
extern "C"
{
  int kdemain( int argc, char **argv )
  {
    KInstance instance( "synce" );

    kdDebug(KIO_SYNCE_KDDEBUG_AREA) << "*** Starting kio_synce " << endl;

    if (argc != 4)
    {
      kdDebug(KIO_SYNCE_KDDEBUG_AREA) << "Usage: kio_synce  protocol domain-socket1 domain-socket2" << endl;
      exit(-1);
    }

    kio_synceProtocol slave(argv[2], argv[3]);
    slave.dispatchLoop();

    kdDebug(KIO_SYNCE_KDDEBUG_AREA) << "*** kio_synce Done" << endl;
    return 0;
  }
} 

kio_synceProtocol::kio_synceProtocol(const QCString &pool, const QCString &app)
  : SlaveBase("synce", pool, app), mIsInitialized(false)
{
  kdDebug() << "kio_synceProtocol::kio_synceProtocol()" << endl;
}
/* ---------------------------------------------------------------------------------- */


kio_synceProtocol::~kio_synceProtocol()
{
  kdDebug() << "kio_synceProtocol::~kio_synceProtocol()" << endl;
  uninit();
}

//
// Helper function to initialize the RAPI library
//
// Returns true on success and false on failure
//
bool kio_synceProtocol::init()
{
  if (!mIsInitialized)
  {
    RAPI::HRESULT result = RAPI::CeRapiInit();

    mIsInitialized = (0 == result);
    if (!mIsInitialized)
    {
      error(KIO::ERR_COULD_NOT_CONNECT, QString::fromLatin1("Unable to initialize RAPI"));
    }
  }

  return mIsInitialized;
}

//
// Uninitialize the RAPI library
//
void kio_synceProtocol::uninit()
{
  if (mIsInitialized)
  {
    RAPI::CeRapiUninit();
    mIsInitialized = false;
  }
}

//
// Convert forward slashes ('/') to back-slashes ('\')
//
QString kio_synceProtocol::slashToBackslash(const QString& path)
{
  QString tmp(path);
  for (unsigned i = 0; i < tmp.length(); i++)
  {
    if (tmp[i].latin1() == '/')
      tmp[i] = QChar('\\');
  }
  return tmp;
}

//
// This is a helper class that does ugly conversions between QString and WCHAR*
//
class RapiString
{
  private:
    mutable QString* mpQtString;
    mutable RAPI::WCHAR* mpWcharString;

  public:
    RapiString(const QString& str)
      : mpQtString(new QString(str)), mpWcharString(NULL)
      {
      }

    RapiString(const RAPI::WCHAR* str)
      : mpQtString(NULL), mpWcharString(NULL)
      {
        mpWcharString = new RAPI::WCHAR[RAPI::wcslen(str)+1];
        RAPI::wcscpy(mpWcharString, str);
      }

    virtual ~RapiString()
    {
      if (mpWcharString)
        delete mpWcharString;

      if (mpQtString)
        delete mpQtString;
    }


    operator const RAPI::WCHAR*() const
    {
      if (!mpWcharString)
      {
        mpWcharString = new RAPI::WCHAR[mpQtString->length()+1];
        unsigned i;
        for (i = 0; i < mpQtString->length(); i++)
          mpWcharString[i] = mpQtString->at(i).unicode();
        mpWcharString[i] = 0;
      }

      return mpWcharString;
    }

    operator const QString&() const
    {
      if (!mpQtString)
      {
        mpQtString = new QString();
        mpQtString->setUnicodeCodes(mpWcharString, RAPI::wcslen(mpWcharString));
      }

      return *mpQtString;
    }
};

//
// This converts from a CE_FIND_DATA structure to a UDSEntry
//
void kio_synceProtocol::createUDSEntry( const RAPI::CE_FIND_DATA* source, UDSEntry & destination )
{
  //  kdDebug(KIO_SYNCE_KDDEBUG_AREA) << "[kio_synceProtocol::createUDSEntry]" << endl;

  UDSAtom atom;
  destination.clear();

  RapiString name(source->cFileName);

  //  kdDebug(KIO_SYNCE_KDDEBUG_AREA) << "name=" << static_cast<QString>(name) << endl;

  atom.m_uds = UDS_NAME;
  atom.m_str = name;
  destination.append(atom);

  atom.m_uds = KIO::UDS_FILE_TYPE;
  if (source->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    atom.m_long = S_IFDIR;
  else
    atom.m_long = S_IFREG;
  destination.append(atom);

  atom.m_uds = UDS_ACCESS;
  if (source->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    atom.m_long = 0777;
  else if (source->dwFileAttributes & (FILE_ATTRIBUTE_READONLY|FILE_ATTRIBUTE_INROM))
    atom.m_long = 0444;
  else
    atom.m_long = 0666;
  destination.append(atom);

  atom.m_uds = UDS_SIZE;
  atom.m_long = source->nFileSizeLow;
  destination.append( atom );

  atom.m_uds = UDS_MODIFICATION_TIME;
  atom.m_long = DOSFS_FileTimeToUnixTime(&source->ftLastWriteTime, NULL);
  destination.append( atom );

  atom.m_uds = UDS_ACCESS_TIME;
  atom.m_long = DOSFS_FileTimeToUnixTime(&source->ftLastAccessTime, NULL);
  destination.append( atom );

  atom.m_uds = UDS_CREATION_TIME;
  atom.m_long = DOSFS_FileTimeToUnixTime(&source->ftCreationTime, NULL);
  destination.append( atom );


}

//
// Read a file
//
void kio_synceProtocol::get(const KURL& url )
{
  kdDebug() << "kio_synce::get(const KURL& url)" << endl ;

  RapiString rapi_path(slashToBackslash(url.path()));

  kdDebug(KIO_SYNCE_KDDEBUG_AREA) << "Calling CeCreateFile with path \"" << (QString)rapi_path << "\"" << endl;

  // Initialize RAPI if needed
  if (!init())
    return;

  // Open file for reading, fail if it does not exist
  RAPI::HANDLE handle = RAPI::CeCreateFile(
      rapi_path, 
      GENERIC_READ, 
      FILE_SHARE_READ, 
      NULL, 
      OPEN_EXISTING, 
      FILE_ATTRIBUTE_NORMAL, 
      0);

  if ((RAPI::HANDLE)-1 == handle)
  {
    kdDebug(KIO_SYNCE_KDDEBUG_AREA) << "[kio_synceProtocol::stat] CeCreateFile failed with error code " << RAPI::CeGetLastError() << endl;
    error(KIO::ERR_CANNOT_OPEN_FOR_READING, url.prettyURL());
    return;
  }

  // TODO: call totalSize() with total file size

  // Read data from file
  static const size_t BUFFER_SIZE = 16*1024;

  QByteArray buffer;
  unsigned total_read = 0;
  unsigned part_read = 0;
  do
  {
    buffer.resize(BUFFER_SIZE);

    RAPI::BOOL success = RAPI::CeReadFile(
        handle, 
        buffer.data(), 
        BUFFER_SIZE, 
        &part_read, 
        NULL);

    if (!success)
    {
      kdDebug(KIO_SYNCE_KDDEBUG_AREA) << "[kio_synceProtocol::stat] CeReadFile failed with error code " << RAPI::CeGetLastError() << endl;
      error(KIO::ERR_COULD_NOT_READ, url.path());
      return;
    }

    buffer.resize(part_read);
    data(buffer);

    total_read += part_read;
    processedSize(total_read);
  }
  while(part_read);

  RAPI::CeCloseHandle(handle);

  finished();
}


/* ---------------------------------------------------------------------------------- */

//
// Get info about a file or directory
//
void kio_synceProtocol::stat(const KURL & url)
{
  kdDebug(KIO_SYNCE_KDDEBUG_AREA) << "[kio_synceProtocol::stat]" << endl;

  QString path(slashToBackslash(url.path()));

  //
  // Special case: root directory
  //
  if (path == QString::fromLatin1("\\"))
  {
    kdDebug(KIO_SYNCE_KDDEBUG_AREA) << "[kio_synceProtocol::stat] Root directory" << endl;

    UDSEntry entry;
    UDSAtom atom;

    atom.m_uds = KIO::UDS_NAME;
    atom.m_str = url.fileName();
    entry.append(atom);

    atom.m_uds = KIO::UDS_FILE_TYPE;
    atom.m_long = S_IFDIR;
    entry.append( atom );

    statEntry(entry);

    finished();
    return;
  }

  //
  // If we get a path ending with a backslash, we want to exclude it
  // before we continue
  //
  else if (path[path.length()-1].latin1() == '\\')
  {
    path = path.left(path.length()-1);
  }

  // Initialize RAPI if needed
  if (!init())
    return;

  RapiString rapi_path(path);

  RAPI::CE_FIND_DATA find_data;
  memset(&find_data, 0, sizeof(find_data));

  RAPI::HANDLE handle = RAPI::CeFindFirstFile(rapi_path, &find_data);

  if ((RAPI::HANDLE)-1 == handle)
  {
    kdDebug(KIO_SYNCE_KDDEBUG_AREA) << "[kio_synceProtocol::stat] CeFindFirstFile failed with error code " << RAPI::CeGetLastError() << endl;
    error(KIO::ERR_DOES_NOT_EXIST, url.path());
    return;
  }

  UDSEntry entry;
  createUDSEntry(&find_data, entry);
  statEntry(entry);

  finished();
}


/* --------------------------------------------------------------------------- */


//
// List the contents of a directory
//
// TODO: use FindFirstFile/FindNextFile and not FindAllFiles
//
void kio_synceProtocol::listDir( const KURL & url )
{
  kdDebug(KIO_SYNCE_KDDEBUG_AREA) << "[kio_synceProtocol::listDir]" << endl;

  // Initialize RAPI if needed
  if (!init())
    return;

  QString pattern(slashToBackslash(url.path()));
  if (pattern[pattern.length()-1].latin1() != '\\')
  {
    KURL redir( QString::fromLatin1( "synce:/") );
    redir.setPath( url.path() + QString::fromLatin1("/") );
    redirection( redir );
    finished();
    return;
  }

  pattern.append(QString::fromLatin1("*.*"));
  kdDebug(KIO_SYNCE_KDDEBUG_AREA) << "[kio_synceProtocol::listDir] Searching for \"" << pattern << "\"" << endl;

  RapiString rapi_pattern(pattern);

  RAPI::CE_FIND_DATA* find_data = NULL;
  RAPI::DWORD file_count = 0;

  RAPI::CeFindAllFiles(rapi_pattern, 
      FAF_ATTRIBUTES|FAF_CREATION_TIME|FAF_LASTACCESS_TIME|FAF_LASTWRITE_TIME|FAF_NAME|FAF_SIZE_LOW,
      &file_count, &find_data);

  UDSEntry entry;
  for (unsigned i = 0; i < file_count; i++)
  {
    createUDSEntry(find_data + i, entry);
    listEntry(entry, false);
  }

  listEntry( entry, true ); // ready

  RAPI::CeRapiFreeBuffer(find_data);


  finished();
}


//
// Delete file or directory
//
void kio_synceProtocol::del(const KURL &url, bool isfile)
{
  // Initialize RAPI if needed
  if (!init())
    return;

  RapiString rapi_path(slashToBackslash(url.path()));

  if (isfile)
  {
    if (0 == RAPI::CeDeleteFile(rapi_path))
    {
      kdDebug(KIO_SYNCE_KDDEBUG_AREA) << "[kio_synceProtocol::del] CeDeleteFile failed with error code " << RAPI::CeGetLastError() << endl;
      error(KIO::ERR_CANNOT_DELETE, url.prettyURL());
      return;
    }
  }
  else
  {
    // Let's not hope we have to to recursive remove here
    if (0 == RAPI::CeRemoveDirectory(rapi_path))
    {
      kdDebug(KIO_SYNCE_KDDEBUG_AREA) << "[kio_synceProtocol::del] CeRemoveDirectory failed with error code " << RAPI::CeGetLastError() << endl;
      error(KIO::ERR_CANNOT_DELETE, url.prettyURL());
      return;
    }
  }

  finished();
}

//
// Write file
//
void kio_synceProtocol::put(const KURL& url, int /*permissions*/, bool overwrite, bool resume)
{
  kdDebug(KIO_SYNCE_KDDEBUG_AREA) << "[kio_synceProtocol::put]" << endl;

  if (resume)
  {
    error(KIO::ERR_UNSUPPORTED_ACTION, url.prettyURL());
    return;
  }

  // Initialize RAPI if needed
  if (!init())
    return;

  RapiString rapi_path(slashToBackslash(url.path()));

  RAPI::HANDLE handle = RAPI::CeCreateFile(
      rapi_path,
      GENERIC_WRITE,
      0,
      NULL,
      overwrite ? CREATE_ALWAYS : CREATE_NEW,
      FILE_ATTRIBUTE_NORMAL,
      0);

  if ((RAPI::HANDLE)-1 == handle)
  {
    kdDebug(KIO_SYNCE_KDDEBUG_AREA) << "[kio_synceProtocol::put] CeCreateFile failed with error code " << RAPI::CeGetLastError() << endl;
    error(KIO::ERR_CANNOT_OPEN_FOR_WRITING, url.prettyURL());
    return;
  }

  int part_read = 0;

  do
  {
    QByteArray buffer;
    unsigned part_written = 0;

    dataReq(); // Request for data

    part_read = readData( buffer );

    //kdDebug(1739)<<"kio_synceProtocol::put(): after readData(), read "<<result<<" bytes"<<endl;
    if (part_read > 0)
    {
      RAPI::BOOL success = RAPI::CeWriteFile(
          handle, 
          buffer.data(), 
          buffer.size(), 
          &part_written, 
          NULL);

      if (!success || part_written != (unsigned)part_read)
      {
        RAPI::CeCloseHandle(handle);

        kdDebug(KIO_SYNCE_KDDEBUG_AREA) << "[kio_synceProtocol::stat] CeWriteFile failed with error code " << RAPI::CeGetLastError() << endl;
        error(KIO::ERR_COULD_NOT_WRITE, url.prettyURL());
        return;
      }
    }
  }
  while (part_read > 0);

  RAPI::CeCloseHandle(handle);

  finished();
}

//
// Create directory
//
void kio_synceProtocol::mkdir(const KURL&url, int /*permissions*/)
{
  kdDebug(KIO_SYNCE_KDDEBUG_AREA) << "[kio_synceProtocol::mkdir]" << endl;

  // Initialize RAPI if needed
  if (!init())
    return;

  RapiString rapi_path(slashToBackslash(url.path()));

  if (0 == RAPI::CeCreateDirectory(rapi_path, NULL))
  {
    kdDebug(KIO_SYNCE_KDDEBUG_AREA) << "[kio_synceProtocol::stat] CeCreateDirectory failed with error code " << RAPI::CeGetLastError() << endl;
    error(KIO::ERR_COULD_NOT_MKDIR, url.prettyURL());
    return;
  }

  finished();
}

void kio_synceProtocol::rename(const KURL& src, const KURL& dest, bool overwrite)
{
  kdDebug(KIO_SYNCE_KDDEBUG_AREA) << "[kio_synceProtocol::rename]" << endl;

  // Initialize RAPI if needed
  if (!init())
    return;

  RapiString rapi_src(slashToBackslash(src.path()));
  RapiString rapi_dest(slashToBackslash(dest.path()));

  if (overwrite)
  {
    // TODO: Check return status and error code, fail if not File Not Found
    RAPI::CeDeleteFile(rapi_dest);
  }

  if (0 == RAPI::CeMoveFile(rapi_src, rapi_dest))
  {
    kdDebug(KIO_SYNCE_KDDEBUG_AREA) << "[kio_synceProtocol::rename] CeMoveFile failed with error code " << RAPI::CeGetLastError() << endl;
    error(KIO::ERR_CANNOT_RENAME, src.prettyURL());
    return;
  }

  finished();
}
