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

<h3>Chybová hlášení</h3>

<a href="#tao">3.1 "Varování: 2 sektor(y) chybí na konci disku".</a><p>
<a href="#block">3.2 Program se zablokuje ihned po spuštění.</a><p>
<a href="#crc">3.3 Co znamená "Chyba CRC, sektor: n"?</a><p>
<a href="#plusrw">3.4 Chyby čtení nebo nesprávná velikost obrazu s médii -RW/+RW?</a><p>
<a href="#dvdrom">3.5 Mnou vypálená média jsou detekována jako "DVD-ROM" a odmítnuta.</a><p>
<a href="#freebsd">3.6 Žádné mechaniky se neobjeví ve FreeBSD.</a><p>

<pre> </pre><hr><pre> </pre>

<b><a name="tao">3.1 "Varování: 2 sektor(y) chybí na konci disku"</a></b><p>
Toto upozornění se objevuje u médií CD zapsaných v režimu "TAO" (stopa najednou).
Některé mechaniky hlásí velikost obrazu, která je o 2 sektory delší než je konec
takového média, a tak vytváří 2 klamné chyby čtení na konci média, které v tomto
případě <i>neznamenají</i> ztrátu dat.<p>

Vzhledem k tomu, že nelze z média určit režim zápisu, program dvdisaster předpokládá
CD v režimu "TAO", pokud jsou přesně dva poslední sektory nečitelné, a zkrátí podle
toho obraz. Rozhodnutí, zda je to v pořádku, zůstává na vás. Můžete nastavit program
dvdisaster, aby ošetřil tyto sektory jako skutečné chyby čtení pomocí volby
<a href="example90.html#dao">--dao</a> nebo 
na <a href="example82.html#image">záložce možností čtení/skenování</a>.<p>

Abyste sy vyhnuli těmto problémům, zvažte použití režimu "DAO / Disk najednou"
(někdy také nazývaný "SAO / Sezení najednou") pro zápis médií s jedním sezením.
<div align=right><a href="#top">&uarr;</a></div>


<b><a name="block">3.2 Program se zablokuje ihned po spuštění</a></b><p>
Ve starých verzích Linuxu (jádro 2.4.x) se program občas zablokuje
ihned po spuštění a před vykonáním jakékoli akce.
Nelze jej ukončit použitím Ctrl-C nebo "kill -9".<p>

Vysuňte médium, aby se mohl program ukončit. Vložte médium znovu
a počkejte, až mechanika médium rozpozná a zastaví se.
Opětovné spuštění programu dvdisaster by nyní mělo fungovat.
<div align=right><a href="#top">&uarr;</a></div>

<b><a name="crc">3.3 Co znamená "Chyba CRC, sektor: n"?</a></b><p>
Odpovídající sektor je čitelný, ale kontrolní součet jeho obsahu
neodpovídá hodnotě zaznamenané v souboru oprav chyb. Některé možné příčiny jsou:<p>

<ul>
<li>Obraz byl připojen s oprávněními pro zápis a byl tedy změněn
(typický důkaz: chyby CRC v sektoru 64 a v sektorech 200 až 400).</li>
<li>Počítač má nějaké problémy s hardwarem, zvláště, když
komunikuje se zařízeními pro hromadné ukládání dat.</li>
</ul>


Pokud máte podezření na technické problémy, zkuste znovu načíst další verzi obrazu
a vytvořit soubor oprav chyb, a pak je <a href="example50.html">znovu porovnejte</a>.
Pokud chyba zmizí nebo se objeví na jiném místě, váš počítač může mít vadnou paměť,
zlomenou kabeláž mechanik nebo chybné nastavení frekvence CPU/systému.
<div align=right><a href="#top">&uarr;</a></div>


<b><a name="plusrw">3.4 Chyby čtení nebo nesprávná velikost obrazu s médii -RW/+RW?</a></b><p>

Některé mechaniky hlásí nesprávnou velikost obrazu u médií -RW/+RW. Dvě
obvyklé příčiny jsou:<p>

<table>
<tr><td valign="top">Problém:</td>
<td>Mechanika hlásí velikost největšího obrazu, který kdy byl zapsán na médium,
ale ne velikost aktuálního obrazu.
</td></tr>
<tr><td valign="top">Symptomy:</td>
<td>Po vymazání média je zapsáno se souborem o velikosti asi 100 MB.
Avšak zpět načtený obraz je několik GB velký a obsahuje zbývající nebo
starší obrazy.
</td></tr>
<tr><td><pre> </pre></td><td></td></tr>
<tr><td valign="top">Problém:</td>
<td>Mechanika hlásí maximální možnou kapacitu média (typicky 2295104 sektorů)
namísto počtu aktuálně využitých sektorů.
</td></tr>
<tr><td valign="top">Symptomy:</td>
<td>Pokud se načítá za určitým bodem média, objevují se jen chyby čtení,
ale všechny soubory na médiu jsou stále čitelné a kompletní.
</td></tr>
</table>
<p></p>

Možná náprava:<p>

<table width=100%><tr><td bgcolor=#000000 width=2><img width=1 height=1 alt=""></td><td>
Aktivujte volbu <a href="example81.html#iso">Použít informace ze souborového
systému ISO/UDF</a>, aby byla velikost obrazu určena z informací souborového
systému ISO/UDF.
</td></tr></table>
<p></p>

Pokud jsou požadované sektory ISO/UDF nečitelné, když se pokoušíte obnovit
poškozené médium, lze použít dvě dočasná řešení:

<ul>
<li>Spusťte funkci <a href="example50.html">"Porovnat"</a> jen se zadaným
souborem oprav chyb. Všimněte si dole ve výstupu správné velikosti obrazu
a příslušně <a href="example82.html#read">omezte rozsah čtení</a>.
</li>
<li>Jednoduše načtěte obraz s nesprávnou (větší) velikostí.
Když spustíte funkci <a href="example40.html#repair">"Opravit"</a>,
odpovězte "OK" na otázku, zda má být obraz zkrácen.
</li>
</ul>

<div align=right><a href="#top">&uarr;</a></div>

<b><a name="dvdrom">3.5 Mnou vypálená média jsou detekována jako "DVD-ROM" a odmítnuta.</a></b><p>

To se může stát kvůli následujícím příčinám:
 
<ol>
<li>Formát média (book type) nebyl správně nastaven na "DVD-ROM".<br>
V takovém případě musí mechanika podporovat jiný způsob zjištění skutečného
typu média. Pouze pak program dvdisaster akceptuje médium a ohlásí jej
jako "DVD-ROM (podvržený formát disku)".<p>

Některé mechaniky však nepodporují zjištění podvržených formátů disku.

<li>Některé mechaniky nedokážou rozlišit DVD+R9 (dvě vrstvy) od DVD-ROM.
</li>
</ol>

V těchto případech zkuste jinou mechaniku pro čtení obrazů.

<div align=right><a href="#top">&uarr;</a></div>


<b><a name="freebsd">3.6 Žádné mechaniky se neobjeví ve FreeBSD.</a></b><p>

<ul>
<li>FreeBSD může vyžadovat <a href="download20.html#freebsd">rekompilaci jádra</a>,
aby byly mechaniky ATAPI (téměř všechny současné modely) použitelné pro program
dvdisaster.
<li>Musíte mít oprávnění ke čtení a zápisu na odpovídající zařízení
(t.j. /dev/pass0).
</ul>

<div align=right><a href="#top">&uarr;</a></div>

<!--- do not change below --->

<?php
# end_page() adds the footer line and closes the HTML properly.

end_page();
?>
