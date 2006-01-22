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

   trans_version="verze"

   trans_to_internet="Na Internetovou verzi"
   trans_to_hoster="Přehled (na BerliOS)"

   trans_back="Zpět"

   trans_contents="Obsah"
   trans_hosting="Hostováno pod"

   trans_fdl="Doslovné kopírování a distribuce celého tohoto článku jsou pro jakékoli médium povoleny za předpokladu, že tato poznámka zůstane zachována."
   trans_copyright="Autorská práva 2004-2006 Carsten Gnörlich, k překladu 2006 Luboš Staněk."
}

# ----- These are the "inlined" html pages.

function inline_contents_cs()
{  title="Stránka s obrázky"
}

function inline1cs()
{  cat >> $1 <<EOF

<tr align="center"><td><h3>Obrázek: Silně poškrábané médium</h3><p></td></tr>
<tr align="center"><td>
<center><img src="../images/scratch-img.jpg"></center>
<br clear="all">
</td></tr>
<tr><td>
Toto médium bylo přenášeno bez ochranného obalu;
kruhové stopy poškrábání byly vytvořeny lahví vody. Škrábance jsou
nadměrně zvýrazněny díky odrazům fotografického blesku.</tr></td>
EOF
}


function inline2cs()
{  cat >> $1 <<EOF

<tr align="center"><td><h3>Obrázky: Analýza chyb</h3><p></td></tr>
<tr align="center"><td>
<center><img src="../images/scratch-scan-cs.png"></center>
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
</tr></td>
<tr align="center"><td>
<center><img src="../images/scratch-corr-cs.png"></center>
<br clear="all">
</td></tr>
<tr><td>
Tento graf zobrazuje, jak velké množství chyb na blok oprav bylo
skutečně opraveno, a podává tak odhad pracovního zatížení při
<a href="background10.html" title="Technické vlastnosti opravy chyb">opravě chyb</a> programu dvdisaster.

Graf chyb <i>neodpovídá</i> prostorovému rozmístění chyb čtení na disku,
protože metoda opravy chyb je úmyslně postavena tak, aby dosáhla
rovnoměrného rozložení chyb čtení média ve všech blocích oprav chyb.

</p>Při použití <a href="example93.html#redundancy">standardního
nastavení</a> lze kompenzovat až 32 chyb čtení na blok oprav chyb
(zelená čára). Překročení této čáry znamená, že obsah média nelze obnovit celý.
</tr></td>
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
chyb čtení, které nejsou opravitelné mechanikou CD/DVD.<p></li>
<li>dvdisaster se pokouší načíst co možná nejvíce dat z poškozeného média.
Později jsou nečitelné sektory obnoveny s pomocí dříve vytvořeného
kódu pro opravu chyb. Maximální kapacita opravy chyb je uživatelem volitelná.
</li>
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
V průběhu <a href="example50.html">opravy</a> obrazu média
(s použitím <a href="example93.html#redundancy">standardního nastavení</a>)
dosáhne počet chyb maximální hodnoty 22 chyb na <a href="background10.html" title="Technické vlastnosti opravy chyb">blok ECC</a>,
což představuje 69% využití opravy chyb při použitém nastavení.

<p>
</td><td width=110>
<a href="inline1.html">
<img src="../images/scratch-img.jpg" width=100 height=97></a>
<br clear="all">
<a href="inline2.html">
<img src="../images/scratch-scan-cs.png" width=100 height=74></a>
<br clear="all">
<a href="inline2.html">
<img src="../images/scratch-corr-cs.png" width=100 height=74></a>
</td></tr></table>

<a href="index20.html">Proč není sken PI/PO dostatečný...</a>
EOF
}


function index20cs()
{  cat >> $1 <<EOF
<h3>Program dvdisaster doplňuje PI/PO skeny</h3>

PI/PO skeny jsou cenným nástrojem pro měření a optimalizaci
kvality vypálených médií.<p>

Přesto <b>nejsou</b> PI/PO skeny spolehlivým prostředkem <b>předpovědi
životnosti</b> DVD média!

<ul>
<li> PI/PO skeny nepodávají žádnou informaci o tom, kolik dalších
škrábanců nebo týdnů stárnutí médium ještě vydrží, než začne vykazovat
první nečitelný sektor.<p></li>

<li>Kopírování média jen kvůli špatným hodnotám PI/PO se často
ukazuje jako předčasné - někdy takováto média zůstanou čitelná
mnohem déle, než se očekávalo.<p></li>

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
(v rámci <a href="background10.html" title="Technické vlastnosti opravy chyb">jistých hranic</a>).<p></li>
<li>Testy chyb čtení běží <b>rychleji</b>, než probíhají skeny PI/PO;<br>
při rychlosti až 16x podle použité mechaniky DVD.<p></li>
<li><b>Efektivita nákladů:</b> Média se musí nahradit novou kopií jen v případě,
že jsou skutečně poškozená.</li>
</ul>

<h3>Omezení při používání programu dvdisaster:</h3>
<ul>
<li>Soubor oprav chyb <b>musí být vytvořen dříve, než médium selže</b>.<p></li>
<li>Soubory oprav chyb vyžadují <b>další úložný prostor</b> a je nutné
je uchovávat na spolehlivém médiu.
Při použití <a href="example93.html#redundancy">standardního nastavení</a>
dosahuje dodatečný úložný prostor velikosti 15% původního datového rozsahu
(přibližně 700 MB pro plné 4.7 GB DVD).<p></li>
<li><a href="background10.html" title="Technické vlastnosti opravy chyb">Žádná garantovaná ochrana</a> proti ztrátě dat.</li>
</ul>

Viz také kolekce <a href="background.html">základních informací</a>,
kde zjistíte více o tom, jak program dvdisaster funguje.
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
   link10)   link_title="Vyvolání grafického uživatelského rozhraní" ;;
   link20)   link_title="Načtení obrazu média" ;;
   link30)   link_title="Vytvoření souboru oprav chyb" ;;
   link40)   link_title="Skenování média na chyby" ;;
   link50)   link_title="Oprava obrazu média" ;;
   link60)   link_title="Porovnání obrazu proti datům oprav chyb" ;;

   link90)   link_title="Konfigurace" ;;
   link91)   link_title="Obecná nastavení" ;;
   link92)   link_title="Nastavení čtení" ;;
   link93)   link_title="Nastavení oprav chyb" ;;

   content*) eval "example$page$lang $file" ;;
   esac 
}


function example0cs()
{  cat >> $1 <<EOF
<h3>Příklady</h3>
Následující stránky obsahují komentovanou prohlídku použití programu dvdisaster:

<ol>
<li><a href="example10.html">Vyvolání grafického uživatelského rozhraní</a><p></li>
<li><a href="example20.html">Načtení obrazu média</a><p></li>
<li><a href="example30.html">Vytvoření souboru oprav chyb</a><p></li>
<li><a href="example40.html">Skenování média na chyby</a><p></li>
<li><a href="example50.html">Oprava obrazu média</a><p></li>
<li><a href="example60.html">Porovnání obrazu proti datům oprav chyb</a><p></li>
</ol>
<pre> </pre>

Další témata: 

<ul>
<li><a href="example90.html">Konfigurace</a><p></li>

<ul>
<li><a href="example91.html">Obecná nastavení</a><p></li>
<li><a href="example92.html">Nastavení čtení</a><p></li>
<li><a href="example93.html">Nastavení oprav chyb</a><p></li>
</ul>

<li><a href="background60.html">Tipy pro ukládání souborů oprav chyb</a></li>
</ul>
EOF
}

function example10cs()
{  
cat >> $1 <<EOF
<h3>Vyvolání grafického uživatelského rozhraní</h3>

Program dvdisaster spustí grafické uživatelské rozhraní automaticky,<br>
když kliknete na ikonu programu dvdisaster ve vašem okenním systému.<p>

Po krátké spouštěcí době se zobrazí počáteční okno programu dvdisaster:<p>

<img src="../images/ex-start-cs.png" alt="Počáteční obrazovka"><p>

<b>Obecné tipy:</b>

<ul>
<li>Vzhled prvků okna závisí na operačním systému a osobních nastaveních
okenního systému.<p></li>
<li>Program dvdisaster také poskytuje <a href="syntax10.html">režim příkazové řádky</a>.<p></li>
<li>Je-li program dvdisaster vyvolán v grafickém režimu, vytvoří si
konfigurační soubor <i>.dvdisaster</i>, kam ukládá volby
<a href="example90.html">dialogu Nastavení</a>. Pokud tento soubor existuje,
pak v grafickém režimu parametry příkazové řádky nemají vliv.<p>

</ul>

<b>Tipy pro verzi pro Windows:</b>

<ul>
<li>Ve Windows 2000 a XP jsou vyžadována buď <b>oprávnění administrátora</b> nebo nainstalovaný
ovladač <a href="qa10.html#aspi">ASPI</a>, aby bylo možno pracovat s mechanikami CD/DVD.<p></li>
<li>Z technických důvodů existují dvě verze programu pro Windows:
<table>
<tr><td><i>dvdisaster<b>-win</b>.exe</i></td><td>--</td><td>obsahuje grafické uživatelské rozhraní</td></tr>
<tr><td><i>dvdisaster.exe</i></td><td>--</td><td>poskytuje rozhraní příkazové řádky</td></tr>
</table>
</li>
</ul>

<a href="example20.html">Načtení obrazu média...</a>
EOF
}


