#! /bin/bash

# Notice: UTF-8

#  dvdisaster: Czech homepage translation
#  Copyright (C) 2006 Luboš Staněk
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA,
#  or direct your browser at http://www.gnu.org.

# ----- Translations for phrases used in create.bash

function dictionary_cs()
{  trans_encoding="UTF-8"

   trans_version="verze 0.66 - not yet updated for"

   trans_to_internet="Na Internetovou verzi"
   trans_to_berlios="Přehled (na BerliOS)"
   trans_to_sourceforge="Přehled (na SourceForge.net)"

   trans_back="Zpět"

   trans_contents="Obsah"
   trans_hosting="Hostováno pod"

   trans_fdl="Doslovné kopírování a distribuce celého tohoto článku jsou pro jakékoli médium povoleny za předpokladu, že tato poznámka zůstane zachována."
   trans_copyright="Autorská práva 2004-2006 Carsten Gnörlich, k překladu 2006 Luboš Staněk."
   trans_modified="Information in this handbook relates to the original version of dvdisaster and may not be applicable."
}

# ----- These are the "inlined" html pages.

function inline_contents_cs()
{  title="Stránka s obrázky"
}

function inline1cs()
{  cat >> $1 <<EOF

<tr align="center"><td><h3>Obrázek: Silně poškrábané médium</h3><p></td></tr>
<tr align="center"><td>
<center><img src="../images/scratch-img.jpg" alt="Silně poškrábané médium" title="Silně poškrábané médium"></center>
<br clear="all">
</td></tr>
<tr><td>
Toto médium bylo přenášeno bez ochranného obalu;
kruhové stopy poškrábání byly vytvořeny lahví vody. Škrábance jsou
nadměrně zvýrazněny díky odrazům fotografického blesku.</td></tr>
EOF
}


function inline2cs()
{  cat >> $1 <<EOF

<tr align="center"><td><h3>Obrázky: Analýza chyb</h3><p></td></tr>
<tr align="center"><td>
<center><img src="images/scratch-scan.png" alt="Analýza chyb" title="Analýza chyb"></center>
<br clear="all">
</td></tr>
<tr><td>
Při čtení nebo skenování média zobrazuje dvdisaster rychlost čtení (modrá křivka)
a počet chyb čtení. Spirála představuje odhadované rozmístění chyb mezi
vnitřním a vnějším okrajem média.
Spirála však není vykreslena v měřítku, skutečný úhel mezi segmenty média
neodpovídá z technických důvodů vykreslené datové stopě.
<pre>

</pre>
</td></tr>
<tr align="center"><td>
<center><img src="images/scratch-corr.png" alt="Oprava obrazu" title="Oprava obrazu"></center>
<br clear="all">
</td></tr>
<tr><td>
Tento graf zobrazuje, jak velké množství chyb na blok oprav bylo
skutečně opraveno, a podává tak odhad pracovního zatížení při
<a href="background10.html" title="Technické vlastnosti opravy chyb">opravě chyb</a> programu dvdisaster.

Graf chyb <i>neodpovídá</i> prostorovému rozmístění chyb čtení na disku,
protože metoda opravy chyb je úmyslně postavena tak, aby dosáhla
rovnoměrného rozložení chyb čtení média ve všech blocích oprav chyb.<p>

Při použití <a href="example83.html#redundancy">standardního
nastavení</a> lze kompenzovat až 32 chyb čtení na blok oprav chyb
(zelená čára). Překročení této čáry znamená, že obsah média nelze obnovit celý.
</td></tr>
EOF
}

# ----- Individual page contents 

# ----- Default home page

function index_contents_cs()
{  local file="$1.html"
   local query=$2
   local page=$3
   local lang=$4

   case $query in
   title)   title="Přehled" ;;

   link)    link_title="Přehled" ;;
   link0)   link_title="Přehled" ;;
   link10)   link_title="Příklady oprav chyb" ;;
   link20)   link_title="Program dvdisaster doplňuje PI/PO skeny" ;;
   link30)   link_title="Pro a proti programu dvdisaster" ;;

   content*) eval "index$page$lang $file" ;;
   esac 
}


function index0cs()
{  cat >> $1 <<EOF
<h3>Projekt dvdisaster:</h3>

dvdisaster poskytuje dodatečné zabezpečení proti <b>ztrátě dat</b> na
<a href="qa10.html#media">CD</a> a <a href="qa10.html#media">DVD</a> médiích
způsobené <b>stárnutím</b> nebo <b>poškrábáním</b>.  

<ul>
<li>dvdisaster vytváří <b>data oprav chyb</b> pro kompenzaci
chyb čtení, které nejsou opravitelné mechanikou CD/DVD.</li>
<li>dvdisaster se pokouší načíst co možná nejvíce dat z poškozeného média.
Později jsou nečitelné sektory obnoveny s pomocí dříve vytvořeného
kódu pro opravu chyb. Maximální kapacita opravy chyb je uživatelem volitelná.</li>
<li>dvdisaster pracuje na <a href="background20.html">úrovni obrazu</a> 
a není závislý na souborovém systému.</li>
</ul>

Pokud vytvoříte soubor oprav chyb včas a uchováte jej na bezpečném místě,
máte dobré předpoklady k obnovení obsahu média při typických chybách čtení
a přenesení obnovených dat na nové médium.

<p>
<a href="index10.html">Příklady oprav chyb...</a>
EOF
}


function index10cs()
{
   create_inline inline cs 1 index10.html "na Příklady oprav chyb"
   create_inline inline cs 2 index10.html "na Příklady oprav chyb"

   cat >> $1 <<EOF
<h3>Příklady oprav chyb</h3>

<table><tr><td valign="top">
<b><a name="scratches">Silně poškrábané médium.</a></b> &nbsp;

Toto médium bylo přenášeno bez ochranného obalu;
kruhové stopy poškrábání byly vytvořeny lahví vody. Škrábance jsou
nadměrně zvýrazněny díky odrazům fotografického blesku.

<p>Sken povrchu média odhalí asi 135000 nečitelných sektorů
(z celkového počtu 2200000, což představuje asi 6% poškozených sektorů).
V průběhu <a href="example40.html">opravy</a> obrazu média
(s použitím <a href="example83.html#redundancy">standardního nastavení</a>)
dosáhne počet chyb maximální hodnoty 22 chyb na <a href="background10.html" title="Technické vlastnosti opravy chyb">blok ECC</a>,
což představuje 69% využití opravy chyb při použitém nastavení.

<p>
</td><td width=110>
<a href="inline1.html">
<img src="../images/scratch-img.jpg" width=100 height=97 alt="Silně poškrábané médium" title="Silně poškrábané médium"></a>
<br clear="all">
<a href="inline2.html">
<img src="images/scratch-scan.png" width=100 height=74 alt="Analýza chyb" title="Analýza chyb"></a>
<br clear="all">
<a href="inline2.html">
<img src="images/scratch-corr.png" width=100 height=74 alt="Oprava obrazu" title="Oprava obrazu"></a>
</td></tr></table>

<a href="index20.html">Proč není sken PI/PO dostatečný...</a>
EOF
}


function index20cs()
{  cat >> $1 <<EOF
<h3>Program dvdisaster doplňuje PI/PO skeny</h3>

<a href="qa.html#pipo">PI/PO skeny</a> jsou cenným nástrojem pro měření a optimalizaci
kvality vypálených médií.<p>

Přesto <b>nejsou</b> PI/PO skeny spolehlivým prostředkem <b>předpovědi
životnosti</b> DVD média!

<ul>
<li> PI/PO skeny nepodávají žádnou informaci o tom, kolik dalších
škrábanců nebo týdnů stárnutí médium ještě vydrží, než začne vykazovat
první nečitelný sektor.<p></p></li>

<li>Kopírování média jen kvůli špatným hodnotám PI/PO se často
ukazuje jako předčasné - někdy takováto média zůstanou čitelná
mnohem déle, než se očekávalo.<p></p></li>

<li>Bez použití dodatečných nástrojů, jako je dvdisaster, ale není
možné odkládat zálohu média, až se objeví první chyby čtení - to by
rozhodně vedlo ke ztrátě dat.</li>
</ul>

<a href="index30.html">Přehled pro a proti programu dvdisaster...</a>
EOF
}


function index30cs()
{  cat >> $1 <<EOF
<h3>Výhody používání programu dvdisaster:</h3>

<ul>
<li><b>Chrání</b> před neúmyslným poškozením médií
(v rámci <a href="background10.html" title="Technické vlastnosti opravy chyb">jistých hranic</a>).<p></p></li>
<li>Testy chyb čtení běží <b>rychleji</b>, než probíhají skeny PI/PO;<br>
při rychlosti až 16x podle použité mechaniky DVD.<p></p></li>
<li><b>Efektivita nákladů:</b> Média se musí nahradit novou kopií jen v případě,
že jsou skutečně poškozená.</li>
</ul>

<h3>Omezení při používání programu dvdisaster:</h3>
<ul>
<li>Soubor oprav chyb <b>musí být vytvořen dříve, než médium selže</b>.<p></p></li>
<li>Soubory oprav chyb vyžadují <b>další úložný prostor</b> a je nutné
je uchovávat na spolehlivém médiu.
Při použití <a href="example83.html#redundancy">standardního nastavení</a>
dosahuje dodatečný úložný prostor velikosti 15% původního datového rozsahu
(přibližně 700 MB pro plné 4.7 GB DVD).<p></p></li>
<li><a href="background10.html" title="Technické vlastnosti opravy chyb">Žádná garantovaná ochrana</a> proti ztrátě dat.</li>
</ul>

V kolekci <a href="background.html">základních informací</a>
zjistíte více o tom, jak program dvdisaster funguje.
EOF
}

# ----- Examples

function example_contents_cs()
{  local file="$1.html"
   local query=$2
   local page=$3
   local lang=$4

   case $query in
   title)   title="Příklady" ;;

   link)    link_title="Příklady a snímky obrazovek" ;;

   link10)   link_title="Skenování médií na chyby" ;;

   link20)   link_title="Vytváření opravných dat" ;;
   link21)   link_title="v samostatném souboru" ;;
   link22)   link_title="v rámci obrazu média" ;;

   link30)   link_title="Čtení poškozených médií" ;;
   link40)   link_title="Obnovení obrazů médií" ;;
   link50)   link_title="Získání informací o obrazech a opravných datech" ;;

   link80)   link_title="Konfigurace" ;;
   link81)   link_title="Obecná nastavení" ;;
   link82)   link_title="Nastavení čtení" ;;
   link83)   link_title="Nastavení oprav chyb" ;;

   link90)   link_title="Syntaxe příkazu" ;;

   content*) eval "example$page$lang $file" ;;
   esac 
}


function example0cs()
{  cat >> $1 <<EOF
<h3>Které téma vás zajímá?</h3>

<table>
<tr>
  <td valign="top">
    <a href="example10.html"><b>Skenování médií na chyby</b></a><br>
    Sken média analyzuje rychlost čtení a čitelnost.
  </td>
  <td>&nbsp;</td>
  <td valign="top">
    <a href="example20.html"><b>Vytváření opravných dat</b></a><br>
    Opravná data mohou být přidána do obrazu nebo mohou
    existovat jako samostatný soubor.
  </td>
</tr>
<tr>
  <td>
    <a href="example1.html">
    <img src="images/ex-scan.png" border=0 width=264 alt="Skenování média" title="Skenování média"></a>
  </td>
  <td>&nbsp;</td>
  <td>
    <a href="example3.html#ecc">
    <img src="images/ex-create.png" border=0 width=264 alt="Vytvoření opravných dat" title="Vytvoření opravných dat"></a>
  </td>
</tr>

<tr><td colspan=3>&nbsp;</td></tr>

<tr>
  <td valign="top">
    <a href="example30.html"><b>Čtení poškozených médií</b></a><br>
    Optimalizovaná <a href="background50.html">strategie čtení</a>
    vytěží data z poškozeného média.
  </td>
  <td>&nbsp;</td>
  <td>
    <a href="example40.html"><b>Obnovení obrazů médií</b></a><br>
    Nečitelné sektory jsou rekonstruovány z
    <a href="example20.html">opravných dat</a>.
  </td>
</tr>
<tr>
  <td>
    <a href="example4.html">
    <img src="images/ex-read-a.png" border=0 width=264 alt="Čtení poškozeného média" title="Čtení poškozeného média"></a>
  </td>
  <td>&nbsp;</td>
  <td>
    <a href="example5.html">
    <img src="images/ex-fix.png" border=0 width=264 alt="Oprava média" title="Oprava média"></a>
  </td>
</tr>
</table>

<pre> </pre>

Další témata: 

<ul>
<li><a href="example50.html">Získání informací o obrazech a opravných datech</a></li>

<li><a href="example80.html">Konfigurace</a>

<ul>
<li><a href="example81.html">Obecná nastavení</a></li>
<li><a href="example82.html">Nastavení čtení</a></li>
<li><a href="example83.html">Nastavení oprav chyb</a></li>
</ul></li>

<li><a href="example90.html">Syntaxe příkazu</a></li>

<li><a href="background70.html">Tipy pro ukládání souborů oprav chyb</a></li>
</ul>
EOF
}

function example1cs()
{  cat >> $1 <<EOF

<tr align="center"><td><h3>Snímek obrazovky: Skenování média na chyby</h3><p></td></tr>
<tr align="center"><td>
<center><img src="images/ex-scan.png" alt="Skenování média" title="Skenování média"></center>
<br clear="all">
</td></tr>
<tr><td>

Grafické zobrazení podává informaci o průběhu skenování a stavu média:<p>

<ul>
<li><b>Graf rychlosti</b> poskytuje hrubý odhad stavu média, protože většina
mechanik zpomalí, když se kvalita média snižuje.<p>

Abyste se vyhnuli vlivu dalších systémových aktivit, neměli byste spouštět
žádný další program v průběhu skenování média. Také při skenování nepoužívejte
žádné z ovládacích prvků v okně programu dvdisaster.<p></p></li>

<li><b>Spirála</b> poskytuje (není vykreslena v měřítku)
přehled o <b>stavu média</b>.
Červené značky indikují přítomnost chyb čtení!
<table width=100%><tr><td bgcolor=#000000 width=2><img width=1 height=1 alt=""></td><td>
Jakmile si všimnete chyb čtení na vašem médiu, pokuste se
<a href="example40.html">opravit obraz média</a> 
a přeneste jej na nové médium. 
</td></tr></table>
</li>

<li>Kliknutím na symbol
<img src="images/btn-protocol.png" alt="Tlačítko Zobrazit záznam" title="Tlačítko Zobrazit záznam" align="middle">
zobrazíte další informace o procesu čtení.<p></p></li>
</ul>

</td></tr>
EOF
}

