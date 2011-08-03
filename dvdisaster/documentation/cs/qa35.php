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

<h3>Adaptivní strategie čtení</h3>

Program dvdisaster obsahuje dvě rozdílné
<a href="example82.html#read">strategie čtení</a>.<p>

<b>Adaptivní strategie čtení je doporučena pro:</b><p>
<ul>
<li>vytěžení dat z poškozeného média
</li>
</ul>

<b><a href="background40.html">Lineární strategie čtení</a> je doporučena pro:</b><p>
<ul>
<li>načtení obrazů z nepoškozených médií, t.j. pro tvorbu souboru oprav chyb</li>
<li>skenování média na rychlost a chyby čtení</li>
</ul>

<pre> </pre>

<b>Vlastnosti adaptivní strategie čtení.</b><p>

Adaptivní strategie čtení používá přístup "rozděl a panuj" pro umístění
stále nečitelných částí poškozeného média. Strategie je založena na článku
publikovaném Haraldem Bögeholzem v c't-Magazin z 16/2005, kde byl publikován spolu
s programem <i>h2cdimage</i>:

<ol>
<li> 
Na začátku je médium považováno za jeden nenačtený úsek. Čtení začíná v sektoru
nula.<p>
</li>
<li>
Proces čtení pokračuje sekvenčně, dokud není buď dosaženo konce aktuálního úseku
nebo nedojde k chybě čtení.<p>
</li>
<li>
Proces čtení je ukončen, pokud je načteno buď (3a) dostatek sektorů pro úspěšnou
opravu chyb nebo (3b) nezůstaly nečitelné úseky přesahující danou velikost.
<p>
</li>
<li>
Jinak se určí nejdelší zbývající úsek. Čtení pokračuje od středu (t.j. druhé polovina)
tohoto úseku; první polovina tohoto úseku je zachována pro pozdější průchod čtení.
</li>
</ol>

Kritérium ukončení (3a) je zvláště efektivní: Čtení skončí, jakmile bylo nashromážděno
dostatek sektorů pro úspěšnou opravu obrazu s použitím souboru oprav chyb.
Tak se dá zkrátit čas čtení až o 90 procent v porovnání s pokusem o úplné
načtení, ale funguje jen tehdy, je-li dostupný soubor oprav chyb.<p>

<center><img src="images/bg-adaptive.png" alt="Adaptivní strategie čtení" title="Adaptivní strategie čtení"><br>
Částečný snímek obrazovky: adaptivní strategie čtení</center>

<pre> </pre>

<a name="configure"></a>
<b>Konfigurace</b><p>

<b>Soubor oprav chyb.</b> 
Adaptivní čtení pracuje nejlépe, když je dostupný soubor oprav chyb. 
Samozřejmě, že soubor oprav chyb musel být <a href="example20.html">vytvořen</a>
v době, kdy bylo médium ještě plně čitelné. Pro použití souboru oprav chyb
v průběhu adaptivního čtení
<a href="example30.html">zadejte jeho název</a> před spuštěním procesu čtení.<p>

<b>Omezení rozsahu adaptivního čtení.</b> Čtení může být
<a href="example82.html#read">omezeno</a> na část média.

Omezování se nedoporučuje při použití souboru oprav chyb, protože limit může
zabránit načtení sektorů, které jsou potřeba pro úspěšnou opravu chyb
Pokud není dostupný soubor oprav chyb, může být omezení rozsahu čtení užitečné
v průběhu násobných pokusů o čtení.<p>

<b>Časné ukončení čtení.</b>
Pokud není dostupný soubor oprav chyb, adaptivní čtení skončí, když nezbyly žádné
nepřečtené úseky <a href="example82.html#read">větší než zvolená velikost</a>.<p>

Hodnota pro ukončení by měla být menší než 128.
Jinak musí laserová optika provádět mnoho vystavovacích pohybů v průběhu
konečné fáze čtení. To negativně ovlivňuje jak očekávanou životnost mechaniky,
tak i její schopnosti čtení. Lepším postupem je zastavení adaptivního čtení
dříve, a poté se pokusit načíst zbývající sektory dalšími průběhy
<a href="background40.html">lineárního čtení</a>.

<!--- do not change below --->

<?php
# end_page() adds the footer line and closes the HTML properly.

end_page();
?>
