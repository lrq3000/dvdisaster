<?php
# dvdisaster: German homepage translation
# Copyright (C) 2004-2014 Carsten Gnörlich
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

<h3 class="top">Ostatní zdroje informací</h3>Online dokumentace kterou
právě čtete je dodávána jako součást programových balíčků
dvdisaster. Není třeba ji stahovat samostatně.<p>K dispozici jsou
další dodatečné materiály:<p>

<b>RS01, RS02 and RS03 specification</b><p>

This document contains the 
<a href="http://dvdisaster.net/papers/codecs.pdf">RS01, RS02 and RS03 codec specification (codecs.pdf)</a>.
It is meant to discuss the codec implementation among developers.<p>

RS03 is a new encoding format for upcoming dvdisaster versions,
capable of using multiple processor cores for its
calculations. This can not be done with the current RS01
und RS02 methods due to limits in their internal structure.
The RS03 specification may still be subject to changes in some details.
<p>

Reading this document requires knowledge in the area of
coding theory. It is not meant as end user documentation.

<hr>

<table>
<tr><td>
<a href="http://pdfreaders.org/"><img src="http://pdfreaders.org/graphics/pdfreaders-lang-neutral.png" alt="pdfreaders.org" /></a></td>
<td>Software for reading the PDF document is available at <a href="http://pdfreaders.org">PDFreaders.org</a>.</td>
</tr>
</table>

<!-- do not change below --> 
<?php 
#end_page() adds the footer line and closes the HTML properly.

end_page();
?>