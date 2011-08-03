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

<!--- do not change below --->

<?php
# end_page() adds the footer line and closes the HTML properly.

end_page();
?>
