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

<h3>Základní informace</h3>

Informace v této podsekci nejsou vyžadovány pro práci s programem dvdisaster.
Přesto jsou užitečné k pochopení, jak program dvdisaster pracuje, a mohou vám
pomoci, abyste program maximálně využili pro vaše potřeby.

<ol>
<li><a href="background10.html">Vlastnosti opravy chyb kódu Reed-Solomon</a><p></p></li>
<li><a href="background20.html">Oprava dat na úrovni obrazu</a><p></p></li>
<li><a href="background30.html">Metody RS01 a RS02</a><p></p></li>
<li><a href="background40.html">Podrobnosti o lineární strategii čtení</a><p></p></li>
<li><a href="background50.html">Podrobnosti o adaptivní strategii čtení</a><p></p></li>
<li><a href="background60.html">Některé poznámky k chybám čtení</a><p></p></li>   
<li><a href="background70.html">Tipy pro ukládání souborů oprav chyb</a><p></p></li>
</ol>

<!--- do not change below --->

<?php
# end_page() adds the footer line and closes the HTML properly.

end_page();
?>
