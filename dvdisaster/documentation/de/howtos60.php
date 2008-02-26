<?php
# dvdisaster: German homepage translation
# Copyright (C) 2004-2008 Carsten Gnörlich
#
# UTF-8 trigger: äöüß 
#
# Include our PHP sub routines, then call begin_page()
# to start the HTML page, insert the header, 
# navigation and news if appropriate.

require("../include/dvdisaster.php");
begin_page();
?>

<!--- Insert actual page content below --->

<h3>Das große Bild - wie die einzelnen Funktionen zusammenspielen</h3>

Die nächsten beiden Unterkapitel enthalten zwei Beispiele zur Anwendung
von dvdisaster.<p>

Jane User verwendet dvdisaster <a href="howtos61.php"> in der vorgesehenen Weise</a>.
Sie erstellt rechtzeitig Fehlerkorrektur-Daten und kann damit alle Daten von 
einem kaputt gegangenen Datenträger retten.<p>

Joe User zeigt hingegen <a href="howtos62.php">wie man es nicht machen sollte</a>. 
Er verwendet keine Fehlerkorrektur-Dateien und stellt fest,
daß er defekte Datenträger auch durch mehrmalige Versuche nicht mehr
komplett einlesen kann. Auf diese Weise verliert er Daten von einem 
kaputt gegangenen Datenträger.<p>

Die Geschichten sind natürlich frei erfunden und Ähnlichkeiten mit real existierenden
Personen oder Situationen sind rein zufällig.


<!--- do not change below --->

<?php
# end_page() adds the footer line and closes the HTML properly.

end_page();
?>
