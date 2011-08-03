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

<h3>Poznámky k chybám čtení</h3>

DVD média mají svůj vlastní kód pro opravu chyb, který chrání data
před malými výrobními vadami a nepřesnostmi v průběhu zápisu.
Pokud jsou mechanika pro zápis DVD a médium kompatibilní a mají vysokou kvalitu,
oprava chyb zabudovaná do média bude zpočátku zcela nevyužita.
Poskytuje to však dostatek rezerv ke kompenzaci efektů normálního opotřebení
a stárnutí v průběhu mnoha let používání média.
<p>

Pokud je nakonec kapacita zabudované opravy chyb vyčerpána, začnou se na médiu
objevovat chyby čtení. Ty budou nahlášeny operací
<a href="example10.html">"Skenovat"</a> programu dvdisaster.
V závislosti na době prvního výskytu nás zvláště zajímají dva typy
chyb čtení:<p>

<b>Chyby čtení, které se objevují bezprostředně po zápisu média.</b>
Ty jsou příznakem:

<ul>
<li>média z vadné výrobní série nebo</li>
<li>média, které není kompatibilní s mechanikou pro zápis DVD.</li>
</ul>

Opatrná uvážlivost nám říká, abychom se zbavili vadných médií
a zapsali data na bezchybná média a případně se obrátili na
jiného výrobce.<p>

Odolejte prosím pokušení snažit se zachovat vadná média
prostředky souboru oprav chyb - to s největší pravděpodobností
skončí ztrátou dat.<p>

<b>Chyby čtení po několika měsících/letech.</b> 
Zabudovaná oprava chyb média bude stále více používána v průběhu životnosti média,
až nakonec selže a objeví se chyby čtení.
Důvodem bývají mechanické příčiny (škrábnutí, zkroucení plastické hmoty)
a stejně tak i chemické příčiny (deformace barviva anebo odrazové vrstvy).<p>

Tyto následky se typicky vyskytují, pokud bylo médium uskladněno po několik měsíců,
a pak nemusí být možné načíst všechny sektory.<p>

Proto je rozhodující vytvořit
<a href="example20.html">opravná data</a> včas.
Soubor ecc obsahuje informace pro dopočítání obsahu chybějících sektorů
<a href="background10.html" title="Technické vlastnosti opravy chyb">(v rámci určitých omezení)</a>.
Takže s pomocí dat ecc může program dvdisaster obnovit obrazy dokonce
i v případě, že ne všechny sektory mohly být opravdu mechanikou načteny.<p>

Vzhledem k tomu, že oprava chyb může rekonstruovat chybějící sektory do určitého
množství, není nezbytné vyždímat z poškozeného média každý čitelný sektor.
<a href="background50.html">Adaptivní strategie čtení</a> kontroluje v průběhu
čtení, zda již bylo nashromážděno dostatek dat pro opravu chyb.
Jakmile tento stav nastane, čtení končí a dosud nenačtené sektory
budou obnoveny s použitím souboru ecc.<p>

<a name="reading-tips"><b>
Některé tipy pro efektivní čtení poškozených médií</b></a><p>

Výsledek čtení poškozených médií závisí na několika faktorech:

<ul>
<li><b>Ne všechny mechaniky jsou vyráběny stejně.</b><br>
Různé mechaniky mají různé schopnosti čtení.
Využijte výhody funkce programu dvdisaster pro kompletaci obrazu
několika průchody čtení a použijte různé mechaniky v každém průchodu.
Přeneste soubor obrazu mezi počítači s použitím sítě nebo přepisovatelného média,
abyste mohli využít mechaniky nainstalované v jiných strojích.
<p></p></li>
<li><b>Vysuňte a znovu vložte médium.</b><br>
Někdy to udělá rozdíl, když se médium vysune, pootočí se asi o čtvrtinu,
a pak se znovu zasune pro další průchod čtení.
<p></p></li>
<li><b>Některé mechaniky čtou lépe, pokud jsou chladné.</b><br>
Vypněte počítač přes noc a proveďte další pokus o načtení následující ráno.<p>
Ale pozor: "chladem" jsou míněny podmínky normální obytné místnosti - umístění
hardwaru nebo médií do ledničky pro ně nemusí být zdravé.<p></p></li>
</ul>

<!--- do not change below --->

<?php
# end_page() adds the footer line and closes the HTML properly.

end_page();
?>
