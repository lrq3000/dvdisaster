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

<h3><a name="top">Technické otázky</a></h3>

<a href="#nls">2.1 Jaké lokalizace programu jsou k dispozici?</a><p>
<a href="#media">2.2 Jaké typy médií jsou podporovány?</a><p>
<a href="#filesystem">2.3 Které souborové systémy jsou podporovány?</a><p>
<a href="#aspi">2.4 Co znamená "SPTI" a "ASPI"?</a><p>

<pre> </pre><hr><pre> </pre>

<b><a name="nls">2.1 Jaké lokalizace programu jsou k dispozici?</a></b><p>

Aktuální verze programu dvdisaster obsahuje programová hlášení v následujících jazycích:<p>

<table>
<tr><td>&nbsp;&nbsp;&nbsp;</td><td>Čeština</td><td>--</td><td>kompletní</td></tr>
<tr><td></td><td>Angličtina</td><td>--</td><td>kompletní</td></tr>
<tr><td>&nbsp;&nbsp;&nbsp;</td><td>Němčina</td><td>--</td><td>kompletní</td></tr>
<tr><td>&nbsp;&nbsp;&nbsp;</td><td>Italština</td><td>--</td><td>kompletní</td></tr>
</table>
<p></p>

Překladatelé pro další jazyky jsou vítáni!<p>

Program dvdisaster získává automaticky nastavení jazyka od operačního systému.
Pokud místní jazyk ještě není podporován, použijí se hlášení v angličtině. 
Jiný jazyk lze zvolit použitím proměnné prostředí.<p>

Příklad pro shell bash a německý jazyk:

<pre>export LANG=de_DE</pre>

Pokud nejsou speciální znaky jako německé přehlásky zobrazeny správně,
zkuste následující:<p>

<tt>export OUTPUT_CHARSET=iso-8859-1</tt> (X11, XTerm)
<div align=right><a href="#top">&uarr;</a></div>


<b><a name="media">2.2 Jaké typy médií jsou podporovány?</a></b><p>

Program dvdisaster podporuje (pře-)zapisovatelná CD a DVD média. <br>
Média obsahující více sezení nebo ochranu proti kopírování <i>nelze</i> použít.<p>

Použitelná média podle typu:<p>

<b>DVD-R, DVD+R</b><p>

<ul>
<li>Nejsou známa žádná další omezení.</li>
</ul>

<b>DVD+R9 (dvě vrstvy)</b>
<ul>
<li>Mechanika musí být schopna <a href="qa20.php#dvdrom">identifikovat typ média</a>.</li>
</ul>

<b>DVD-RW, DVD+RW</b><p>

<ul>
<li>Některé mechaniky hlásí chybné <a href="qa20.php#plusrw">velikosti obrazů</a>.<br>
Pomoc: aktivujte možnost <a href="example81.html#iso">"Použít informace ze souborového systému ISO/UDF"</a>.
</li></ul>

<b>CD-R, CD-RW</b><p>

<ul>
 <li>Pouze datová CD jsou podporována.</li>
</ul>

<b>Nepoužitelné typy</b> (obraz nelze načíst):<p>
CD-Audio a CD-Video stejně jako DVD-ROM a DVD-RAM.

<div align=right><a href="#top">&uarr;</a></div><p>


<b><a name="filesystem">2.3 Které souborové systémy jsou podporovány?</a></b><p>

Program dvdisaster pracuje výhradně na <a href="background20.html">úrovni obrazu</a>,
který je čten po sektorech.
To znamená, že není podstatné, s jakým souborovým systémem bylo médium naformátováno.<p>

Vzhledem k tomu, že program dvdisaster ani nezná ani nepoužívá strukturu souborového
systému, nemůže opravovat logické chyby na jeho úrovni.
Není schopen obnovit ztracené nebo smazané soubory.
<div align=right><a href="#top">&uarr;</a></div><p>


<b><a name="aspi">2.4 Co znamená "SPTI" a "ASPI"?</a></b><p>

Windows 98 a ME používají ovladače nazývané "ASPI" k přístupu na mechaniky CD/DVD.
Windows NT a jejich následovníci poskytují obdobné funkce prostřednictvím
rozhraní nazvaného "SPTI", ale stále umožňují instalaci dodatečných
ovladačů ASPI.<p>

<i>&gt;&nbsp; Pro a Proti instalace ASPI pod Windows NT/2000/XP:</i><p>

<table>
<tr valign=top><td>+</td><td>Nejsou potřeba oprávnění administrátora pro přístup na mechaniky.</td></tr>
<tr valign=top><td>-</td><td>ASPI se nemapuje dobře do písmenného systému mechanik Windows.</td></tr>
<tr valign=top><td>-</td><td>Některé programy pro zápis na CD/DVD si instalují
své vlastní ovladače ASPI. Instalace dalšího ovladače ASPI může vadit
již existujícímu softwaru.</td></tr>
</table>
<p></p>

<i>&gt;&nbsp; Jaké verze ASPI jsou doporučeny pro použití s programem dvdisaster?</i><p>

Společnost Adaptec poskytuje své ovladače ASPI zdarma pro stažení.
Někdy mají verze 4.72.* obtíže při zjišťování mechanik pod
Windows 2000/XP. Obvykle v takových případech pomůže návrat
ke starší verzi 4.60.<p>

<i>&gt;&nbsp; Písmena mechanik jsou pod ASPI špatně!</i><p>

Ovladače ASPI používají svůj vlastní systém identifikace mechanik, která
nepodporuje schéma písmen mechanik Windows.<br>
Aby zajistil jednotný přístup k SPTI a ASPI,
tak program dvdisaster zkouší odhadnout mapování mechanik ASPI na písmena
mechanik. To však nefunguje ve všech konfiguracích.<p>

Funkce <a href="example90.html#list">--list</a> zobrazí
aktuální mapování písmen mechanik.<p>

<i>&gt;&nbsp; Jak zvolit mezi SPTI a ASPI?</i><p>

Program dvdisaster si automaticky vybere mezi SPTI a ASPI, 
s přikloněním ve prospěch SPTI, pokud jsou dostupné oba ovladače.
Použití ASPI může být vynuceno; viz podrobnosti k popisu funkce
<a href="example90.html#list">--list</a>.

<div align=right><a href="#top">&uarr;</a></div><p>

<!--- do not change below --->

<?php
# end_page() adds the footer line and closes the HTML properly.

end_page();
?>