function example10cs()
{  
  create_inline example cs 1 example10.html "na Skenování média na chyby"

cat >> $1 <<EOF
<h3>Skenování média na chyby</h3>

Kontrolu média na chyby čtení provedete tak následujete tyto kroky:<p>

(Ve Windows 2000 a XP jsou vyžadována buď <b>oprávnění administrátora</b> nebo nainstalovaný
ovladač <a href="qa10.html#aspi">ASPI</a>, aby bylo možno pracovat s mechanikami CD/DVD.)<p>

<table width="100%" border="0" cellspacing="0" cellpadding="10">
<tr valign="top" $BGCOLOR1>
<td>1.</td>
<td>Vložte médium do mechaniky.</td>
<td></td>
</tr>

<tr valign="top" $BGCOLOR2>
<td>2.</td>
<td>Zvolte mechaniku.</td>
<td><img src="images/btn-drive.png" alt="Volba mechaniky" title="Volba mechaniky"></td>
</tr>

<tr valign="top" $BGCOLOR1>
<td>3.</td>
<td>Pokud je dostupný soubor oprav chyb příslušný k vašemu médiu,
zadejte jeho název. Skenování soubor oprav chyb nezbytně nepotřebuje, 
ale může z něj výhodně načíst nějaké informace.
</td>
<td><img src="images/btn-eccfile.png" alt="Soubor oprav chyb" title="Soubor oprav chyb"><p>
Symbol <img src="../images/open-ecc.png" alt="Výběr souboru" title="Výběr souboru" align="middle"> otevírá výběr souborů.</td>
</tr>
 
<tr valign="top" $BGCOLOR2>
<td>4.</td>
<td>Kliknutím na tlačítko "Skenovat" začne kontrola média.</td>
<td><img src="images/btn-scan.png" alt="Tlačítko Skenovat" alt="Tlačítko Skenovat" title="Tlačítko Skenovat" align="middle"></td>
</tr>

<tr valign="top" $BGCOLOR1>
<td>5.</td>
<td>Sledujte proces skenování v graphické reprezentaci.</td>
<td><a href="example1.html"><img src="images/ex-scan.png" width="200" alt="Skenování média" title="Skenování média" align="middle"></a><p>
</td>
</tr>
</table>
<p></p>

<b>Jak často by se měla média skenovat?</b><br>
To závisí na kvalitě vašich médií a skladovacích podmínkách.
Použijte tento návod pro média CD a DVD neznámé kvality:

<ol>
<li>3 dny po vypálení (uchovejte obrazy médií na pevném disku do provedení úspěšného testu!),</li>
<li>14 dní po vypálení,</li>
<li>3 měsíce po vypálení,</li>
<li>a pak skenovat jedenkrát každých 6 měsíců.</li>
</ol>

Pokud používáte hodně médií ze stejné výrobní řady od velmi kvalitního výrobce,
mohlo by být dostatečné skenovat vzorek 2-3 médií měsíčně a dávkově.<p>

<hr><p>

<a name="cli"></a>
<h3>Skenování média z příkazové řádky</h3>

Skenování vyžaduje následující parametry (výchozí hodnoty jsou uvedeny v závorce):

<table>
<tr>
<td><a href="example90.html#scan">-s / --scan</a></td>
<td>Skenovat</td>
</tr>
<tr>
<td><a href="example90.html#device">-d / --device</a></td>
<td>Volba mechaniky (/dev/cdrom)</td>
</tr>
<tr>
<td><a href="example90.html#ecc">-e / --ecc</a></td>
<td>pouze pro <a href="background30.html">RS01</a>: Soubor oprav chyb (medium.ecc)</td>
</tr>
<tr>
<td><a href="example90.html#parse-ecc">--parse-ecc</a></td>
<td>pouze pro <a href="background30.html">RS02</a>: Použít informace z hlaviček ECC</td>
</tr>
<tr>
<td><a href="example90.html#jump">-j / --jump</a></td>
<td>Přeskočit sektory po chybě čtení (16)</td>
</tr>
</table>
<p></p>

<table class="example" width="100%" bgcolor="#ffffe0"><tr><td>
user@linux&gt; <b>dvdisaster -s -d /dev/hdc -e medium.ecc</b><br>
dvdisaster-${project_version} Copyright 2004-2006 Carsten Gnoerlich.<br>
[... zbytek oznámení GPL ...]<p>

Zařízení: /dev/cdrom, HL-DT-STDVDRAM GSA-4165B DL04<br>
Médium: DVD+R, 2224288 sektorů, 1 vrstva(y)<p>

Skenování média na chyby čtení.<br>
Čekám 5 sekund na roztočení disku...<br>
Sektor 57664: Medium Error; Unrecovered read error. Přeskakuji 15 sektorů.<br>
Sektor 57728: Medium Error; Unrecovered read error. Přeskakuji 15 sektorů.<br>
[... další hlášení o chybách čtení ...] <br>
Sektor 716640: Medium Error; Unrecovered read error. Přeskakuji 15 sektorů.<br>
Pozice čtení: 100.0% ( 16.0x)<br>
33840 nečitelných sektorů.
</td></tr>
</table>
<p></p>

Nahlášené chyby čtení mohou být jiné než "Medium Error; ...", obsah hlášení
závisí na firmwaru mechaniky.
EOF
}

function example20cs()
{  
   cat >> $1 <<EOF

<h3>Vytváření opravných dat</h3>

Opravná data mají v programu dvdisaster důležitou roli:<br>
Jsou nezbytná k obnovení nečitelných sektorů média.<p>

Opravná data mohou být uložena buď v samostatném
<a href="background30.html#file">souboru oprav chyb</a>
nebo mohou být s příslušnými
<a href="background30.html#table">výhodami a nevýhodami</a>
vepsána <a href="background30.html#image">do obrazu</a>.<p>

Rychlá pomoc při rozhodování:<p>

<b>Potřebujete opravná data pro existující médium?</b><p>

&nbsp; Ano: <a href="example21.html">Vytvořte soubor oprav chyb.</a><p>

&nbsp; Ne: viz následující otázka.<p>

<b>Zůstane na médiu, které se bude vytvářet, alespoň 20% volného prostoru?</b><p>

&nbsp; Ano: <a href="example22.html">Přidejte do obrazu opravná data.</a><p>

&nbsp; Ne: <a href="example21.html">Vytvořte soubor oprav chyb.</a><p>

EOF

}

function example2cs()
{  cat >> $1 <<EOF

<tr align="center"><td><h3>Snímek obrazovky: Načtení obrazu média</h3><p></td></tr>
<tr align="center"><td>
<center><img src="images/ex-read.png" alt="Načtení obrazu média" title="Načtení obrazu média"></center>
<br clear="all">
</td></tr>

<tr><td>
Průběh čtení se bude zobrazovat graficky.<p>

<ul>
<li><b>Spirála</b> poskytuje (není vykreslena v měřítku)
přehled o <b>stavu média</b>.
Červené značky indikují přítomnost chyb čtení!<p></p></li>

<li>
Soubor oprav chyb <i>nelze vytvořit</i>, pokud
není obraz kompletní nebo obsahuje nečitelné sektory.
Vytvořte soubor oprav chyb bezprostředně po vypálení média.
<p></p></li>

<li>
Kliknutím na symbol
<img src="images/btn-protocol.png" alt="Zobrazit záznam" align="middle">
zobrazíte další informace o procesu čtení.</li>
</ul>

</td></tr>
EOF
}

function example3cs()
{  cat >> $1 <<EOF

<tr align="center"><td>
<h3>Snímek obrazovky: Vytváření souboru oprav chyb</h3><p></td></tr>
<tr align="center"><td>
<center><img src="images/ex-create.png" alt="Vytváření souboru oprav chyb" title="Vytváření souboru oprav chyb"></center>
<br clear="all">
</td></tr>

<tr><td>
Průběh se zobrazuje pomocí procent. 
Zpracování obrazu jednovrstvého DVD ve
<a href="example83.html#redundancy">standardním nastavení redundance</a>
trvá asi 10 minut na průměrném 2 GHz systému.<p>

<b>Poznámky</b>:

<ul>
<li>Jakmile byl úspěšně vytvořen soubor oprav chyb,
soubor obrazu není dále potřeba.<br>
Program dvdisaster automaticky přepíše existující soubor obrazu, pokud se
načítá odlišné médium.<p></p></li>

<li>
Vždy uchovávejte soubor oprav chyb na
<a href="background70.html">spolehlivém úložném médiu</a>!<p>
</li>

<li>Od této chvíle byste měli pravidelně skenovat
médium na chyby čtení.</li>

</ul>
</td></tr>


EOF
}

function example21cs()
{  create_inline example cs 2 example21.html "na Vytváření souboru oprav chyb"
   create_inline example cs 3 example21.html "na Vytváření souboru oprav chyb"
 
   cat >> $1 <<EOF
<h3>Vytváření souboru oprav chyb</h3>

<b>Nejprve načtěte obraz média na pevný disk:</b><p>

<table width="100%" border="0" cellspacing="0" cellpadding="10">
<tr valign="top" $BGCOLOR1>
<td>1.</td>
<td>Vložte médium do mechaniky.</td>
<td></td>
</tr>

<tr valign="top" $BGCOLOR2>
<td>2.</td>
<td>Zvolte mechaniku.</td>
<td><img src="images/btn-drive.png" alt="Volba mechaniky" title="Volba mechaniky"></td>
</tr>

<tr valign="top" $BGCOLOR1>
<td>3.</td>
<td>Zvolte název souboru obrazu.

Ujistěte se, že máte dostatek volného místa pro obraz. Aktivujte
<a href="example81.html#localfiles">volbu rozdělení souboru</a>, pokud
váš souborový systém nepodporuje soubory větší než 2 GB.<p>
</td>
<td><img src="images/btn-image.png" alt="Soubor obrazu" title="Soubor obrazu"><p>
Symbol <img src="../images/open-img.png" alt="Výběr souboru" title="Výběr souboru" align="middle"> otevírá výběr souborů.</td>
</tr>

<tr valign="top" $BGCOLOR2>
<td>4.</td>
<td>Kliknutím na tlačítko "Načíst" spustíte čtení.</td>
<td><img src="images/btn-read.png" alt="Tlačítko Načíst" title="Tlačítko Načíst" align="middle"></td>
</tr>

<tr valign="top" $BGCOLOR1>
<td>5.</td>
<td>Sledujte proces čtení v graphické reprezentaci.</td>
<td><a href="example2.html"><img src="images/ex-read.png" width="200" alt="Načtení obrazu média" title="Načtení obrazu média" align="middle"></a><p>
</td>
</tr>
</table>

<p></p>

<b>Nyní vytvořte soubor oprav chyb:</b><p>
Obraz nesmí obsahovat žádné chyby čtení.<p>

<table width="100%" border="0" cellspacing="0" cellpadding="10">
<tr valign="top" $BGCOLOR1>
<td>1.</td>
<td>
Zvolte soubor obrazu.
Dříve načtený soubor obrazu bude přednastaven.<p>
</td>
<td><img src="images/btn-image.png" alt="Soubor obrazu" title="Soubor obrazu"><p>
Symbol <img src="../images/open-img.png" alt="Výběr souboru" title="Výběr souboru" align="middle"> otevírá výběr souborů.</td>
</tr>

<tr valign="top" $BGCOLOR2>
<td>2.</td>
<td>
Zvolte název pro soubor oprav chyb.<p>
</td>
<td><img src="images/btn-eccfile.png" alt="Soubor oprav chyb" title="Soubor oprav chyb"><p>
Symbol <img src="../images/open-ecc.png" alt="Výběr souboru" title="Výběr souboru" align="middle"> otevírá výběr souborů.</td>
</tr>

<tr valign="top" $BGCOLOR1>
<td>3.</td>
<td>
Nastavte volby opravy chyb.<p>
</td>
<td><a href="example83.html"><img src="images/prefs-ecc.png" width="200" alt="Nastavení oprav chyb" title="Nastavení oprav chyb"></a></td>
</tr>

<tr valign="top" $BGCOLOR2>
<td>4.</td>
<td>Kliknutím na tlačítko "Vytvořit" vytvoříte soubor oprav chyb.</td>
<td><img src="images/btn-create.png" alt="Tlačítko Vytvořit" title="Tlačítko Vytvořit"></td>
</tr>

<tr valign="top" $BGCOLOR1>
<td>5.</td>
<td>Sledujte průběh tvorby souboru oprav chyb.</td>
<td><a href="example3.html"><img src="images/ex-create.png" width="200" alt="Vytvoření souboru oprav chyb" title="Vytvoření souboru oprav chyb"></a><p>
</td>
</tr>
</table>
<p></p>

Vždy uchovávejte soubor oprav chyb na
<a href="background70.html">spolehlivém úložném médiu</a>.
Skenujte pravidelně chráněné médium na chyby čtení.<p>

<hr><p>

<h3>Vytvoření souboru oprav chyb z příkazové řádky</h3>

Pro načtení obrazu budete potřebovat následující parametry
(výchozí hodnoty jsou uvedeny v závorce):

<table>
<tr>
<td><a href="example90.html#read">-r/ --read</a></td>
<td>Načíst</td>
</tr>
<tr>
<td><a href="example90.html#device">-d / --device</a></td>
<td>Volba mechaniky (/dev/cdrom)</td>
</tr>
<tr>
<td><a href="example90.html#image">-i / --image</a></td>
<td>Soubor obrazu (medium.iso)</td>
</tr>
</table>
<p></p>

<table class="example" width="100%" bgcolor="#ffffe0"><tr><td>
user@linux&gt; <b>dvdisaster -r -d /dev/hdc -i medium.iso</b><br>
dvdisaster-${project_version} Copyright 2004-2006 Carsten Gnoerlich.<br>
[... zbytek oznámení GPL ...]<p>

Zařízení: /dev/cdrom, HL-DT-STDVDRAM GSA-4165B DL04<br>
Médium: DVD-R, 2287168 sektorů, 1 vrstva(y)<p>

Vytváření nového obrazu medium.iso.<br>
Čekám 5 sekund na roztočení disku...<br>
Pozice čtení: 100.0% (10.2x)<br>
Všechny sektory byly úspěšně načteny.
</td></tr>
</table>
<p></p>

V průběhu čtení se bude zobrazovat počet načtených sektorů v procentech
a aktuální rychlost mechaniky.  
Jakmile je načtení dokončeno, vytvořte soubor oprav chyb:<p>

<table>
<tr>
<td><a href="example90.html#create">-c/ --create</a></td>
<td>Vytvořte soubor oprav chyb</td>
</tr>
<tr>
<td><a href="example90.html#image">-i / --image</a></td>
<td>Soubor obrazu (medium.iso)</td>
</tr>
<tr>
<td><a href="example90.html#ecc">-e / --ecc</a></td>
<td>Soubor oprav chyb (medium.ecc)</td>
</tr>
<tr>
<td><a href="example90.html#redundancy">-n / --redundancy</a></td>
<td>Volba redundance (32 kořenů = 14.3%)</td>
</tr>
</table>
<p></p>

<table class="example" width="100%" bgcolor="#ffffe0"><tr><td>
user@linux&gt; <b>dvdisaster -c -i medium.iso -e medium.ecc </b><br>
dvdisaster-${project_version} Copyright 2004-2006 Carsten Gnoerlich.<br>
[... zbytek oznámení GPL ...]<p>

Otevírání medium.iso: 2224288 sektorů média.<br>
Skenuji sektory obrazu: 100%<br>
Kódování metodou RS01: 32 kořenů, redundance 14.3%.<br>
Tvorba ecc: 100.0%<br>
Soubor oprav chyb "medium.ecc" byl vytvořen.<br>
Zajistěte, aby byl tento soubor uložen na spolehlivém médiu.<br>
</td></tr>
</table>
<p></p>

Průběh se zobrazuje v procentech. <p>

Po dokončení se objeví v aktuálním adresáři soubor oprav chyb <i>medium.ecc</i>:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>ls -lA</b><br>
-rwx------    1 user     linux    653721680 2004-07-02 22:45 medium.ecc<br>
-rwx------    1 user     linux    4555341824 2004-07-02 21:31 medium.iso<br>
</td></tr>
</table>
<p></p>

Nyní můžete obraz smazat.<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>rm -f medium.iso</b><br>
</td></tr>
</table>
<p></p>

Ale uchovejte soubor oprav chyb na <a href="background70.html">spolehlivém úložném médiu</a>!
<p>

<u>Tip:</u> Zkombinujte akce načtení obrazu
a tvorby souboru oprav chyb. Ušetříte si skenování obrazu
v druhém kroku:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; dvdisaster <b>-r -c</b> -d /dev/hdc -i medium.iso -e medium.ecc<br>
(a tak dál)
</td></tr>
</table>
EOF
}