function example20cs()
{  cat >> $1 <<EOF
<h3>Načtení obrazu média</h3>

Většina akcí programu dvdisaster je prováděna na obrazu média umístěném
na vašem pevném disku. Tak jsou operace rychlejší a snižuje se zatížení
vaší mechaniky CD/DVD.<p>

Proveďte prosím následující kroky k načtení obrazu média (všimněte si
také odpovídajících značek ve snímku obrazovky):<p>

<b>1. Zvolte mechaniku <font color="red">(1)</font>,
kam je vložen disk CD nebo DVD.</b><p>

Verze programu pro Linux v současné době poskytuje na výběr pouze ty mechaniky,
které jsou přítomny v adresáři /dev. V pozdějších verzích programu bude
zavedeno mnohem volnější schéma výběru.<p>

<b>2. Zvolte název souboru obrazu.</b><p>

Kliknutím na symbol
<img src="../images/open-img.png" alt="Image file selection" align="middle">
<font color="red">(2a)</font>
vyvoláte okno pro volbu názvu a umístění souboru obrazu.<br>
Název souboru také můžete přímo vepsat do textové položky
<font color="red">(2b)</font>.<p>

Ujistěte se, že máte dostatek volného místa pro obraz. Aktivujte
<a href="example91.html#localfiles">volbu rozdělení souboru</a>, pokud
váš souborový systém nepodporuje soubory větší než 2 GB.<p>

<b>3. Načtěte obraz média do souboru.</b><p>

kliknutím na tlačítko "Načíst" <font color="red">(3)</font>
spustíte čtení.<p>

<hr>
Snímek obrazovky: Čtení obrazu
<hr>
<img src="../images/ex-read-cs.png" alt="Reading the image"><p>
<hr>

Průběh čtení se bude zobrazovat graficky.<p>

Kliknutím na symbol
<img src="../images/btn-protocol-cs.png" alt="Zobrazit záznam" align="middle">
zobrazíte další informace o procesu čtení.<p>

<a href="example30.html">Vytvoření souboru oprav chyb...</a>
EOF
}


function example30cs()
{  cat >> $1 <<EOF
<h3>Vytvoření souboru oprav chyb</h3>

Soubor oprav chyb má důležitou roli v programu dvdisaster:<br>
Je nezbytný k obnovení nečitelných sektorů média.<p>

Jakmile jste načetli
<a href="example20.html">obraz média</a> na váš pevný disk, 
proveďte následující akce:<p>

<b>1. Zvolte soubor obrazu <font color="red">(1)</font>.</b><p>

Dříve načtený soubor obrazu bude přednastaven.<p>

<b>2. Zvolte název pro soubor oprav chyb.</b><p>

Kliknutím na symbol
<img src="../images/open-ecc.png" alt="Volba souboru oprav chyb" align="middle">
<font color="red">(2a)</font>
vyvoláte okno pro volbu názvu a umístění souboru oprav chyb.<br>
Název souboru také můžete přímo vepsat do textové položky
<font color="red">(2b)</font>.<p>
<p>

<b>3. Vytvořte soubor oprav chyb.</b><p>

Kliknutím na tlačítko "Vytvořit" <font color="red">(3)</font> 
vytvoříte soubor oprav chyb.<p>

<hr>
Snímek obrazovky: Vytvoření souboru oprav chyb
<hr>
<img src="../images/ex-create-cs.png" alt="Vytvoření souboru oprav chyb"><p>
<hr>

Průběh se zobrazuje pomocí procent. 
Zpracování obrazu jednovrstvého DVD ve
<a href="example93.html#redundancy">standardním nastavení redundance</a>
trvá asi 10 minut na průměrném 2 GHz systému.<p>

<b>Poznámky</b>:

<ul>
<li>Soubor oprav chyb nelze vytvořit, pokud není obraz kompletní nebo
obsahuje chyby čtení.<p>

<table width=100%><tr><td bgcolor=#000000 width=2><img width=1 height=1 alt=""></td>
<td>Vytvořte soubor oprav chyb bezprostředně po zápisu média.
</td></tr></table>
</li>

<li>Soubor obrazu není dále potřeba,
jakmile byl úspěšně vytvořen soubor oprav chyb.<br>
Program dvdisaster automaticky přepíše existující soubor obrazu, pokud se
načítá odlišné médium.<p></li>

<li>
Vždy uchovávejte soubor oprav chyb na
<a href="background60.html">spolehlivém úložném médiu</a>!
</li>
</ul>

<a href="example40.html">Od této chvíle byste měli pravidelně skenovat
médium na chyby čtení...</a>
EOF
}


function example40cs()
{  cat >> $1 <<EOF
<h3>Skenování média na chyby</h3>

Kontrolu média na chyby čtení provedete tak, že vložíte médium do mechaniky
a následujete tyto kroky:<p>

<b>1. Zvolte mechaniku <font color="red">(1)</font>,
kam jste vložili CD nebo DVD.</b><p>

Verze programu pro Linux v současné době poskytuje na výběr pouze ty mechaniky,
které jsou přítomny v adresáři /dev. V pozdějších verzích programu bude
zavedeno mnohem volnější schéma výběru.<p>

<b>2. Spusťte skenování média.</b><p>

Kliknutím na tlačítko "Skenovat" <font color="red">(2)</font>
začne kontrola média.<p>

<hr>
Snímek obrazovky: Sken média
<hr>
<img src="../images/ex-scan-cs.png" alt="Sken média"><p>
<hr>

Grafické zobrazení podává informaci o průběhu skenování a stavu média:<p>

<ul>
<li><b>Graf rychlosti</b> poskytuje hrubý odhad stavu média, protože většina
mechanik zpomalí, když se kvalita média snižuje.<p>

Abyste se vyhnuli vlivu dalších systémových aktivit, neměli byste spouštět
žádný další program v průběhu skenování média. Také při skenování nepoužívejte
žádné z ovládacích prvků v okně programu dvdisaster.<p></li>

<li><b>Spirála</b> poskytuje (není vykreslena v měřítku)
přehled o <b>stavu média</b>.
Červené značky indikují přítomnost chyb čtení!
<table width=100%><tr><td bgcolor=#000000 width=2><img width=1 height=1 alt=""></td><td>
Jakmile si všimnete chyb čtení na vašem médiu, pokuste se
<a href="example50.html">opravit obraz média</a> 
a přeneste jej na nové médium. 
</td></tr></table><p>
</li>

<li>Kliknutím na symbol
<img src="../images/btn-protocol-cs.png" alt="Zobrazit záznam" align="middle">
zobrazíte další informace o procesu čtení.<p></li>
</ul>

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

<a href="example50.html">Oprava obrazu média...</a>
EOF
}

function example50cs()
{  cat >> $1 <<EOF
<h3>Oprava obrazu média</h3>

K obnovení dat z poškozeného média potřebujete
<a href="example30.html">odpovídající soubor oprav chyb</a>,
který jste snad vytvořili v době, kdy bylo ještě médium plně čitelné.
Proveďte následující akce:<p>

<b>1. Přepněte strategii čtení.</b><p>

Jděte do dialogu Nastavení čtení a skenování a zvolte
<a href="example92.html#read">adaptivní strategii čtení</a>.<p>

<b>2. Pokuste se načíst co možná nejvíce dat do souboru obrazu.</b><p>

Zvolte mechaniku, soubor obrazu a soubor oprav chyb <font color="red">(2a)</font>.<br>
Soubor oprav chyb musí být v tomto stádiu dostupná, aby program dvdisaster mohl
určit, zda bylo shromážděno dostatek dat pro rekonstrukci obrazu.<br>
Spusťte proces čtení kliknutím na tlačítko "Načíst" <font color="red">(2b)</font>.<p>

Uvědomte si prosím, že načtení poškozeného média může trvat několik hodin.<p>

<hr>
Snímek obrazovky: Čtení obrazu
<hr>
<img src="../images/ex-read-a-cs.png" alt="Čtení obrazu"><p>
<hr>

<pre> </pre>

<a name="repair"></a>
<b>3. Proveďte rekonstrukci obrazu.</b><p>

Klikněte na tlačítko "Opravit".
Normálně se během opravy obrazu místy projevuje vysoká
aktivita pevného disku.

<hr>
Snímek obrazovky: Oprava obrazu
<hr>
<img src="../images/ex-fix-cs.png" alt="Oprava obrazu"><p>
<hr>

Průběh opravy se zobrazuje v procentech.<p>

<b>Další tipy:</b><p>

<ul>
<li>Zpráva "V pořádku! Všechny sektory jsou opraveny." potvrzuje, že obraz média
byl plně obnoven. Opravený obraz může  být dále zpracováván jako běžný obraz ". i s o".
Lze jej zapsat na nové médium za použití vhodného vypalovacího programu
pro CD/DVD.<p></li>

<li>Pokud v tomto stádiu ještě zůstanou neopravitelné sektory, zkuste načtení
obrazu znovu <a href="background50.html#reading-tips">(tipy pro opakované čtení)</a>.
Program dvdisaster se pokusí znovu načíst pouze chybějící sektory, a tak by se mohlo
získat dostatek chybějících sektorů pro úspěšný průběh opravy chyb.<p></li>

<li>Červený chybový graf <i>neodpovídá</i> prostorovému rozmístění
chyb čtení na disku: Metoda opravy chyb je výhradně postavena tak, aby
dosáhla rovnoměrného rozložení chyb čtení média přes všechny bloky oprav chyb.<p>
Zelená čára označuje maximální kapacitu opravy chyb. Pokud je tato čára překročena,
obraz nemůže být opraven s aktuálně dostupnými daty.
V případě malých překročení by mohl pomoci
<a href="background50.html#reading-tips">další pokus o čtení</a>.
</li>

<li>Pokud byl obraz načítán strategií adaptivního čtení,
pak je vždy chybová korekce maximální.</li>
</ul>
EOF
}


