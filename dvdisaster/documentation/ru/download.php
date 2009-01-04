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
?>

<!--- Insert actual page content below --->

<h3>Предстоит сделать</h3>

dvdisaster имеется для <a href="download10.php">последних версий</a>
операционных систем Darwin/Mac OS X, FreeBSD, Linux, NetBSD и Windows.<p>

Ниже приводится список ссылок для скачивания исходного текста или двоичного пакета.
<a href="download20.php">Цифровая подпись</a> приводится для проверки того, что
пакеты - в первоначальном состоянии.<p>

Для <a href="download30.php">установки</a> двоичного варианта выполните 
загруженную программу и следуйте инструкциям.
В составе исходного текста имеется файл <tt>INSTALL</tt> с дальнейшими инструкциями.

<pre> </pre>

<!---
<b>Версии для разработчиков</b> - новые и экспериментальные для опытных пользователей!<p>
<b>Текущая версия</b><p>
-->

<b>Альфа (нестабильные) версии</b> - новые и экспериментальные для опытных пользователей!<p>

Приглашаем тестеров для предстоящих версий dvdisaster, но следует иметь в виду, что
остались еще ошибки и несоответствия.
Текущая нестабильная версия - 
<a href="download40.php"><?php echo ${cooked_version}?></a>.
<pre> </pre>

<b>Стабильная версия</b> - рекомендуется для начала.<p>

<table class="download" cellpadding="0" cellspacing="5">
<tr><td><b>dvdisaster-0.70</b></td><td align="right">04-03-2008</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
  <table>
    <tr><td align="right">&nbsp;&nbsp;Исходный текст для всех операционных систем:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.70.6.tar.bz2?download">dvdisaster-0.70.6.tar.bz2</a></td></tr>
    <tr><td align="right">Цифровая подпись:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.70.6.tar.bz2.gpg?download">dvdisaster-0.70.6.tar.bz2.gpg</a></td></tr>
    <tr><td align="right">Двоичный пакет для Windows:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.70.6-setup.exe?download">dvdisaster-0.70.6-setup.exe</a></td></tr>
    <tr><td align="right">Цифровая подпись:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.70.6-setup.exe.gpg?download">dvdisaster-0.70.6-setup.exe.gpg</a></td></tr>
  </table>
</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
Метод коррекции ошибок RS02
полностью поддерживается в графическом интерфейсе. Образы, созданные
с использованием RS02, могут использоваться
со стратегией адаптивного чтения.<p>

Джулиан Айнваг (Julian Einwag) начал перенос dvdisaster 
на Mac OS X / Darwin.<p>

Дэниэл Найландер (Daniel Nylander) дал шведский перевод экранных текстов.<p>

<b>Исправления</b> (небольшие изменения после версии 0.70; вышеприведенные файлы были обновлены):<p>

<b>pl6</b> Сделан откат назад поддержки локализованных имен файлов, 
поскольку она нарушила поддержку больших файлов под Windows. Новый обработчик
для локализованных имен файлов будет сначала протестирован в экспериментальной версии
0.71.25. <i>(04-мар-2008)</i><p>

<b>pl5</b> 
Исправляет проблему с новыми ядрами Linux, которая может привести к зависанию системы в
некоторых случаях. Улучшена обработка имен файлов: содержащих локализованные
символы. 
Содержит обратный перенос основных исправлений ошибок с 0.71.24. <i>(24-фев-2008)</i>.<p>

<b>pl4</b> обеспечивает лучшую совместимость с двухслойными DVD
(DVD-R DL и DVD+R DL).<br> 
Были исправлены некоторые незначительные ошибки. <i>(20-янв-2007)</i>.<p>

<b>pl3</b> исправляет неправильное распознавание неподдерживаемых форматов CD, которые
приводили к синему экрану Windows при редком стечении обстоятельств. Добавлена возможность отмены
во время инициализации RS02 на носителях DVD RW.
<i>(10-дек-2006)</i>.<p>

<b>pl2</b> исправляет неправильное освобождение памяти, когда закрывается окно программы.
Исправлена распаковка снимков экрана в документации для платформ PPC.
Обновлены только архивы исходных текстов.
<i>(03-окт-2006)</i>.<p>

<b>pl1</b> исправляет ошибку в адаптивном чтении для RS02, которая иногда приводит к
чтению недосточного количества данных при сообщении об успешном восстановлении. Добавляет несколько
небольших улучшений в документации и функциональности. <i>(30-июл-2006)</i>
</td></tr></table><p>

Исходные тексты dvdisaster доступны также
<a href="http://sourceforge.net/cvs/?group_id=157550">через CVS</a>.
<pre> </pre>

<b>Предыдущая версия</b> - рекомендуется обновление до версии 0.70.4.<p>

<table class="download" cellpadding="0" cellspacing="5">
<tr><td><b>dvdisaster-0.66</b></td><td align="right">25-мар-2006</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
  <table>
    <tr><td align="right">&nbsp;&nbsp;Исходные тексты для всех операционных систем:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.66.tar.bz2?download">dvdisaster-0.66.tar.bz2</a></td></tr>
    <tr><td align="right">Цифровая подпись:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.66.tar.bz2.gpg?download">dvdisaster-0.66.tar.bz2.gpg</a></td></tr>
    <tr><td align="right">Двоичный файл для Windows:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.66.3-setup.exe?download">dvdisaster-0.66.3-setup.exe</a></td></tr>
    <tr><td align="right">Цифровая подпись:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.66.3-setup.exe.gpg?download">dvdisaster-0.66.3-setup.exe.gpg</a></td></tr>
  </table>
</td></tr>
</table><p>

<!--- do not change below --->

<?php
# end_page() adds the footer line and closes the HTML properly.

end_page();
?>
