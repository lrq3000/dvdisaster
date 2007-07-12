#! /bin/bash

#  dvdisaster: Russian homepage translation
#  Copyright (C) 2007 Igor Gorbounov
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

function dictionary_ru()
{  trans_encoding="UTF-8"

   trans_version="Версия"

   trans_to_internet="К интернет-версии"
   trans_to_sourceforge="Краткое изложение (на SourceForge.net)"

   trans_back="Назад"
   trans_screen_shot="Screen shot"

   trans_contents="Содержание"
   trans_news="News"
   trans_hosting="Размещено на"

   trans_fdl="Дословное копирование и распространение всей этой статьи разрешается на любом носителе, при условии, что это уведомление сохраняется."
   trans_copyright="Copyright 2007 Igor Gorbounov."
   trans_modified="Информация в этом руководстве относится к первоначальной версии программы dvdisaster и не может быть применима."
}

# ----- Individual page contents 

# ----- News page (special case: has no sub pages)

function news_contents_ru()
{  local file_base=$1;
   local query=$2
   local page=$3
   local lang=$4
   local expand=$5
   local file;

   if [ $expand != 0 ]; 
     then file="${file_base}_${expand}.html";
     else file="${file_base}.html";
   fi

   case $query in
   title)   title="dvdisaster News" ;;

   link)    link_title="News" ;;
   link0)   link_title="News" ;;

   content*) eval "news$page$lang $file $file_base $expand full" ;;
   headlines*) eval "news$page$lang $file $file_base $expand headlines" ;;
   esac 
}

function news0ru()
{  local file=$1
   local file_base=$2
   local expand=$3
   local mode=$4

   if [ $mode == "full"  ]; then
     echo "<h3>dvdisaster News</h3>">>$file
   fi

   news_item $file "xx.xx.2007" "dvdisaster 0.71.24 released" $mode <<END_NEWS_ITEM
   dvdisaster 0.71.24 has just been released. This release marks the start of
   an online documentation rewrite, including a Russian translation made
   by Igor Gorbounov. Also new are the raw CD sector editor and some improvements
   in the raw reading heuristics. 
END_NEWS_ITEM

   news_item $file "xx.xx.2007" "German documentation unavailable" $mode <<END_NEWS_ITEM
   During the transition to the new documentation the German translation will be unavailable.
   Thanks for your understanding.<p>
END_NEWS_ITEM

   news_item $file "07.07.2007" "New documentation started" $mode <<END_NEWS_ITEM
   The dvdisaster documentation is currently being reworked for the upcoming
   V0.72 release. Please be patient; the new documentation will hopefully be more
   useful than the old one, but we will need a few weeks to fill in all parts.
END_NEWS_ITEM
}

# ----- Default home page

function index_contents_ru()
{  local file_base=$1;
   local query=$2
   local page=$3
   local lang=$4
   local expand=$5
   local file;

   if [ $expand != 0 ]; 
     then file="${file_base}_${expand}.html";
     else file="${file_base}.html";
   fi

   case $query in
   title)   title="Обзор" ;;

   link)    link_title="Обзор" ;;
   link0)   link_title="Обзор" ;;
   link10)   link_title="Примеры исправления ошибок" ;;
   link20)   link_title="dvdisaster дополняет проверки качества" ;;
   link30)   link_title="dvdisaster: за и против" ;;

   content*) eval "index$page$lang $file $file_base $expand" ;;
   esac 
}


function index0ru()
{  cat >> $1 <<EOF
<h3>Проект dvdisaster:</h3>

CD и DVD носители хранят данные лишь ограниченное время (обычно в течение многих лет).
По прошествии этого времени постепенно развивается потеря данных, при этом ошибки чтения 
распространяются от внешних областей носителя к внутренним.<p>

<b>Архивирование с защитой от потери данных</b><p>

dvdisaster хранит данные на CD/DVD (<a href="qa10.html#media">поддерживаемые носители</a>)
таким образом, что они полностью восстанавливаются даже после того, как образовалось некоторое количество ошибок чтения. 
Это дает Вам возможность спасти свои данные и сохранить их на новом носителе.<p>

Потеря данных предотвращается за счет 
использования <a href="background11.html">кодов коррекции ошибок</a>.
Данные для исправления ошибок либо добавляются к носителю, либо хранятся в отдельных
файлах коррекции ошибок. dvdisaster работает на 
<a href="background20.html">уровне образа</a>, поэтому восстановление не зависит 
от файловой системы носителя.
Возможности исправления ошибок настраиваются пользователем.<p>

<b>Обычные недоразумения по поводу программы dvdisaster:</b>

<ul>
<li>dvdisaster не может сделать так, чтобы поврежденные носители вновь стали читаемыми.
Содержимое поврежденного носителя <i>не</i> может быть восстановлено без 
данных для исправления ошибок.<p></li> 
<li><img src="images/exclude_from_search_terms.png" align="middle"><br>
Такие функции выходят за рамки целей и внутренней архитектуры программы dvdisaster.</li>
</ul>

<p>
<a href="index10.html">Примеры исправления ошибок...</a>
EOF
}

function index10ru()
{  local file=$1
   local file_base=$2
   local expand=$3

   cat >> $1 <<EOF
<h3>Примеры исправления ошибок</h3>
EOF

   screenshot $file_base "Чтение поврежденного носителя." "recover_linear.png" $expand <<END_SCREENSHOT

   <b>Восстановление старого носителя.</b> Обработанный здесь носитель стал
   обесцвеченным и частично нечитаемым во внешней области. Проверка поверхности
   дает примерно 23.000 нечитаемых секторов из общего числа 342.000 секторов; отсюда
   поврежденных секторов около 7,2%.
END_SCREENSHOT

   screenshot $file_base "Чтение поврежденного образа." "fix_image.png" $expand <<END_SCREENSHOT

   <b>Восстановление поврежденного образа.</b> Полученный образ все еще неполный,
   поскольку примерно 23.000 секторов не могут быть прочитаны. Эти сектора теперь 
   реконструируются с помощью 
   <a href="background11.html#eccblock">данных для исправления ошибок</a>,
   созданных программой dvdisaster.
   В процессе восстановления встретилось максимум 20 ошибок
   на <a href="background11.html#eccblock">ECC блок</a>. Это приводит к пиковой
   загрузке алгоритма коррекции ошибок в 63%, что означает, что с повреждением такой степени хорошо справляются
   данные для исправления ошибок, созданные с помощью стандартных настроек.
END_SCREENSHOT

   cat >> $1 <<EOF

<b>Для восстановления требуются данные для исправления ошибок:</b> В описанном выше процессе восстановления используются
<a href="background11.html">данные для исправления ошибок</a> ("ecc данные"). 
Представьте себе эти данные как особую
форму резервных данных (хотя для них требуется меньше места, чем для обычного резервирования).
Как и в случае обычного резервного копирования, ecc-данные необходимо создать
<i>прежде</i>, чем носитель станет поврежденным.<p>

Поэтому, если у Вас есть поврежденный носитель, но Вы для него не создавали ecc-данных - увы, 
Ваши данные, вероятно, потеряны.<p>


<a href="index20.html">Почему проверки качества носителя недостаточно...</a>
EOF
}