function example60cs()
{  cat >> $1 <<EOF
<h3>Porovnání obrazu proti datům oprav chyb</h3>

Tato funkce poskytuje informaci o souborech obrazu a oprav chyb, které
jsou aktuálně uloženy na vašem pevném disku:<p>

<b>1. Zvolte soubory pro porovnání <font color="red">(1)</font>.</b><p>

Zadejte názvy souboru obrazu a k němu odpovídajícího souboru oprav chyb
(jak je popsáno v krocích 2a./2b. pro načtení <a href="example20.html">obrazu</a>
a pro tvorbu <a href="example30.html">souboru opravy chyb</a>).<p>

<b>2. Spusťte porovnání <font color="red">(2)</font>.</b><p>

Klikněte na tlačítko"Porovnat". Tento proces může chvilku trvat, protože
je třeba načíst celé oba soubory (ale nejsou na nich provedeny žádné změny).
<p>

<hr>
Snímek obrazovky: Porovnání souborů obrazu a oprav chyb
<hr>
<img src="../images/ex-compare-cs.png" alt="Porovnání souborů obrazu a oprav chyb"><p>
<hr>

Když je porovnání dokončeno, získáte následující informace:<p>

<b>"Souhrn souboru obrazu" a "Stav obrazu"</b>. 
Počet chybějících sektorů bude vyšší než nula, pokud obraz stále obsahuje
neopravitelné chyby čtení. Naproti tomu výskyt chyb kontrolního součtu
je obvykle důsledkem
<a href="qa20.html#crc">nesprávného zpracování nebo hardwarových problémů</a>.<p>

<b>"Souhrn souboru oprav chyb"</b>. 
Tato položka zobrazuje nastavení použitá při tvorbě souboru oprav chyb,
a zda může být zpracován vyšší verzí programu dvdisaster.
Pokud soubor oprav chyb neodpovídá souboru obrazu nebo pokud je poškozen,
upozornění jsou zobrazena pod posledními čtyřmi položkami.<p>

<b>Další tipy:</b>

<ul>
<li>Výsledek porovnání je nespolehlivý, pokud byl obraz načten jiným softwarem,
než programem dvdisaster. Zvláště všechny chybějící sektory budou
klasifikovány jako chyby kontrolního součtu, byl-li použit jiný software.
</li>
</ul>
EOF
}


function example90cs()
{  cat >> $1 <<EOF

<h3>Konfigurace</h3>

Stiskněte tlačítko
<img src="../images/open-preferences.png" align="middle" alt="Tlačítko nastavení">
v pravém horním rohu nástrojové lišty k otevření dialogu nastavení.
Dialog obsahuje několik formulářů:<p>

<center>
<a href="example91.html">
<img src="../images/prefs-general-cs.png"><br>
Obecná nastavení
</a>
</center>
<br clear="all">

<center>
<a href="example92.html">
<img src="../images/prefs-read-cs.png"><br>
Nastavení čtení
</a>
</center>
<br clear="all">

<center>
<a href="example93.html">
<img src="../images/prefs-ecc-cs.png"><br>
Nastavení oprav chyb
</a>
</center>

EOF
}

function example91cs()
{  cat >> $1 <<EOF

<h3>Obecná nastavení</h3>

Více informací o formuláři následuje níže na stránce.<p>

<center><img src="../images/prefs-general-cs.png"></center>
<br clear="all">

<p><hr><p>

<a name="iso"></a>
<b>Souborový systém média a obrazu</b><p>

<center><img src="../images/prefs-general-1-cs.png"></center>
<br clear="all">

Program dvdisaster určí velikost obrazu ze souborového systému ISO/UDF,
je-li tato možnost zaškrtnuta. To napravuje některé problémy
způsobené <a href="qa20.html#plusrw">hlášením nesprávné délky obrazů</a>
některými mechanikami pro DVD-RW/+RW média.

<p><hr><p>

<a name="localfiles"></a>
<b>Nastavení pro místní soubory</b><p>
<center><img src="../images/prefs-general-2-cs.png"></center>
<br clear="all">
 
<b>Automatická přípona souborů<font color="red">(1)</font>:</b>
Pokud je tento přepínač nastaven, soubory dostanou automaticky
přípony ".img" nebo ".ecc", pokud dosud není přípona v názvu přítomna.<p>

<b>Rozdělení souborů na segmenty<font color="red">(2)</font>:</b>
Umožňuje pracovat se souborovými systémy, které jsou omezeny na 2 GB na soubor
(t.j. FAT z Windows). Vytvořené soubory jsou rozprostřeny přes až 100 segmentů
nazvaných "medium00.img", "medium01.img" atd. za cenu malého snížení výkonu.<p>

<p><hr><p>

<a name="auto"> </a>
<b>Automatická tvorba a mazání souborů</b><p>

<center><img src="../images/prefs-general-3-cs.png"></center>
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

function example92cs()
{  cat >> $1 <<EOF

<h3>Nastavení čtení</h3>

Více informací o formuláři následuje níže na stránce.<p>

<center><img src="../images/prefs-read-cs.png"></center>
<br clear="all">

<p><hr><p>

<a name="read"></a>
<b>Nastavení čtení</b><p>

<center><img src="../images/prefs-read-1-cs.png"></center>
<br clear="all">

<b>Strategie čtení <font color="red">(1)</font>:</b> Zvolte mezi
<a href="background30.html">lineární strategií čtení</a> a
<a href="background40.html">adaptivní strategií čtení</a>.
Lineární strategie je vhodná pro nepoškozená média, zatímco adaptivní
strategie je lepší pro média, která již vykazují chyby čtení.<p>

<b>Rozsah čtení <font color="red">(2)</font>:</b> Čtení bude omezeno na
daný rozsah sektorů včetně okrajů: 0-100 bude číst 101 sektorů. Tato
nastavení jsou aktivní pouze pro aktuální sezení a nebudou uložena.<p>

<b>Přeskočit x sektorů po chybě čtení / <br>
Přestat číst, je-li nečitelný interval < x <font color="red">(3)</font>:</b><br>
Hodnota posuvníku ovlivňuje zpracování chyb čtení. Má mírně odlišné vlastnosti
pro <a href="background30.html#configure">lineární</a> a
<a href="background40.html#configure">adaptivní</a> strategie čtení.
Vysoké hodnoty zkracují čas zpracování a snižují mechanické opotřebení
mechaniky, ale také nechávají větší mezery v obrazu při čtení poškozených oblastí.

<p><hr><p>

<a name="image"></a>
<b>Vlastnosti obrazu</b><p>

<center><img src="../images/prefs-read-2-cs.png"></center>
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

<center><img src="../images/prefs-read-3-cs.png"></center>
<br clear="all">
Čeká daný počet sekund, až se mechanika roztočí, než začne
skutečné čtení dat. Tak se vyhnete rychlostním skokům na začátku
křivky čtení.
EOF
}

function example93cs()
{  cat >> $1 <<EOF

<h3>Nastavení oprav chyb</h3>

Více informací o formuláři následuje níže na stránce.<p>

<center><img src="../images/prefs-ecc-cs.png"></center>
<br clear="all">

<p><hr><p>

<a name="redundancy"></a>
<b>Redundance pro nové soubory oprav chyb</b><p>

<center><img src="../images/prefs-ecc-1-cs.png"></center>
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
Pouze změny o 5 procent a více mohou mít viditelný efekt na opravu chyb.<p></li>
</ul>

<font color="red">(4)</font>Volba <b>maximální velikosti</b> souboru
oprav chyb v MB. Program dvdisaster zvolí vhodné nastavení redundance tak,
aby celková velikost souboru oprav chyb nepřesáhla daný limit. <p>

Poznámka pro pokročilé: Použijeme-li stejné nastavení velikosti pro obrazy
velmi odlišných velikostí, je více informací pro opravu chyb přiděleno menším obrazům
a méně větším.<p>

<p><hr><p>

<b>Využití paměti</b><p>

<center><img src="../images/prefs-ecc-2-cs.png"></center>
<br clear="all">

Program dvdisaster optimalizuje přístup k obrazům a souborům oprav chyb tím,
že si udržuje vlastní vyrovnávací paměť. Přednastavení 32 MB je
vyhovující pro většinu systémů.<p>

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

<b>Vývojová verze</b> - nová a experimentální pro zkušené uživatele!<p>

<table width="100%" $IDXCOLOR cellpadding="0" cellspacing="5">
<tr><td><b>dvdisaster-0.65</b></td><td align="right">14.01.2006</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
  <table>
    <tr><td align="right">&nbsp;&nbsp;Zdrojový kód pro všechny operační systémy:&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.65.tar.bz2">dvdisaster-0.65.tar.bz2</a></td></tr>
    <tr><td align="right">Digitální podpis:&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.65.tar.bz2.gpg">dvdisaster-0.65.tar.bz2.gpg</a></td></tr>
    <tr><td align="right">Binární verze pro Windows:&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.65-setup.exe">dvdisaster-0.65-setup.exe</a></td></tr>
    <tr><td align="right">Digitální podpis:&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.65-setup.exe.gpg">dvdisaster-0.65-setup.exe.gpg</a></td></tr>
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

</td></tr></table><p>

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
<a href="example91.html#iso">velikosti obrazu podle souborového systému UDF/ISO</a>,
aby se zlepšila
<a href="qa20.html#plusrw">detekce velikosti obrazu pro -RW/+RW média</a>.
Načítání obrazů a vytváření odpovídajících souborů oprav chyb může být nyní vyvoláno
společně <a href="example91.html#auto">jedním kliknutím myši</a>.
Několik drobných vylepšení bylo vytvořeno pro podporu více mechanik CD/DVD,
uhlazení stávajících funkcí a k odstranění některých drobných chyb ;-)<p>
<b>Záplaty</b> (malé změny proti verzi 0.64; soubory nahoře byly aktualizovány):<br>
<b>pl1</b> Stabilizuje pád bezprostředně po spuštění programu, je-li linkován proti gtk+-2.6;
dotčena je pouze verze pro Linux.<br>
<b>pl2</b> Opravuje volbu příkazové řádky -u a vytváří souborová oprávnění pro obrazy
za použití Unixových konvencí pod Linuxem.

</td></tr></table><p>

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
Obsahuje <a href="example50.html">vylepšenou strategii</a> pro
<a href="syntax90.html#adaptiveread">čtení poškozených médií</a>,
která byla motivována článkem ve vydání 16/2005 německého periodika c't.
</td></tr></table><p>

<pre> </pre>

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
</td></tr></table><p>

<pre> </pre>

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
 <li><b>x86 kompatibilní</b> hardware v <b>32 bitovém</b> režimu;<br>
     kompilace jako <b>64 bitová</b> aplikace pod Linuxem je možné, ale nebylo testováno.<p></li>
 <li>současné mechaniky CD nebo DVD s rozhraním ATAPI nebo SCSI<p></li>
 <li>minimálně P4 na 2 GHz nebo srovnatelný procesor<p></b>
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
</td></tr></table><p>

Přejděte do nově vytvořeného adresáře:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>cd $project_package</b><br>
</td></tr></table><p>

Spusťte konfigurační skript:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>bash configure</b><br>
<i>[... spousta výstupu z konfiguračního skriptu ...]</i>
</td></tr></table><p>

Skript možná nabídne nějaké tipy ohledně chybějících knihoven.
Zadejte "bash configure --help" pro zjištění, jak správně nasměrovat
konfigurační skript k nainstalovaným knihovnám.<p>

Když skončí konfigurační skript bez chybových hlášení,
zkompilujte zdrojový kód do spustitelného programu:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>gmake</b><br>
<i>[... spousta výstupu z procesu kompilace ...]</i>
</td></tr></table><p>

Spusťte program na krátký test:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>./dvdisaster</b><br>
<i>[... za okamžik se otevře <a href="example10.html">hlavní okno</a> ...]</i>
</td></tr></table><p>

Práce s programem v aktuálním adresáři je v pořádku.
V podadresáři <i>documentation/en</i> je zahrnuta kopie
domácí stránky programu dvdisaster.<p>

Také je možné nainstalovat program a dokumentaci
permanentně na váš systém:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>gmake install</b><br>
<i>[... výstup instalačního procesu ...]</i>
</td></tr></table><p>

Před provedením instalace prosím zkontrolujte výstup skriptu "<tt>configure</tt>"
nebo zadejte "<tt>gmake show</tt>" ke zjištění cílových adresářů instalace.<p>
Zadejte "<tt>bash configure --help</tt>" k získání informací o změně
instalačních cest.

<pre> </pre>

<font size="+1">Užijte si používání programu dvdisaster!</font>
EOF
}