function example22cs()
{  
   cat >> $1 <<EOF
<h3>Přidání opravných dat do obrazu</h3>

Přidání opravných dat do obrazu je v současné době podporováno pouze
z příkazové řádky. Odpovídající funkce bude zabudována do grafického
uživatelského rozhraní programu dvdisaster ve verzi 0.70.<p>

<b>Vytvořte obraz dat, která mají být chráněna.</b><p>

Použijte svůj oblíbený software pro vypalování CD/DVD k volbě
datových souborů a vytvořte z nich ISO nebo UDF obraz. Použití
jiných typů obrazů může vést ke ztrátě dat!<p>

<b>Přidejte do obrazu opravná data.</b><p>

Akce vyžaduje následující parametry (výchozí hodnoty jsou uvedeny
v závorce):

<table>
<tr>
<td><a href="example90.html#create">-c/ --create</a></td>
<td>Vytvořit opravná data</td>
</tr>
<tr>
<td><a href="example90.html#method">-m / --method</a></td>
<td>Volba metody oprav chyb</td>
</tr>
<tr>
<td><a href="example90.html#redundancy">-n / --redundancy</a></td>
<td>maximální velikost média v sektorech (velikost nejmenšího možného média)</td>
</tr>
</table>
<p></p>

Jakmile vytvoříte obraz "image.iso" za použití softwaru pro vypalování
CD/DVD, zadejte:<p>

<table class="example" width="100%" bgcolor="#ffffe0"><tr><td>
user@linux&gt; <b>dvdisaster -c -mRS02 -i image.iso</b><br>
dvdisaster-${project_version} Copyright 2004-2006 Carsten Gnoerlich.<br>
[... zbytek oznámení GPL ...]<p>

Otevírání /dvd/image.iso: 284234 sektorů média.<p>

Přidávám do obrazu metodou RS02: 555M dat, 143M ecc (52 kořenů; redundance 25.6%)<br>
Vypočítávám sektorové kontrolní součty: 100%<br>
Rozšiřuji obraz: 100%<br>
Tvorba ecc: 100.0%<br>
Do obrazu byla přidána opravná data.<br>
Nová velikost obrazu je 357818 sektorů.
</td></tr>
</table>
<p></p>

<b>Vypalte obraz na médium.</b><p>

Použijte váš software pro vypalování CD/DVD k přenosu rozšířeného
obrazu na médium.<p>

Váš vypalovací software nemusí být kompatibilní s rozšířeným obrazem
a může zničit informace pro opravu chyb. Přinejmenším při prvním použití
proveďte následující kontrolu:<p>

Znovu načtěte obraz pomocí programu dvdisaster po vypálení média:<p>

<table>
<tr>
<td><a href="example90.html#read">-r/ --read</a></td>
<td>Načtení</td>
</tr>
<tr>
<td><a href="example90.html#device">-d / --device</a></td>
<td>Volba mechaniky (/dev/cdrom)</td>
</tr>
<tr>
<td><a href="example90.html#image">-i / --image</a></td>
<td>Soubor obrazu (medium.iso)</td>
</tr>
</table>
<p></p>

<table class="example" width="100%" bgcolor="#ffffe0"><tr><td>
user@linux&gt; <b>dvdisaster -r -d/dev/hdc -i image-new.iso</b><br>
dvdisaster-${project_version} Copyright 2004-2006 Carsten Gnoerlich.<br>
[... zbytek oznámení GPL ...]<p>

Zařízení: /dev/hdc, &nbsp;ATAPI DVD+RW 8X4X12 B2K7<br>
Médium: Datové CD režim 1, 357818 sektorů, 1 vrstva(y)<p>

Vytváření nového obrazu image-new.iso.<br>
Čekám 5 sekund na roztočení disku...<br>
Pozice čtení: 100.0% ( 6.3x)<br>
Všechny sektory byly úspěšně načteny.
</td></tr>
</table>
<p></p>

Zkontrolujte obraz:<p>

<table>
<tr>
<td><b><a href="example90.html#test">-t/ --test</a></b></td>
<td>Kontrola obrazu</td>
</tr>
<tr>
<td><b><a href="example90.html#image">-i / --image</a></b></td>
<td>Soubor obrazu (medium.iso)</td>
</tr>
</table>
<p></p>

<table class="example" width="100%" bgcolor="#ffffe0"><tr><td>
user@linux&gt; <b>dvdisaster -t -i image-new.iso</b><br>
dvdisaster-${project_version} Copyright 2004-2006 Carsten Gnörlich.<br>
[... zbytek oznámení GPL ...]<p>

dvd/new-image.iso: přítomen, obsahuje 357818 sektorů média.<br>
- dobrý obraz      : všechny sektory přítomny<br>
- md5suma dat      : 4eca3615a88a11ba68466226295c4bcb<p>

Data oprav chyb: vytvořil dvdisaster-0.66<br>
- metoda           : RS02, 52 kořenů, redundance 25.6%.<br>
- vyžaduje         : dvdisaster-0.65 (v pořádku)<br>
- md5suma dat      : 4eca3615a88a11ba68466226295c4bcb (v pořádku)<br>
- md5suma crc      : 9776489c12e12cbabfe5145da4f6ae66 (v pořádku)<br>
- md5suma ecc      : e975aed39c962eeabedd9d6cbdbfc45e (v pořádku)
</td></tr>
</table>
EOF
}

function example4cs()
{  cat >> $1 <<EOF

<tr align="center"><td>
<h3>Snímek obrazovky: Čtení poškozeného média</h3><p></td></tr>
<tr align="center"><td>
<center><img src="images/ex-read-a.png" alt="Čtení poškozeného média" title="Čtení poškozeného média"></center>
<br clear="all">
</td></tr>

<tr><td>
Uvědomte si prosím, že načtení poškozeného média může trvat několik hodin.
</td></tr>
EOF
}

function example30cs()
{  create_inline example cs 4 example30.html "na Vytváření obrazů z poškozených médií"
 
   cat >> $1 <<EOF
<h3>Vytváření obrazů z poškozených médií</h3>

dvdisaster ovládá <a href="background50.html">strategii čtení</a>,
která je zvláště vhodná pro čtení poškozených médií:<p>

<table width="100%" border="0" cellspacing="0" cellpadding="10">
<tr valign="top" $BGCOLOR1>
<td>1.</td>
<td>Vložte médium do mechaniky.</td>
<td></td>
</tr>

<tr valign="top" $BGCOLOR2>
<td>2.</td>
<td>Zvolte mechaniku.</td>
<td><img src="images/btn-drive.png" alt="Volba mechaniky" title="Volba mechaniky"></td>
</tr>

<tr valign="top" $BGCOLOR1>
<td>3.</td>
<td>
Zvolte název souboru obrazu.<p>
</td>
<td><img src="images/btn-image.png" alt="Soubor obrazu" title="Soubor obrazu"><p>
Symbol <img src="../images/open-img.png" alt="Výběr souboru" title="Výběr souboru" align="middle"> otevírá výběr souborů.</td>
</tr>

<tr valign="top" $BGCOLOR2>
<td>4.</td>
<td>Zvolte název pro soubor oprav chyb.
Soubor oprav chyb musí být v tomto stádiu dostupný, aby program dvdisaster mohl
určit, zda bylo shromážděno dostatek dat pro rekonstrukci obrazu.<p>
</td>
<td><img src="images/btn-eccfile.png" alt="Soubor oprav chyb" title="Soubor oprav chyb"><p>
Symbol <img src="../images/open-ecc.png" alt="Výběr souboru" title="Výběr souboru" align="middle"> otevírá výběr souborů.</td>
</tr>

<tr valign="top" $BGCOLOR1>
<td>5.</td>
<td>
Zvolte strategii adaptivního čtení.<p>
</td>
<td><a href="example82.html"><img src="images/prefs-read.png" width="200" alt="Nastavení čtení" title="Nastavení čtení"></a></td>
</tr>

<tr valign="top" $BGCOLOR2>
<td>6.</td>
<td>Spusťte proces čtení kliknutím na tlačítko "Načíst".</td>
<td><img src="images/btn-read.png" alt="Tlačítko Načíst" title="Tlačítko Načíst"></td>
</tr>

<tr valign="top" $BGCOLOR1>
<td>7.</td>
<td>Sledujte proces čtení.</td>
<td><a href="example4.html"><img src="images/ex-read-a.png" width="200" alt="Čtení poškozeného média" title="Čtení poškozeného média"></a><p>
</td>
</tr>
</table>
<p></p>

Jakmile proces čtení process hlásí, že bylo shromážděno dostatek dat pro
úspěšnou opravu, <a href="example40.html">opravte chybějící sektory</a> 
za použití <a href="example20.html">opravných dat</a>.<p>

Jinak zkoušejte načíst obraz znovu
<a href="background60.html#reading-tips">(některé tipy pro opakované čtení)</a>.
V takovém případě se program dvdisaster pokusí znovu načíst pouze chybějící sektory.

<hr><p>

<h3>Čtení poškozených médií z příkazové řádky</h3>

Čtení vyžaduje následující parametry (výchozí hodnoty jsou uvedeny v závorce):

<table>
<tr>
<td><a href="example90.html#read">-r/ --read</a></td>
<td>Načíst</td>
</tr>
<tr>
<td><a href="example90.html#adaptiveread">--adaptive-read</a></td>
<td>Použít strategii čtení pro poškozená média</td>
</tr>
<tr>
<td><a href="example90.html#device">-d / --device</a></td>
<td>Volba mechaniky (/dev/cdrom)</td>
</tr>
<tr>
<td><a href="example90.html#image">-i / --image</a></td>
<td>Soubor obrazu (medium.iso)</td>
</tr>
<tr>
<td><a href="example90.html#ecc">-e / --ecc</a></td>
<td>pouze pro <a href="background30.html">RS01</a>: Soubor oprav chyb (medium.ecc)</td>
</tr>
<tr>
<td><a href="example90.html#parse-ecc">--parse-ecc</a></td>
<td>pouze pro <a href="background30.html">RS02</a>: Použít informace z hlaviček ECC</td>
</tr>
<tr>
<td><a href="example90.html#jump">-j / --jump</a></td>
<td>Přeskočit sektory po chybě čtení (16)</td>
</tr>
</table>
<p></p>

<table class="example" width="100%" bgcolor="#ffffe0"><tr><td>
user@linux&gt; <b>dvdisaster -r --adaptive-read -i medium.iso -e medium.ecc</b><br>
dvdisaster-${project_version} Copyright 2004-2006 Carsten Gnoerlich.<br>
[... zbytek oznámení GPL ...]<p>

Zařízení: /dev/cdrom, HL-DT-STDVDRAM GSA-4165B DL04<br>
Médium: DVD+R, 2224288 sektorů, 1 vrstva(y)<p>

Adaptivní čtení: Pokouším se shromáždit dostatek dat pro opravu chyb.<br>
Vytváření nového obrazu medium.iso.<br>
Opravitelné:  2.6% (opravitelné: 0; nyní načítám [0..2224288], velikost 2224288)<br>
Sektory 57264-57279: Medium Error; Unrecovered read error.<br>
Vyplňování oblasti obrazu [57280..1083504]<br>
[... další hlášení o procesu čtení ...] <br>
Opravitelné: 100.0% (opravitelné: 319200; nyní načítám [320304..327065], velikost 6762)<br>
Je k dispozici dostatek dat pro rekonstrukci obrazu.
</td></tr>
</table>
<p></p>
EOF
}

function example5cs()
{  cat >> $1 <<EOF

<tr align="center"><td>
<h3>Snímek obrazovky: Oprava obrazu média</h3><p></td></tr>
<tr align="center"><td>
<center><img src="images/ex-fix.png" alt="Oprava média" title="Oprava média"></center>
<br clear="all">
</td></tr>

<tr><td>
Průběh opravy se zobrazuje v procentech.<p>

<ul>
<li>Zpráva "V pořádku! Všechny sektory jsou opraveny." potvrzuje, že obraz média
byl plně obnoven. Opravený obraz může  být dále zpracováván jako běžný obraz ". i s o".
Lze jej zapsat na nové médium za použití vhodného vypalovacího programu
pro CD/DVD.<p></p></li>

<li>Pokud v tomto stádiu ještě zůstanou neopravitelné sektory, zkuste načtení
obrazu znovu <a href="background60.html#reading-tips">(tipy pro opakované čtení)</a>.
Program dvdisaster se pokusí znovu načíst pouze chybějící sektory, a tak by se mohlo
získat dostatek chybějících sektorů pro úspěšný průběh opravy chyb.<p></p></li>

<li>Červený chybový graf <i>neodpovídá</i> prostorovému rozmístění
chyb čtení na disku: Metoda opravy chyb je výhradně postavena tak, aby
dosáhla rovnoměrného rozložení chyb čtení média přes všechny bloky oprav chyb.<p>
Zelená čára označuje maximální kapacitu opravy chyb. Pokud je tato čára překročena,
obraz nemůže být opraven s aktuálně dostupnými daty.
V případě malých překročení by mohl pomoci
<a href="background60.html#reading-tips">další pokus o čtení</a>.
</li>

<li>Pokud byl obraz načítán strategií adaptivního čtení,
pak je vždy chybová korekce maximální.</li>
</ul>
</td></tr>
EOF
}