function index20ru()
{  cat >> $1 <<EOF
<h3>dvdisaster как дополнение к проверке качества носителя</h3>

<a href="qa.html#pipo">Проверка качества носителя</a>, например, проверка на ошибки C2 или PI/PO, являются ценным  
инструментом проверки результатов процесса записи носителей.<p>

Но проверка качества <b>не</b> является надежным средством <b>предсказания 
срока службы </b> оптических носителей. <br>
Предположим, что мы ищем правильное время
для копирования изношенного носителя на новый:

<ul>
<li>Слишком рано: Копирование носителя из-за плохого результата проверки качества неэффективно с экономической точки зрения.
Иногда такой носитель остается читаемым намного дольше, чем ожидалось.<p></li>
<li>Слишком поздно: Когда проверка качества носителя обнаруживает нечитаемые сектора, некоторые
данные уже потеряны.<p></li>
<li>Как раз перед отказом носителя: Идеальный случай, но как узнать?
</ul>

Однако, мы могли бы сделать это путем dvdisaster:

<ul>
<li><a href="howtos20.html">Создать данные для исправления ошибок</a> для носителя.<p></li>
<li><a href="howtos10.html">Проверять носитель</a> регулярно. Использовать его до появления первых ошибок чтения.<p></li>
<li><a href="howtos30.html">Восстановить</a> места с ошибками чтения, <a href="howtos40.html">используя данные для исправления ошибок</a>.
Записать восстановленный образ на новый носитель.</li>
</ul>

<p>
<a href="index30.html">dvdisaster: за и против при беглом взгляде...</a>
EOF
}


function index30ru()
{  cat >> $1 <<EOF
<h3>Преимущества использования dvdisaster:</h3>

<ul>
<li><b>Защищает</b> от старения и случайного повреждения носителя (в <a href="background12.html">определенных пределах</a>).<p></li>
<li><a href="howtos10.html">Проверки на ошибки чтения</a> проводятся <b>быстрее</b>, чем проверки качества носителя;
вплоть до полной скорости чтения, в зависимости от привода.<p></li>
<li><b>Экономия затрат:</b> Носители должны заменяться новой копией, 
только когда они действительно повреждены.</li>
</ul>

<h3>Ограничения при использовании программы dvdisaster:</h3>

Требуется стратегия резервирования и по меньшей мере 15% дополнительного места.

<ul>
<li>Данные для исправления ошибок <b>должны быть созданы до того, как носитель выйдет из строя</b>, 
желательно тогда же, когда записывается носитель.<p></li>
<li>Данным для исправления ошибок требуется <b>дополнительное место</b>, либо на защищаемом 
носителе, либо на дополнительных носителях. 
При стандартных настройках дополнительное место
составляет до 15% первоначального размера данных
(приблиз. 700 Мб для полного 4.7 Гб DVD).<p></li>
<li><a href="background12.html">нет гарантированной защиты</a> от потери данных.</li>
</ul>

Смотрите также подборку <a href="background.html">справочной информации</a>,
чтобы узнать больше о том, как работает dvdisaster.
EOF
}

# ----- Screenshots

function screenshots_contents_ru()
{  local file_base=$1;
   local query=$2
   local page=$3
   local lang=$4
   local expand=$5
   local file;

   if [ $expand != 0 ]; 
     then file="${file_base}_${expand}.html";
     else file="${file_base}.html";
   fi

   case $query in
   title)   title="Снимки с экрана" ;;

   link)    link_title="Снимки с экрана" ;;

   content*) eval "screenshots$page$lang $file $file_base $expand" ;;
   esac 
}

function screenshots0ru()
{  cat >> $1 <<EOF
<h3>Предстоит сделать</h3>
EOF
}

# ----- Screenshots

function howtos_contents_ru()
{  local file_base=$1;
   local query=$2
   local page=$3
   local lang=$4
   local expand=$5
   local file;

   if [ $expand != 0 ]; 
     then file="${file_base}_${expand}.html";
     else file="${file_base}.html";
   fi

   case $query in
   title)   title="Howto's" ;;
   link10)   link_title="Поиск ошибок на носителе" ;;

   link20)   link_title="Создание данных для исправления ошибок" ;;
   link21)   link_title="в виде файла" ;;
   link22)   link_title="внутри образа" ;;

   link30)   link_title="Чтение поврежденных носителей" ;;
   link40)   link_title="Восстановление образов носителей" ;;
   link50)   link_title="Получение информации об образах и данных для исправления ошибок" ;;

   link)    link_title="Howto's" ;;

   content*) eval "howtos$page$lang $file $file_base $expand" ;;
   esac 
}

function howtos0ru()
{  cat >> $1 <<EOF
<h3>Предстоит сделать</h3>
EOF
}

function howtos10ru()
{  cat >> $1 <<EOF
<h3>Предстоит сделать</h3>
EOF
}

function howtos20ru()
{  cat >> $1 <<EOF
<h3>Предстоит сделать</h3>
EOF
}

function howtos21ru()
{  cat >> $1 <<EOF
<h3>Предстоит сделать</h3>
EOF
}

function howtos22ru()
{  cat >> $1 <<EOF
<h3>Предстоит сделать</h3>
EOF
}

