%define prefix   /usr
%define name     synce-libsynce
%define ver      0.1
%define rel      1

Summary: SynCE: Basic library used by applications in the SynCE project.
Name: %{name}
Version: %{ver}
Release: %{rel}
License: MIT
Group: Development/Libraries
Source: %{name}-%{version}.tar.gz
URL: http://synce.sourceforge.net/
Distribution: SynCE RPM packages
Vendor: The SynCE Project
Packager: David Eriksson <twogood@users.sourceforge.net>
#Buildroot: %{_tmppath}/%{name}-%{version}-%{release}-root
Buildroot: %{_tmppath}/synce-root

%description
Libsynce is part of the SynCE project:

  http://synce.sourceforge.net/

This library is required to compile (at least) the following parts of the
SynCE project:

  o librapi2
  o dccmd

%prep
%setup

%build
%configure
make

%install
%makeinstall

%files
%{prefix}/include/synce.h
%{prefix}/include/synce_log.h
%{prefix}/include/synce_socket.h
%{prefix}/include/synce_types.h
%{prefix}/lib/libsynce.*
%{prefix}/share/aclocal/libsynce.m4