function example40cs()
{  create_inline example cs 5 example40.html "na Oprava obrazu média"

   cat >> $1 <<EOF
<h3>Oprava obrazu média</h3>

Abyste opravili vadné médium, musíte nejprve zkusit
<a href="example30.html">z něj načíst co možná nejvíce dat</a>.
Pak potřebujete
<a href="example20.html">odpovídající opravná data</a>,
která jste snad vytvořili v době, kdy bylo ještě médium plně čitelné.
Proveďte následující akce.<p>

<table width="100%" border="0" cellspacing="0" cellpadding="10">
<tr valign="top" $BGCOLOR1>
<td>1.</td>
<td>
Zvolte soubor obrazu.<p>
</td>
<td><img src="images/btn-image.png" alt="Soubor obrazu" title="Soubor obrazu"><p>
Symbol <img src="../images/open-img.png" alt="Výběr souboru" title="Výběr souboru" align="middle">otevírá výběr souborů.</td>
</tr>

<tr valign="top" $BGCOLOR2>
<td>2.</td>
<td>
Zvolte odpovídající soubor oprav chyb.<p>
</td>
<td><img src="images/btn-eccfile.png" alt="Soubor oprav chyb" title="Soubor oprav chyb"><p>
Symbol <img src="../images/open-ecc.png" alt="Výběr souboru" title="Výběr souboru" align="middle"> otevírá výběr souborů.</td>
</tr>

<tr valign="top" $BGCOLOR1>
<td>3.</td>
<td>Klikněte na tlačítko "Opravit".</td>
<td><img src="images/btn-fix.png" alt="Tlačítko Opravit" title="Tlačítko Opravit"></td>
</tr>

<tr valign="top" $BGCOLOR2>
<td>4.</td>
<td>Sledujte proces opravy.
Normálně se během opravy obrazu místy projevuje vysoká
aktivita pevného disku.</td>
<td><a href="example5.html"><img src="images/ex-fix.png" width="200" alt="Oprava obrazu média" title="Oprava obrazu média"></a><p>
</td>
</tr>
</table>
<p></p>

Jakmile je obraz úspěšně opraven, přeneste jej na nové médium za
použití vypalovacího softwaru pro CD/DVD.

<hr><p>

<h3>Oprava obrazu z příkazové řádky</h3>

Oprava vyžaduje následující parametry (výchozí hodnoty jsou uvedeny v závorce):

<table>
<tr>
<td><a href="example90.html#fix">-f/ --fix</a></td>
<td>Opravit</td>
</tr>
<tr>
<td><a href="example90.html#image">-i / --image</a></td>
<td>Soubor obrazu (medium.iso)</td>
</tr>
<tr>
<td><a href="example90.html#ecc">-e / --ecc</a></td>
<td>Soubor oprav chyb (medium.ecc)</td>
</tr>
</table>
<p></p>

<table class="example" width="100%" bgcolor="#ffffe0"><tr><td>
user@linux&gt; <b>dvdisaster -f -i medium.iso -e medium.ecc</b><br>
dvdisaster-${project_version} Copyright 2004-2006 Carsten Gnoerlich.<br>
[... zbytek oznámení GPL ...]<p>

Otevírání medium.iso: 2224288 sektorů média.<p>

Režim opravy: Opravitelné sektory budou v obrazu opraveny.<br>
&nbsp;&nbsp;&nbsp;    5 opravených sektorů: 708225 59850 69825 79800 199500 <br>
&nbsp;&nbsp;&nbsp;    6 opravených sektorů: 708226 59851 69826 79801 89776 199501<br>
[... další hlášení o opravených sektorech ...] <br>
&nbsp;&nbsp;&nbsp;    5 opravených sektorů: 708224 59849 69824 79799 199499<br>
Průběh ecc: 100.0% <br>
Opraveno sektorů: 33264 <br>  
V pořádku! Všechny sektory jsou opraveny.<br>
Počet výmazů na blok ecc:  průměr =  3.3; nejvíce = 7.
</td></tr>
</table>
<p></p>

EOF
}

function example6cs()
{  cat >> $1 <<EOF

<tr align="center"><td>
<h3>Snímek obrazovky: Porovnání souborů obrazu a oprav chyb</h3><p></td></tr>
<tr align="center"><td>
<center><img src="images/ex-compare.png" title="Porovnání souborů obrazu a oprav chyb"></center>
<br clear="all">
</td></tr>

<tr><td>

Tento proces může chvilku trvat, protože
je třeba načíst celé oba soubory (ale nejsou na nich provedeny žádné změny).<p>

Když je porovnání dokončeno, získáte následující informace:<p>

<ul>
<li><b>"Souhrn souboru obrazu" a "Stav obrazu"</b>. 
Počet chybějících sektorů bude vyšší než nula, pokud obraz stále obsahuje
neopravitelné chyby čtení. Naproti tomu výskyt chyb kontrolního součtu
je obvykle důsledkem
<a href="qa20.html#crc">nesprávného zpracování nebo hardwarových problémů</a>.<p></p></li>

<li><b>"Souhrn souboru oprav chyb"</b>.<br> 
Tato položka zobrazuje nastavení použitá při tvorbě souboru oprav chyb,
a zda může být zpracován vyšší verzí programu dvdisaster.
Pokud soubor oprav chyb neodpovídá souboru obrazu nebo pokud je poškozen,
upozornění jsou zobrazena pod posledními čtyřmi položkami.<p></p></li>

<li><b>Další tipy:</b> Výsledek porovnání je nespolehlivý, pokud byl obraz načten jiným softwarem,
než programem dvdisaster. Zvláště všechny chybějící sektory budou
klasifikovány jako chyby kontrolního součtu, byl-li použit jiný software.</li>

</ul>
</td></tr>
EOF
}

function example50cs()
{  create_inline example cs 6 example50.html "na Získání informací o obrazech a opravných datech"

   cat >> $1 <<EOF
<h3>Získání informací o obrazech a opravných datech</h3>

Tato funkce poskytuje informaci o souborech obrazu a oprav chyb, které
jsou aktuálně uloženy na vašem pevném disku:<p>

<table width="100%" border="0" cellspacing="0" cellpadding="10">
<tr valign="top" $BGCOLOR1>
<td>1.</td>
<td>
Zvolte soubor obrazu.<p>
</td>
<td><img src="images/btn-image.png" alt="Soubor obrazu" title="Soubor obrazu"><p>
Symbol <img src="../images/open-img.png" alt="Výběr souboru" title="Výběr souboru" align="middle"> otevírá výběr souborů.</td>
</tr>

<tr valign="top" $BGCOLOR2>
<td>2.</td>
<td>
Zvolte odpovídající soubor oprav chyb.<p>
</td>
<td><img src="images/btn-eccfile.png" alt="Soubor oprav chyb" title="Soubor oprav chyb"><p>
Symbol <img src="../images/open-ecc.png" alt="Výběr souboru" title="Výběr souboru" align="middle"> otevírá výběr souborů.</td>
</tr>

<tr valign="top" $BGCOLOR1>
<td>3.</td>
<td>Klikněte na tlačítko "Porovnat".</td>
<td><img src="images/btn-compare.png" alt="Tlačítko Porovnat" title="Tlačítko Porovnat"></td>
</tr>

<tr valign="top" $BGCOLOR2>
<td>4.</td>
<td>Sledujte proces porovnávání.</td>
<td><a href="example6.html"><img src="images/ex-compare.png" width="200" alt="Porovnání souborů obrazu a oprav chyb" title="Porovnání souborů obrazu a oprav chyb"></a><p>
</td>
</tr>
</table>
<p></p>

<hr><p>

<h3>Získání informací z příkazové řádky</h3>

Budete potřebovat následující parametry (výchozí hodnoty jsou uvedeny v závorce):

<table>
<tr>
<td><a href="example90.html#test">-t/ --test</a></td>
<td>Porovnat</td>
</tr>
<tr>
<td><a href="example90.html#image">-i / --image</a></td>
<td>Soubor obrazu (medium.iso)</td>
</tr>
<tr>
<td><a href="example90.html#ecc">-e / --ecc</a></td>
<td>Soubor oprav chyb (medium.ecc)</td>
</tr>
</table>
<p></p>

<table class="example" width="100%" bgcolor="#ffffe0"><tr><td>
user@linux&gt; <b>dvdisaster -t -i medium.iso -e medium.ecc</b><br>
dvdisaster-${project_version} Copyright 2004-2006 Carsten Gnörlich.<br>
[... zbytek oznámení GPL ...]<p>

/dvd/medium.iso: přítomen, obsahuje 2224288 sektorù média.<br>
- dobrý obraz      : všechny sektory jsou přítomny<br>
- md5suma obrazu   : 55cdd507e8d96af0da0667ce4365c7ae<p>

/dvd/medium.ecc: vytvořil dvdisaster-0.62<br>
- metoda           : RS01, 32 kořenù, redundance 14.3%.<br>
- vyžaduje         : dvdisaster-0.55 (v pořádku)<br>
- sektorů média    : 2224288 (v pořádku)<br>
- md5suma obrazu   : 55cdd507e8d96af0da0667ce4365c7ae (v pořádku)<br>
- porovnání otisku : v pořádku<br>
- ecc bloky        : 20428800 (v pořádku)<br>
- md5suma ecc      : 4bdf5ed398e7662ac93c9d08e1ba9ff2 (v pořádku)
</td></tr>
</table>
<p></p>
EOF
}

function example80cs()
{  cat >> $1 <<EOF

<h3>Konfigurace</h3>

Stiskněte tlačítko
<img src="../images/open-preferences.png" align="middle" alt="Tlačítko nastavení" title="Tlačítko nastavení">
v pravém horním rohu nástrojové lišty k otevření dialogu nastavení.
Dialog obsahuje několik formulářů:<p>

<center>
<a href="example81.html">
<img src="images/prefs-general.png" alt="Obecná nastavení" title="Obecná nastavení"><br>
Obecná nastavení
</a>
</center>
<br clear="all">

<center>
<a href="example82.html">
<img src="images/prefs-read.png" alt="Nastavení čtení" title="Nastavení čtení"><br>
Nastavení čtení
</a>
</center>
<br clear="all">

<center>
<a href="example83.html">
<img src="images/prefs-ecc.png" alt="Nastavení oprav chyb" title="Nastavení oprav chyb"><br>
Nastavení oprav chyb
</a>
</center>

EOF
}

function example81cs()
{  cat >> $1 <<EOF

<h3>Obecná nastavení</h3>

Více informací o formuláři následuje níže na stránce.<p>

<center><img src="images/prefs-general.png" alt="Obecná nastavení" title="Obecná nastavení"></center>
<br clear="all">

<p><hr><p>

<a name="iso"></a>
<b>Souborový systém média a obrazu</b><p>

<center><img src="images/prefs-general-1.png" alt="Souborový systém média a obrazu" title="Souborový systém média a obrazu"></center>
<br clear="all">

Tyto volby napravují některé problémy způsobené
<a href="qa20.html#plusrw">hlášením nesprávné délky obrazů</a>
některými mechanikami pro média DVD-RW/+RW.<p>

<b>Použít informace z hlaviček ECC <font color="red">(1)</font>:</b>
Velikost obrazu bude určena z hlaviček opravných informací
<a href="background30.html">RS02</a>.
Pokud obraz neobsahuje opravná data RS02, bude při použití této
volby zahájení procesu čtení podstatně zpožděno.<p>

<b>Použít informace ze souborového systému ISO/UDF <font color="red">(2)</font>:</b>
Program dvdisaster určí velikost obrazu ze souborového systému ISO/UDF,
je-li tato možnost zaškrtnuta. Pokud obraz obsahuje opravné informace
<a href="background30.html">RS02</a>, musíte také nastavit volbu
<font color="red">(1)</font>. Jinak nebudou do obrazu opravná
data zahrnuta.


<p><hr><p>

<a name="localfiles"></a>
<b>Nastavení pro místní soubory</b><p>
<center><img src="images/prefs-general-2.png" alt="Nastavení pro místní soubory" title="Nastavení pro místní soubory"></center>
<br clear="all">
 
<b>Automatická přípona souborů<font color="red">(1)</font>:</b>
Pokud je tento přepínač nastaven, soubory dostanou automaticky
přípony ".iso" nebo ".ecc", pokud dosud není přípona v názvu přítomna.<p>

<b>Rozdělení souborů na segmenty<font color="red">(2)</font>:</b>
Umožňuje pracovat se souborovými systémy, které jsou omezeny na 2 GB na soubor
(t.j. FAT z Windows). Vytvořené soubory jsou rozprostřeny přes až 100 segmentů
nazvaných "medium00.iso", "medium01.iso" atd. za cenu malého snížení výkonu.<p>

<p><hr><p>

<a name="auto"> </a>
<b>Automatická tvorba a mazání souborů</b><p>

<center><img src="images/prefs-general-3.png" alt="Automatická tvorba a mazání souborů" title="Automatická tvorba a mazání souborů"></center>
<br clear="all">

<b>Vytvořit soubor oprav chyb po načtení obrazu <font color="red">(1)</font>:</b>
Automaticky vytvoří soubor oprav chyb, jakmile je načten obraz.
Spolu s volbou "Odstranit obraz" urychluje tvorbu souborů
oprav chyb pro sadu různých médií.<p>

<b>Odstranit obraz <font color="red">(2)</font>:</b>
Pokud je toto nastavení aktivováno, soubor obrazu bude
smazán, jakmile skončí úspěšně tvorba odpovídajícího
souboru oprav chyb.
EOF
}