function howtos30ru()
{  cat >> $1 <<EOF
<h3>Предстоит сделать</h3>
EOF
}

function howtos40ru()
{  cat >> $1 <<EOF
<h3>Предстоит сделать</h3>
EOF
}

function howtos50ru()
{  cat >> $1 <<EOF
<h3>Предстоит сделать</h3>
EOF
}

# ----- Downloads

function download_contents_ru()
{  local file_base=$1;
   local query=$2
   local page=$3
   local lang=$4
   local expand=$5
   local file;

   if [ $expand != 0 ]; 
     then file="${file_base}_${expand}.html";
     else file="${file_base}.html";
   fi

   case $query in
   title)   title="Скачать" ;;

   link)    link_title="Скачать" ;;
   link10)   link_title="Системные требования" ;;
   link20)   link_title="Digital signature" ;;
   link30)   link_title="Установка" ;;
   link40)   link_title="Alpha (unstable) versions" ;;

   content*) eval "download$page$lang $file $file_base $expand" ;;
   esac 
}


function download0ru()
{  cat >> $1 <<EOF
<h3>Предстоит сделать</h3>

dvdisaster is available for <a href="download10.html">recent versions</a>
of the Darwin/Mac OS X, FreeBSD, Linux, NetBSD and Windows operating systems.<p>

Please download either the source code or a binary version from the list below.
A <a href="download20.html">digital signature</a> is provided for verification that the
packets are in their original state.<p>

To <a href="download30.html">install</a> the binary version execute 
the downloaded program and follow the dialog.
The source code version contains a file <tt>INSTALL</tt> with further instructions.

<pre> </pre>

<!---
<b>Developer versions</b> - new and experimental for experienced users!<p>
<b>Current version</b><p>
-->

<b>Alpha (unstable) versions</b> - new and experimental for experienced users!<p>

Testers are welcome for the upcoming dvdisaster release, but should be aware of remaining
bugs and incompatibilities.
The current unstable release is 
<a href="download40.html">${cooked_version}</a>.
<pre> </pre>

<b>Stable version</b> - recommended for getting started.<p>

<table width="100%" $IDXCOLOR cellpadding="0" cellspacing="5">
<tr><td><b>dvdisaster-0.70</b></td><td align="right">20-Jan-2007</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
  <table>
    <tr><td align="right">&nbsp;&nbsp;Source code for all operating systems:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.70.4.tar.bz2?download">dvdisaster-0.70.4.tar.bz2</a></td></tr>
    <tr><td align="right">Digital signature:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.70.4.tar.bz2.gpg?download">dvdisaster-0.70.4.tar.bz2.gpg</a></td></tr>
    <tr><td align="right">Binary for Windows:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.70.4-setup.exe?download">dvdisaster-0.70.4-setup.exe</a></td></tr>
    <tr><td align="right">Digital signature:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.70.4-setup.exe.gpg?download">dvdisaster-0.70.4-setup.exe.gpg</a></td></tr>
  </table>
</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
The RS02 error correction method
is fully supported in the graphical user interface. Images created
with RS02 can be used 
with the adaptive reading strategy.<p>

Julian Einwag started porting dvdisaster 
to Mac OS X / Darwin.<p>

Daniel Nylander provided Swedish translations of the screen texts.<p>

<b>Patches</b> (small changes after version 0.70; files above have been updated):<br>

<b>pl4</b> provides better compatibility with two layered DVDs 
(DVD-R DL and DVD+R DL).<br> 
Some minor bugs have been fixed. <i>(20-Jan-2007)</i>.<p>

<b>pl3</b> fixes incorrect recognization of unsupported CD formats which
produced a Windows blue screen under rare circumstances. Added cancel option
during RS02 initialization on DVD RW media.
<i>(10-Dec-2006)</i>.<p>

<b>pl2</b> fixes incorrect memory freeing when the program window is closed.
Unpacking of documentation screen shots is fixed for PPC platforms.
Only the source code archives have been updated.
<i>(03-Oct-2006)</i>.<p>

<b>pl1</b> fixes a bug in adaptive reading for RS02 which would sometimes result in
reading insufficient data for a successful recovery. Adds some small
improvements of documentation and usability. <i>(30-Jul-2006)</i>
</td></tr></table><p>

The dvdisaster source code is also
<a href="http://sourceforge.net/cvs/?group_id=157550">available via CVS</a>.
<pre> </pre>

<b>Previous release</b> - upgrading to version 0.70.4 is recommended.<p>

<table width="100%" $IDXCOLOR cellpadding="0" cellspacing="5">
<tr><td><b>dvdisaster-0.66</b></td><td align="right">25-Mar-2006</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
  <table>
    <tr><td align="right">&nbsp;&nbsp;Source code for all operating systems:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.66.tar.bz2?download">dvdisaster-0.66.tar.bz2</a></td></tr>
    <tr><td align="right">Digital signature:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.66.tar.bz2.gpg?download">dvdisaster-0.66.tar.bz2.gpg</a></td></tr>
    <tr><td align="right">Binary for Windows:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.66.3-setup.exe?download">dvdisaster-0.66.3-setup.exe</a></td></tr>
    <tr><td align="right">Digital signature:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.66.3-setup.exe.gpg?download">dvdisaster-0.66.3-setup.exe.gpg</a></td></tr>
  </table>
</td></tr>
</table><p>
EOF
}

function download10ru()
{  cat >> $1 <<EOF
<h3>System requirements</h3>

<ul>
 <li>Processors: x86, PowerPC or Sparc;<p></li>
 <li>with processing speed equal or better than a P4 at 2Ghz;<p></li>
 <li>an up-to-date CD/DVD/HD DVD/BD drive with ATAPI or SCSI interface;<p></li>
 <li>enough hard disk space for creating .iso images from processed media.<p>
</ul>

<h3>Operating systems</h3>
<ul>
 <li><b>FreeBSD</b> version <b>6.0</b> or later<br>
     (using ATAPI drives requires a kernel recompile -- see INSTALL doc)<p>
 </li>
 <li><b>Linux</b> with kernel <b>2.6.7</b> or later,<br>
     (using kernels >= 2.4.20 is possible, but not recommended!)<p>
 </li>
 <li><b>Mac OS X</b> version 10.4 (Tiger) or later,<br> 
      on x86 and PowerPC hardware.<p>
 <li><b>NetBSD</b> version 3.1 or later.<p></li> 
 <li><b>Windows</b> <b>2000</b> or <b>XP</b><br>
     Windows 98 and ME are possible, but are not recommended for handling DVD
     and larger media.<br>
     Running under Windows Vista (R) is possible, but mostly untested.
 </li>
</ul>
EOF
}

