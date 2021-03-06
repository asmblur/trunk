<?php include 'header.php'; ?>

<div class="PAGE">

<p>Return to <a href="index.php">main page</a>.</p>

<h1>SynCE - Questions and Answers for common issues</h1>

<p>For issues with synchronization (MultiSync or the rra module), please visit
the <a href="multisync.php">Using MultiSync</a> page.</p>

<p>For issues regarding Mac OS X, please visit the <a
href="macosx.php">Mac OS X hints</a> page.</p>

<h2>Unable to initialize RAPI / Failed to get connection info</h2>

<p><b>Q:</b> I setup everything and connected my PDA but when I try the <a
href="tools.php">tools</a> I get a message similar to this in SynCE 0.8 or later:</p>

<blockquote><pre>pstatus: Unable to initialize RAPI: An unspecified failure has occurred</pre></blockquote>

<p>Or this message in earlier versions of SynCE:</p>

<blockquote><pre>ReadConfigFile: stat: No such file or directory
pstatus: Unable to initialize RAPI: Failure</pre></blockquote>

<p>If I add <tt>-d 4</tt> as parameters to <tt>pstatus</tt>, I get this a
message like this too:</p>

<blockquote><pre>[synce_info_new:31] unable to open file: /home/david/.synce/active_connection
[rapi_context_connect:88] Failed to get connection info</pre></blockquote>

<p><b>A:</b> This means that the PDA has not connected to dccm or that you run
the tools and dccm as different users. Please make sure that:</p>

<ul>
<li>you are <i>not</i> using an <a href="usb_linux_hub.php">external USB hub</a></li>
<li>you have installed the patch for the <a href="usb_linux.php#werestuffed">"we're stuffed" bug</a></li>
<li>you supplied a password to dccm if your device is password-protected</li>
<li>dccm was started before you ran synce-serial-start</li>
<li>that the PPP connection is successful (look in your system logs)</li>
<li>no firewall configuration prevents the PDA from connecting to dccm</li>
<li>you run dccm and the tools as the same user</li>
</ul>

<p>See <a href="start.php">connect</a> for more information.</p>

<h2>PPP error: "LCP terminated by peer"</h2>

<p><b>Q:</b> My PPP session was terminated with the message with "LCP
terminated by peer" after a very short connection time. I see something like
this in my logs:</p>

<blockquote><pre>pppd[21318]: pppd 2.4.2 started by root, uid 0
pppd[21318]: Serial connection established.
pppd[21318]: Using interface ppp0
pppd[21318]: Connect: ppp0 <--> /dev/ttyUSB0
kernel: PPP BSD Compression module registered
kernel: PPP Deflate Compression module registered
pppd[21318]: local  IP address 192.168.131.102
pppd[21318]: remote IP address 192.168.131.201
pppd[21318]: LCP terminated by peer
pppd[21318]: Connection terminated.
pppd[21318]: Connect time 0.1 minutes.
pppd[21318]: Sent 579 bytes, received 857 bytes.
pppd[21318]: Connect time 0.1 minutes.
pppd[21318]: Sent 579 bytes, received 857 bytes.
pppd[21318]: Exit.</pre></blockquote>

<p><b>A:</b> The device password provided to <tt>dccm</tt> is either wrong or
missing. See <a href="start.php">Connect</a> for more information.</p>

<h2>PPP error: "Couldn't set tty to PPP discipline"</h2>

<p><b>Q:</b> I setup everything and connected my PDA but when I run
<tt>synce-serial-start</tt> I get messages similar to this in my system
log:</p>

<blockquote><pre>kernel: PPP generic driver version 2.4.2
pppd[2156]: pppd 2.4.1 started by root, uid 0
pppd[2156]: Serial connection established.
modprobe: modprobe: Can't locate module tty-ldisc-3
pppd[2156]: Couldn't set tty to PPP discipline: Invalid argument
pppd[2156]: Exit.</pre></blockquote>

<p><b>A:</b> The missing "tty-ldisc-3" module is the "ppp_async" kernel module.
Make sure that you have compiled that module. You may also have to add this
line to /etc/modules.conf:</p>

<blockquote><pre>alias tty-ldisc-3 ppp_async</pre></blockquote>


<h2>PPP error: "Connect script failed"</h2>

<p><b>Q:</b> I setup everything and connected my PDA but when I run
<tt>synce-serial-start</tt> I get messages similar to this in my system
log:</p>

<blockquote><pre>pppd[6083]: pppd 2.4.1 started by root, uid 0
pppd[6083]: Terminating on signal 15.
pppd[6083]: Connect script failed
pppd[6083]: Exit.</pre></blockquote>

<p><b>A:</b> Please use this checklist to find a solution to your problem:</p>

<ul>

<li class=SPACED>If you are using Mandrake 9.2, make sure you read the special
Mandrake 9.2 section on the <a href="usb_linux.php">USB on Linux</a> page.</li>

<li class=SPACED>If you are using an iPAQ 5550 or similar, make sure you read
the special iPAQ 5550 section on the <a href="usb_linux.php">USB on Linux</a>
page.</li>

<li class=SPACED>Make sure that the serial port is enabled in BIOS (serial cable only)</li>

<li class=SPACED>Make sure you are using the correct serial port (serial cable only)</li>

<li class=SPACED>Decrease the connection speed by changing 115200 to 19200
in <tt>/etc/ppp/peers/synce-device</tt></li>

<li class=SPACED>Remove hotplug script for SynCE (USB cable only)</li>
  
<li class=SPACED>Make a soft reset of your PDA</li>
<li class=SPACED>Disconnect other USB devices (USB cable only)</li>
<li class=SPACED>Reboot your PC</li>
<li class=SPACED>Make sure your device works with Microsoft ActiveSync</li>

</ul>



<h2>PPP error: "No response to 2 echo-requests"</h2>

<p>(SynCE 0.7 or earlier)</p>

<p><b>Q:</b> I use a <a href="serial.php">serial cable</a> to connect my PDA to
my PC. My PPP connection dies when I transfer large files to my PDA. I see a
message similar to this in my logs:</p>

<blockquote><pre>pppd[10387]: No response to 2 echo-requests
pppd[10387]: Serial link appears to be disconnected.
pppd[10387]: Connection terminated.</pre></blockquote>

<p><b>A:</b> This is a known problem with SynCE 0.7 and earlier. After running
<tt>synce-serial-config</tt>, please edit <tt>/etc/ppp/peers/synce-device</tt>. Remove these two lines:</p>

<blockquote><pre>lcp-echo-failure 2
lcp-echo-interval 2</pre></blockquote>

<p>Add this line:</p>

<blockquote><pre>crtscts</pre></blockquote>

<p>You can also upgrade to SynCE 0.8 or later and re-run
synce-serial-config.</p>

<p>Thanks to Lamar Owen for the solution to this problem!</p>

<h2>Accessing the Internet from the PDA via the PC</h2>

<p><b>Q:</b> I want to access the Internet from my PDA while it is connected
via SynCE. What shall I do?</p>

<p><b>A:</b> Easy. Just visit the <a href="ip_forward.php">IP forwarding</a>
page and follow the instructions.</p>

<p>Return to <a href="index.php">main page</a>.</p>

<h2>Auto-start of synce-serial-connect when I connect my USB cable</h2>

<p><b>Q:</b> I want synce-serial-connect to run automatically when I connect
the USB cable to my device.</p>

<p><b>A:</b> You need a <a href="hotplug.php">hotplug script</a>.</p>

</div>
<?php include 'footer.php'; ?>
