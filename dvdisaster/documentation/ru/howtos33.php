<?php
# dvdisaster: English homepage translation
# Copyright (C) 2004-2010 Carsten Gnörlich
#
# UTF-8 trigger: äöüß 
#
# Include our PHP sub routines, then call begin_page()
# to start the HTML page, insert the header, 
# navigation and news if appropriate.

require("../include/dvdisaster.php");
require("../include/screenshot.php");
begin_page();
$way=$_GET["way"];
if($way & 1) $make_iso_action=$way&2;
else         $make_iso_action=$way|1;
if($way & 2) $write_iso_action=$way&1;
else	     $write_iso_action=$way|2;
?>

<!--- Insert actual page content below --->

<?php
howto_headline("Дополнение образов данными для исправления ошибок", "Прохождение", "images/create-icon.png");
?>

dvdisaster специализируется на работе с данными для исправления ошибок и чтении
поврежденных носителей. Создание ISO- или UDF-образов и запись их на носитель
является совершенно другим делом и тоже отличается высокой сложностью. 
Мы не хотим в dvdisaster'е вновь изобретать запись на носители. Вместо этого 
по-прежнему используется программное обеспечение для записи CD/DVD/BD, полученное при покупке привода.<p>

<pre> </pre>

<a name="a"></a>
<table>
<tr>
<td width="200px" align="center">
<?php
echo "<a href=\"howtos33.php?way=$make_iso_action\">\n";
?>
<img src="thumbnails/make-iso1.png" border="0">
<br><img src="../images/down-arrow.png" border="0"></a></td>
<td>&nbsp;&nbsp;</td>
<td valign="top"><b>Сначала создайте ISO-образ</b> с помощью своего ПО
для записи CD/DVD/BD. Выберите файлы, которые нужно записать на носитель,
но пока не начинайте процесс записи. Вместо этого создайте ISO-образ
на жестком диске. Нажмите на картинку слева и увидите 
<?php
echo "<a href=\"howtos33.php?way=$make_iso_action\">\n";
echo "подробное прохождение</a>.\n";
?>
</td>
</tr>
</table>