function download20ru()
{  cat >> $1 <<EOF

<h3>Digital signature</h3>

The downloadable dvdisaster packages have been digitally signed using
<a href="http://www.gnupg.org/gnupg.html">GnuPG</a> so that you can verify
that the software is in its original state.<p>


The signature has been made with the following <a href="../pubkey.asc">public key</a>:

<a href="../pubkey.asc">
<pre>
pub   1024D/F5F6C46C 2003-08-22
      Key fingerprint = 12B3 1535 AF90 3ADE 9E73  BA7E 5A59 0EFE F5F6 C46C
uid                  dvdisaster (pkg signing key #1)
sub   1024g/091AD320 2003-08-22
</pre></a>

Feel free to send an email to <img src="../images/email.png" align="top"> to obtain 
the fingerprint directly from the developers. 
Please include "GPG finger print" in the subject line.
EOF
}

function download30ru()
{  cat >> $1 <<EOF
<h3>Installation of the binary versions (Windows only)</h3>

Please install the Windows version by executing the setup binary
(e.g. ${project_package}-setup.exe) and follow the 
installation dialog. <p>

<b>Caution:</b> dvdisaster can NOT be installed from
manually unpacking the setup.exe program, or by copying it from
an already existing installation. Doing so may result in strange
errors which seem to be totally unrelated to an improper installation.

<h3>Installation of the source packages</h3>

dvdisaster uses the common <tt>./configure; make; make install</tt> mechanism
for installation from source. Please see the file <tt>INSTALL</tt> in the
source distribution for further information.
EOF
}

function download40ru()
{  cat >> $1 <<EOF
<h3>Alpha (developer) versions</h3>

<b>Help us testing!</b> We have some new functions under development and
would like them to be tested by more people before making the next official release.<p>

Especially, support for BD und HD DVD drives has been introduced and needs more
field testing. The <a href="howtos90.html">image compatibility test</a> described
in the Howtos section is very useful for testing new drives. Please 
<a href="feedback.html">report both positive and negative results</a> 
so that we can get a better picture of how the new formats are working.<p>

<b>A word of caution:</b> The alpha versions might lock up the drive and/or the
operating system and require you to reboot. They might contain other bugs
and should not be used to process media containing important data.<p>

If in doubt, please continue using the <a href="download.html">stable version 0.70</a>
and wait for the release of version 0.72.

<hr>

<h3>Downloads</h3>

The alpha versions use the same package format as the regular releases.<p>

<table width="100%" $IDXCOLOR cellpadding="0" cellspacing="5">
<tr><td><b>dvdisaster-0.71 (devel24)</b></td><td align="right">xx-xxx-2007</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
  <table>
    <tr><td align="right">&nbsp;&nbsp;Source code for all operating systems:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.devel24.tar.bz2?download">dvdisaster-0.71.devel24.tar.bz2</a></td></tr>
    <tr><td align="right">Digital signature:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.devel24.tar.bz2.gpg?download">dvdisaster-0.71.devel24.tar.bz2.gpg</a></td></tr>
    <tr><td align="right">Binary for Windows:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.devel24-setup.exe?download">dvdisaster-0.71.devel24-setup.exe</a></td></tr>
    <tr><td align="right">Digital signature:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.devel24-setup.exe.gpg?download">dvdisaster-0.71.devel24-setup.exe.gpg</a></td></tr>
  </table>
</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
This release marks the start of
an online documentation rewrite, including a Russian translation made
by Igor Gorbounov. Also new are the raw CD sector editor and some improvements
in the raw reading heuristics. 
</td></tr></table><p>

<table width="100%" $IDXCOLOR cellpadding="0" cellspacing="5">
<tr><td><b>dvdisaster-0.71 (devel23)</b></td><td align="right">06-May-2007</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
  <table>
    <tr><td align="right">&nbsp;&nbsp;Source code for all operating systems:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.devel23.tar.bz2?download">dvdisaster-0.71.devel23.tar.bz2</a></td></tr>
    <tr><td align="right">Digital signature:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.devel23.tar.bz2.gpg?download">dvdisaster-0.71.devel23.tar.bz2.gpg</a></td></tr>
    <tr><td align="right">Binary for Windows:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.devel23-setup.exe?download">dvdisaster-0.71.devel23-setup.exe</a></td></tr>
    <tr><td align="right">Digital signature:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.devel23-setup.exe.gpg?download">dvdisaster-0.71.devel23-setup.exe.gpg</a></td></tr>
  </table>
</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
Added support for C2 scanning of CD media.<br>
Andrei Grecu submitted improved versions of the RAW recovery heuristics.
</td></tr></table><p>

<table width="100%" $IDXCOLOR cellpadding="0" cellspacing="5">
<tr><td><b>dvdisaster-0.71 (devel22)</b></td><td align="right">22-Apr-2007</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
  <table>
    <tr><td align="right">&nbsp;&nbsp;Source code for all operating systems:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.devel22.tar.bz2?download">dvdisaster-0.71.devel22.tar.bz2</a></td></tr>
    <tr><td align="right">Digital signature:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.devel22.tar.bz2.gpg?download">dvdisaster-0.71.devel22.tar.bz2.gpg</a></td></tr>
    <tr><td align="right">Binary for Windows:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.devel22-setup.exe?download">dvdisaster-0.71.devel22-setup.exe</a></td></tr>
    <tr><td align="right">Digital signature:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.devel22-setup.exe.gpg?download">dvdisaster-0.71.devel22-setup.exe.gpg</a></td></tr>
  </table>
</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
Igor Gorbounov started the Russian translation of on-screen texts.<p>
Added selection of 20h/21h raw reading modes to enable defective sector 
reading on more drives. Fixed some inefficiencies during raw sector recovery.
</td></tr></table><p>

<table width="100%" $IDXCOLOR cellpadding="0" cellspacing="5">
<tr><td><b>dvdisaster-0.71 (devel21)</b></td><td align="right">15-Apr-2007</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
  <table>
    <tr><td align="right">&nbsp;&nbsp;Source code for all operating systems:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.devel21.tar.bz2?download">dvdisaster-0.71.devel21.tar.bz2</a></td></tr>
    <tr><td align="right">Digital signature:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.devel21.tar.bz2.gpg?download">dvdisaster-0.71.devel21.tar.bz2.gpg</a></td></tr>
    <tr><td align="right">Binary for Windows:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.devel21-setup.exe?download">dvdisaster-0.71.devel21-setup.exe</a></td></tr>
    <tr><td align="right">Digital signature:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.devel21-setup.exe.gpg?download">dvdisaster-0.71.devel21-setup.exe.gpg</a></td></tr>
  </table>
</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
Andrei Grecu provided an algorithm for recovering defective CD sectors
from multiple incomplete RAW reading attempts.
Sergey Svishchev helped porting dvdisaster to NetBSD.<p>

Other features include: a selectable number of reading attempts, verification
of CD sectors using the raw L-EC and EDC data, and a redesigned preferences dialog.
RS02 checksum handling has been improved in the adaptive reading strategy. 
</td></tr></table><p>
EOF
}

# ----- Questions and Answers

function qa_contents_ru()
{  local file_base=$1;
   local query=$2
   local page=$3
   local lang=$4
   local expand=$5
   local file;

   if [ $expand != 0 ]; 
     then file="${file_base}_${expand}.html";
     else file="${file_base}.html";
   fi

   case $query in
   title)   title="Вопросы и ответы" ;;

   link)    link_title="Вопросы и ответы" ;;
   link10)   link_title="Технические вопросы" ;;
   link20)   link_title="Сообщения об ошибках" ;;

   content*) eval "qa$page$lang $file $file_base $expand" ;;
   esac 
}


