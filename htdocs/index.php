<?php include 'header.php'; ?>

<div class="PAGE">

<A href="http://sourceforge.net"><IMG
src="http://sourceforge.net/sflogo.php?group_id=30550&amp;type=5" width="210"
height="62" border="0" alt="SourceForge Logo" align=right></A> 

<h1>The SynCE Project</h1>

<p>The purpose of the SynCE project is to provide a means of communication with
a Windows CE device from a computer running Linux, FreeBSD or a similar
operating system.</p>

<a name="news"></a>
<h2>News</h2>

<p><b>July 7, 2004</b> Added a <a href="contrib/ipaq">start/stop script</a>
contributed by Tony Stout. Added info about my packet decoders for Ethereal to
the <a href="capture.php">How to capture packets for analyzing</a> page. </p>

<p><b>May 28, 2004</b> New page linking to all <a href="notes.php">Developer's
notes</a></p>

<p><b>May 4, 2004</b> Support for recurring appointments is coming along
nicely, the PWI file format is getting decoded, a script called <a
href="contrib/pcp-r">pcp-r</a> by Henrik Isacsson seems very useful, our new
project member Markus Biermaier has written a tool called <a
href="pyrapi/ce_dirview/">CE_DirView</a> that uses the Python bindings, Volker
Christian is back after his knee injury and the sun shines!</p>

<p><b>January 3, 2004</b> Happy New Year from the SynCE team!</p>

<p><b>December 21, 2003</b> Released SynCE 0.8.9, a development preview of the
upcoming SynCE 0.9.0 release. See the <a
href="http://sourceforge.net/project/shownotes.php?release_id=205447">release
notes</a> for more information. This release is only available for download as
<a
href="http://sourceforge.net/project/showfiles.php?group_id=30550&package_id=22569&release_id=205447">.tar.gz
archives</a>.</p>

<p><b>December 18, 2003</b> I just read that <a href="http://kernel.org/">Linux
kernel 2.6.0</a> has been released. However, I'm not sure that the
<tt>ipaq</tt> kernel driver <a
href="http://synce.sourceforge.net/synce/usb_linux.php">used by SynCE</a> is
working yet. I probably don't have time to test this before christmas, but
reports about success/failure are welcome.</p>

<p><b>December 8, 2003</b> New <a
href="http://sourceforge.net/mailarchive/forum.php?thread_id=3562405&forum_id=1226">SynCE
Developer's notes</a>.</p>

<p><b>November 16, 2003</b> Created the IRC channel <tt>#synce</tt> on
<tt>irc.freenode.net</tt>. You are all very welcome!</p>