# ----- Command line syntax

function syntax_contents_cs()
{  local file="$1.html"
   local query=$2
   local page=$3
   local lang=$4

   case $query in
   title)   title="Režim příkazové řádky" ;;

   link)    link_title="Režim příkazové řádky" ;;
   link10)   link_title="Použití příkazové řádky" ;;
   link20)   link_title="Načtení obrazu média" ;;
   link30)   link_title="Vytvoření souboru oprav chyb" ;;
   link40)   link_title="Skenování média na chyby" ;;
   link50)   link_title="Oprava obrazu média" ;;

   link60)   link_title="Volba alternativních mechanik a názvů souborů" ;;
   link70)   link_title="Provádění více akcí v rámci jednoho spuštění programu" ;;

   link90)   link_title="Syntaxe příkazu" ;;

   content*) eval "syntax$page$lang $file" ;;
   esac 
}


function syntax0cs()
{  cat >> $1 <<EOF
<h3>Program dvdisaster jako nástroj na příkazovou řádku</h3>

Přehled základních funkcí:

<ol>
<li><a href="syntax10.html">Použití příkazové řádky</a><p></li>
<li><a href="syntax20.html">Načtení obrazu média</a><p></li>
<li><a href="syntax30.html">Vytvoření souboru oprav chyb</a><p></li>
<li><a href="syntax40.html">Skenování média na chyby</a><p></li>
<li><a href="syntax50.html">Oprava obrazu média</a><p></li>
</ol>
<pre> </pre>

Některé tipy pro snadnější použití: 

<ul>
<li><a href="syntax60.html">Volba alternativních mechanik a názvů souborů</a><p></li>
<li><a href="syntax70.html">Provádění více akcí v rámci jednoho spuštění programu</a><p></li>
<li><a href="background60.html">Tipy pro ukládání souborů oprav chyb</a></li>
</ul>

<pre> </pre>

Úplný seznam voleb příkazové řádky:
<ul>
<li><a href="syntax90.html">Syntaxe příkazu</a><p></li>
</ul>

EOF
}


function syntax10cs()
{  cat >> $1 <<EOF
<h3>Použití příkazové řádky</h3>

Program dvdisaster se přepne do režimu příkazové řádky, pokud je vyvolán
s <a href="syntax90.html#modes">akčním parametrem</a> 
(jako je read, create, scan, fix, test). Jinak se otevře okno
<a href="example10.html">grafického uživatelského rozhraní</a>.<p>

Režim příkazové řádky ignoruje jakákoli nastavení grafického
uživatelského rozhraní a konfiguračního souboru <i>.dvdisaster</i>.<p>

<b>Tipy pro verzi pro Windows:</b>

<ul>
<li>Pod Windows 2000 a XP jsou vyžadována buď <b>oprávnění administrátora</b> nebo nainstalovaný ovladač
<a href="qa10.html#aspi">ASPI</a> pro přístup na mechaniky CD/DVD.<p></li>
<li>Z technických důvodů existují dvě verze programu pro Windows:
<table>
<tr><td><i><b>dvdisaster</b>.exe</i></td><td>--</td><td>poskytuje rozhraní příkazové řádky</td></tr>
<tr><td><i>dvdisaster-win.exe</i></td><td>--</td><td>obsahuje grafické uživatelské rozhraní</td></tr>
</table>
</li>
</ul>

<a href="syntax20.html">Načtení obrazu média...</a>

EOF
}


function syntax20cs()
{  cat >> $1 <<EOF
<h3>Načtení obrazu média</h3>

Většina akcí programu dvdisaster je prováděna na obrazu média umístěném
na vašem pevném disku.
Tak jsou operace rychlejší a snižuje se zatížení vaší mechaniky CD/DVD.<p>

Abyste načetli obraz média:
<ul>
<li>Přejděte do adresáře s dostatkem volného místa.
<li>Vložte médium do mechaniky.</li>
<li>Načtěte obraz média na váš pevný disk:</li>
</ul>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>dvdisaster -r</b><br>
dvdisaster-${project_version} Copyright 2004-2006 Carsten Gnoerlich.<br>
Tento software není vybaven ABSOLUTNĚ ŽÁDNOU ZÁRUKOU.<br>
Jedná se o svobodný software a můžete jej šířit<br>
podle podmínek VŠEOBECNÉ VEŘEJNÉ LICENCE GNU. Celá,<br>
právně závazná licence je uvedena v souboru "COPYING".<p>

Zařízení: /dev/cdrom, HL-DT-STDVDRAM GSA-4165B DL04<br>
Médium: DVD-R, 2287168 sektorů, 1 vrstva(y)<p>

Vytváření nového obrazu medium.img.<br>
Čekám 5 sekund na roztočení disku...<br>
Pozice čtení: 100.0% (10.2x)<br>
Všechny sektory byly úspěšně načteny.
</td></tr></table><p>

V průběhu čtení se bude zobrazovat počet načtených sektorů v procentech
a aktuální rychlost mechaniky.<p>

Po dokončení najdete soubor obrazu <i>medium.img</i> v aktuálním adresáři:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>ls -lA</b><br>
-rwx------    1 user     linux    4555341824 2004-07-02 21:31 medium.img<br>
</td></tr></table><p>

Ve výchozím nastavení se čte z mechaniky <i>/dev/cdrom</i> a vytváří se
soubory s názvy začínajícími na <i>medium</i>. Samozřejmě je možné zvolit
jiné <a href="syntax60.html">mechaniky</a>
a <a href="syntax60.html">názvy souborů</a>.<p>

<a href="syntax30.html">Vytvoření souboru oprav chyb...</a>
EOF
}