function qa0ru()
{  cat >> $1 <<EOF
<h3><a name="top">Вопросы и ответы общего характера</a></h3>

<a href="#pronounce">1.1 Как правильно произносить "dvdisaster"?</a><p>
<a href="#pipo">1.2 Что такое проверка качества и почему бы вам не поддерживать больше возможностей?</a><p>
<a href="#compat">1.3 dvdisaster совместим с будущими версиями?</a><p>

<hr><p>

<b><a name="pronounce">1.1 Как правильно произносить "dvdisaster"?</a></b><p>
Поскольку это слово берет начало из английского языка, просто произносите "dv" перед
"disaster". Возможно, "ди-ви-дизастер" является подходящим
фонетическим представлением слова.
<div align=right><a href="#top">&uarr;</a></div>

<b><a name="pipo">1.2 Что такое проверка качества и почему бы вам не поддерживать больше возможностей?</a></b><p>
Оптические носители имеют встроенный механизм коррекции ошибок, похожий на 
<a href="background10.html">метод</a>, используемый в dvdisaster. 
Некоторые приводы могут сообщать число ошибок, исправленных
при чтении носителя. Это дает приблизительную оценку качества 
записи и носителя.<p>

Так как dvdisaster - свободное программное обеспечение, в него могут входить только исходные тексты и информация,
которые могут свободно распространяться. В настоящий момент это справедливо 
для C2-<a href="howtos10.html">проверки</a> CD-носителей, 
которая официально стандартизирована, и у нее есть свободная документация.<p>

С другой стороны, проверки качества DVD ("PI/PO-проверки") не стандартизированы. 
Те поставщики приводов, которые поддерживают ее, используют патентованные прикладные
интерфейсы. Соответствующие спецификации, похоже, недоступны 
для использования в свободном ПО. Поэтому мы должны терпеливо ждать, пока изготовители 
не поймут, что если для привода будет больше ПО (и свободного), 
то это приведет к продаже еще большего количества приводов. <p>

<div align=right><a href="#top">&uarr;</a></div>

<b><a name="compat">1.3 dvdisaster совместим с будущими версиями?</a></b><p>
Да, файлы dvdisaster предназначены для архивирования на многие годы.
При обновлении dvdisaster до новой версии Вы можете по-прежнему использовать
существующий образ и файлы коррекции ошибок от предыдущих версий.
<i>Нет</i> необходимости вновь пересоздавать их.
<div align=right><a href="#top">&uarr;</a></div><p>

EOF
}


