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

<h3>Metody RS01 a RS02</h3>

Program dvdisaster obsahuje dvě metody opravy chyb pojmenované RS01 a RS02.
RS01 je existující a vyzkoušená metoda, zatímco RS02 je stále ve vývoji.
RS02 je v současnosti dostupná pouze na příkazové řádce a bude plně
integrována do grafického uživatelského rozhraní ve verzi 0.70.<p>

<b>Srovnání obou metod.</b>

RS01 a RS02 vytváří stejnou opravu chyb
<a href="background10.html">Reed-Solomon</a>.
Vypočítávají pro obrazy CD/DVD opravné informace, které jsou použity
k obnově nečitelných sektorů, jakmile se později disk poškodí.<p>

Metody se liší ve způsobu, jak je opravná informace ukládána:<p>

<ul>
<li>
<a name="file"> </a>
RS01 vytváří <b>soubory oprav chyb</b>, které jsou uloženy odděleně
od obrazů, ke kterým patří. Vzhledem k tomu, že je ochrana dat na
<a href="background20.html#file">úrovni souborů</a> obtížná,
musí být soubory oprav chyb uloženy na média, která jsou také chráněna
proti ztrátě dat programem dvdisaster.<p></li>

<li>
<a name="image"> </a>
Metoda RS02 se použije tak, že se nejprve vytvoří obraz na pevném disku
za použití softwaru pro vypalování CD/DVD. Než se obraz zapíše na médium,
programem dvdisaster <b>rozšíří obraz</b> o opravná data.
Takže jsou data, která se mají chránit, a informace opravy chyb umístěna
na stejné médium. Poškozené sektory v opravných informacích snižují
kapacitu opravy dat, ale nečiní opravu nemožnou - druhé médium pro
uložení nebo ochranu opravných informací není požadováno.<p></li>
</ul>


<a name="table"> </a>
<b>Porovnání ukládání opravných dat.</b><p>

<table width="100%" border="1" cellspacing="0" cellpadding="5">
<tr>
<td width="50%"><i>Soubory oprav chyb</i></td>
<td width="50%"><i>Obrazy rozšířené o opravná data</i></td>
</tr>
<tr valign="top">
<td> 
lze použít jakoukoli možnou redundanci</td>
<td>redundance je omezena volným prostorem na médiu<br>
(= kapacita média - velikost obrazu dat)</td>
</tr>

<tr valign="top">
<td>je efektivní již při 15% redundance;
médium může být zcela zaplněno daty</td>
<td>vyžaduje nejméně 20%-30% redundance;
použitelná kapacita média je odpovídajícím způsobem zmenšena</td> 
</tr>

<tr valign="top">
<td>může být vytvořen pro již existující média</td>
<td>lze použít pouze před zápisem nového média, protože
obraz musí být předem rozšířen o informace opravy chyb</td>
</tr>

<tr valign="top">
<td>oddělené ukládání souboru oprav chyb a uživatelských dat
posiluje ochranu dat</td>
<td>společný úložný prostor pro uživatelská i opravná data
může snížit kapacitu opravy chyb</td>
</tr>

<tr valign="top">
<td>Musí se zachovávat přiřazení souborů oprav chyb k médiím.
Soubory oprav chyb musí být chráněny proti poškození.</td>
<td>Snadné řešení s jedním médiem; informace opravy chyb se
nemusí katalogizovat nebo výlučně chránit.</td></tr>

<tr valign="top">
<td>žádné problémy s kompatibilitou v přehrávacích zařízeních</td>
<td>média s rozšířenými obrazy se nemusí přehrávat správně ve
všech zařízeních</td>
</tr>
</table><p>

<!--- do not change below --->

<?php
# end_page() adds the footer line and closes the HTML properly.

end_page();
?>