<?php /*

<p><b>November 14, 2003</b> New <a
href="http://sourceforge.net/mailarchive/forum.php?thread_id=3464828&forum_id=1226">SynCE
Developer's notes</a>. (Previous notes: <a
href="http://sourceforge.net/mailarchive/message.php?msg_id=5825027">August
16</a>, <a
href="http://sourceforge.net/mailarchive/message.php?msg_id=5874600">August
22</a>.)</p>

<p><b>October 28, 2003</b> Improved this web site.</p>

<p><b>August 16, 2003</b> A GNOME 2 version of the MultiSync plugin was
released today. It has version 0.8.2 and can be <a
href="http://sourceforge.net/project/showfiles.php?group_id=30550">downloaded</a>
in .tar.gz or RPM format.</p>

<p><b>August 14, 2003</b> We would like to welcome <a
href="mailto:snaggen@users.sourceforge.net">Mattias Eriksson</a> to the SynCE
project!  He was the one who ported the <tt>multisync_plugin</tt> module to
GNOME 2 and he will continue to support it.</p>

<p><b>August 11, 2003</b> Added some information about the new <a
href="unshield.php">Unshield</a> tool that can be used to extract files from
InstallShield installers.</p>

<p><b>July 30, 2003</b> SynCE 0.8 is released! SynCE now includes the
possiblity to synchronize calendar and tasks, in addition to the previous
address book synchronization support. However, recurring events are not yet
supported. <a href="download.php">Download</a>.</p>

<p><b>June 13, 2003</b> Some documentation improvements, most notably a <a
href="qa.php">Questions and Answers</a> page.</p>

<p><b>June 8, 2003</b> The June issue of the swedish computer magazine <a
href="http://datormagazin.se/">Datormagazin</a> has an article about SynCE on
page 54! Cool! I would also like to welcome <a
href="mailto:jonmcd@users.sourceforge.net">Jonathan McDowell</a> as a new SynCE
developer. He is a Debian developer so this can only be good! :-)</p>

<p><b>June 1, 2003</b> Provided a how-to on making synce-serial-start run
automatically on connect with a <a href="hotplug.php">Linux hotplug
script</a>.</p>

<p><b>April 9, 2003</b> Added documentation on <a href="ip_forward.php">IP
forwarding</a> some very small <a href="macosx.php">documentation for Mac OS X
users</a>, and documentation of the <a href="rrac.php">RRAC protocol</a>.</p>

<p><b>March 23, 2003</b> The Debian packages are now available in an <a
href="debian.php">apt repository</a>.</p>

<p><b>March 22, 2003</b> Take a look at the <a href="kde/">KDE Integration</a>
of SynCE, including screenshots!</p>

<p><b>March 9, 2003</b> Thanks to <a
href="mailto:tbutter@users.sourceforge.net">Thomas Butter</a>, there are now <a
href="debian.php">Debian packages</a> of SynCE available for download!</p>

<p><b>March 7, 2003</b> Since late february there is a Perl language binding
available for RAPI. This has been implemented by <a
href="mailto:osar@users.sourceforge.net">Andreas Pohl</a> and is available <a
href="http://sourceforge.net/project/showfiles.php?group_id=30550&release_id=142515">as
tarball</a> or <a href="cvs.php">directly
from the CVS module <tt>perlrapi</tt></a>.</p>

<p><b>March 1, 2003</b> Updated the <a
href="http://cvs.sourceforge.net/cgi-bin/viewcvs.cgi/synce/twogood-files/roadmap.txt?rev=HEAD&amp;content-type=text/vnd.viewcvs-markup">roadmap</a>.
Please join the <a
href="http://lists.sourceforge.net/mailman/listinfo/synce-devel">synce-devel</a>
mailing list to discuss it!</p>

<p><b>February 24, 2003</b> Added documentation on <a
href="multisync.php">Using MultiSync for synchronization</a>.</p>

<p><b>February 23, 2003</b> SynCE 0.7 released!</p>

<p>SynCE now includes the possiblity to synchronize the Pocket PC address book
with for example Evolution with a plugin to the <a
href="http://multisync.sourceforge.net">MultiSync</a> program.  This requires
the <tt>rra</tt> and <tt>multisync_plugin</tt> modules. People who want to
write code to convert beteen vCalendar and Tasks/Calendar on Pocket PC, please
send a mail to the <a
href="http://sourceforge.net/mail/?group_id=30550">development mailing
list</a>!</p>

<p>News for RedHat 8.0 RPM users:</p>
<ul>

<li>A single RPM (<tt>synce-0.7-1.i386.rpm</tt>) now includes the modules <tt>libsynce</tt>,
<tt>librapi2</tt>, <tt>dccm</tt>, <tt>serial</tt> and <tt>rra</tt>.</li>


<li>There is a separate RPM (<tt>synce-devel-0.7-1.i386.rpm</tt>) that contains header
files and development libraries.</li>

<li>The <tt>gnomevfs</tt> module is now also available as an RPM package.</li>

</ul>

<p>I would also like to mention that Volker Christian has joined the SynCE
project to improve the KDE integration. KDE 3.x users, please have look at the
<tt>rapip</tt> module in <a
href="cvs.php">CVS</a>!</p>

<p><b>January 30, 2003</b> The absence of news does fortunately not mean that
nothing is happening with the project. We have recevied several KDE I/O-slaves
for KDE 3 and hope to release one of them as soon as possible. By the way,
SynCE 0.6 was released a few weeks ago. Other good news is that Contacts
synchronization with Pocket PC is now possible with the <tt>rra</tt> and
<tt>multisync_plugin</tt> <a
href="http://sourceforge.net/cvs/?group_id=30550">CVS modules</a> together with
the <a href="http://multisync.sourceforge.net/">Multisync</a> program. If you
want to try this right now you also need to get <tt>libsynce</tt> and <tt>librapi2</tt>
from CVS.  People who want to write code to convert beteen vCalendar and
Tasks/Calendar on Pocket PC, please send a mail to the development mailing
list!</p>

<p><b>December 16, 2002</b> We would like to welcome Fredrik Nilson as a
developer in the SynCE project. He has authored a GNOME-VFS module for
SynCE. We hope to include this module in the next SynCE release.</p>

<p><b>November 30, 2002</b> Released SynCE version 0.5 which includes
dccm 0.5, librapi2 0.5, libsynce 0.5, synce-serial 0.5 and trayicon 0.3. <a
href="using.php">Download and use</a>.</p>

<p><b>November 28, 2002</b> Created mailing lists <a
href="http://lists.sourceforge.net/mailman/listinfo/synce-announce">synce-announce</a>
and <a
href="http://lists.sourceforge.net/mailman/listinfo/synce-users">synce-users</a>
in case <a
href="http://lists.sourceforge.net/mailman/listinfo/synce-devel">synce-devel</a>
gets to technical for someone.</p>

<p><b>November 24, 2002</b> Released SynCE version 2002-11-24 which includes
dccm 0.4, librapi2 0.4, libsynce 0.4, synce-serial 0.4 and trayicon 0.2.</p>

<p><b>November 21, 2002</b>. Added USB connection instructions to the <a
href="using.php">Download and use</a> page.</p>

<p><b>November 20, 2002</b>. SynCE-Serial 0.4 released. This should fix the
connection problems some people have experienced. <a
href="http://sourceforge.net/project/showfiles.php?group_id=30550&release_id=123266">Download</a>.</p>

<p><b>November 18, 2002</b>. SynCE 0.3.1 released, including the GNOME 2 tray
icon!</p>

<p><b>November 17, 2002</b>. New homepage released!</p>

-->

<a name="helpwanted"></a>
<h2>Help wanted!</h2>

<p>We need some responsbile people who want to develop the following extensions
on top of the SynCE project. Please send a mail to <a
href="mailto:synce-devel@lists.sourceforge.net">synce-devel@lists.sourceforge.net</a>
if you want to help us!</p>

<ul>

<li class=SPACED>Integration with KDE's KitchenSync framework.</li>

</ul>

*/ ?>