function syntax30cs()
{  cat >> $1 <<EOF
<h3>Vytvoření souboru oprav chyb</h3>

Soubor oprav chyb má v programu dvdisaster důležitou roli:<br>
Je nezbytný k opravě nečitelných sektorů média.<p>

Jakmile jste načetli
<a href="syntax20.html">obraz média</a> na váš pevný disk,
zadejte následující:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>dvdisaster -c</b><br>
dvdisaster-${project_version} Copyright 2004-2006 Carsten Gnoerlich.<br>
<i>[... zbytek oznámení GPL ...]</i><p>

Otevírání medium.img: 2224288 sektorů média.<br>
Skenuji sektory obrazu: 100%<br>
Kódování metodou RS01: 32 kořenů, redundance 14.3%.<br>
Tvorba ecc: 100.0%<br>
Soubor oprav chyb "medium.ecc" byl vytvořen.<br>
Zajistěte, aby byl tento soubor uložen na spolehlivém médiu.<br>
</td></tr></table><p>

Program dvdisaster nejprve zkontroluje, zda je obraz kompletní,
<i>protože soubor oprav chyb nelze vytvářet pro média, která již
obsahují nečitelné sektory!</i> <br>

<table width=100%><tr><td bgcolor=#000000 width=2><img width=1 height=1 alt=""></td>
<td>Vytvořte soubor oprav chyb bezprostředně po vypálení média.
</td></tr></table>

Průběh se zobrazuje v procentech. 
Zpracování obrazu jednovrstvého DVD se standardním nastavením
<a href="syntax90.html#redundancy">redundance</a>
trvá asi 10 minut na průměrném 2 GHz systému.<p>

Po dokončení se objeví v aktuálním adresáři soubor oprav chyb <i>medium.ecc</i>:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>ls -lA</b><br>
-rwx------    1 user     linux    653721680 2004-07-02 22:45 medium.ecc<br>
-rwx------    1 user     linux    4555341824 2004-07-02 21:31 medium.img<br>
</td></tr></table><p>

Nyní můžete obraz smazat. <p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>rm -f medium.img</b><br>
</td></tr></table><p>

Ale uchovejte soubor oprav chyb na <a href="background60.html">spolehlivém úložném médiu</a>!
<p>

<a href="syntax40.html">Od této chvíle byste měli pravidelně skenovat médium
na chyby čtení...</a>
EOF
}


function syntax40cs()
{  cat >> $1 <<EOF
<h3>Skenování média na chyby</h3>

Médium otestujete na chyby čtení, když jej vložíte do mechaniky a zadáte:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>dvdisaster -s</b><br>
dvdisaster-${project_version} Copyright 2004-2006 Carsten Gnoerlich.<br>
<i>[... zbytek oznámení GPL ...]</i><p>

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
</td></tr></table><p>

Nahlášené chyby čtení mohou být jiné než "Medium Error; ...", protože
závisí na firmwaru mechaniky.<p>

<table width=100%><tr><td bgcolor=#000000 width=2><img width=1 height=1 alt=""></td><td>
Jakmile zaznamenáte chyby čtení na vašem médiu, zkuste
<a href="syntax50.html">opravit obraz média</a> 
a přenést jej na nové médium.</td></tr></table><p>

<b>Jak často by se měla média skenovat?</b> 
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

<a href="syntax50.html">Oprava obrazu média...</a>
EOF
}


function syntax50cs()
{  cat >> $1 <<EOF
<h3>Oprava obrazu média</h3>

Pro obnovení dat z poškozeného média se nejdříve musíte pokusit
načíst co možná nejvíce dat do souboru obrazu.<br>
Pokud použijete <a href="background40.html">adaptivní strategii čtení</a> 
a nasměrujete program dvdisaster na název souboru oprav chyb,
čtení se zastaví, jakmile je shromážděno dostatek dat
pro obnovu obrazu - výsledkem je velká úspora času!<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>dvdisaster -r --adaptive-read -e medium.ecc</b><br>
dvdisaster-${project_version} Copyright 2004-2006 Carsten Gnoerlich.<br>
<i>[... zbytek oznámení GPL ...]</i><p>

Zařízení: /dev/cdrom, HL-DT-STDVDRAM GSA-4165B DL04<br>
Médium: DVD+R, 2224288 sektorů, 1 vrstva(y)<p>

Adaptivní čtení: Pokouším se shromáždit dostatek dat pro opravu chyb.<br>
Vytváření nového obrazu medium.img.<br>
Opravitelné:  2.6% (opravitelné: 0; nyní načítám [0..2224288], velikost 2224288)<br>
Sektory 57264-57279: Medium Error; Unrecovered read error.<br>
Vyplňování oblasti obrazu [57280..1083504]<br>
[... další hlášení o procesu čtení ...] <br>
Opravitelné: 100.0% (opravitelné: 319200; nyní načítám [320304..327065], velikost 6762)<br>
Je k dispozici dostatek dat pro rekonstrukci obrazu.
</td></tr></table><p>

Pak znovu potřebujete <a href="syntax30.html">odpovídající soubor oprav chyb</a>,
který byl snad vytvořen v době, kdy bylo ještě médium plně čitelné.
Zadejte:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>dvdisaster -f</b><br>
dvdisaster-${project_version} Copyright 2004-2006 Carsten Gnoerlich.<br>
<i>[... zbytek oznámení GPL ...]</i><p>

Otevírání medium.img: 2224288 sektorů média.<p>

Režim opravy: Opravitelné sektory budou v obrazu opraveny.<br>
&nbsp;&nbsp;&nbsp;    5 opravených sektorů: 708225 59850 69825 79800 199500 <br>
&nbsp;&nbsp;&nbsp;    6 opravených sektorů: 708226 59851 69826 79801 89776 199501<br>
[... další hlášení o opravených sektorech ...] <br>
&nbsp;&nbsp;&nbsp;    5 opravených sektorů: 708224 59849 69824 79799 199499<br>
Průběh ecc: 100.0% <br>
Opraveno sektorů: 33264 <br>  
V pořádku! Všechny sektory jsou opraveny.<br>
Počet výmazů na blok ecc:  průměr =  3.3; nejvíce = 7.
</td></tr></table><p>

Hlášení "V pořádku! Všechny sektory jsou opraveny." potvrzuje, že obraz média
("medium.img") byl plně obnoven.
Opravený obraz může  být dále zpracováván jako běžný obraz ". i s o".
Lze jej zapsat na nové médium za použití vhodného vypalovacího programu
pro CD/DVD.<p>

Pokud v tomto stádiu ještě zůstanou neopravitelné sektory,
zkuste načtení obrazu znovu
<a href="background50.html#reading-tips">(tipy pro opakované čtení)</a>.
Program dvdisaster se pokusí znovu načíst pouze chybějící sektory, a tak
by se mohlo získat dostatek chybějících sektorů pro úspěšný průběh opravy chyb.
EOF
}


function syntax60cs()
{  cat >> $1 <<EOF
<h3>Volba mechaniky pro čtení</h3>

Použijte <a href="syntax90.html#device">volbu "-d"</a>
k zadání mechaniky pro čtení obrazů médií:<p>

<table class="example-bsd" width=100%><tr><td>
user@freebsd&gt; dvdisaster <b>-d /dev/pass0</b> -r
</td></tr></table><p>

<table class="example" width=100%><tr><td>
user@linux&gt; dvdisaster <b>-d /dev/hdc</b> -r
</td></tr></table><p>

<table class="example-win" width=100%><tr><td>
user@windows&gt; dvdisaster <b>-d E:</b> -r
</td></tr></table><p>

Verze pro FreeBSD přistupuje k zařízením za použití průchodových ovladačů
<tt>/dev/pass*</tt>.
Pod Linuxem jsou některé obecně používané názvy zařízení
<tt>/dev/hd*</tt> pro mechaniky ATAPI a <tt>/dev/scd*</tt>
pro mechaniky SCSI. Verze pro Windows pracuje s obvyklými písmeny
pro mechaniky.

<pre> </pre>

<h3>Zadání názvů souborů obrazu a oprav chyb</h3>

Můžete změnit názvy souborů obrazu a oprav chyb pomocí
<a href="syntax90.html#image">voleb "-i a -e"</a>:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; dvdisaster <b>-i mujimage.img -e medium01.ecc</b> -c
</td></tr></table><p>

Je možné vynechat přípony souborů ".img" nebo ".ecc" nebo použít jiné.
EOF
}


