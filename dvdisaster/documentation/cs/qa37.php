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

<!--- do not change below --->

<?php
# end_page() adds the footer line and closes the HTML properly.

end_page();
?>