function example82cs()
{  cat >> $1 <<EOF

<h3>Nastavení čtení</h3>

Více informací o formuláři následuje níže na stránce.<p>

<center><img src="images/prefs-read.png" alt="Nastavení čtení" title="Nastavení čtení"></center>
<br clear="all">

<p><hr><p>

<a name="read"></a>
<b>Nastavení čtení</b><p>

<center><img src="images/prefs-read-1.png" alt="Nastavení čtení" title="Nastavení čtení"></center>
<br clear="all">

<b>Strategie čtení <font color="red">(1)</font>:</b> Zvolte mezi
<a href="background40.html">lineární strategií čtení</a> a
<a href="background50.html">adaptivní strategií čtení</a>.
Lineární strategie je vhodná pro nepoškozená média, zatímco adaptivní
strategie je lepší pro média, která již vykazují chyby čtení.<p>

<b>Rozsah čtení <font color="red">(2)</font>:</b> Čtení bude omezeno na
daný rozsah sektorů včetně okrajů: 0-100 bude číst 101 sektorů. Tato
nastavení jsou aktivní pouze pro aktuální sezení a nebudou uložena.<p>

<b>Přeskočit x sektorů po chybě čtení / <br>
Přestat číst, je-li nečitelný interval < x <font color="red">(3)</font>:</b><br>
Hodnota posuvníku ovlivňuje zpracování chyb čtení. Má mírně odlišné vlastnosti
pro <a href="background40.html#configure">lineární</a> a
<a href="background50.html#configure">adaptivní</a> strategie čtení.
Vysoké hodnoty zkracují čas zpracování a snižují mechanické opotřebení
mechaniky, ale také nechávají větší mezery v obrazu při čtení poškozených oblastí.

<p><hr><p>

<a name="image"></a>
<b>Vlastnosti obrazu</b><p>

<center><img src="images/prefs-read-2.png" alt="Vlastnosti obrazu" title="Vlastnosti obrazu"></center>
<br clear="all">

<b>Obraz DAO <font color="red">(1)</font>:</b>
Opravuje <a href="qa20.html#tao">řídký problém</a> při čtení obrazů
z "DAO" ("disc at once") CD-R médií.<p>

<b>Vyplnit nečitelné sektory <font color="red">(2)</font>:</b> 
Poskytuje kompatibilitu s dalším softwarem pro obnovu dat. 
V současné době jsou známá tato nastavení a programy:<p>
<i>0xb0 (desítkově 176)</i> -- h2cdimage/dares (publikováno v "c't", německém periodiku)<p>
Nezaškrtávejte (zakázáno) tuto položku, pokud obrazy zpracováváte výlučně
programem dvdisaster.

<p><hr><p>

<b>Inicializace mechaniky</b><p>

<center><img src="images/prefs-read-3.png" alt="Inicializace mechaniky" title="Inicializace mechaniky"></center>
<br clear="all">
Čeká daný počet sekund, až se mechanika roztočí, než začne
skutečné čtení dat. Tak se vyhnete rychlostním skokům na začátku
křivky čtení.
EOF
}

function example83cs()
{  cat >> $1 <<EOF

<h3>Nastavení oprav chyb</h3>

Více informací o formuláři následuje níže na stránce.<p>

<center><img src="images/prefs-ecc.png" alt="Nastavení oprav chyb" title="Nastavení oprav chyb"></center>
<br clear="all">

<p><hr><p>

<a name="redundancy"></a>
<b>Redundance pro nové soubory oprav chyb</b><p>

<center><img src="images/prefs-ecc-1.png" alt="Redundance pro nové soubory oprav chyb" title="Redundance pro nové soubory oprav chyb"></center>
<br clear="all">

Redundance určuje množství opravitelných chyb
<a href="background10.html" title="Technické vlastnosti opravy chyb">v nejlepším případě</a>.
Vzhledem k tomu, že ideální případy jsou řídké, doporučuje se použít rozumnou hranici,
když volíte redundanci:<p>

<font color="red">(1)</font> / <font color="red">(2)</font>
Předvolby <b>normální</b> a <b>vysoká</b> poskytují redundanci 
14,3% a 33,5%. Jsou prováděny optimalizovanou částí programu, aby se
urychlilo vytváření souboru oprav chyb.<p>

<font color="red">(3)</font>Volba redundance <b>podle procent</b>. Všimněte si prosím:

<ul>
<li>Soubor oprav chyb s x% redundancí bude mít přibližně x% velikosti
odpovídajícího souboru obrazu.</li>
<li>Schopnost opravy chyb závisí na statistickém rozmístění chyb čtení. 
Pouze změny o 5 procent a více mohou mít viditelný efekt na opravu chyb.<p></p></li>
</ul>

<font color="red">(4)</font>Volba <b>maximální velikosti</b> souboru
oprav chyb v MB. Program dvdisaster zvolí vhodné nastavení redundance tak,
aby celková velikost souboru oprav chyb nepřesáhla daný limit. <p>

Poznámka pro pokročilé: Použijeme-li stejné nastavení velikosti pro obrazy
velmi odlišných velikostí, je více informací pro opravu chyb přiděleno menším obrazům
a méně větším.<p>

<p><hr><p>

<b>Využití paměti</b><p>

<center><img src="images/prefs-ecc-2.png" alt="Využití paměti" title="Využití paměti"></center>
<br clear="all">

Program dvdisaster optimalizuje přístup k obrazům a souborům oprav chyb tím,
že si udržuje vlastní vyrovnávací paměť. Přednastavení 32 MB je
vyhovující pro většinu systémů.<p>

EOF
}

