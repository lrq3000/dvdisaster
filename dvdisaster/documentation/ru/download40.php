<?php
# dvdisaster: Russian homepage translation
# Copyright (C) 2007-2008 Igor Gorbounov
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

<h3>Альфа версии (для разработчиков)</h3>

<b>Помогите нам тестировать!</b> У нас в разработке находится несколько новых функций, и
нам хотелось бы, чтобы они были протестированы большим количеством людей, прежде чем мы выпустим следующую официальную версию.<p>

Самое главное, была добавлена поддержка приводов BD и HD DVD, и она требует более интенсивных
полевых испытаний. Описанный в разделе "Howtos" <a href="howtos90.php">тест совместимости образов</a> 
очень полезен для тестирования новых приводов. Пожалуйста,
<a href="feedback.php">сообщайте как о положительных, так и негативных результатах</a>,
чтобы мы смогли получить лучшее представление о том, как эти новые форматы работают.<p>

<b>Предупреждение:</b> Альфа-версии могут заблокировать привод и/или операционную
систему, и потребуется перезагрузка. Они могут содержать другие ошибки,
и их не следует использовать для обработки носителей с важными данными.<p>

Если сомневаетесь, продолжайте пользоваться <a href="download.php">стабильной версией 0.70</a>
и ждите выпуска версии 0.72.

<hr>

<h3>Загрузки</h3>

Для альфа-версий используется такой же формат пакетов, как и для нормальных версий.<p>


<table class="download" cellpadding="0" cellspacing="5">
<tr><td><b>dvdisaster-0.71 (devel26)</b></td><td align="right">30-Mar-2008</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
  <table>
    <tr><td align="right">&nbsp;&nbsp;Source code for all operating systems:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.26.tar.bz2?download">dvdisaster-0.71.26.tar.bz2</a></td></tr>
    <tr><td align="right">Digital signature:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.26.tar.bz2.gpg?download">dvdisaster-0.71.26.tar.bz2.gpg</a></td></tr>
    <tr><td align="right">Binary for Windows:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.26-setup.exe?download">dvdisaster-0.71.26-setup.exe</a></td></tr>
    <tr><td align="right">Digital signature:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.26-setup.exe.gpg?download">dvdisaster-0.71.26-setup.exe.gpg</a></td></tr>
  </table>
</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
This version contains a new dialog providing information about inserted media;
during this course detection of media types (CD/DVD/BD) was changed. Improved
support for BD media.
</td></tr></table><p>

<table class="download" cellpadding="0" cellspacing="5">
<tr><td><b>dvdisaster-0.71 (devel25)</b></td><td align="right">05-Mar-2008</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
  <table>
    <tr><td align="right">&nbsp;&nbsp;Source code for all operating systems:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.25.tar.bz2?download">dvdisaster-0.71.25.tar.bz2</a></td></tr>
    <tr><td align="right">Digital signature:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.25.tar.bz2.gpg?download">dvdisaster-0.71.25.tar.bz2.gpg</a></td></tr>
    <tr><td align="right">Binary for Windows:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.25-setup.exe?download">dvdisaster-0.71.25-setup.exe</a></td></tr>
    <tr><td align="right">Digital signature:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.25-setup.exe.gpg?download">dvdisaster-0.71.25-setup.exe.gpg</a></td></tr>
  </table>
</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
The method for processing localized file names was replaced with a new
approach as the old one was broken under Windows.
</td></tr></table><p>

<table class="download" cellpadding="0" cellspacing="5">
<tr><td><b>dvdisaster-0.71 (devel24)</b></td><td align="right">24-02-2007</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
  <table>
    <tr><td align="right">&nbsp;&nbsp;Исходный код для всех операционных систем:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.24.tar.bz2?download">dvdisaster-0.71.24.tar.bz2</a></td></tr>
    <tr><td align="right">Цифровая подпись:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.24.tar.bz2.gpg?download">dvdisaster-0.71.24.tar.bz2.gpg</a></td></tr>
    <tr><td></td><td>Version 0.71.24 is broken under Windows.</td></tr>
  </table>
