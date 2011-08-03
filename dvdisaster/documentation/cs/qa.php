<?php
# dvdisaster: Czech homepage translation
# Copyright (C) 2006 Luboš Staněk
#
# Include our PHP sub routines, then call begin_page()
# to start the HTML page, insert the header, 
# navigation and news if appropriate.

require("../include/dvdisaster.php");
begin_page();
?>

<!--- Insert actual page content below --->

<h3><a name="top">Obecné otázky a odpovědi</a></h3>

<a href="#pronounce">1.1 Jak se vyslovuje "dvdisaster"?</a><p>
<a href="#pipo">1.2 Kde získám více informací o skenování PI/PO?</a><p>
<a href="#compat">1.3 Je program dvdisaster kompatibilní s příštími verzemi?</a><p>

<pre> </pre><hr><pre> </pre>

<b><a name="pronounce">1.1 Jak se vyslovuje "dvdisaster"?</a></b><p>
Vzhledem k tomu, že slovo pochází z anglického jazyka, jednoduše spelujte "dv"
před slovem "disaster". Snad je "dý-vý-disaster" vhodný fonetický
přepis.
<div align=right><a href="#top">&uarr;</a></div>

<b><a name="pipo">1.2 Kde získám více informací o skenování PI/PO?</a></b><p>
Oprava chyb na médiích DVD je obdobná metodě, která je
<a href="qa31.php" title="Technické vlastnosti opravy chyb">použita v programu dvdisaster</a>.
DVD mají dva kroky (nebo vrstvy) opravy chyb nazývané PI a PO.
Pouze pokud obě vrstvy selžou při opravě chyby, 
mechanika oznámí nečitelný sektor a program dvdisaster dostane něco na práci
(nebo může být médium vyhozeno).<p>

Pro určité mechaniky byl publikován software, který
může podat zprávu, jak často jsou obě opravné vrstvy využity, když
se čte z DVD. Existuje mnoho zajímavých internetových fór s odkazy
na takové nástroje. Je k dispozici také mnoho článků z dobrých zdrojů
o interpretování hodnot PI/PO se zřetelem na kvalitu média.
<div align=right><a href="#top">&uarr;</a></div><p>

<b><a name="compat">1.3 Je program dvdisaster kompatibilní s příštími verzemi?</a></b><p>
Ano, soubory programu dvdisaster jsou určeny pro archivaci po mnoho let.
Pokud aktualizujete na novější verzi programu dvdisaster, můžete dále používat
existující obrazy a soubory oprav chyb z předchozích verzí.
Není <i>vůbec</i> potřeba je znovu vytvářet.
<div align=right><a href="#top">&uarr;</a></div><p>

<!--- do not change below --->

<?php
# end_page() adds the footer line and closes the HTML properly.

end_page();
?>
