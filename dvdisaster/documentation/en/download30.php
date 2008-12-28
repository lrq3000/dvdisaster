<?php
# dvdisaster: English homepage translation
# Copyright (C) 2004-2009 Carsten Gnörlich
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

<h3>Installation of the binary versions (Windows only)</h3>

Please install the Windows version by executing the setup binary
(e.g. <?php echo ${pkgname}?>-setup.exe) and follow the 
installation dialog. <p>

<b>Caution:</b> dvdisaster can NOT be installed from
manually unpacking the setup.exe program, or by copying it from
an already existing installation. Doing so may result in strange
errors which seem to be totally unrelated to an improper installation.

<h3>Installation of the source packages</h3>

dvdisaster uses the common <tt>./configure; make; make install</tt> mechanism
for installation from source. Please see the file <tt>INSTALL</tt> in the
source distribution for further information.

<!--- do not change below --->

<?php
# end_page() adds the footer line and closes the HTML properly.

end_page();
?>