function example90cs()
{  cat >> $1 <<EOF
<h3>Syntaxe příkazu</h3>

<b>Používání příkazové řádky.</b> 
Program dvdisaster se přepne do režimu příkazové řádky, pokud je vyvolán
s <a href="example90.html#modes">akčním parametrem</a> 
(jako je read, create, scan, fix, test). Jinak se otevře okno
<a href="example10.html">grafického uživatelského rozhraní</a>.<p>

Režim příkazové řádky ignoruje jakákoli nastavení grafického
uživatelského rozhraní a konfiguračního souboru <i>.dvdisaster</i>.
Stejně tak zde popisované volby příkazové řádky nemají žádný
efekt v grafickém uživatelském rozhraní.<p>

Z technických důvodů existují dvě verze programu pro Windows:<p>

<table>
<tr><td><i>dvdisaster.exe</i></td><td>--</td><td>poskytuje rozhraní příkazové řádky</td></tr>
<tr><td><i>dvdisaster-win.exe</i></td><td>--</td><td>obsahuje grafické uživatelské rozhraní</td></tr>
</table>
<p></p>

<b><a name="modes">Akce.</a>&nbsp;</b>Program dvdisaster musí být volán s alespoň
jednou zadanou akcí buď v dlouhé formě (--read) nebo jen zadáním jejího počátečního
písmene (-r):<p>

<table>
<tr valign=top><td>&nbsp; &nbsp;</td><td><a href="#scan">--scan</a></td><td>Skenuje médium na chyby čtení</tr>
<tr valign=top><td></td><td><a href="#read">--read</a></td><td>Načte obraz média na pevný disk</td></tr>
<tr valign=top><td></td><td><a href="#create">--create</a> &nbsp; &nbsp;</td><td>Vytvoří soubor oprav chyb</td></tr>
<tr valign=top><td></td><td><a href="#fix">--fix</a></td><td>Zkusí opravit obraz s použitím souboru oprav chyb</td></tr>
<tr valign=top><td></td><td><a href="#test">--test</a></td><td>Otestuje, zda si obraz a soubor oprav chyb odpovídají</td></tr>
<tr valign=top><td></td><td><a href="#unlink">--unlink</a></td><td>Vymaže soubor obrazu při ukončení programu</td></tr>
</table>
<pre> </pre> 

<b><a name="drive">Zařízení a názvy souborů.</a></b> Tyto volby mají také dlouhou formu (--device)
a jednopísmennou zkratku (-d). Používají se pro změnu přednastavené mechaniky a názvů souborů.<p>

<table>
<tr><td>&nbsp; &nbsp;</td><td><a href="#device">--device</a></td><td>Výběr zařízení</td></tr>
<tr><td></td><td><a href="#prefix">--prefix</a></td><td>Název pro soubory obrazu a oprav chyb</td></tr>
<tr><td></td><td><a href="#image">--image</a></td><td>Název souboru obrazu</td></tr>
<tr valign=top><td></td><td><a href="#ecc">--ecc</a></td><td>Název souboru oprav chyb<p></td></tr>
<tr><td></td><td><a href="#list">--list</a></td><td>Zobrazit mechaniky dostupné pod ASPI (pouze verze pro Windows)</td></tr>
</table><pre> </pre> 

<b><a name="options">Další volby.</a></b> Tyto méně používané volby jsou dostupné pouze
v dlouhé formě, pokud není zmíněno jinak.<p>

<table>
<tr valign=top><td>&nbsp; &nbsp;</td><td><a href="#adaptiveread">--adaptive-read</a></td><td>Použít adaptivní strategii čtení poškozených médií</td></tr>
<tr valign=top><td>&nbsp; &nbsp;</td><td><a href="#autosuffix">--auto-suffix</a></td><td>Automaticky doplnit souborové přípony .iso a .ecc</td></tr>
<tr valign=top><td>&nbsp; &nbsp;</td><td><a href="#cache">--cache-size</a> &nbsp; &nbsp;</td><td>Velikost vyrovnávací paměti v průběhu vytváření souboru oprav chyb</td></tr>
<tr valign=top><td></td><td><a href="#dao">--dao</a></td><td>Předpokládat médium zapsané v režimu "disk at once"</td></tr>
<tr valign=top><td></td><td><a href="#fillunreadable">--fill-unreadable [n]</a></td><td>Vyplnit nečitelné sektory daným bajtem</td></tr>
<tr valign=top><td></td><td><a href="#jump">-j / --jump</a></td><td>Přeskočit sektory po chybě čtení</td></tr>
<tr valign=top><td></td><td><a href="#parse-ecc">--parse-ecc</a></td><td>Použít informace z hlaviček opravných informací</td></tr>
<tr valign=top><td></td><td><a href="#parse-udf">--parse-udf</a></td><td>Použít informace ze souborového systému ISO/UDF</td></tr>
<tr valign=top><td></td><td><a href="#redundancy">-n / --redundancy</a></td><td>Nastavit redundanci pro opravný kód</td></tr>
<tr valign=top><td></td><td><a href="#method">-m / --method</a>&nbsp; &nbsp;</td><td>Zvolit metodu opravy chyb</td></tr>
<tr valign=top><td></td><td><a href="#speedwarn">--speed-warning [n]</a>&nbsp; &nbsp;</td><td>Varovat při určité hodnotě poklesu rychlosti čtení</td></tr>
<tr valign=top><td></td><td><a href="#spinup">--spinup-delay [n]</a>&nbsp; &nbsp;</td><td>Poskytnout mechanice čas na roztočení</td></tr>
<tr valign=top><td></td><td><a href="#split">--split-files</a>&nbsp; &nbsp;</td><td>Rozdělit soubory do segmentů <= 2 GB</td></tr>
</table>

<h3>Akce.</h3>

<a name="scan"><b>--scan [n-m]: Skenovat médium na chyby čtení</b></a><p>

Čte každý sektor média
a vytiskne po dokončení počet nečitelných sektorů.<p>

Je možné omezit skenování na určitý rozsah sektorů. Počítání sektorů
začíná 0; "end" znamená poslední sektor.
Zadaný interval je včetně; 0-100 bude číst 101 sektorů.<p>

Nepoužívejte žádnou mezeru ve zkrácené formě mezi "-s" a intervalem: 
<pre>
dvdisaster -s0-100          # čte sektory 0 až 100
dvdisaster --scan 0-100     # dlouhá forma
dvdisaster --scan 3000-end  # čte od sektoru 3000 do konce média
</pre>
<div align=right><a href="#modes">&uarr;</a></div><p>


<a name="read"><b>--read [n-m]: Načíst obraz média na pevný disk</b></a><p>

Vytvoří obraz média na pevném disku.<p>

Pokud již soubor obrazu existuje, budou se načítat jen chybějící sektory.
To umožňuje zkompletovat obraz několika průchody čtení a případně
s použitím jiných mechanik v každém z nich.<p>

--read bude standardně používat <a href="background40.html">lineární strategii čtení</a>.
Místo toho můžete přidat volbu <a href="example90.html#adaptiveread">--adaptive-read</a>
pro použití <a href="background50.html">adaptivní strategie čtení</a>.<p>

Rozsah načítaných sektorů může být omezen analogicky jako volba
<a href="example90.html#scan">--scan</a>.

<div align=right><a href="#modes">&uarr;</a></div><p>


<a name="create"><b>--create: Vytvořit soubor oprav chyb</b></a><p>

Vytvoří pro obraz soubor oprav chyb.<p>

<div align=right><a href="#modes">&uarr;</a></div><p>



<a name="fix"><b>--fix: Opravit obraz</b></a><p>

Pokusí se opravit obraz s použitím souboru oprav chyb.<p>

Pokud nemohou být všechny sektory opraveny během prvního průchodu,
zkuste opakované čtení obrazu dalším průchodem příkazu <a href="#read">--read</a>
<a href="background60.html#reading-tips">(tipy pro opakované čtení)</a>.
Tak by se mohlo získat dostatek chybějících sektorů
pro úspěšný průběh opravy chyb.<p>

<div align=right><a href="#modes">&uarr;</a></div><p>



<a name="test"><b>--test: Otestovat, zda si obraz a soubor oprav chyb odpovídají</b></a><p>

Tato akce jednoduše ověří, zda k sobě patří obraz a soubor oprav chyb
(t.j. patří ke stejnému médiu) a zda jsou jejich vnitřní kontrolní součty správné.<p>

Viz také <a href="qa20.html#crc">téma 3.3</a> v <a href="qa20.html">Dotazech
a odpovědích</a>.


<div align=right><a href="#modes">&uarr;</a></div><p>


<a name="unlink"><b>--unlink: Smazat soubor obrazu při ukončení programu</b></a><p>

Tato funkce smaže soubor obrazu, pokud byly všechny předchozí akce úspěšné.

<div align=right><a href="#modes">&uarr;</a></div><p>



<h3>Zařízení a názvy souborů.</h3>

<a name="device"><b>--device &lt;mechanika&gt;: Výběr mechaniky</b></a><p>

Přednastavení pro Linux je "/dev/cdrom". Pod Windows je předvolena
první mechanika CD/DVD (podle pořadí písmen mechanik).<p>

<b>FreeBSD:</b><br>
<table class="example-bsd" width=100%><tr><td>
user@freebsd&gt; dvdisaster <b>-d /dev/pass0</b> -r
</td></tr></table>
<p></p>

program dvdisaster podporuje mechaniky, které jsou přístupné
prostřednictvím průchozího ovladače <tt>/dev/pass*</tt> a které jsou MMC3 kompatibilní. 
Přístup na mechaniky ATAPI může vyžadovat <a href="download20.html#freebsd">rekompilaci jádra</a>.<p>

<b>Linux:</b><br>
 
<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; dvdisaster <b>-d /dev/hdc</b> -r
</td></tr></table>
<p></p>

program dvdisaster podporuje mechaniky, které jsou přístupné
prostřednictvím jednotného Linuxového ovladače CD-ROM a které jsou MMC3 kompatibilní. 
To jsou zvláště mechaniky ATAPI přiřazené symbolickým zařízením
<tt>/dev/hd*</tt> a mechaniky SCSI používající symbolická zařízení <tt>/dev/scd*</tt>.
Externí mechaniky používající jiné sběrnicové systémy (jako USB, IEEE 1394)
se obvykle také zobrazují jako zařízení SCSI.<p>

Použití modulu <i>ide-scsi</i> pro přístup k mechanikám ATAPI se nedoporučuje,
ale je to možné, protože jsou tato zařízení dostupná pod rozhraním
<tt>/dev/scd*</tt>.<p>

Obecný ovladač SCSI <i>sg</i> (<tt>/dev/sg*</tt>) není podporován.
To by neměl být problém, protože ospovídající mechaniky jsou také dostupné
při použití zařízení <tt>/dev/scd*</tt> nebo <tt>/dev/sr*</tt>.<p>

<b>Windows:</b><br>
<table class="example-win" width=100% bgcolor=#e0e0ff><tr><td>
user@windows&gt; dvdisaster <b>-d E:</b> -r
</td></tr></table>
<p></p>

program dvdisaster podporuje lokální mechaniky implementující
standard MMC3, na které lze odkazovat pomocí písmena disku v systému.


<div align=right><a href="#drive">&uarr;</a></div><p>

<a name="prefix"><b>--prefix &lt;název&gt;: Název pro soubory obrazu a oprav chyb</b></a><p>

Přednastavený název je "medium"; soubor obrazu a soubor oprav chyb automaticky dostanou
přípony ".iso" a ".ecc".

<div align=right><a href="#drive">&uarr;</a></div><p>

<a name="image"><b>--image &lt;název souboru&gt;: Název souboru obrazu</b></a><p>

Přednastavený název je "medium.iso"; zadaný název souboru bude použit pro soubor obrazu
přesně (bez přidání přípony), pokud není zadána také volba <a href="#autosuffix">--auto-suffix</a>.

<div align=right><a href="#drive">&uarr;</a></div><p>

<a name="ecc"><b>--ecc &lt;název souboru&gt;: Název souboru oprav chyb</b></a><p>

Přednastavený název je "medium.ecc"; zadaný název souboru bude použit pro soubor oprav chyb
přesně (bez přidání přípony), pokud není zadána také volba <a href="#autosuffix">--auto-suffix</a>.

<div align=right><a href="#drive">&uarr;</a></div><p>

<a name="list"><b>--list: Zobrazit mechaniky dostupné pod ASPI</b></a><p>

Tato volba zobrazí seznam všech mechanik CD/DVD, které jsou přístupné prostřednictvím
vrstvy <a href="qa10.html#aspi">ASPI</a> ve Windows:<p>

<table class="example-win" width="100%" bgcolor="#e0e0ff"><tr><td>
user@windows&gt; dvdisaster <b>-l</b><br>
dvdisaster-${project_version} Copyright 2004-2006 Carsten Gnoerlich.<br>
[... zbytek oznámení GPL ...]<p>

Seznam ASPI CD/DVD mechanik:<p>
 1: (H:) ATAPI DVD+RW 8X4X12 B2K7<br>
 2: (I:) MYDRIVE CD-R MY-401610X 1.05<p>

K vynucení ASPI místo SPTI odkazujte na mechaniku pomocí<br>
čísel nahoře (použijte 1:, 2:,... místo C:, D:,...)
</td></tr>
</table>
<p></p>

Implicitně se zkouší přístup k mechanice nejprve za použití SPTI
a vrací se k ASPI pouze, pokud předchozí nefunguje.
Použití vrstvy ASPI může být vynuceno zadáním mechaniky
jejím číslem místo jejím písmenem.
K načtení disku s použitím ASPI a jednotky "MYDRIVE" z příkladu nahoře
byste pak zadali:<p>

<table class="example-win" width="100%" bgcolor="#e0e0ff"><tr><td>
user@windows&gt; dvdisaster <b>-d 2:</b> -r<br>
</td></tr>
</table>

<div align=right><a href="#drive">&uarr;</a></div><p>


<h3>Další volby.</h3>

<a name="adaptiveread"><b>--adaptive-read: Použít adaptivní strategii čtení poškozených médií</b></a><p>

Tento přepínač volí <a href="background50.html">adaptivní strategii čtení</a>,
která je zvláště vhodná pro načtení poškozených médií.
Použijte tento přepínač ve spojení s <a href="#read">-r/--read</a> a
<a href="#ecc">-e/--ecc</a>. Zadání souboru oprav chyb způsobí,
že proces čtení je ukončen, jakmile je shromážděno dostatek informací
pro opravu obrazu média.
<p>

<div align=right><a href="#options">&uarr;</a></div><p>



<a name="autosuffix"><b>--auto-suffix: Automaticky doplnit souborové přípony .iso a .ecc</b></a><p>

Pokud je tato volba zadána, k názvům souborů zadaných volbami
<a href="#image">-i/--image</a>- nebo <a href="#ecc">-e/--ecc</a>
bude automaticky přidána přípona ".iso" respektive ".ecc",
pokud dosud není žádná jiná souborová přípona přítomna.

<div align=right><a href="#options">&uarr;</a></div><p>



<a name="cache"><b>--cache-size &lt;size in MB&gt;: Určení velikosti vyrovnávací paměti</b></a><p>

Program dvdisaster optimalizuje přístup k souboru obrazu a oprav chyb
tak, že udržuje svou vlastní vyrovnávací paměť.
Velikost vyrovnávací paměti může být mezi 1 a 2048 MB.
Přednastavení je na 32 MB, tato velikost by měla být vhodná
pro většinu systémů.<p>

Tato volba je účinná pouze v průběhu akce <a href="#create">--create</a>.

<div align=right><a href="#options">&uarr;</a></div><p>



<a name="dao"><b>--dao: Předpokládat médium "disk najednou"</b></a><p>

Média zapsaná v režimu "TAO" ("stopa najednou") mohou obsahovat dva sektory
s <a href="qa20.html#tao">pseudo chybami čtení</a> na konci.
Ve výchozím nastavení jsou tyto dvě chyby programem dvdisaster ignorovány.<p>

Pokud máte skutečně tu smůlu, že máte médium "DAO" ("disk najednou")
s přesně jednou nebo dvěma opravdovými chybami čtení na konci,
použijte volbu "--dao", abyste zpracovali tyto chyby čtení správně.

<div align=right><a href="#options">&uarr;</a></div><p>



<a name="fillunreadable"><b>--fill-unreadable &lt;plnící bajt&gt;: Vyplnit nečitelné sektory daným bajtem</b></a><p>

Program dvdisaster označuje nečitelné sektory speciální vyplňovací sekvencí, která se
může velmi nepravděpodobně objevit na nepoškozeném médiu.<br>
V jiném softwaru pro obnovu dat je běžné, že se vyplňují nečitelné sektory
určitou hodnotou bajtu. Aby byla umožněna interoperabilita s takovými programy,
můžete zadat hodnotu bajtu, kterou používají:<p>

<ul>
<li><b>0xb0 (176 desítkově)</b>: pro kompatibilitu s <i>h2cdimage</i> 
publikováno v "c't", německém periodiku.
</li>
</ul>

Použití vyplňování nulou (0x00, desítkově 0) se <b>důrazně nedoporučuje</b>.
Většina médií obsahuje správné nulou vyplněné sektory. Při pozdějším průchodu
opravou chyb je nelze odlišit od nečitelných sektorů, pokud je použito
vyplňování nulou.

<div align=right><a href="#options">&uarr;</a></div><p>



<a name="jump"><b>-j / --jump &lt;počet sektorů&gt;: Přeskočit sektory po chybě čtení</b></a><p>

Tato volba má lehce odlišné chování v závislosti na použité strategii čtení:
<ul>
<li>když se použije <a href="background40.html#configure">lineární strategie čtení</a>
(výchozí nastavení), daný počet sektorů se po chybě čtení přeskočí.</li>
<li><a href="background50.html#configure">adaptivní strategie čtení</a> 
(volba <a href="#adaptiveread">--adaptive-read</a>) skončí, když již nezůstaly
žádné nečitelné oblasti obrazu, které jsou větší než daná hodnota.</li>
</ul> 

Vyšší hodnoty zkracují dobu zpracování a mechanické opotřebení mechaniky,
ale také ponechávají větší mezery v obrazu při čtení poškozených oblastí.
Počet přeskočených sektorů musí být násobek 16.

<div align=right><a href="#options">&uarr;</a></div><p>



<a name="parse-ecc"><b>--parse-ecc: Použít informace z hlaviček opravných informací</b></a><p>
Program dvdisaster určí velikost obrazu z hlaviček vytvořených
<a href="background30.html">metodou RS02</a>, je-li tato volba použita. 
Tato volba napravuje některé problémy způsobené
<a href="qa20.html#plusrw">hlášením nesprávné délky obrazů</a>
pro média DVD-RW/+RW.<p>

Poznámka: Použijte tuto volbu jen pro načítání obrazů, do kterých byla
přidána opravná data. Jinak bude proces čtení na začátku
podstatně zpožděn.

<div align=right><a href="#options">&uarr;</a></div><p>



<a name="parse-udf"><b>--parse-udf: Použít informace ze souborového systému ISO/UDF</b></a><p>
Program dvdisaster určí velikost obrazu z informací ze souborového systému ISO/UDF,
je-li tato volba zapnuta. Tím se opravují některé problémy způsobené mechanikami
<a href="qa20.html#plusrw">hlásícími nesprávné délky obrazu</a> pro média
DVD-RW/+RW.<p>

Upozornění: Vždy použijte tuto volbu spolu s <a href="#parse-ecc">--parse-ecc</a>
k načítání obrazů, které byly rozšířeny o opravná data pomocí
<a href="background30.html">metody RS02</a>. Jinak nebudou opravná data
načtena.

<div align=right><a href="#options">&uarr;</a></div><p>



<a name="redundancy"><b>-n / --redundancy: Nastavit redundanci pro opravný kód</b></a><p>

Redundance určuje procentní podíl opravitelných chyb
<a href="background10.html" title="Technické vlastnosti opravy chyb">v nejlepším případě</a>.
Vzhledem k tomu, že ideální případy jsou řídké, doporučuje se aplikovat přiměřený rozsah 
redundance. Také vezměte v úvahu následující vlastnosti opravy chyb:<p>

<ul>
<li>Soubor oprav chyb s redundancí x% bude mít přibližně velikost x% velikosti
odpovídajícího obrazu.</li>
<li>Schopnost opravy chyb závisí na statistickém rozmístění chyb čtení. 
Pouze změny o 5 procent a více mohou mít viditelný
efekt na opravu chyb.<p></p></li>
</ul>

Existuje několik způsobů zadání redundance:<p>

<ol>
<li> <b>"normální" nebo "vysoká"</b><p>

Po nastavení hodnot "normální" nebo "vysoká" získáte následující redundance:<p>

<table border="1" cellpadding="3">
<tr><td align=center>Hodnota</td><td>Redundance</td></tr>
<tr><td><tt>-n normal</tt></td><td align=center>14.3%</td></tr>
<tr><td><tt>-n high</tt></td><td align=center>33.5%</td></tr>
</table>
<p></p>

Tyto hodnoty spouští optimalizovaný programový kód ke zrychlení
tvorby souboru oprav chyb. Výchozí hodnota je "normální".<p>
</li>

<li> <b>Procentní hodnoty</b><p>

Je také možné zadání redundance v procentech:<p>

<table border="1" cellpadding="3">
<tr><td>Příklad</td><td>Platný rozsah</td></tr>
<tr><td><tt>-n 25%</tt></td><td align=center>3.2% až 64.5%</td></tr>
</table>
<p></p>

Z technických důvodů se může aktuální redundance odchýlit o několik
desetin od zadané hodnoty.<p>
</li>

<li> <b>Opravitelné chyby na blok ECC</b><p>

Pokud není zadána žádná jednotka redundance, je hodnota interpretována
k vytvoření kódu (255, 255-x)-Reed-Solomon:<p>

<table border="1" cellpadding="3">
<tr><td>Příklad</td><td>Platný rozsah</td></tr>
<tr><td><tt>-n 32</tt></td><td align=center>8 až 100</td></tr>
</table>
<p></p>

Tak se kombinuje 255-x sektorů média do jednoho bloku ECC. V rámci tohoto bloku
lze opravit až x nečitelných sektorů.<p>

Mimochodem: &nbsp; &nbsp; Redundance v procentech = (100 * x) / (255 - x)<p>
</li>

<li> <b>Maximální velikost souboru oprav chyb</b><p>

Dalším způsobem zadání redundance je nastavit
velikost souboru oprav chyb v MB:<p>

<table border="1" cellpadding="3">
<tr><td>Příklad</td><td>Platný rozsah</td></tr>
<tr><td><tt>-n 50m</tt></td><td align=center>3.2% až 64.5% velikosti obrazu</td></tr>
</table>
<p></p>

V tomto případě program dvdisaster vybere vhodné nastavení redundance, aby
celková velikost souboru oprav chyb nepřekročila daný limit.<p>

Poznámka pro pokročilé: Použijeme-li stejné nastavení velikosti pro obrazy
velmi odlišných velikostí, je více informací pro opravu chyb přiděleno menším obrazům
a méně větším.<p>
</li>
</ol>

<div align=right><a href="#options">&uarr;</a></div><p>


<a name="method"><b>-m / --method &lt;m&gt;: Zvolit metodu opravy chyb</b></a><p>

Zvolí mezi metodami
<a href="background30.html">RS01</a> (výchozí) 
a <a href="background30.html">RS02</a>.

<div align=right><a href="#options">&uarr;</a></div><p>



<a name="speedwarn"><b>--speed-warning [n]: Varovat při určité hodnotě poklesu rychlosti čtení</b></a><p>

Zobrazí varování, změní-li se rychlost čtení o více než <i>n</i> procent.
To je užitečné pro zjištění počínajícího zhoršování kvality média.
Pokud je volitelná hodnota [n] vynechána, je použito výchozí nastavení 10 procent.

<div align=right><a href="#options">&uarr;</a></div><p>


<a name="spinup"><b>--spinup-delay [n]: Poskytnout mechanice čas na roztočení</b></a><p>

Počká zadaný počet sekund na roztočení disku v mechanice, než odstartuje skutečný
proces čtení.
Zabraňuje to falešným varováním o změnách rychlosti na začátku média,
je-li použita volba <a href="#speedwarn">--speed-warning</a>.<br>

Pokud není zadána volitelná hodnota [n], program dvdisaster bude čekat 5 sekund.

<div align=right><a href="#options">&uarr;</a></div><p>

<a name="split"><b>--split-files: Rozdělit soubory do segmentů <= 2 GB</b></a><p>

Umožňuje tvorbu velkých obrazů a souborů oprav chyb na souborových
systémech s omezením na 2 GB na soubor (t.j. FAT z Windows).
Soubory jsou rozprostřeny až do 100 segmentů "medium00.iso", "medium01.iso" atd.
za cenu mírného snížení výkonu.<p>

Název souboru musí být stále zadán jako
"medium.iso" s použitím volby <a href="#image">--image</a>;
číslování bude automaticky vloženo, je-li tento přepínač aktivní.<p>

<div align=right><a href="#options">&uarr;</a></div><p>

EOF
}

