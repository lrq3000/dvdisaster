<?php
# dvdisaster: Russian homepage translation
# Copyright (C) 2007-2009 Igor Gorbounov
#
# UTF-8 trigger: äöüß 
#
# Include our PHP sub routines, then call begin_page()
# to start the HTML page, insert the header, 
# navigation and news if appropriate.

require("../include/dvdisaster.php");
begin_page();
$show_all=$_GET["showall"];
?>

<!--- Insert actual page content below --->

<h3>Альфа-версии (для разработчиков)</h3>

<b>Помогите нам с тестированием!</b> На этой странице находятся экспериментальные версии dvdisaster,
создаваемые на пути к следующему стабильному выпуску.<p>

<b>Предупреждение:</b> Альфа-версии не прошли тщательного тестирования. В них может
быть больше ошибок, чем в стабильной версии, и их не следует использовать
для обработки важных данных.<p>

Если есть сомнения, то продолжайте использовать <a href="download.php">стабильную версию 0.72</a>
и ждите выпуска версии 0.74.

<hr>

<h3>Planned changes in the new version</h3>

All platforms:

<ul>
<li> Implement some small additions which have been put on hold
during the 0.72 development cycle.</li>
<li> Remove obsolete functionality.</li>
<li> Clean up source code and prepare for multithreading and multi core 
processors.</li>
</ul>

Windows:

<ul>
<li> Update the GTK+ toolkit and development system.</li>
<li> Raise system requirements to Windows 2000 or newer (older
Windows releases are no longer supported by the development tools).
This makes support for ASPI drivers and splitting files into 2G
segments obsolete.</li>
</ul>

MacOS:

<ul>
<li> Update the GTK+ toolkit and provide more workarounds
for the graphical user interface.</li>
</ul>

<hr>

<h3>Загрузки</h3>
<a name="download"></a>

Для альфа-версий используется такой же формат пакетов, как и для нормальных версий.<p>

<table class="download" cellpadding="0" cellspacing="5">
<tr><td><b>dvdisaster-0.73</b></td><td align="right">4 июля 2009</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
  <table>
    <tr><td align="right">&nbsp;&nbsp;Исходные тексты для всех операционных систем:&nbsp;</td>
        <td><a href="http://dvdisaster.net/downloads/dvdisaster-0.73.1.tar.bz2">dvdisaster-0.73.1.tar.bz2</a></td></tr>
    <tr><td align="right">Цифровая подпись:&nbsp;</td>
        <td><a href="http://dvdisaster.net/downloads/dvdisaster-0.73.1.tar.bz2.gpg">dvdisaster-0.73.1.tar.bz2.gpg</a></td></tr>
    <tr><td align="right">Двоичная версия для Mac OS X 10.5 / x86:&nbsp;</td>
        <td><a href="http://dvdisaster.net/downloads/dvdisaster-0.73.1.app.zip">dvdisaster-0.73.1.app.zip</a>&nbsp;--&nbsp;сначала прочитайте эти <a href="download30.php#mac">советы</a></td></tr>
    <tr><td align="right">Цифровая подпись:&nbsp;</td>
        <td><a href="http://dvdisaster.net/downloads/dvdisaster-0.73.1.app.zip.gpg">dvdisaster-0.73.1.app.zip.gpg</a></td></tr>
    <tr><td align="right">Двоичная версия для Windows:&nbsp;</td>
        <td><a href="http://dvdisaster.net/downloads/dvdisaster-0.73.1-setup.exe">dvdisaster-0.73.1-setup.exe</a></td></tr>
    <tr><td align="right">Цифровая подпись:&nbsp;</td>
        <td><a href="http://dvdisaster.net/downloads/dvdisaster-0.73.1-setup.exe.gpg">dvdisaster-0.73.1-setup.exe.gpg</a></td></tr>
    <tr><td colspan="2"> </td></tr>
<?php
  if($show_all == 0) {
?>
    <tr><td colspan="2"><a href="download40.php?showall=1#download">Show older releases in the 0.73 version branch</a></td></tr>
<?php
  }
  else {
?> 
    <tr><td colspan="2"><a href="download40.php?showall=0#download">Hide older releases in the 0.73 version branch</a></td></tr>
    <tr><td align="right">&nbsp;&nbsp;Исходные тексты для всех операционных систем:&nbsp;</td>
        <td><a href="http://dvdisaster.net/downloads/dvdisaster-0.73.0.tar.bz2">dvdisaster-0.73.0.tar.bz2</a></td></tr>
    <tr><td align="right">Цифровая подпись:&nbsp;</td>
        <td><a href="http://dvdisaster.net/downloads/dvdisaster-0.73.0.tar.bz2.gpg">dvdisaster-0.73.0.tar.bz2.gpg</a></td></tr>
    <tr><td align="right">Двоичная версия для Mac OS X 10.5 / x86:&nbsp;</td>
        <td><a href="http://dvdisaster.net/downloads/dvdisaster-0.73.0.app.zip">dvdisaster-0.73.0.app.zip</a>&nbsp;--&nbsp;сначала прочитайте эти <a href="download30.php#mac">советы</a></td></tr>
    <tr><td align="right">Цифровая подпись:&nbsp;</td>
        <td><a href="http://dvdisaster.net/downloads/dvdisaster-0.73.0.app.zip.gpg">dvdisaster-0.73.0.app.zip.gpg</a></td></tr>
    <tr><td align="right">Двоичная версия для Windows:&nbsp;</td>
        <td><a href="http://dvdisaster.net/downloads/dvdisaster-0.73.0-setup.exe">dvdisaster-0.73.0-setup.exe</a></td></tr>
    <tr><td align="right">Цифровая подпись:&nbsp;</td>
        <td><a href="http://dvdisaster.net/downloads/dvdisaster-0.73.0-setup.exe.gpg">dvdisaster-0.73.0-setup.exe.gpg</a></td></tr>
<?php
  }
?>
  </table>
</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
<b>0.73.1</b> Not yet released. (19 апреля 2009)
</td></tr></table><p>

<!--- do not change below --->

<?php
# end_page() adds the footer line and closes the HTML properly.

end_page();
?>