function qa10ru()
{  cat >> $1 <<EOF
<h3><a name="top">Технические вопросы</a></h3>

<a href="#nls">2.1 На какие языки переведена программа?</a><p>
<a href="#media">2.2 Какие типы носителей поддерживаются?</a><p>
<a href="#filesystem">2.3 Какие файловые системы поддерживаются?</a><p>
<a href="#aspi">2.4 Что означает "SPTI" и "ASPI"?</a><p>

<hr><p>

<b><a name="nls">2.1 На какие языки переведена программа?</a></b><p>

Текущая версия dvdisaster содержит экранные тексты на следующих языках:<p>

<table>
<tr><td>&nbsp;&nbsp;&nbsp;</td><td>Чешский</td><td>--</td><td>до версии 0.66</td></tr>
<tr><td></td><td>Английский</td><td>--</td><td>полный</td></tr>
<tr><td>&nbsp;&nbsp;&nbsp;</td><td>Немецкий</td><td>--</td><td>полный</td></tr>
<tr><td>&nbsp;&nbsp;&nbsp;</td><td>Итальянский</td><td>--</td><td>до версии 0.65</td></tr>
<tr><td>&nbsp;&nbsp;&nbsp;</td><td>Русский</td><td>--</td><td>полный</td></tr>
<tr><td>&nbsp;&nbsp;&nbsp;</td><td>Шведский</td><td>--</td><td>до версии 0.70</td></tr>
</table><p>

Переводчики для других языков - добро пожаловать!<p>

dvdisaster автоматически получает языковые настройки из операционной системы.
Если локальный язык еще не поддерживается, то будет использоваться английский текст. 
С помощью переменных окружения можно выбрать другие языки.<p>

Пример для оболочки bash и немецкого языка:

<pre>export LANG=de_DE</pre>

Если специальные символы, такие как немецкие умлауты, отображаются неправильно,
попробуйте следующее:<p>

<tt>export OUTPUT_CHARSET=iso-8859-1</tt> (X11, XTerm)

<div align=right><a href="#top">&uarr;</a></div>


<b><a name="media">2.2 Какие типы носителей поддерживаются?</a></b><p>

dvdisaster поддерживает (пере-)записываемые CD и DVD. <br>
Носители с многосессионными записями или с защитой от копирования <i>не</i> могут быть использованы.<p>

Пригодные носители по типу:<p>

<b>CD-R, CD-RW</b><p>

<ul>
 <li>поддерживаются только Data CD.</li>
</ul>

<b>DVD-R, DVD+R</b><p>

<ul>
<li>Дополнительные ограничения неизвестны.</li>
</ul>

<b>DVD-R DL, DVD+R DL (два слоя)</b>
<ul>
<li>Привод должен быть способен <a href="qa20.html#dvdrom">идентифицировать тип носителя</a>.
Обычно это имеет место только в тех случаях, когда приводы могут также и писать на двухслойный носитель.</li>
</ul>

<b>DVD-RW, DVD+RW</b><p>

<ul>
<li>Некоторые приводы сообщают неправильные <a href="qa20.html#rw">размеры образов</a>.<br>
Выход из положения: определяйте размер образа из файловой системы ISO/UDF или файла ECC/RS02.
</li></ul>

<b>DVD-RAM</b><p>
<ul>
<li>Пригодны только, если записывать с помощью ISO/UDF образов, как DVD-R/-RW;</li>
<li>Не поддерживаются, если используются как извлекаемый носитель / для пакетной записи.</li>
<li>То же самое и с распознаванием <a href="qa20.html#rw">размера образа</a>,
как уже упоминалось выше.</li>
</ul>

<b>Непригодные типы</b> (образ нельзя извлечь):<p>
DVD-ROM (печатаемые DVDs), CD-Audio и CD-Video.

<div align=right><a href="#top">&uarr;</a></div><p>


<b><a name="filesystem">2.3 Какие файловые системы поддерживаются?</a></b><p>

dvdisaster работает исключительно на <a href="background20.html">уровне образа</a>,
доступ к которому осуществляется посекторно.
Это означает, что не имеет значения, под какую файловую систему сформатирован носитель.<p>

Поскольку dvdisaster как не знает, так и не использует структуру файловой системы,
он не может исправлять логические ошибки на уровне файловой системы.
Он не может восстанавливать потерянные или удаленные файлы.
<div align=right><a href="#top">&uarr;</a></div><p>


<b><a name="aspi">2.4 Что означает "SPTI" и "ASPI"?</a></b><p>

Windows 98 и ME используют драйвера, называемые "ASPI", для доступа к приводам CD/DVD.
Windows NT и ее наследники обеспечивают схожую функциональность
через интерфейс, названный "SPTI", но все еще допускают установку
дополнительных ASPI драйверов.<p>

<i>&gt;&nbsp; Установка ASPI в среде Windows NT/2000/XP - за и против:</i><p>

<table>
<tr valign=top><td>+</td><td>Для доступа к приводам не требуются прав администратора.</td></tr>
<tr valign=top><td>-</td><td>ASPI не очень хорошо отображет приводы на буквы устройств Windows.</td></tr>
<tr valign=top><td>-</td><td>Некоторые программы для записи на CD/DVD устанавливают
свои собственные драйвера ASPI. Установка еще одного драйвера ASPI может вызвать конфликт 
с существующим ПО.</td></tr>
</table><p>

<i>&gt;&nbsp; Какие версии ASPI рекомендуются для использования с dvdisaster?</i><p>

Adaptec предоставляет свои драйверы ASPI для бесплатного скачивания.
Иногда у версий 4.72.* есть трудности распознавания приводов в среде
Windows 2000/XP. Понижение версии до 4.60 обычно помогает в таких
случаях.<p>

<i>&gt;&nbsp; Неправильные буквы дисков при использовании ASPI!</i><p>

Драйвера ASPI используют свою собственную систему идентификации приводов, которая
не поддерживает схему буквенного обозначения приводов Windows.<br>
Чтобы обеспечить унифицированный доступ к SPTI и ASPI,
dvdisaster пытается угадать отображение приводов ASPI на буквенные обозначения.
Это, однако, не работает во всех конфигурациях.<p>

Функция --list распечатывает
текущее отображение букв приводов.<p>

<i>&gt;&nbsp; Как сделать выбор между SPTI и ASPI?</i><p>

dvdisaster автоматически сделает выбор между SPTI и ASPI, 
делая предпочтение в пользу SPTI, если оба драйвера присутствуют. 
Использование ASPI может быть сделано принудительно; подробности смотрите в описании
функции --list.

<div align=right><a href="#top">&uarr;</a></div><p>

EOF
}