<hr size=1>
<p>

<form action="https://www.paypal.com/cgi-bin/webscr" method="post">
<input type="hidden" name="cmd" value="_xclick">
<input type="hidden" name="business" value="twogood@users.sourceforge.net">
<input type="hidden" name="item_name" value="The SynCE Project">
<input type="hidden" name="no_shipping" value="1">
<input type="hidden" name="return" value="http://synce.sourceforge.net/synce/thanks.php">
<input type="hidden" name="cancel_return" value="http://synce.sourceforge.net/synce/">
<input type="hidden" name="cn" value="Comments">
<input type="hidden" name="currency_code" value="USD">
<input type="hidden" name="tax" value="0">

<table cellpadding=5 width="100%"><tr>

<td class=MIDDLE><input type="image"
src="https://www.paypal.com/images/x-click-but21.gif" border="0" name="submit"
alt="Make a donation" title="Make a donation"></td>

<td class=MIDDLE>You can now support the SynCE project by donating!</td>

</tr></table>
</form>

</p>
<hr size=1>

<h2>Site contents</h2>

<table widht="100%"><tr><td width="50%" valign="top">

<h3>General</h3>

<ul>
<li><a href="#news">News</a></li>
<li><a href="#overview">Overview</a></li>
<li class=SPACED><a href="links.php">Links</a></li>