# ----- Downloads

function download_contents_cs()
{  local file="$1.html"
   local query=$2
   local page=$3
   local lang=$4

   case $query in
   title)   title="Stažení" ;;

   link)    link_title="Stažení" ;;
   link10)   link_title="Systémové požadavky" ;;
   link20)   link_title="Instalace" ;;

   content*) eval "download$page$lang $file" ;;
   esac 
}


function download0cs()
{  cat >> $1 <<EOF
<h3>Stažení programu dvdisaster</h3>

Program dvdisaster je dostupný pro <a href="download10.html">aktuální verze</a>
operačních systémů FreeBSD, Linux a Windows.<p>

Stáhněte si prosím buď zdrojový kód nebo binární verzi ze seznamu níže.
Je poskytnut <a href="#signature">digitální podpis</a>, kterým lze ověřit, že
pakety jsou ve svém původním stavu.<p>

Binární verzi nainstalujete spuštěním staženého programu a následováním dialogu.
U verze ve zdrojovém kódu si přečtěte <a href="download20.html">instalační instrukce</a>.

<pre> </pre>

<b>Vývojové verze</b> - nové a experimentální pro zkušené uživatele!<p>


<table width="100%" $IDXCOLOR cellpadding="0" cellspacing="5">
<tr><td><b>dvdisaster-0.70</b></td><td align="right">09-Jul-2006</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
  <table>
    <tr><td align="right">&nbsp;&nbsp;Source code for all operating systems:&nbsp;</td>
        <td><a href="${tar_0_70}">dvdisaster-0.70.tar.bz2</a></td></tr>
    <tr><td align="right">Digital signature:&nbsp;</td>
        <td><a href="${tar_0_70_sig}">dvdisaster-0.70.tar.bz2.gpg</a></td></tr>
    <tr><td align="right">Binary for Windows:&nbsp;</td>
        <td><a href="${setup_0_70}">dvdisaster-0.70-setup.exe</a></td></tr>
    <tr><td align="right">Digital signature:&nbsp;</td>
        <td><a href="${setup_0_70_sig}">dvdisaster-0.70-setup.exe.gpg</a></td></tr>
  </table>
</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
The <a href="background30.html">RS02 error correction method</a>
is fully supported in the graphical user interface. Images created
with RS02 can be used 
with the <a href="background50.html">adaptive reading strategy</a>.<p>

Julian Einwag started porting dvdisaster 
to <a href="../en/download20.html#darwin">Mac OS X / Darwin</a>.<p>

Daniel Nylander provided Swedish translations of the screen texts.
</td></tr></table><p>
<pre> </pre>

<table width="100%" $IDXCOLOR cellpadding="0" cellspacing="5">
<tr><td><b>dvdisaster-0.66</b></td><td align="right">25.03.2006</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
  <table>
    <tr><td align="right">&nbsp;&nbsp;Zdrojový kód pro všechny operační systémy:&nbsp;</td>
        <td><a href="${tar_0_66}">dvdisaster-0.66.tar.bz2</a></td></tr>
    <tr><td align="right">Digitální podpis:&nbsp;</td>
        <td><a href="${tar_0_66_sig}">dvdisaster-0.66.tar.bz2.gpg</a></td></tr>
    <tr><td align="right">Binární verze pro Windows:&nbsp;</td>
        <td><a href="${setup_0_66}">dvdisaster-0.66.3-setup.exe</a></td></tr>
    <tr><td align="right">Digitální podpis:&nbsp;</td>
        <td><a href="${setup_0_66_sig}">dvdisaster-0.66.3-setup.exe.gpg</a></td></tr>
  </table>
</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
Obrazy lze nyní porovnat se soubory oprav chyb v průběhu
funkce <a href="example10.html">Skenovat</a>. Ve vytváření souborů lze
pokračovat po zjištění a odstranění nedostatku místa na pevném disku.<p>

<i>Nová <a href="background30.html">metoda opravy chyb</a> bez
souborů oprav chyb:</i>
Informace oprav chyb lze přidat přímo do obrazu. Tato metoda je
ještě experimentální a v současnosti je dostupná pouze na příkazové
řádce. Navštivte prosím
the <a href="http://sourceforge.net/cvs/?group_id=157550">Archiv CVS</a>
a sledujte vývoj nové metody (vyžaduje určité zkušenosti se sestavováním
programů ze zdroje).<p>

<b>Záplaty</b> (malé změny proti verzi 0.66; soubory nahoře byly aktualizovány):<br>

<b>pl3</b> fixes incomplete installation of the user manual under some versions
of Windows. Other operating systems are not affected.
</td></tr></table><p>

<pre> </pre>

<table width="100%" $IDXCOLOR cellpadding="0" cellspacing="5">
<tr><td><b>dvdisaster-0.65</b></td><td align="right">14.01.2006</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
  <table>
    <tr><td align="right">&nbsp;&nbsp;Zdrojový kód pro všechny operační systémy:&nbsp;</td>
        <td><a href="${tar_0_65}">dvdisaster-0.65.tar.bz2</a></td></tr>
    <tr><td align="right">Digitální podpis:&nbsp;</td>
        <td><a href="${tar_0_65_sig}">dvdisaster-0.65.tar.bz2.gpg</a></td></tr>
    <tr><td align="right">Binární verze pro Windows:&nbsp;</td>
        <td><a href="${setup_0_65}">dvdisaster-0.65-setup.exe</a></td></tr>
    <tr><td align="right">Digitální podpis:&nbsp;</td>
        <td><a href="${setup_0_65_sig}">dvdisaster-0.65-setup.exe.gpg</a></td></tr>
  </table>
</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
<i>Všechny operační systémy:</i>
<ul>
<li>Přeskočí skenování obrazu v průběhu tvorby souboru oprav chyb,<br>
pokud je tvorba vyvolána bezprostředně po čtení obrazu.</li>
<li>Soubory oprav chyb mohou být rozděleny do 2 GB segmentů.</li>
<li>Český překlad od Luboše Staňka (programová hlášení i dokumentace).</li>
</ul>
<i>FreeBSD:</i>
<ul>
<li>Začátek podpory v této verzi.</li>
</ul>
<i>Windows:</i>
<ul>
<li>Plná podpora pro souborové systémy FAT32 (viz výše).</li>
<li>Binární verze je nyní distribuována s knihovnami Gtk+2.6.</li>
</ul>

</td></tr></table>
<p></p>

<pre> </pre>

<b>Stabilní verze</b> - doporučeno pro začátek.<p>

<table width="100%" $IDXCOLOR cellpadding="0" cellspacing="5">
<tr><td><b>dvdisaster-0.64 (pl2)</b></td><td align="right">01.11.2005</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
  <table>
    <tr><td align="right">&nbsp;&nbsp;Zdrojový kód (Linux/Windows):&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.64.2.tar.bz2">dvdisaster-0.64.2.tar.bz2</a></td></tr>
    <tr><td align="right">Digitální podpis:&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.64.2.tar.bz.gpg">dvdisaster-0.64.2.tar.bz.gpg</a></td></tr>
    <tr><td align="right">Binární verze pro Windows:&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.64.2-setup.exe">dvdisaster-0.64.2-setup.exe</a></td></tr>
    <tr><td align="right">Digitální podpis:&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.64.2-setup.exe.gpg">dvdisaster-0.64.2-setup.exe.gpg</a></td></tr>
  </table>
</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
Tato verze je schopna určení
<a href="example81.html#iso">velikosti obrazu podle souborového systému UDF/ISO</a>,
aby se zlepšila
<a href="qa20.html#plusrw">detekce velikosti obrazu pro média -RW/+RW</a>.
Načítání obrazů a vytváření odpovídajících souborů oprav chyb může být nyní vyvoláno
společně <a href="example81.html#auto">jedním kliknutím myši</a>.
Několik drobných vylepšení bylo vytvořeno pro podporu více mechanik CD/DVD,
uhlazení stávajících funkcí a k odstranění některých drobných chyb ;-)<p>
<b>Záplaty</b> (malé změny proti verzi 0.64; soubory nahoře byly aktualizovány):<br>
<b>pl1</b> Stabilizuje pád bezprostředně po spuštění programu, je-li linkován proti gtk+-2.6;
dotčena je pouze verze pro Linux.<br>
<b>pl2</b> Opravuje volbu příkazové řádky -u a vytváří souborová oprávnění pro obrazy
za použití Unixových konvencí pod Linuxem.

</td></tr></table>
<p></p>

<table width="100%" $IDXCOLOR cellpadding="0" cellspacing="5">
<tr><td><b>dvdisaster-0.63</b></td><td align="right">01.08.2005</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
  <table>
    <tr><td align="right">&nbsp;&nbsp;Zdrojový kód (Linux/Windows):&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.63.tbz">dvdisaster-0.63.tbz</a></td></tr>
    <tr><td align="right">Digitální podpis:&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.63.tbz.gpg">dvdisaster-0.63.tbz.gpg</a></td></tr>
    <tr><td align="right">Binární verze pro Windows:&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.63-setup.exe">dvdisaster-0.63-setup.exe</a></td></tr>
    <tr><td align="right">Digitální podpis:&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.63-setup.exe.gpg">dvdisaster-0.63-setup.exe.gpg</a></td></tr>
  </table>
</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
Obsahuje <a href="example30.html">vylepšenou strategii</a> pro
<a href="example80.html#adaptiveread">čtení poškozených médií</a>,
která byla motivována článkem ve vydání 16/2005 německého periodika c't.
</td></tr></table>
<p></p>

<b>Historické verze</b><p>

<table width="100%" $IDXCOLOR cellpadding="0" cellspacing="5">
<tr><td><b>dvdisaster-0.55</b></td><td align="right">20.02.2005</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
  <table>
    <tr><td align="right">&nbsp;&nbsp;Zdrojový kód (Linux/Windows):&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.55.tgz">dvdisaster-0.55.tgz</a></td></tr>
    <tr><td align="right">Digitální podpis:&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.55.tgz.gpg">dvdisaster-0.55.tgz.gpg</a></td></tr>
    <tr><td align="right">Binární verze pro Windows:&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.55.zip">dvdisaster-0.55.zip</a></td></tr>
    <tr><td align="right">Digitální podpis:&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.55.zip.gpg">dvdisaster-0.55.zip.gpg</a></td></tr>
  </table>
</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
Toto je poslední verze "jen pro příkazovou řádku". 
</td></tr></table>
<p></p>

<a name="signature"><b>Co je to digitální podpis?</b></a><p>


Balíky zdrojového kódu ke stažení byly digitálně podepsány s použitím
<a href="http://www.gnupg.org/gnupg.html">GnuPG</a>, takže můžete ověřit,
že software je ve své původní podobě.<p>

Kontrolu autenticity <a href="../pubkey.asc">veřejného klíče</a>
lze nejlépe provést porovnáním jeho "otisku" s tím, který je připojen
v zápatí mých poštovních zpráv. Bez obav mi pošlete email s předmětem
"GPG finger print", pokud to bude potřeba. 
EOF
}


function download10cs()
{  cat >> $1 <<EOF
<h3>Systémové požadavky</h3>

<ul>
 <li><b>x86 kompatibilní</b> hardware v <b>32 bitovém</b> režimu
     (64 bitová podpora viz Operační systémy),<p></p></li>
 <li>současné mechaniky CD nebo DVD s rozhraním ATAPI nebo SCSI<p></p></li>
 <li>minimálně P4 na 2 GHz nebo srovnatelný procesor<p></li>
</ul>

<h3>Operační systémy</h3>
<ul>
 <li><b>FreeBSD</b> verze <b>6.0</b> nebo novější<br>
     (použití ATAPI mechanik vyžaduje rekompilaci jádra)<p>
 </li>
 <li><b>Linux</b> s jádrem <b>2.6.7</b> nebo novějším<br>
     (použití jader >= 2.4.20 je možné, ale nedoporučuje se!)<p>
 </li>
 <li><b>Windows</b> <b>2000</b> nebo <b>XP</b><br>
     (Windows 98 a ME jsou možná, ale nedoporučují se kvůli ovládání DVD)
 </li>
</ul>
EOF
}