</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
Fixes a problem with newer Linux kernels which may lead to a frozen system under some circumstances. Treatment of file names containing localized characters has been improved. This release marks the start of an online documentation rewrite, including a Russian translation made by Igor Gorbounov.
</td></tr></table><p>


<table class="download" cellpadding="0" cellspacing="5">
<tr><td><b>dvdisaster-0.71 (devel23)</b></td><td align="right">06-май-2007</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
  <table>
    <tr><td align="right">&nbsp;&nbsp;Исходный код для всех операционных систем:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.devel23.tar.bz2?download">dvdisaster-0.71.devel23.tar.bz2</a></td></tr>
    <tr><td align="right">Цифровая подпись:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.devel23.tar.bz2.gpg?download">dvdisaster-0.71.devel23.tar.bz2.gpg</a></td></tr>
    <tr><td align="right">Двоичный файл для Windows:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.devel23-setup.exe?download">dvdisaster-0.71.devel23-setup.exe</a></td></tr>
    <tr><td align="right">Цифровая подпись:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.devel23-setup.exe.gpg?download">dvdisaster-0.71.devel23-setup.exe.gpg</a></td></tr>
  </table>
</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
Добавлена поддержка C2-сканирования CD-носителей.<br>
Андрей Грекю (Andrei Grecu) предложил улучшенные версии эвристики низкоуровневого восстановления.
</td></tr></table><p>

<table class="download" cellpadding="0" cellspacing="5">
<tr><td><b>dvdisaster-0.71 (devel22)</b></td><td align="right">22-апр-2007</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
  <table>
    <tr><td align="right">&nbsp;&nbsp;Исходный код для всех операционных систем:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.devel22.tar.bz2?download">dvdisaster-0.71.devel22.tar.bz2</a></td></tr>
    <tr><td align="right">Цифровая подпись:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.devel22.tar.bz2.gpg?download">dvdisaster-0.71.devel22.tar.bz2.gpg</a></td></tr>
    <tr><td align="right">Двоичный файл для Windows:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.devel22-setup.exe?download">dvdisaster-0.71.devel22-setup.exe</a></td></tr>
    <tr><td align="right">Цифровая подпись:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.devel22-setup.exe.gpg?download">dvdisaster-0.71.devel22-setup.exe.gpg</a></td></tr>
  </table>
</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
Игорь Горбунов начал русский перевод экранных текстов.<p>
Добавлен выбор режимов низкоуровневого чтения 20h/21h для того, чтобы дать возможность чтения
поврежденных секторов на большем количестве приводов. Исправлены некоторые моменты неэффективной работы во время низкоуровневого восстановления секторов.
</td></tr></table><p>

<table class="download" cellpadding="0" cellspacing="5">
<tr><td><b>dvdisaster-0.71 (devel21)</b></td><td align="right">15-апр-2007</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
  <table>
    <tr><td align="right">&nbsp;&nbsp;Исходный код для всех операционных систем:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.devel21.tar.bz2?download">dvdisaster-0.71.devel21.tar.bz2</a></td></tr>
    <tr><td align="right">Цифровая подпись:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.devel21.tar.bz2.gpg?download">dvdisaster-0.71.devel21.tar.bz2.gpg</a></td></tr>
    <tr><td align="right">Двоичный файл для Windows:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.devel21-setup.exe?download">dvdisaster-0.71.devel21-setup.exe</a></td></tr>
    <tr><td align="right">Цифровая подпись:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.devel21-setup.exe.gpg?download">dvdisaster-0.71.devel21-setup.exe.gpg</a></td></tr>
  </table>
</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
Андрей Грекю (Andrei Grecu) предоставил алгоритм восстановления поврежденных CD-секторов
с помощью множественных попыток незавершенного низкоуровневого чтения.
Сергей Свишчев (Sergey Svishchev) помог портировать dvdisaster на NetBSD.<p>

Среди других возможностей: выбираемое число попыток чтения, проверка
CD секторов с помощью необработанных данных L-EC и EDC и переделанный диалог настроек.
В стратегии адаптивного чтения была улучшена работа с контрольной суммой RS02. 
</td></tr></table><p>

<!--- do not change below --->

<?php
# end_page() adds the footer line and closes the HTML properly.

end_page();
?>