function syntax70cs()
{  cat >> $1 <<EOF
<h3>Provádění více akcí v rámci jednoho spuštění programu</h3>

Program dvdisaster může provést několik <a href="syntax90.html#modes">akcí</a>
v rámci jednoho spuštění programu.<br>
Například pro načtení obrazu z <tt>/dev/hdc</tt>,
vytvoření jemu odpovídajícímu souboru oprav chyb "medium42.ecc"
a nakonec smazání dočasného souboru obrazu "medium.img", zadáte:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; dvdisaster <b>-r -c -u</b> -d /dev/hdc -e medium42.ecc
</td></tr></table><p>

Akce se budou vždy provádět ve stejném pořadí, jak byly
<a href="syntax90.html#modes">vypsány v sekci syntaxe příkazu</a>,
i když jsou zadány na příkazovou řádku v odlišném pořadí.
EOF
}


function syntax90cs()
{  cat >> $1 <<EOF
<h3>Syntaxe příkazu</h3>

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
<tr valign=top><td></td><td><a href="#eccfile">--eccfile</a></td><td>Název souboru oprav chyb<p></td></tr>
<tr><td></td><td><a href="#list">--list</a></td><td>Zobrazit mechaniky dostupné pod ASPI (pouze verze pro Windows)</td></tr>
</table><pre> </pre> 

<b><a name="options">Další volby.</a></b> Tyto méně používané volby jsou dostupné pouze
v dlouhé formě, pokud není zmíněno jinak.<p>

<table>
<tr valign=top><td>&nbsp; &nbsp;</td><td><a href="#adaptiveread">--adaptive-read</a></td><td>Použít adaptivní strategii čtení poškozených médií</td></tr>
<tr valign=top><td>&nbsp; &nbsp;</td><td><a href="#autosuffix">--auto-suffix</a></td><td>Automaticky doplnit souborové přípony .img a .ecc</td></tr>
<tr valign=top><td>&nbsp; &nbsp;</td><td><a href="#cache">--cache-size</a> &nbsp; &nbsp;</td><td>Velikost vyrovnávací paměti v průběhu vytváření souboru oprav chyb</td></tr>
<tr valign=top><td></td><td><a href="#dao">--dao</a></td><td>Předpokládat médium zapsané v režimu "disk at once"</td></tr>
<tr valign=top><td></td><td><a href="#fillunreadable">--fill-unreadable [n]</a></td><td>Vyplnit nečitelné sektory daným bajtem</td></tr>
<tr valign=top><td></td><td><a href="#jump">-j / --jump</td><td>Přeskočit sektory po chybě čtení</td></tr>
<tr valign=top><td></td><td><a href="#parse-udf">--parse-udf</td><td>Použít informace ze souborového systému ISO/UDF</td></tr>
<tr valign=top><td></td><td><a href="#redundancy">-n / --redundancy</td><td>Nastavit redundanci pro opravný kód</td></tr>
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

--read bude standardně používat <a href="background30.html">lineární strategii čtení</a>.
Místo toho můžete přidat volbu <a href="syntax90.html#adaptiveread">--adaptive-read</a>
pro použití <a href="background40.html">adaptivní strategie čtení</a>.<p>

Rozsah načítaných sektorů může být omezen analogicky jako volba
<a href="syntax90.html#scan">--scan</a>.

<div align=right><a href="#modes">&uarr;</a></div><p>


<a name="create"><b>--create: Vytvořit soubor oprav chyb</b></a><p>

Vytvoří pro obraz soubor oprav chyb.<p>

<div align=right><a href="#modes">&uarr;</a></div><p>



<a name="fix"><b>--fix: Opravit obraz</b></a><p>

Pokusí se opravit obraz s použitím souboru oprav chyb.<p>

Pokud nemohou být všechny sektory opraveny během prvního průchodu,
zkuste opakované čtení obrazu dalším průchodem příkazu <a href="#read">--read</a>
<a href="background50.html#reading-tips">(tipy pro opakované čtení)</a>.
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

<b>FreeBSD:</b> &nbsp; program dvdisaster podporuje mechaniky, které jsou přístupné
prostřednictvím průchodového ovladače <tt>/dev/pass*</tt> a které jsou MMC3 kompatibilní. 
Přístup na mechaniky ATAPI může vyžadovat <a href="download20.html#freebsd">rekompilaci jádra</a>.<p>

<b>Linux:</b> &nbsp; program dvdisaster podporuje mechaniky, které jsou přístupné
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

<b>Windows:</b> &nbsp; program dvdisaster podporuje lokální mechaniky implementující
standard MMC3, na které lze odkazovat pomocí písmena disku v systému.


<div align=right><a href="#drive">&uarr;</a></div><p>

<a name="prefix"><b>--prefix &lt;název&gt;: Název pro soubory obrazu a oprav chyb</b></a><p>

Přednastavený název je "medium"; soubor obrazu a soubor oprav chyb automaticky dostanou
přípony ".img" a ".ecc".

<div align=right><a href="#drive">&uarr;</a></div><p>

<a name="image"><b>--image &lt;název souboru&gt;: Název souboru obrazu</b></a><p>

Přednastavený název je "medium.img"; zadaný název souboru bude použit pro soubor obrazu
přesně (bez přidání přípony), pokud není zadána také volba <a href="#autosuffix">--auto-suffix</a>.

<div align=right><a href="#drive">&uarr;</a></div><p>

<a name="eccfile"><b>--eccfile &lt;název souboru&gt;: Název souboru oprav chyb</b></a><p>

Přednastavený název je "medium.ecc"; zadaný název souboru bude použit pro soubor oprav chyb
přesně (bez přidání přípony), pokud není zadána také volba <a href="#autosuffix">--auto-suffix</a>.

<div align=right><a href="#drive">&uarr;</a></div><p>

<a name="list"><b>--list: Zobrazit mechaniky dostupné pod ASPI</b></a><p>

Tato volba zobrazí seznam všech mechanik CD/DVD, které jsou přístupné prostřednictvím
vrstvy <a href="qa10.html#aspi">ASPI</a> ve Windows:<p>

<table class="example-win" width=100% bgcolor=#e0e0ff><tr><td>
user@windows&gt; dvdisaster <b>-l</b><br>
dvdisaster-${project_version} Copyright 2004-2006 Carsten Gnoerlich.<br>
<i>[... zbytek oznámení GPL ...]</i><p>

Seznam ASPI CD/DVD mechanik:<p>
 1: (H:) ATAPI DVD+RW 8X4X12 B2K7<br>
 2: (I:) MYDRIVE CD-R MY-401610X 1.05<p>

K vynucení ASPI místo SPTI odkazujte na mechaniku pomocí<br>
čísel nahoře (použijte 1:, 2:,... místo C:, D:,...)
</td></tr></table><p>

Implicitně se zkouší přístup k mechanice nejprve za použití SPTI
a vrací se k ASPI pouze, pokud předchozí nefunguje.
Použití vrstvy ASPI může být vynuceno zadáním mechaniky
jejím číslem místo jejím písmenem.
K načtení disku s použitím ASPI a jednotky "MYDRIVE" z příkladu nahoře
byste pak zadali:<p>

<table class="example-win" width=100% bgcolor=#e0e0ff><tr><td>
user@windows&gt; dvdisaster <b>-d 2:</b> -r<br>
</table>

<div align=right><a href="#drive">&uarr;</a></div><p>


<h3>Další volby.</h3>

<a name="adaptiveread"><b>--adaptive-read: Použít adaptivní strategii čtení poškozených médií</b></a><p>

Tento přepínač volí <a href="background40.html">adaptivní strategii čtení</a>,
která je zvláště vhodná pro načtení poškozených médií.
Použijte tento přepínač ve spojení s <a href="#read">-r/--read</a> a
<a href="#eccfile">-e/--eccfile</a>. Zadání souboru oprav chyb způsobí,
že proces čtení je ukončen, jakmile je shromážděno dostatek informací
pro opravu obrazu média.
<p>

<div align=right><a href="#options">&uarr;</a></div><p>



<a name="autosuffix"><b>--auto-suffix: Automaticky doplnit souborové přípony .img a .ecc</b></a><p>

Pokud je tato volba zadána, k názvům souborů zadaných volbami
<a href="#image">-i/--image</a>- nebo <a href="#eccfile">-e/--eccfile</a>
bude automaticky přidána přípona ".img" respektive ".ecc",
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
</li>
</ul>

<div align=right><a href="#options">&uarr;</a></div><p>



<a name="jump"><b>-j / --jump &lt;počet sektorů&gt;: Přeskočit sektory po chybě čtení</b></a><p>

Tato volba má lehce odlišné chování v závislosti na použité strategii čtení:
<ul>
<li>když se použije <a href="background30.html#configure">lineární strategie čtení</a>
(výchozí nastavení), daný počet sektorů se po chybě čtení přeskočí.</li>
<li><a href="background40.html#configure">adaptivní strategie čtení</a> 
(volba <a href="#adaptiveread">--adaptive-read</a>) skončí, když již nezůstaly
žádné nečitelné oblasti obrazu, které jsou větší než daná hodnota.</li>
</ul> 

Vyšší hodnoty zkracují dobu zpracování a mechanické opotřebení mechaniky,
ale také ponechávají větší mezery v obrazu při čtení poškozených oblastí.
Počet přeskočených sektorů musí být násobek 16.

<div align=right><a href="#options">&uarr;</a></div><p>



<a name="parse-udf"><b>--parse-udf: Použít informace ze souborového systému ISO/UDF</b></a><p>
Program dvdisaster určí velikost obrazu z informací ze souborového systému ISO/UDF,
je-li tato volba zapnuta. Tím se opravují některé problémy způsobené mechanikami
<a href="qa20.html#plusrw">hlásícími nesprávné délky obrazu</a> pro
DVD-RW/+RW média.
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
efekt na opravu chyb.<p></li>
</ul>

Existuje několik způsobů zadání redundance:<p>

<ol>
<li> <b>"normální" nebo "vysoká"</b><p>

Po nastavení hodnot "normální" nebo "vysoká" získáte následující redundance:<p>

<table border="1" cellpadding="3">
<tr><td align=center>Hodnota</td><td>Redundance</td></tr>
<tr><td><tt>-n normal</tt></td><td align=center>14.3%</td></tr>
<tr><td><tt>-n high</tt></td><td align=center>33.5%</td></tr>
</table><p>

Tyto hodnoty spouští optimalizovaný programový kód ke zrychlení
tvorby souboru oprav chyb. Výchozí hodnota je "normální".<p>
</li>

<li> <b>Procentní hodnoty</b><p>

Je také možné zadání redundance v procentech:<p>

<table border="1" cellpadding="3">
<tr><td>Příklad</td><td>Platný rozsah</td></tr>
<tr><td><tt>-n 25%</tt></td><td align=center>3.2% až 64.5%</td></tr>
</table><p>

Z technických důvodů se může aktuální redundance odchýlit o několik
desetin od zadané hodnoty.<p>
</li>

<li> <b>Opravitelné chyby na blok ECC</b><p>

Pokud není zadána žádná jednotka redundance, je hodnota interpretována
k vytvoření kódu (255, 255-x)-Reed-Solomon:<p>

<table border="1" cellpadding="3">
<tr><td>Příklad</td><td>Platný rozsah</td></tr>
<tr><td><tt>-n 32</tt></td><td align=center>8 až 100</td></tr>
</table><p>

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
</table><p>

V tomto případě program dvdisaster vybere vhodné nastavení redundance, aby
celková velikost souboru oprav chyb nepřekročila daný limit.<p>

Poznámka pro pokročilé: Použijeme-li stejné nastavení velikosti pro obrazy
velmi odlišných velikostí, je více informací pro opravu chyb přiděleno menším obrazům
a méně větším.<p>
</li>
</ol>

<div align=right><a href="#options">&uarr;</a></div><p>


<a name="speedwarn"><b>--speed-warning [n]: Varovat při určité hodnotě poklesu rychlosti čtení</b></a><p>

Zobrazí varování, změní-li se rychlost čtení o více než <i>n</i> procent.
To je užitečné pro zjištění počínajícího zhoršování kvality média.
Pokud je volitelná hodnota [n] vynechána, je použito výchozí nastavení 10 procent.

<div align=right><a href="#options">&uarr;</a></div><p>


<a name="--spinup"><b>--spinup-delay [n]: Poskytnout mechanice čas na roztočení</b></a><p>

Počká zadaný počet sekund na roztočení disku v mechanice, než odstartuje skutečný
proces čtení.
Zabraňuje to falešným varováním o změnách rychlosti na začátku média,
je-li použita volba <a href="#speedwarn">--speed-warning</a>.<br>

Pokud není zadána volitelná hodnota [n], program dvdisaster bude čekat 5 sekund.

<div align=right><a href="#options">&uarr;</a></div><p>

<a name="split"><b>--split-files: Rozdělit soubory do segmentů <= 2 GB</b></a><p>

Umožňuje tvorbu velkých obrazů a souborů oprav chyb na souborových
systémech s omezením na 2 GB na soubor (t.j. FAT z Windows).
Soubory jsou rozprostřeny až do 100 segmentů "medium00.img", "medium01.img" atd.
za cenu mírného snížení výkonu.<p>

Název souboru musí být stále zadán jako
"medium.img" s použitím volby <a href="#image">--image</a>;
číslování bude automaticky vloženo, je-li tento přepínač aktivní.<p>

<div align=right><a href="#options">&uarr;</a></div><p>

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
</table><p>

Překladatelé pro další jazyky jsou vítáni!<p>

Program dvdisaster získává automaticky nastavení jazyka od operačního systému.
Pokud místní jazyk ještě není podporován, použijí se hlášení v angličtině. 
Jiný jazyk lze zvolit použitím proměnné prostředí.<p>

Příklad pro shell bash a německý jazyk:

<pre>export LANG=de_DE</pre>

Pokud nejsou speciální znaky jako německé přehlásky zobrazeny správně,
zkuste následující:<p>

<tt>export OUTPUT_CHARSET=iso-8859-1</tt> (X11, XTerm)<p>

nebo<p>

<tt>export OUTPUT_CHARSET=CP850</tt> (Windows)<p>

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
Pomoc: aktivujte možnost <a href="example91.html#iso">"Použít informace ze souborového systému ISO/UDF"</a>.
</li></ul>

<b>CD-R, CD-RW</b><p>

<ul>
 <li>Pouze datová CD jsou podporována.</li>
</ul>

<b>Nepoužitelné typy</b> (obraz nelze načíst):<p>
CD-Audio a CD-Video stejně jako DVD-ROM a DVD-RAM.

<div align=right><a href="#top">&uarr;</a></div><p>


<b><a name="filesystem">2.3 Které souborové systémy jsou podporovány?</a></b><p>

Program dvdisaster pracuje výhradně na úrovni obrazu, který je čten po sektorech.
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
</table><p>

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

Funkce <a href="syntax90.html#list">--list</a> zobrazí
aktuální mapování písmen mechanik.<p>

<i>&gt;&nbsp; Jak zvolit mezi SPTI a ASPI?</i><p>

Program dvdisaster si automaticky vybere mezi SPTI a ASPI, 
s přikloněním ve prospěch SPTI, pokud jsou dostupné oba ovladače.
Použití ASPI může být vynuceno; viz podrobnosti k popisu funkce
<a href="syntax90.html#list">--list</a>.

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
<a href="#freebsd">3.6 Žádné mechaniky se neobjeví pod FreeBSD.</a><p>

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
<a href="syntax90.html#dao">--dao</a> nebo 
na <a href="example92.html#image">záložce možností čtení/skenování</a>.<p>

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
a vytvořit soubor oprav chyb, a pak je <a href="example60.html">znovu porovnejte</a>.
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
</table><p>

Možná náprava:<p>

<table width=100%><tr><td bgcolor=#000000 width=2><img width=1 height=1 alt=""></td><td>
Aktivujte volbu <a href="example91.html#iso">Použít informace ze souborového
systému ISO/UDF</a>, aby byla velikost obrazu určena z informací souborového
systému ISO/UDF.
</td></tr></table><p>

Pokud jsou požadované sektory ISO/UDF nečitelné, když se pokoušíte obnovit
poškozené médium, lze použít dvě dočasná řešení:

<ul>
<li>Spusťte funkci <a href="example60.html">"Porovnat"</a> jen se zadaným
souborem oprav chyb. Všimněte si dole ve výstupu správné velikosti obrazu
a příslušně <a href="example92.html#read">omezte rozsah čtení</a>.
</li>
<li>Jednoduše načtěte obraz s nesprávnou (větší) velikostí.
Když spustíte funkci <a href="example50.html#repair">"Opravit"</a>,
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


<b><a name="freebsd">3.6 Žádné mechaniky se neobjeví pod FreeBSD.</a></b><p>

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
   link30)   link_title="Lineární strategie čtení" ;;
   link40)   link_title="Adaptivní strategie čtení" ;;
   link50)   link_title="Poznámky k chybám čtení" ;;
   link60)   link_title="Tipy pro ukládání souborů oprav chyb" ;;

   content*) eval "background$page$lang $file"
	    return 0
	    ;;
   esac 
}


