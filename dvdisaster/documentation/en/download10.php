<?php
# dvdisaster: English homepage translation
# Copyright (C) 2004-2012 Carsten Gnörlich
#
# UTF-8 trigger: äöüß 
#
# Include our PHP sub routines, then call begin_page()
# to start the HTML page, insert the header, 
# navigation and news if appropriate.

require("../include/dvdisaster.php");
begin_page();
?>

<!-- Insert actual page content below -->

<h3 class="top">Hardware requirements</h3>

<ul>
 <li>x86, PowerPC or Sparc processor;</li>
 <li>an up-to-date CD/DVD/BD drive with ATAPI, SATA or SCSI interface;</li>
 <li>enough hard disk space for creating .iso images from processed media.
</ul>
<p>

<h3>Supported operating systems</h3>
The following table gives an overview of the supported operating
systems. The specified releases have been used for developing and
testing the current dvdisaster version. Typically, slightly older
and newer OS versions will also work.<p>

<table border="1">
<tr>
<td>Operating System</td>
<td>Release</td>
</tr>
<tr>
<td>GNU/Linux</td>
<td>Debian Squeeze (6.0.4)<br>Kernel 2.6.32</td>
</tr>
<tr>
<td>FreeBSD<sup>1)</sup></td>
<td>9.0</td>
</tr>
<tr>
<td>NetBSD</td>
<td>5.1.2</td>
</tr>
</table><p>

<sup>1)</sup>FreeBSD: using ATAPI drives requires loading the <i>atapicam</i> kernel module -- see INSTALL doc<br>

<h3 class="top">Not supported operating systems</h3>

<ul>
<li> Support for Mac OS X has been terminated and is not planned to be resumed
in the future.
</li>
<li> Windows will be supported until the end of version branch 0.72.x.
Support in version 0.79.x and beyond is not planned.</li>
</ul>

See also the <a href="qa10.php#unsupported">related topic in the Q&A section</a>.


<!-- do not change below -->

<?php
# end_page() adds the footer line and closes the HTML properly.

end_page();
?>