function qa20ru()
{  cat >> $1 <<EOF
<h3>Сообщения об ошибках</h3>

<a href="#tao">3.1 "Предупреждение: 2 секторов не хватает в конце диска".</a><p>
<a href="#block">3.2 Программа блокируется сразу после запуска.</a><p>
<a href="#crc">3.3 Что означает "CRC-ошибка, сектор: n"?</a><p>
<a href="#rw">3.4 Ошибки чтения или неправильный размер образа в случае носителей -RW/+RW/-RAM</a><p>
<a href="#dvdrom">3.5 Записанные мною носители распознаются как "DVD-ROM" и отвергаются.</a><p>
<a href="#freebsd">3.6 Не появляются устройства в FreeBSD.</a><p>
<a href="#v40error">3.7 "Ecc-файл создан версией 0.40.7."</a><p>

<hr><p>

<b><a name="tao">3.1 "Предупреждение: 2 секторов не хватает в конце диска"</a></b><p>
Это предупреждение появляется в случае носителей CD, записанных в режиме "TAO" (дорожка целиком).
Некоторые приводы сообщают размер образа, который на 2 сектора больше, чем должно быть для таких носителей,
что дает 2 ложных ошибки чтения в конце носителя, 
что <i>не</i> означает потерю данных в этом случае.<p>

Поскольку режим записи невозможно определить по носителю, dvdisaster предполагает,
что это CD в режиме "TAO", если точно два последних сектора не читаются, и обрезает образ
соответственно. Вам решать, правильно ли это. Вы можете
посоветовать программе dvdisaster обращаться с этими секторами как с реальными ошибками чтения, используя параметр
--dao, указав это 
на вкладке настроек для чтения/проверки.<p>

Чтобы избежать этих проблем, подумайте об использовании режима "DAO / Диск целиком" (иногда
также называемого "SAO / Сессия целиком") для записи односессионных носителей. 
<div align=right><a href="#top">&uarr;</a></div>


<b><a name="block">3.2 Программа блокируется сразу после запуска</a></b><p>
В старых версиях Линукса (ядро 2.4.x) программа иногда  
блокируется сразу после запуска и прежде, чем будут выполнены какие-либо
действия. Она не завершается с помощью Ctrl-C или "kill -9".<p>

Извлеките носитель, чтобы заставить программу завершиться. Вставьте носитель снова
и подождите, пока привод не распознает носитель и не остановит вращение.
Повторный запуск dvdisaster теперь должен сработать.
<div align=right><a href="#top">&uarr;</a></div>

<b><a name="crc">3.3 Что означает "CRC-ошибка, сектор: n"?</a></b><p>
Этот сектор может быть прочитан, но контрольная сумма его содержимого не
совпадает со значением, отмеченном в в файле коррекции ошибок.  Некоторые из возможных причин:<p>

<ul>
<li>Образ был смонтирован с разрешением для записи и
был, следовательно, изменен (типичный признак: CRC-ошибки в секторе 64 и в
секторах с 200 по 400).</li>
<li>В компьютере есть аппаратные проблемы, в особенности при
работе с устройствами хранения данных.</li>
</ul>


Если Вы подозреваете технические проблемы, попробуйте создать еще одну версию образа 
и файла коррекции ошибок и <a href="howtos50.html">еще раз проверьте
их</a>.
Если ошибка исчезает или находится в другом месте,
у Вашего компьютера может быть дефектная память, 
поврежденные интерфейсные кабели, или неправильные настройки частоты CPU/системы.
<div align=right><a href="#top">&uarr;</a></div>


<b><a name="rw">3.4 Ошибки чтения или неправильный размер образа в случае носителей -RW/+RW/-RAM</a></b><p>

Некоторые приводы сообщают неправильные размеры образов, когда используются носители -RW/+RW/-RAM. Два распространенных
случая:<p>

<table>
<tr><td valign="top">Проблема:</td>
<td>Привод сообщает размер самого большого образа, когда-либо записанного на носитель, а не 
размер фактического образа.
</td></tr>
<tr><td valign="top">Симптомы:</td>
<td>После стирания носителя на него записывается файл размером около 100 Мб.
Но прочитанный образ имеет размер в несколько Гб и содержит остатки 
старых образов.
</td></tr>
<tr><td><pre> </pre></td><td></td></tr>
<tr><td valign="top">Проблема:</td>
<td>Привод сообщает максимально возможную вместимость носителя (обычно 2295104 секторов)
вместо числа фактически использованных секторов.
</td></tr>
<tr><td valign="top">Симптомы:</td>
<td>При чтении за определенной точкой на носителе имеют место только ошибки чтения,
но все файлы все еще читаются, и они целые.
</td></tr>
</table><p>

Возможный выход из положения:<p>

<table width=100%><tr><td bgcolor=#000000 width=2><img width=1 height=1 alt=""></td><td>
Включите параметр для определения
размера образа из файловой системы ISO/UDF или файла ECC/RS02.
</td></tr></table><p>

Если требуемые сектора ISO/UDF нечитаемые, и Вы используете файлы коррекции ошибок
для восстановления поврежденных носителей, то есть два возможных выхода из положения:

<ul>
<li>Выполните функцию <a href="howtos50.html">"Проверить"</a> так, чтобы только
файл коррекции ошибок был выбран/дан. Запишите правильный размер образа 
из выходных данных и ограничьте 
диапазон чтения соответственно.
</li>
<li>Просто считайте образ с неправильным (большим, чем надо) размером.
При запуске функции <a href="howtos40.html#repair">"Исправить"</a>
ответьте "OK" на вопрос, следует ли обрезать образ.
</li>
</ul>

<div align=right><a href="#top">&uarr;</a></div>

<b><a name="dvdrom">3.5 Записанные мною носители распознаются как "DVD-ROM" и отвергаются.</a></b><p>

Вероятно, тип носителя (book type) установлен в "DVD-ROM". Обычно для обработки
носителя в dvdisaster требуется привод, способный писать на носитель того же формата.<p>

Например, двухслойный DVD+R с неправильным типом носителя 
может быть принят только пишущим приводом, который может писать на такие носители.<p>

Попробуйте другой привод для чтения образов в этих случаях.

<div align=right><a href="#top">&uarr;</a></div>


<b><a name="freebsd">3.6 Не появляются устройства в FreeBSD.</a></b><p>

<ul>
<li>FreeBSD может потребоваться <a href="download20.html#freebsd">перекомпиляция ядра</a>,
чтобы ATAPI-приводы (почти все современные модели) можно было использовать
с dvdisaster.
<li>Вам нужны права на чтение и запись для соответствующего устройства
(например, /dev/pass0).
</ul>

<div align=right><a href="#top">&uarr;</a></div>


<b><a name="v40error">3.7 "Ecc-файл создан версией 0.40.7."</a></b><p>

<a href="http://sourceforge.net/cvs/?group_id=157550">CVS-версии</a>
dvdisaster помечают свои ecc-файлы специальным битом. Это является причиной того,
что dvdisaster до версии 0.65 ошибочно выдает вышеприведенное сообщение об ошибке. Используйте
CVS-версии только совместно с dvdisaster версии 0.66 или новее.

<div align=right><a href="#top">&uarr;</a></div>

EOF
}

# ----- Feedback/Bug reporting

