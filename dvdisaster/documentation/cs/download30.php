<?php
# dvdisaster: English homepage translation
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

<h3 class="top">Instalace zdrojových balíčků</h3>dvdisaster používá
běžný mechanismus instalace ze zdrojů pomocí <tt>./configure; make;
make install</tt>. Podrobnější informace získáte v souboru
<tt>INSTALL</tt>.
<pre> </pre>

<a name="win"></a><h3>Instalace binárních souborů pro
Windows</h3>Verzi pro Windows nainstalujte spuštěním instalačního
programu (např. <?php echo ${pkgname}?>-setup.exe) a pokračujte podle
zobrazovaných pokynů.<p><b>Varování:</b> dvdisaster není možné
nainstalovat jednoduchým rozbalením setup.exe, nebo zkopírováním
existující instalace. Pokud to tak uděláte, může docházet k podivným
chybám, které s chybnou instalací zdánlivě vůbec nesouvisí.

<!-- do not change below --> 
<?php # end_page() adds the footer line and closes the HTML properly.

end_page();
?>