<?php
if($way&1)
{
?>

<hr>

<b>Подробный пример: создание ISO-образа на жестком диске</b>. 
Так как существует много разных программ для записи носителей,
мы демонстрируем требуемую последовательность действий с помощью популярной
GNU/Linux программы <i>K3b</i>, взятой в качестве примера.
Если вы используете другое программное обеспечение, то вам будет легко понять,
что нужно делать, по приведенным ниже описаниям.
<p>

<hr>

<?php begin_screen_shot("Начало нового проекта","make-iso1.png"); ?>
<b>Начать новый проект.</b>
Сначала откройте свое приложение для записи носителей. Многие программы ожидают, что вы
начнете новый проект, в котором затем выберете запись нового носителя. <p>
При использовании K3b: <i>Начните новый проект, нажав мышкой на выделенное поле
("Создать проект CD с данными") в главном окне.</i>
<?php end_screen_shot(); ?>

<hr>

<?php begin_screen_shot("Выбор файла","make-iso2.png"); ?>
<b>Выберите файлы, которые нужно записать на носитель.</b>
Обычно есть диалог выбора файлов, в котором можно выбрать файлы
или перетащить их в проект.<p>

При использовании K3b: <i>Выберите нужные файлы в верхней половине окна.
В примере для записи на CD выбраны файлы <i>backup.tar.gz</i>,
<i>win.zip</i> и <i>work.tar.gz</i>.
Выбранные файлы показываются в нижней половине окна.</i><p>

<b>Важно:</b> Не заполняйте полностью весь носитель. Убедитесь, что остается
не менее 20% места на носителе для данных для исправления ошибок.<p>

При использовании K3b: <i> Использованное место на носителе показывается зеленой полосой
внизу экрана (539,8 MB).</i>
<?php end_screen_shot(); ?>

<hr>

<?php begin_screen_shot("Настройка программного обеспечения для записи","make-iso2.png"); ?>
<b>Настройка программного обеспечения для записи.</b> Перед началом самого процесса записи программа даст вам выбрать цель для записи.
<b>Не</b> выбирайте здесь пишущий привод CD/DVD/BD, а настройте
создание ISO/UDF-образа на жестком диске, как описывается ниже.<p>

<b>Совет:</b> Прежде, чем продолжить, уберите все носители из приводов, чтобы случайно
не начать процесс записи.<p>

При использовании K3b: <i>Откройте вкладку диалога записи, нажав на кнопку "Записать" 
вблизи левого края окна.</i>
<?php end_screen_shot(); ?>

<?php begin_screen_shot("Выбор создания образа","make-iso3.png"); ?>
<b>Выбор создания образа.</b> Большинство записывающих программ просто позволяют
сделать выбор для создания ISO-образа на жестком диске.
Если в вашей программе, по-видимому, нет такой возможности, вам, возможно, придется выбрать
"image recorder" вместо реального пишущего привода.<p>

При использовании K3b: <i>Выберите вкладку "Запись". 
Активируйте параметр "Создать только образ" (помечен зеленым).</i>
<?php end_screen_shot(); ?>

<?php begin_screen_shot("Выбора файла образа","make-iso4.png"); ?>
<b>Выберите файл и тип образа.</b>
Выберите целевой каталог, имя и тип для файла образа.
Используйте только файлы образов типа ".iso" или ".udf"! Другие форматы образов, например,
".nrg", не поддерживаются dvdisaster; обработка таких образов с помощью
dvdisaster приведет к тому, что они будут считаться непригодными без дополнительного уведомления
или сообщения об ошибке.<p>

При использовании K3b: <i>Выберите вкладку "Образ". 
Введите целевой каталог для файла 
(в примере файл "medium.iso" будет помещен в подкаталог 
"/var/tmp/cg"). K3b всегда создает образы .iso, поэтому
не предлагается выбор типа образа.</i>
<?php end_screen_shot(); ?>

<hr>

<table>
<tr>
<td width="200px" align="center">
<img src="../images/down-arrow.png" border="0"></td>
<td>&nbsp;&nbsp;</td>
<td valign="top"></td>
</tr>
</table>

<?php
}  /* end from if($way&1) above */
?>

<a name="b"></a>
<table>
<tr>
<td width="200px" align="center">
<img src="../images/good-image.png" border="0">
<br><img src="../images/down-arrow.png" border="0"></a></td>
<td>&nbsp;&nbsp;</td>
<td valign="top">Когда образ будет готов,
<b>переключитесь в dvdisaster</b>. Убедитесь, что он сконфигурирован,
как описано в <a href="howtos32.php">основных параметрах</a>.
</td>
</tr>
</table>

<table>
<tr>
<td width="200px" align="center">
<a href="howtosa2.php">
<img src="../images/select-image2.png" border="0">
<br><img src="../images/down-arrow.png" border="0"></a></td>
<td>&nbsp;&nbsp;</td>
<td valign="top"><b>Выберите каталог и имя файла</b> 
ISO-образа, который вы только что создали.</td>
</tr>
</table>

<table>
<tr>
<td width="200px" align="center">
<a href="howtosa4.php">
<img src="images/create-icon.png" border="0">
<br><img src="../images/down-arrow.png" border="0"></a></td>
<td>&nbsp;&nbsp;</td>
<td valign="top"><b>Дополните образ данными для исправления ошибок,</b> нажав
на кнопку "Создать".</td>
</tr>
</table>

<?php begin_howto_shot("Создание данных для исправления ошибок.","make-ecc1.png", "down-arrow.png"); ?>
<b>Подождите, пока не будут созданы данные для исправления ошибок.</b>
Это займет некоторое время в зависимости от размера образа и 
имеющегося свободного места на носителе. Обработка DVD-образа с приблизительно
20-30% свободного места должна занять примерно 5-10 минут на современном оборудовании.
<?php end_howto_shot(); ?>