function download20cs()
{  cat >> $1 <<EOF
<h3>Instalace distribuce ve zdrojovém kódu</h3>

<b>Vyžadované nástroje a knihovny</b><p>

Program dvdisaster vyžaduje obvyklé vývojové nástroje <a href="http://www.gnu.org/">GNU</a>
pro kompilaci programů v C. Distribuce FreeBSD a Linuxu jsou typicky vybaveny vhodnými
balíky na svých instalačních médiích.<p>
Pro Windows je dostupný odpovídající rámec nástrojů
<a href="http://www.mingw.org">Mingw</a>.
Sestavení pod Cygwin není podporováno.<p>

V závislosti na vašem operačním systému budete také potřebovat:

<ul>
<li>Knihovnu <a href="http://www.gnu.org/software/gettext/">gettext</a></li>
<li>Knihovnu <a href="http://www.gnu.org/software/libiconv/">iconv</a> (jen pro Windows)</li>
<li>Nástroj <a href="http://www.freedesktop.org/software/pkgconfig/releases">pkgconfig</a></li>
<li>Knihovnu <a href="http://www.gtk.org">GTK+</a> verze 2.2 nebo lepší včetně všech pomocných knihoven</li>
</ul>

Ty by také měly být zahrnuty na distribučních médiích FreeBSD a Linuxu.
Projekt <a href="http://www.gtk.org">Gtk+</a> nabízí předkompilované
<a href="http://www.gimp.org/~tml/gimp/win32/downloads.html">verze pro Windows</a>.
Potřebujete jak normální, tak i "-devel" verze knihoven.<p>

<a name="freebsd"></a>
<b>Tipy pro FreeBSD</b><p>

FreeBSD nepředinstalovává jednotný ovladač CD-ROM pro SCSI a ATAPI mechaniky
(jako je to v případě Linuxu a Windows). Proto program dvdisaster nemůže
použít žádné mechaniky ATAPI v distribuční instalaci FreeBSD.
Pro instalaci vhodného ovladače prosím překompilujte jádro s dalším zařízením<p> 

<tt>device atapicam</tt><p>

Zařízení <i>ata</i>, <i>scbus</i>, <i>cd</i>
a <i>pass</i> jsou také vyžadována, ale ta jsou již implicitně zahrnuta ve FreeBSD 6.0.
Kompilace zdrojů programu dvdisaster vyžaduje GNU make (gmake) a shell bash.<p>

<b>Kompilace zdrojového kódu</b><p>

Otevřete si prosím okno s příkazovým řádkem a pokračujte následujícími kroky.<p>

Rozbalte archiv zdrojového kódu:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>tar xjf $project_package.tar.bz2</b><br>
</td></tr></table>
<p></p>

Přejděte do nově vytvořeného adresáře:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>cd $project_package</b><br>
</td></tr></table>
<p></p>

Spusťte konfigurační skript:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>bash configure</b><br>
<i>[... spousta výstupu z konfiguračního skriptu ...]</i>
</td></tr></table>
<p></p>

Skript možná nabídne nějaké tipy ohledně chybějících knihoven.
Zadejte "bash configure --help" pro zjištění, jak správně nasměrovat
konfigurační skript k nainstalovaným knihovnám.<p>

Když skončí konfigurační skript bez chybových hlášení,
zkompilujte zdrojový kód do spustitelného programu:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>gmake</b><br>
<i>[... spousta výstupu z procesu kompilace ...]</i>
</td></tr></table>
<p></p>

Spusťte program na krátký test:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>./dvdisaster</b><br>
<i>[... za okamžik se otevře hlavní okno ...]</i>
</td></tr></table>
<p></p>

Práce s programem v aktuálním adresáři je v pořádku.
V podadresáři <i>documentation/en</i> je zahrnuta kopie
domácí stránky programu dvdisaster.<p>

Také je možné nainstalovat program a dokumentaci
permanentně na váš systém:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>gmake install</b><br>
<i>[... výstup instalačního procesu ...]</i>
</td></tr></table>
<p></p>

Před provedením instalace prosím zkontrolujte výstup skriptu "<tt>configure</tt>"
nebo zadejte "<tt>gmake show</tt>" ke zjištění cílových adresářů instalace.<p>
Zadejte "<tt>bash configure --help</tt>" k získání informací o změně
instalačních cest.

<pre> </pre>

<font size="+1">Užijte si používání programu dvdisaster!</font>
EOF
}

# ----- Questions and Answers

function qa_contents_cs()
{  local file="$1.html"
   local query=$2
   local page=$3
   local lang=$4

   case $query in
   title)   title="Otázky a odpovědi" ;;

   link)    link_title="Otázky a odpovědi" ;;
   link10)   link_title="Technické otázky" ;;
   link20)   link_title="Chybová hlášení" ;;

   content*) eval "qa$page$lang $file" ;;
   esac 
}


function qa0cs()
{  cat >> $1 <<EOF
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
<a href="background10.html" title="Technické vlastnosti opravy chyb">použita v programu dvdisaster</a>.
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

EOF
}


function qa10cs()
{  cat >> $1 <<EOF
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
<li>Mechanika musí být schopna <a href="qa20.html#dvdrom">identifikovat typ média</a>.</li>
</ul>

<b>DVD-RW, DVD+RW</b><p>

<ul>
<li>Některé mechaniky hlásí chybné <a href="qa20.html#plusrw">velikosti obrazů</a>.<br>
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

EOF
}


function qa20cs()
{  cat >> $1 <<EOF
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

EOF
}

# ----- Background information

function background_contents_cs()
{  local file="$1.html"
   local query=$2
   local page=$3
   local lang=$4

   case $query in
   title)   title="Základní informace" ;;
 
   link)    link_title="Základní informace" ;;
   link10)   link_title="Vlastnosti opravy chyb" ;;
   link20)   link_title="Oprava dat na úrovni obrazu" ;;
   link30)   link_title="RS01 a RS02" ;;
   link40)   link_title="Lineární strategie čtení" ;;
   link50)   link_title="Adaptivní strategie čtení" ;;
   link60)   link_title="Poznámky k chybám čtení" ;;
   link70)   link_title="Tipy pro ukládání souborů oprav chyb" ;;

   content*) eval "background$page$lang $file"
	    return 0
	    ;;
   esac 
}


function background0cs()
{  cat >> $1 <<EOF

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

EOF
}


function background10cs()
{  cat >> $1 <<EOF
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

EOF
}

function background20cs()
{  cat >> $1 <<EOF
<h3>Oprava dat na úrovni obrazu</h3>

Obnova média pomocí opravných dat probíhá ve dvou krocích:

<ol>
<li>Nejprve se načte co nejvíce dat z poškozeného média.<p></p></li>
<li>Pak se dosud chybějící data obnoví s pomocí kódu pro opravu chyb.</li>
</ol>

Množství čitelných dat (krok 1) nezávisí jenom na čtecích schopnostech
mechaniky, ale také na jaké logické úrovni se proces čtení provádí.
Tato stránka pojednává o logických úrovních a vysvětluje, proč program
dvdisaster používá čtení na úrovni obrazu.<p>

<b>Logické úrovně média</b><p>

CD a DVD média jsou organizována v <i>datových sektorech</i> obsahujících po 2048 bajtech.
Posloupné čtení a ukládání těchto sektorů vytvoří <i>obraz</i> média.<p>

Ale práce s jednotlivými sektory je z hlediska uživatele nepraktická.
Proto média obsahují <i>souborové systémy</i>, které kombinují datové
sektory do <i>souborů</i>. To vyžaduje přesnou evidenci, ze kterých
datových sektorů jsou soubory složeny a dalších atributů, jako jsou
názvy souborů a přístupová oprávnění. Pro tuto evidenci jsou na médiu
některé datové sektory rezervovány a vyplněny odpovídajícími datovými
strukturami.<p>

Ve výsledku média obsahují různé <i>logické úrovně</i>:
Pohled na médium jako posloupnost datových sektorů znamená práci na úrovni
<i>obrazu</i>. Avšak pohled na médium jako kolekci souborů je perspektiva
<i>úrovně souborů (souborového systému)</i>.<p>

Tyto dvě úrovně mají různé vlastnosti ohledně obnovy dat:<p>

<a name="file"> </a>
<b>Nedostatky při čtení média na úrovni souborů</b><p>

Čtení poškozeného média na <b>souborové úrovni</b> znamená pokus
o načtení co možná nejvíce dat z každého souboru.<p>

Problém však nastane, když jsou poškozeny sektory, které mají v souborovém
systému evidenční funkci. Seznam souborů na médiu může být zkrácen.
Nebo není organizace datových sektorů na soubory kompletní. Proto mohou
být soubory nebo jejich části ztraceny, i když by byly odpovídající datové
sektory hardwarově čitelné. To je velmi špatné, protože dokonce malé
čitelné části poškozených souborů jsou pro kód oprav chyb cenné.<p>

Extrémně ošklivý případ nastane, když jsou data oprav chyb také uložena
v souborech. Pak jsou vyžadována data oprav chyb k opravě souborového
systému, ale poškozený souborový systém brání přístupu k datům oprav chyb.
Znamená to úplnou ztrátu dat a vyzvihuje některé sporné otázky
o <a href="#eccfile">nakládání se soubory oprav chyb</a>
(o tom více později).<p>

Avšak situace se značně zlepší při použití přístupu na základě obrazu:<p>

<a name="image"> </a>
<b>Výhody čtení na úrovni obrazu</b><p>

Čtení na úrovni obrazu používá přímou komunikaci s hardwarem mechaniky
pro přístup k datovým sektorům.<p>

Počet čitelných sektorů závisí jenom na čtecích schopnostech mechaniky,
ale nezávisí na stavu souborového systému. Chyba čtení v jednom sektoru
neblokuje přístup k dalším datovým sektorům. Protože jsou obnoveny
<i>všechny</i> sektory, které jsou dosud čitelné hardwarem, poskytuje
tato metoda nejlepší základ pro opravu chyb.<p>

Obraz obsahuje všechny datové sektory média. Je-li obraz úplně obnoven,
souborový systém na něm uložený je také zcela opraven. Ochrana na úrovni
obrazu je proto mnohem širší, než oprava chyb na úrovni souborů.<p>

Program dvdisaster pracuje výhradně na úrovni obrazu, aby využil výhod
těchto vlastností. Nová <a href="background30.html">metoda RS02</a>
dokonce umožňuje uložení dat oprav chyb na stejné médium. To je možné,
protože čtení informací o opravě chyb na úrovni obrazu nemůže být
blokováno chybami na jiných místech média (poškozené sektory v opravných
datech zmenší kapacitu oprav chyb, ale neučiní obnovu nemožnou).<p>

<a href="background30.html">Metoda RS01</a> chrání média na úrovni obrazu
také, ale ukládá data oprav chyb do souborů. Následující sekce naznačuje
některá z toho plynoucí úskalí.<p>

<a name="eccfile"> </a>
<b>Důsledky pro ukládání souboru oprav chyb</b><p>

Opravná data, která vytváří program dvdisaster, chrání média na úrovni
obrazu. Jak jsou ale chráněny <i>soubory</i> oprav chyb?<p>
 
Vzhledem k tomu, že soubory oprav chyb jsou čteny na souborové úrovni,
jsou předmětem problémů zmíněných výše. Pokud se médium obsahující soubory
oprav chyb poškodí, nemusí být možné je zpřístupnit nebo načíst úplně.
<p>

<table width=100%><tr><td bgcolor=#000000 width=2><img width=1 height=1 alt=""></td><td>
Z toho důvodu je důležité chránit soubory oprav chyb na úrovni obrazu také:
<a href="background70.html">Média obsahující soubory oprav chyb</a> musí být
rovněž chráněna programem dvdisaster.
</td></tr></table><p>

Protože se předpokládá ochrana na úrovni obrazu, soubory oprav chyb
neobsahují <i>žádnou další ochranu proti poškození</i>! To by beztak
moc nepomohlo: Soubory oprav chyb by mohly být vytvářeny způsobem, který
by jim umožnil poskytovat omezenou kapacitu opravy chyb i v případě
jejich poškození. Ale i kdyby byla takováto vnitřní ochrana navržena,
soubory oprav chyb by byly stále chráněny jen na úrovni souborového
systému se všemi jeho nevýhodami prodiskutovanými výše!<p>

Navíc, výpočetní čas a redundanci použitou pro vnitřní ochranu je lépe
spotřebovat na úrovni obrazu: Oprava chyb Reed-Solomon pracuje nejlépe,
když se opravné informace rozprostírají přes velká množství dat.
Je lepší chránit obraz média jako celek, než chránit jednotlivé soubory
v jeho rámci.<p></li>

EOF
}

function background30cs()
{  cat >> $1 <<EOF
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

EOF
}

function background40cs()
{  cat >> $1 <<EOF
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
EOF
}


function background50cs()
{  cat >> $1 <<EOF
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
EOF
}


function background60cs()
{  cat >> $1 <<EOF
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
EOF
}


function background70cs()
{  cat >> $1 <<EOF
<h3>Tipy pro ukládání souborů oprav chyb</h3>

V současné dob� existuje málo technologií pro výměnná média,
které mohou být cenově efektivní alternativou pro různé formáty CD/DVD.
Z toho důvodu budete pravděpodobně používat CD/DVD nejen pro archivaci,
ale budete na CD/DVD také ukládat odpovídající soubory oprav chyb.<p>

Na tom není nic špatného, ale mějte na paměti, že jsou vaše archivovaná
data a soubory oprav chyb uloženy na médiích se stejnou úrovní
spolehlivosti. Jakmile se chyby čtení objeví na archivovaných datech,
buďte připraveni na to, že disk s odpovídajícím souborem oprav chyb
mohl degradovat pod hranici plné čitelnosti také.<p>

Proto je důležité chránit vaše soubory oprav chyb se stejnou péčí,
jako vaše data. Toho nejlépe dosáhnete začleněním souborů oprav chyb
do vašeho normálního schématu zálohování dat. Zde jsou dva návrhy:<p>

<b>1. Ukládání souborů oprav chyb na vyhrazená média:</b><p>

Pokud se rozhodnete ukládat soubory oprav chyb na samostatná média,
je <a href="background20.html#eccfile">důležité</a> chránit tato média
pomocí programu dvdisaster také. Abyste se vyhnuli nikdy nekončícímu
cyklu (soubory oprav chyb pro média souborů oprav chyb pro...),
zkuste tohle:<p>

Předpokládejme, že na každé médium může být uloženo pět souborů oprav chyb.
Vypalte prvních pět souborů oprav chyb na první médium a vytvořte další
soubor oprav chyb pro toto médium. Nyní uložte tento soubor oprav chyb
spolu s dalšími čtyřmi soubory oprav chyb na druhé médium.
Pokud budete tímto způsobem pokračovat, budou všechny soubory oprav chyb
s výjimkou toho z posledního média a dalších médií (které lze ještě
udržovat na pevném disku) chráněny programem dvdisaster.<p>

<b>2. Umístění souboru oprav chyb na následující médium řady:</b><p>

Pokud nenaplníte vaše DVD na maximum (t.j. méně než 4 GB pro jednovrstvé médium),
můžete uložit soubor oprav chyb jednoho média na následující médium z řady.<p>

EOF
}

# ----- Impressum

function imprint_contents_cs()
{  local file="$1.html"
   local query=$2
   local page=$3
   local lang=$4

   case $query in
   title)   title="Tiráž" ;;
 
   link)    link_title="Tiráž" ;;

   content*) eval "imprint$page$lang $file"
	    return 0
	    ;;
   esac 
}


function imprint0cs()
{  cat >> $1 <<EOF

   <h3>Tiráž</h3>

   Carsten Gnörlich <br>
   Im Hagen 10 <br>
   59514 Welver / Germany<br>
   Tel.: +49 2384 941174<pre> </pre>
   E-Mail: carsten@dvdisaster.de<br>
   Internet: www.dvdisaster.de

   <pre> </pre>

   Odpovědný za obsah webových stránek podle § 10 MDStV: <p>
   Carsten Gnörlich (viz výše uvedená adresa).

   <pre> </pre>

   Tato tiráž zahrnuje webové stránky pod<br>
   <a href="http://www.dvdisaster.com">www.dvdisaster.com</a>, &nbsp;
   <a href="http://www.dvdisaster.de">www.dvdisaster.de</a>, &nbsp;
   <a href="http://www.dvdisaster.org">www.dvdisaster.org</a><br>
   a v subdoméně <a href="http://dvdisaster.berlios.de">dvdisaster.berlios.de</a>.

EOF
}