<li><a href="download.php">Download and install</a></li>
<li><a href="setup.php">Setup</a></li>
<li><a href="start.php">Connect</a></li>
<li><a href="tools.php">Tools</a></li>
<li><a href="disconnect.php">Disconnect</a></li>
<li><a href="multisync.php">Using MultiSync for synchronization</a></li>
<li><a href="qa.php">Questions and Answers</a></li>
<li class=SPACED><a href="help.php">Get help</a></li>

<li><a href="http://sourceforge.net/projects/synce/">SourceForge Project Page</a></li>
</ul>

<h3>Extras</h3>

<ul>
<li><a href="orange.php">The Orange tool and library</a></li>
<li><a href="unshield.php">The Unshield tool and library</a></li>
<li class=SPACED><a href="dynamite.php">The Dynamite tool and library</a></li>
</ul>

</td><td width="50%" valign="top">

<h3>Integration with desktop environments</h3>

<ul>

<li><a href="gnome.php">GNOME</a></li>
<li><a href="kde/">KDE</a></li>

<li><a href="pyrapi/ce_dirview/">CE_DirView</a></li>

</ul>

<h3>Operating system-specific details</h3>

<ul>

<li><a href="freebsd.php">FreeBSD</a></li>
<li><a href="linux.php">Linux</a></li>
<li><a href="macosx.php">MacOS X</a></li>
<li><a href="openbsd.php">OpenBSD</a></li>

</ul>

<h3>Development information</h3>

<ul>
<li><a href="notes.php">Developer's notes</a></li>
<li><a href="tasks.php">Help wanted!</a></li>
<li><a href="architecture.php">Architecture</a></li>
<li><a href="developers.php">The SynCE developers</a></li>
<li><a href="capture.php">How to capture packets for analyzing</a></li>
<li><a href="#future">Future</a></li>
</ul>

</td></tr></table>

<hr size=1>


<a name="overview"></a>
<h2>Overview</h2>

<p>SynCE is composed by the following parts:</p>

<table>

<tr>
<th>libsynce</th>
<td>Common functions used by other parts of SynCE.</td>
</tr>

<tr>
<th>librapi2</th>
<td>A library that implements RAPI, the Remote Access Programming
Interface, which allows remote control of a device connected through SynCE.
Also included are simple command line tools that uses RAPI.</td>
</tr>

<tr>
<th>dccm</th>
<td>An application that maintains the connection with a device
connected through SynCE.</td>
</tr>

<tr>
<th>serial</th>
<td>Tools for configuring, starting and aborting a serial connection for 
use with SynCE.</td>
</tr>

<tr>
<th>rra</th>
<td>A library needed for synchronization functions.</td>
</tr>

<tr>
<th>multisync_plugin</th>
<td>A plugin for <a href="http://multisync.sourceforge.net">MultiSync</a> to
synchronize Contacts.</td> 
</tr>

<tr>
<th>trayicon</th>
<td>A tray icon for GNOME 2 showing if a device is connected or not.</td>
</tr>

</table>

<a name="future"></a>
<h2>Plans for the future</h2>

<p>In the future, SynCE must be more user-friendly.</p>

<p>Please read the <a href="http://cvs.sourceforge.net/cgi-bin/viewcvs.cgi/synce/twogood-files/roadmap.txt?rev=HEAD&amp;content-type=text/vnd.viewcvs-markup">roadmap</a> for more details.</p>

</div>
<?php include 'footer.php'; ?>