function feedback_contents_ru()
{  local file_base=$1;
   local query=$2
   local page=$3
   local lang=$4
   local expand=$5
   local file;

   if [ $expand != 0 ]; 
     then file="${file_base}_${expand}.html";
     else file="${file_base}.html";
   fi

   case $query in
   title)   title="Bug reporting" ;;

   link)    link_title="Bug reporting" ;;
   link0)   link_title="Bug reporting" ;;

   content*) eval "feedback$page$lang $file $file_base $expand" ;;
   esac 
}

function feedback0ru()
{  local file=$1
   local file_base=$2
   local expand=$3

   cat >> $1 <<EOF
<h3>Bug reporting</h3>
Like all complex software, dvdisaster may contain bugs (programming errors) and
incompatibilities with certain (drive) hardware and software setups.
You are invited to tell us about any difficulties you encounter with the program
or the documentation so that we can improve things in future releases.<p>

To make sure that we are getting the right information, we have provided the
following checklist for bug reporting:


<h4>Please check first that you are really experiencing a bug:</h4>

<ul>
<li>Make sure that you are using the latest genuine version from our
<a href="http://dvdisaster.sourceforge.net/en/download.html">download site 
at SourceForge</a>.
dvdisaster versions provided by third parties
may contain functions and bugs which are not present in the original version.</li>
<li>Double check that the issue you have encountered is not already covered in the
<a href="qa20.html">Questions and Answers</a> section.</li>
<li>Please note that dvdisaster will only work with the (re-)writeable varieties
of media, so seeing it reject DVD-ROM, BD-ROM and HD DVD-ROM is <b>not an
error</b>. Also, CD-Audio, VCD, SVCD and multisession CD are not supported 
(<a href="qa10.html#media">complete list of supported media formats</a>).</li>
<li>dvdisaster works only with real drives. Not supported are
network drives, software drives (e.g. alcohol) and drives in virtual 
machines like VMWare(R) and Xen.</li>
</ul>

<h4>How to report issues with the program:</h4>

Please report your findings by sending an email to 
<img src="../images/email.png" align="top">. Your report should contain:<p>

<ul>
<li>Information about the operating system and dvdisaster version you are using;</li>
<li>the drive and media type(s) which exhibited the problem; </li>
<li>a textual description of the issue you encountered;</li>
<li>a screen shot of the error message and/or output which might provide
further information about the problem;</li>
<li>differences between working and non-working configurations if the issue is
experienced only on certain drives/computers;</li>
<li>a log file if you suspect that the issue is related to a drive or medium
incompatibility.</li>
</ul>
EOF

screenshot $file_base "Creating a log file." "activate_logfile.png" $expand <<END_SCREENSHOT

<b>How to create a log file:</b> If you suspect incompatibilities with your drive and/or media
as the cause of your issue, please activate the log file feature in the preferences 
dialog as shown in the screen shot. Then perform a scanning or reading action 
and attach the log file to your bug report.
END_SCREENSHOT

cat >>$file <<EOF
Thanks for your feedback!
EOF
}

# ----- Background information

function background_contents_ru()
{  local file_base=$1;
   local query=$2
   local page=$3
   local lang=$4
   local expand=$5
   local file;

   if [ $expand != 0 ]; 
     then file="${file_base}_${expand}.html";
     else file="${file_base}.html";
   fi

   case $query in
   title)   title="Справочная информация" ;;
 
   link)    link_title="Справочная информация" ;;
   link10)   link_title="Исправление ошибок" ;;
   link11)   link_title="Общая идея" ;;
   link12)   link_title="Ограничения" ;;
   link20)   link_title="Восстановление данных на уровне образа" ;;
   link30)   link_title="Стратегии чтения" ;;
   link40)   link_title="Стратегия линейного чтения" ;;
   link50)   link_title="Стратегия адаптивного чтения" ;;
   link60)   link_title="Замечания по ошибкам чтения" ;;
   link70)   link_title="Советы по хранению файлов коррекции ошибок" ;;

   content*) eval "background$page$lang $file $file_base $expand"
	    return 0
	    ;;
   esac 
}


function background0ru()
{  cat >> $1 <<EOF

<h3>Предстоит сделать</h3>

EOF
}

function background10ru()
{  cat >> $1 <<EOF

<h3>Предстоит сделать</h3>

EOF
}

function background11ru()
{  cat >> $1 <<EOF

<h3>Предстоит сделать</h3>

EOF
}

function background12ru()
{  cat >> $1 <<EOF

<h3>Предстоит сделать</h3>

EOF
}

function background20ru()
{  cat >> $1 <<EOF

<h3>Предстоит сделать</h3>

EOF
}

function background30ru()
{  cat >> $1 <<EOF

<h3>Предстоит сделать</h3>

EOF
}

# ----- Impressum

function imprint_contents_ru()
{  local file_base=$1;
   local query=$2
   local page=$3
   local lang=$4
   local expand=$5
   local file;

   if [ $expand != 0 ]; 
     then file="${file_base}_${expand}.html";
     else file="${file_base}.html";
   fi

   case $query in
   title)   title="Выходные данные" ;;
 
   link)    link_title="Выходные данные" ;;

   content*) eval "imprint$page$lang $file $file_base $expand"
	    return 0
	    ;;
   esac 
}


function imprint0ru()
{  cat >> $1 <<EOF

   <h3>Выходные данные</h3>

   Carsten Gnörlich <br>
   Beerenkamp 23 <br>
   59514 Welver / Germany<br>
   Tel.: +49 2384 941174<pre> </pre>
   E-Mail: carsten@dvdisaster.de<br>
   Internet: www.dvdisaster.de

   <pre> </pre>

   Ответственный за содержание сайта: <p>
   Carsten Gnörlich (адрес смотри выше).

   <pre> </pre>

   Эти выходные данные охватывают веб-сайты на:<br>
   <a href="http://www.dvdisaster.com">www.dvdisaster.com</a>, &nbsp;
   <a href="http://www.dvdisaster.de">www.dvdisaster.de</a>, &nbsp;
   <a href="http://www.dvdisaster.org">www.dvdisaster.org</a><br>
   и в поддомене
   <a href="http://dvdisaster.sourceforge.net">dvdisaster.sourceforge.net</a>.

EOF
}
