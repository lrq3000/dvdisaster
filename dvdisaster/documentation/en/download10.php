<?php
# dvdisaster: English homepage translation
# Copyright (C) 2004-2008 Carsten Gnörlich
#
# UTF-8 trigger: äöüß 
#
# Include our PHP sub routines, then call begin_page()
# to start the HTML page, insert the header, 
# navigation and news if appropriate.

require("../include/dvdisaster.php");
begin_page();
?>

<!--- Insert actual page content below --->

<h3>System requirements</h3>

<ul>
 <li>Processors: x86, PowerPC or Sparc;<p></li>
 <li>with processing speed equal or better than a P4 at 2Ghz;<p></li>
 <li>an up-to-date CD/DVD/HD DVD/BD drive with ATAPI or SCSI interface;<p></li>
 <li>enough hard disk space for creating .iso images from processed media.<p>
</ul>

<h3>Operating systems</h3>
<ul>
 <li><a name="#freebsd"></a><b>FreeBSD</b> version <b>6.0</b> or later<br>
     (using ATAPI drives requires a kernel recompile -- see INSTALL doc)<p>
 </li>
 <li><b>GNU/Linux</b> with kernel <b>2.6.7</b> or later,<br>
     (using kernels >= 2.4.20 is possible, but not recommended!)<p>
 </li>
 <li><b>Mac OS X</b> version 10.4 (Tiger) or later,<br> 
      on x86 and PowerPC hardware.<p>
 <li><b>NetBSD</b> version 3.1 or later.<p></li> 
 <li><b>Windows</b> <b>2000</b> or <b>XP</b><br>
     Windows 98 and ME are possible, but are not recommended for handling DVD
     and larger media.<br>
     Running under Windows Vista (R) is possible, but mostly untested.
 </li>
</ul>

<!--- do not change below --->

<?php
# end_page() adds the footer line and closes the HTML properly.

end_page();
?>