function background0cs()
{  cat >> $1 <<EOF

<h3>Základní informace</h3>

Informace v této podsekci nejsou vyžadovány pro práci s programem dvdisaster.
Přesto jsou užitečné k pochopení, jak program dvdisaster pracuje a mohou vám
pomoci, abyste program maximálně využili pro vaše potřeby.

<ol>
<li><a href="background10.html" title="Technické vlastnosti opravy chyb">Vlastnosti opravy chyb kódu Reed-Solomon</a><p></li>
<li><a href="background30.html">Podrobnosti o lineární strategii čtení</a><p></li>   
<li><a href="background40.html">Podrobnosti o adaptivní strategii čtení</a><p></li>   
<li><a href="background50.html">Některé poznámky k chybám čtení</a><p></li>   
<li><a href="background60.html">Tipy pro ukládání souborů oprav chyb</a><p></li>
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

Při použití <a href="example93.html#redundancy">standardního nastavení</a>
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
<a href="index10.html#scratches">Škrábance</a> způsobí dosažení této hranice dříve, 
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
Je možné <a href="example93.html#redundancy">zvolit jiné hodnoty</a>
pro vyšší nebo nižší schopnosti opravy chyb.
<p>
</font>

EOF
}


function background30cs()
{  cat >> $1 <<EOF
<h3>Lineární strategie čtení.</h3>

Program dvdisaster obsahuje dvě rozdílné
<a href="example92.html#read">strategie čtení</a>.<p>

<b>Lineární strategie čtení je doporučena pro:</b><p>
<ul>
<li>načtení obrazů z nepoškozených médií, t.j. pro tvorbu souboru oprav chyb</li>
<li>skenování média na rychlost a chyby čtení</li>
</ul>

<b><a href="background40.html">Adaptivní strategie čtení</a>  je doporučena pro:</b><p>
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
informaci o <a href="background30.html#quality">kvalitě média</a>:<p>

<center><img src="../images/bg-linear-cs.png"><br>
Částečný snímek obrazovky: lineární strategie čtení</center>

<pre> </pre>


<a name="configure"></a>
<b>Konfigurace.</b><p>

<b>Počet sektorů pro přeskočení po chybě čtení.</b>
Opakování čtení vadných sektorů je pomalé a může za nepříznivých podmínek opotřebit
mechanické díly jednotky. Řada chyb čtení rozprostírající se po souvislé oblasti sektorů
je mnohem běžnější než samostatné bodové defekty. Proto existuje
<a href="example92.html#read">konfigurační volba</a>, aby se dal přeskočit určitý počet
sektorů po výskytu chyby čtení. U přeskočených sektorů se předpokládá jejich chybovost
bez dalších pokusů o čtení. Několik návodů pro volbu počtu přeskočených sektorů:<p>

<ul>
<li>Přeskočení velkého množství sektorů (t.j. <b>1024</b>) podává rychlý přehled
o stupni poškození, ale obvykle neshromáždí dostatek dat pro opravu obrazu média.
<p></li> 
<li>Menší hodnoty jako <b>16, 32 nebo 64</b> jsou dobrým kompromisem:
Čas zpracování bude znatelně zkrácen, ale stále bude shromážděno dostatek dat pro
opravu obrazu.<p></li></ul>

Na DVD médiích se z technických důvodů chyby čtení obvykle rozšiřují přes nejméně
16 sektorů. Proto není doporučeno přeskakovat v případě DVD médií méně než 16 sektorů.
</li>
</ul>
<p>

<a name="range"></a>
<b>Omezení rozsahu čtení.</b>
Čtení může být
<a href="example92.html#read">omezeno na daný rozsah sektorů</a>. To bývá užitečné
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
<a href="example40.html">"Skenovat"</a>. V průběhu operace
<a href="example20.html">"Načíst"</a> se budou čtená data současně zapisovat
na pevný disk, což může způsobit nepravidelnosti v průběhu křivky čtení
v závislosti na operačním systému a použitém hardwaru.<p>

<a name="error"></a>
<b>Chyby čtení.</b>
Chyby čtení vytvářejí <a href="example40.html">červené značky na spirále</a> nebo
se zobrazují odpovídající hlášení <a href="syntax40.html">na příkazové řádce</a>.
Znamenají, že médium nelze v těchto místech číst v současném průchodu načítáním:
<ul>
<li>Médium je s největší pravděpodobností defektní.</li>
<li>Obraz by měl být
<a href="example50.html">opraven</a> co možná nejdříve, a pak by měl být přenesen
na nové médium.</li>
</ul>
EOF
}


