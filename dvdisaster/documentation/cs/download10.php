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

<h3 class="top">Systémové požadavky</h3>

<ul>
 <li>Procesory: x86, PowerPC nebo Sparc;</li>
 <li>aktuální CD/DVD/BD mechanika s ATAPI nebo SCSI rozhraním;</li>
 <li>dostatek volného prostoru pro vytvoření bitových kopií zpracovávaných disků.</ul>
<p><h3>Podporované operační systémy</h3>V následující tabulce je uveden přehled podporovaných operačních systémů. Uvedené verze byly použity při vývoji a testování aktuální verze dvdisaster. Typicky by měly být podporovány i o něco starší nebo novější verze.<p>

<table border="1">
<tr>
<td>Operační systém</td>
<td>Verze</td>
</tr>
<tr>
<td>GNU/Linux</td>
<td>Debian Squeeze (6.0.4)<br>kernel 2.6.32</td>
</tr>
<tr>
<td>FreeBSD<sup>1)</sup></td>
<td>9.0</td>
</tr>
<tr>
<td>NetBSD</td>
<td>5.1.2</td>
</tr>
</table><p><sup>1)</sup>FreeBSD: použití ATAPI vyžaduje načtení jaderného modulu <i>atapicam</i> -- podrobnosti v souboru INSTALL<br> 

<h3 class="top">Not supported operating systems</h3>

<ul>
<li> Support for Mac OS X has been terminated and is not planned to be resumed
in the future.
</li>
<li> Windows will be supported until the end of version branch 0.72.x.
Support in version 0.79.x and beyond is not planned.</li>
</ul>

See also the <a href="qa10.php#unsupported">related topic in the Q&A section</a>.


<!-- do not change below --> <?php
# end_page() adds the footer line and closes the HTML properly.

end_page();
?>