<?php begin_howto_shot("Сравнение размеров образов.","make-ecc2.png", "down-arrow.png"); ?>
<b>Имейте в виду:</b> dvdisaster не создает новый образ, а дополняет существующий
. Посмотрите на файл образа в диспетчере файлов до
и после обработки образа dvdisaster'ом и обратите внимание на то, как увеличивается его размер.
<?php end_howto_shot(); ?>

<a name="c"></a>
<table>
<tr>
<td width="200px" align="center">
<?php
echo "<a href=\"howtos33.php?way=$write_iso_action\">\n";
?>
<img src="thumbnails/write-iso1.png" border="0"></a></td>
<td>&nbsp;&nbsp;</td>
<td valign="top"><b>Записать дополненный ISO-образ</b> на носитель.
Выберите дополненный образ в своей программе для записи дисков и
начните процесс записи. Нажмите на снимок с экрана слева для получения
<?php
echo "<a href=\"howtos33.php?way=$write_iso_action\">\n";
echo "подробного примера</a>.\n";
?>
</td>
</tr>
</table>

<?php
if($way&2)
{
?>
<table>
<tr>
<td width="200px" align="center">
<img src="../images/down-arrow.png" border="0"></td>
<td>&nbsp;&nbsp;</td>
<td valign="top"></td>
</tr>
</table>

<hr>

<b>Поробный пример: запись ISO-образа на носитель</b>.<p>

<?php begin_screen_shot("Выбор записи образа","write-iso1.png"); ?>
<b>Выберите запись образа.</b>
Еще раз откройте свое программное обеспечение для записи носителей. Запустите режим записи
подготовленных .iso-образов на носитель.<p>

При использовании K3b: <i>Нажмите мышкой на выделенное поле
("Записать образ CD") в главном окне.</i>
<?php end_screen_shot(); ?>

<hr>

<?php begin_screen_shot("Выбор образа","write-iso2.png"); ?>
<b>Выбор образа.</b>
Выберите образ, который был только что создан и дополнен с помощью dvdisaster.
<p>

При использовании K3b: <i>С помощью поля, помеченного зеленым, выберите файл образа или
прямо введите его имя.</i>
<?php end_screen_shot(); ?>

<?php begin_screen_shot("Дополнительные параметры","write-iso2.png"); ?>
<b>Дополнительные параметры.</b>
Выберите режим записи "DAO" ("диск целиком"), если он поддерживается вашим приводом.
Это повышает совместимость носителя и исправления ошибок.
Вдобавок, это предотвращает случайное добавление дополнительных сессий на 
диск, что уничтожит данные для исправления ошибок.
<p>

При использовании K3b: <i>Выберите "DAO" в поле, отмеченном желтым.</i>
<?php end_screen_shot(); ?>

<?php begin_screen_shot("Запись носителя","write-iso3.png"); ?>
<b>Запись носителя.</b>
Теперь начните процесс записи.
<p>

При использовании K3b: <i>Нажмите на кнопку "Начать" в окне на предыдущем снимке с экрана.</i>
<?php end_screen_shot(); ?>

<hr>

<?php
}  /* end from if($way$2) above */
?>

<table>
<tr>
<td width="200px" align="center">
<img src="../images/down-arrow.png" border="0"></td>
<td>&nbsp;&nbsp;</td>
<td valign="top"></td>
</tr>
</table>

<table>
<tr>
<td width="200px" align="center">
<img src="../images/good-cd-ecc.png" border="0"></td>
<td>&nbsp;&nbsp;</td>
<td valign="top"><b>Готово!</b> Вы только что создали CD, защищенный
данными для исправления ошибок.</td>
</tr>
</table>

<pre> </pre>

<b>Соответствующая информация</b>

<ul>
<li><a href="howtos90.php">Проверьте, не повредил ли процесс записи
данные для исправления ошибок.</a><p>
Рекомендуется проводить этот тест всякий раз при смене
версии (или поставщика) программного обеспечения для записи носителей, чтобы убедиться, что оно хорошо взаимодействует с dvdisaster.
</li>
</ul>


<!--- do not change below --->

<?php
# end_page() adds the footer line and closes the HTML properly.

end_page();
?>
