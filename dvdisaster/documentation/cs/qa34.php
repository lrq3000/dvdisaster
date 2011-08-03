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

<h3>Lineární strategie čtení</h3>

Program dvdisaster obsahuje dvě rozdílné
<a href="example82.html#read">strategie čtení</a>.<p>

<b>Lineární strategie čtení je doporučena pro:</b><p>
<ul>
<li>načtení obrazů z nepoškozených médií, t.j. pro tvorbu souboru oprav chyb</li>
<li>skenování média na rychlost a chyby čtení</li>
</ul>

<b><a href="background50.html">Adaptivní strategie čtení</a>  je doporučena pro:</b><p>
<ul>
<li>vytěžení dat z poškozeného média
</li>
</ul>

<pre> </pre>

<b>Vlastnosti lineární strategie čtení.</b><p>

CD a DVD média jsou organizována do sektorů, které jsou souvisle očíslovány
počínaje nulou. Každý sektor obsahuje 2048 bajtů dat.<p>

Lineární strategie čtení načítá médium od počátečního (sektor 0)
do konce (poslední sektor). Rychlost čtení je zobrazována graficky, aby poskytla
informaci o <a href="background40.html#quality">kvalitě média</a>:<p>

<center><img src="images/bg-linear.png" alt="Lineární strategie čtení" title="Lineární strategie čtení"><br>
Částečný snímek obrazovky: lineární strategie čtení</center>

<pre> </pre>


<a name="configure"></a>
<b>Konfigurace.</b><p>

<b>Počet sektorů pro přeskočení po chybě čtení.</b>
Opakování čtení vadných sektorů je pomalé a může za nepříznivých podmínek opotřebit
mechanické díly jednotky. Řada chyb čtení rozprostírající se po souvislé oblasti sektorů
je mnohem běžnější než samostatné bodové defekty. Proto existuje
<a href="example82.html#read">konfigurační volba</a>, aby se dal přeskočit určitý počet
sektorů po výskytu chyby čtení. U přeskočených sektorů se předpokládá jejich chybovost
bez dalších pokusů o čtení. Několik návodů pro volbu počtu přeskočených sektorů:<p>

<ul>
<li>Přeskočení velkého množství sektorů (t.j. <b>1024</b>) podává rychlý přehled
o stupni poškození, ale obvykle neshromáždí dostatek dat pro opravu obrazu média.
<p></p></li>
<li>Menší hodnoty jako <b>16, 32 nebo 64</b> jsou dobrým kompromisem:
Čas zpracování bude znatelně zkrácen, ale stále bude shromážděno dostatek dat pro
opravu obrazu.<p></p></li>
</ul>

Na DVD médiích se z technických důvodů chyby čtení obvykle rozšiřují přes nejméně
16 sektorů. Proto není doporučeno přeskakovat v případě DVD médií méně než 16 sektorů.
<p>

<a name="range"></a>
<b>Omezení rozsahu čtení.</b>
Čtení může být
<a href="example82.html#read">omezeno na daný rozsah sektorů</a>. To bývá užitečné
při násobných pokusech o čtení poškozeného média.

<pre> </pre>

<a name="quality"></a>
<b>Odhadování kvality média.</b><p>

<a name="error"></a>
<b>Křivka rychlosti.</b>
Většina mechanik zpomalí, když čte oblasti média, které jsou ve špatném stavu:
<ul>
<li>
Propady rychlosti čtení mohou být varováním pro blížící se selhání média.
</li>
<li>
Avšak některé mechaniky čtou v plné rychlosti až do hořkého konce.
U takových mechanik se zhoršení média nemusí zobrazit na křivce čtení, dokud
se neprojeví skutečné chyby čtení.
</li>
</ul><p>

Křivka čtení je mnohem přesnější, pokud se použije funkce
<a href="example10.html">"Skenovat"</a>. V průběhu operace
"Načíst" se budou čtená data současně zapisovat
na pevný disk, což může způsobit nepravidelnosti v průběhu křivky čtení
v závislosti na operačním systému a použitém hardwaru.<p>

<b>Chyby čtení.</b>
Chyby čtení vytvářejí <a href="example1.html">červené značky na spirále</a> nebo
se zobrazují odpovídající hlášení na <a href="example10.html#cli">příkazové řádce</a>.
Znamenají, že médium nelze v těchto místech číst v současném průchodu načítáním:
<ul>
<li>Médium je s největší pravděpodobností defektní.</li>
<li>Obraz by měl být
<a href="example40.html">opraven</a> co možná nejdříve, a pak by měl být přenesen
na nové médium.</li>
</ul>

<!--- do not change below --->

<?php
# end_page() adds the footer line and closes the HTML properly.

end_page();
?>