function background40cs()
{  cat >> $1 <<EOF
<h3>Adaptivní strategie čtení.</h3>

Program dvdisaster obsahuje dvě rozdílné
<a href="example92.html#read">strategie čtení</a>.<p>

<b>Adaptivní strategie čtení je doporučena pro:</b><p>
<ul>
<li>vytěžení dat z poškozeného média
</li>
</ul>

<b><a href="background30.html">Lineární strategie čtení</a> je doporučena pro:</b><p>
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

<center><img src="../images/bg-adaptive-cs.png"><br>
Částečný snímek obrazovky: adaptivní strategie čtení</center>

<pre> </pre>

<a name="configure">
<b>Konfigurace</b><p>

<b>Soubor oprav chyb.</b> 
Adaptivní čtení pracuje nejlépe, když je dostupný soubor oprav chyb. 
Samozřejmě, že soubor oprav chyb musel být <a href="example30.html">vytvořen</a>
v době, kdy bylo médium ještě plně čitelné. Pro použití souboru oprav chyb
v průběhu adaptivního čtení
<a href="example30.html">zadejte jeho název</a> před spuštěním procesu čtení.<p>

<b>Omezení rozsahu adaptivního čtení.</b> Čtení může být
<a href="example92.html#read">omezeno</a> na část média.

Omezování se nedoporučuje při použití souboru oprav chyb, protože limit může
zabránit načtení sektorů, které jsou potřeba pro úspěšnou opravu chyb
Pokud není dostupný soubor oprav chyb, může být omezení rozsahu čtení užitečné
v průběhu násobných pokusů o čtení.<p>

<b>Časné ukončení čtení.</b>
Pokud není dostupný soubor oprav chyb, adaptivní čtení skončí, když nezbyly žádné
nepřečtené úseky <a href="example92.html#read">větší než zvolená velikost</a>.<p>

Hodnota pro ukončení by měla být menší než 128.
Jinak musí laserová optika provádět mnoho vystavovacích pohybů v průběhu
konečné fáze čtení. To negativně ovlivňuje jak očekávanou životnost mechaniky,
tak i její schopnosti čtení. Lepším postupem je zastavení adaptivního čtení
dříve, a poté se pokusit načíst zbývající sektory dalšími průběhy
<a href="background30.html">lineárního čtení</a>.
EOF
}


function background50cs()
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
<a href="example40.html">"Skenovat"</a> programu dvdisaster.
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
<a href="example30.html">soubor oprav chyb (soubor ecc)</a> včas.
Soubor ecc obsahuje informace pro dopočítání obsahu chybějících sektorů
<a href="background10.html" title="Technické vlastnosti opravy chyb">(v rámci určitých omezení)</a>.
Takže s pomocí souboru ecc může program dvdisaster obnovit obrazy dokonce
i v případě, že ne všechny sektory mohly být opravdu mechanikou načteny.<p>

Vzhledem k tomu, že oprava chyb může rekonstruovat chybějící sektory do určitého
množství, není nezbytné vyždímat z poškozeného média každý čitelný sektor.
<a href="background40.html">Adaptivní strategie čtení</a> kontroluje v průběhu
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
<p></li>
<li><b>Vysuňte a znovu vložte médium.</b><br>
Někdy to udělá rozdíl, když se médium vysune, pootočí se asi o čtvrtinu,
a pak se znovu zasune pro další průchod čtení.
<p></li>
<li><b>Některé mechaniky čtou lépe, pokud jsou chladné.</b><br>
Vypněte počítač přes noc a proveďte další pokus o načtení následující ráno.<p>
Ale pozor: "chladem" jsou míněny podmínky normální obytné místnosti - umístění
hardwaru nebo médií do ledničky pro ně nemusí být zdravé.<p></li>
</ul>
EOF
}


function background60cs()
{  cat >> $1 <<EOF
<h3>Tipy pro ukládání souborů oprav chyb</h3>

Zde je několik doporučení pro ukládání souborů oprav chyb:<p>

<b>1. Ukládání souborů oprav chyb na vyhrazená média:</b><p>

Prázdné DVD může typicky pojmout asi 7-10 souborů pro opravu chyb ze stejně
velkých médií, pokud se použije výchozí <a href="example93.html#redundancy">redundance</a>.

Takže se dá ukládat na 9+1 DVD (9 nesoucích skutečná data a 1 použité
pro soubory oprav chyb) s využitím běžného balení po 10 kusech. Avšak:<p>

<table width=100%><tr><td bgcolor=#000000 width=2><img width=1 height=1 alt=""></td>
<td>Vyhněte se ukládání souborů oprav chyb na média pocházející ze stejné dávky
jako ta, ze které jsou média nesoucí data, která chcete zabezpečit.
</td></tr></table><p>

Média ze stejné výrobní série mají podobné charakteristiky stárnutí,
takže mohou všechna začít selhávat ve stejnou dobu. To není dobré, protože
my chceme, aby soubory oprav chyb přežily ostatní média.<p>

Vzhledem k tomu, že dokonce média s odlišnými natištěnými štítky mohou ve skutečnosti
pocházet ze stejné výroby, je dobrou myšlenkou použít odlišné typy médií:

DVD-R a DVD+R média stejně jako média certifikovaná pro různé rychlosti zápisu
(t.j. média pro rychlosti 8x a 16x) velmi pravděpodobně pocházejí z odlišných
výrobních linek a byla vyrobena v různých dobách.<p>

<b>2. Umístění souboru oprav chyb na následující médium řady:</b><p>

Pokud nenaplníte vaše DVD na maximum (t.j. méně než 4 GB pro jednovrstvé médium),
můžete uložit soubor oprav chyb jednoho média na následující médium z řady.<p>

<b>3. Použití (externího) pevného disku pro uložení souborů oprav chyb:</b><p>

Pevný disk o kapacitě 400 GB může pojmout přibližně 600 souborů oprav chyb
(při předpokladu plných 4.7 GB médií a výchozího nastavení <a href="example93.html#redundancy">redundance</a>). 
To by mohla být další cenová alternativa, pokud budete mít na paměti,
že ani současné pevné disky ATA nejsou stavěny na věčnost. ;-)

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

