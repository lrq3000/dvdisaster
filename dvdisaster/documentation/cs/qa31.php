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

<h3>Technické vlastnosti opravy chyb</h3>

Tato stránka vysvětluje základní myšlenky v pozadí programu dvdisaster,
abyste sami mohli zjistit, zda vyhovuje vašim požadavkům na zabezpečení dat.
Pokud máte pochybnosti, neměli byste program dvdisaster používat nebo
nasadit další strategie zálohování dat.<p>

<b>Metoda opravy chyb.</b> &nbsp; Program dvdisaster používá kódování
<a href="http://en.wikipedia.org/wiki/Reed-Solomon_error_correction">Reed-Solomon</a>
(odkaz na anglický text) spolu s algoritmem opravy chyb optimalizovaným pro zpracování výmazů.
Tato implementace si vzala hodně inspirace a programového kódu z vynikající
<a href="http://www.ka9q.net/code/fec/">knihovny kódu Reed-Solomon</a> napsané
<a href="http://www.ka9q.net/">Philem Karnem</a>.

<p>

Při použití <a href="example83.html#redundancy">standardního nastavení</a>
je 223 sektorů média zkombinováno do jednoho bloku opravného kódu ("ECC").
Chyby čtení média jsou považovány za "výmazy"; z toho vyplývá, že je možné
maximálně opravit 32 vadných sektorů<sup><a href="#footnote1" title="Poznámka k nastavení hranice oprav">*)</a></sup>
média na blok ECC.<p>

Těchto 223 sektorů je zvoleno tak, aby byly rovnoměrně rozmístěny po celé ploše média.
Tím je umožněno, aby mohly být opraveny velké souvislé oblasti vadných sektorů
dříve, než je dosažena hranice 32 defektů na blok ECC<sup><a href="#footnote1" title="Poznámka k nastavení hranice oprav">*)</a></sup>.
Tento druh vzoru chyb je zvláště obvyklý pro stárnoucí médium, kde začíná
degenerovat vnější okrajová oblast, a pro škrábance podél datové spirály.<p>

Na druhé straně se předpokládá, že hvězdicovité nebo příčné škrábance opraví sama
mechanika CD/DVD. Pokud ne, tak v těchto případech pracuje použitá strategie oprav
chyb spíše neutrálně (není zvlášť dobrá ani mimořádně špatná).<p>

<b>Hranice opravy chyb.</b> &nbsp; V nejhorším případě stačí 33
vadných sektorů<sup><a href="#footnote1" title="Poznámka k nastavení hranice oprav">*)</a></sup>, aby zabránily
úplné obnově dat. Avšak k dosažení tohoto efektu by musely být chyby
rozloženy na médiu podobným způsobem, jako jsou umístěny v samotném bloku ECC
- takový vzorek je velmi nepravděpodobný.<br>
Pokusné testy ukázaly, že na stárnoucím médiu může být okolo 10% počtu všech sektorů
vadných, než je dosažena hranice 33 defektů na blok ECC<sup><a href="#footnote1" title="Poznámka k nastavení hranice oprav">*)</a></sup>.<br>
<a href="index10.html">Škrábance</a> způsobí dosažení této hranice dříve, 
takže se doporučuje vizuálně kontrolovat média v pravidelných intervalech.
Média s chybami čtení způsobenými poškrábáním by měla být nahrazena okamžitě.<p>

<b>Hardwarová omezení.</b> &nbsp; Většina mechanik nerozpozná médium, je-li poškozena
zaváděcí oblast (lead-in) před prvním sektorem (blízko středového otvoru). V takových
případech nebude program dvdisaster schopen obnovit žádný obsah z média.<p>

<i>Není vhodné</i> vylepšovat spolehlivost médií nízké kvality použitím programu
dvdisaster. Levná média se mohou zkazit během několika dní do takové míry, která
přesáhne schopnosti kódu pro opravu chyb.<p>

<pre> </pre>
<table width="50%"><tr><td><hr></td></tr></table>

<font size="-1">
<a name="footnote1"><sup>*)</sup></a> 
Tato hranice 32 opravitelných chyb na blok ECC je dána standardním nastavením.
Je možné <a href="example83.html#redundancy">zvolit jiné hodnoty</a>
pro vyšší nebo nižší schopnosti opravy chyb.
</font>
<p>

<!--- do not change below --->

<?php
# end_page() adds the footer line and closes the HTML properly.

end_page();
?>
