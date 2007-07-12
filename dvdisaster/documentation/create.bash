#! /bin/bash

#  dvdisaster: Homepage generation script
#  Copyright (C) 2004-2007 Carsten Gnörlich
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

# Some global settings

BGCOLOR="bgcolor=\"#ffffff\""     # Background of body
IDXCOLOR="bgcolor=\"#f0f0f0\""    # Background of index
NEWSCOLOR="bgcolor=\"#e0e0ff\""   # Background of news page
ITEMCOLOR="bgcolor=\"#e0e0e0\""   # Background of itemized examples

case $1 in
  local-mod) major_mode="local"
             modified_source="true"
             ;;
  local-orig) major_mode="local"
             ;;
  *) major_mode=$1
     ;;
esac

workdir=`pwd`
project_title=dvdisaster
project_version=$2
project_package=$3
cooked_version="$4"

# Change into the homepage directory

if ! test -d "$workdir"; then
  echo "$workdir does not exist"
  exit 1
fi

# Make sure we've got a valid mode

case $major_mode in
  local) echo "Creating local documentation for $project_title $cooked_version" ;;

  sf)    echo "Creating WWW homepage for $project_title $cooked_version at SourceForge" 
         project_at_hoster="http://sourceforge.net/projects/dvdisaster"
         ;;

  *)     echo "Error: mode $major_mode unknown. Use \"local\" or \"sf\"." ;;
esac


###
### HTML file assembly
###
# All HTML pages are created by calling a sequence of the
# begin, heading, contents, footer and finish functions.
# If this appears weird, just think of creating HTML with PHP,
# then substitute PHP with bash ;-)

#
# Begin a new html file.
# Creates the <html><head> ... </head><body> part of the HTML file.
# TODO: Create a separate style sheet.
#

function begin()
{  local section=$1
   local lang=$2
   local page=$3
   local variant=$4
   local file_prefix
   local file

   if [ $page == 0 ]
     then file_prefix=$section
     else file_prefix=$section$page
   fi

   if [ $variant != 0 ]
     then file_prefix="${file_prefix}_${variant}"
   fi

   file=${file_prefix}.html

   eval "${section}_contents_${lang} $file_prefix title ignore $lang 0"

   if test -e $file; then rm -f $file; fi
   cat >> $file <<EOF
<html>
<head>
  <meta http-equiv="content-type" content="text/html; charset=$trans_encoding">
  <title>${title}</title>
  <style type="text/css">
<!--
  body { font-family:Helvetica,Arial,sans-serif,Verdana }
  a:link { font-weight:normal; color:#0000ff; text-decoration:none }
  a:visited { font-weight:normal; color:#0000ff; text-decoration:none }
  a:hover { font-weight:normal; background-color:#e0e0e0; color:#0000FF; text-decoration:none }
  a:active { font-weight:normal; background-color:#e0e0e0; color:#0000FF; text-decoration:none }
  a:focus { font-weight:normal; background-color:#e0e0e0; color:#0000FF; text-decoration:none }
  table.thumbnail {border-width:1px; border-style:solid; border-color:#000000; background-color:#e0e0ff}
  table.screenshot {width:100%; border-width:0px; background-color:#e0e0ff}
  .command { font-weight:bold }
-->
  </style>
</head>

<body $BGCOLOR>
EOF
}

#
# Creates the head bar.
#
# This includes the "dvdisaster version ..." header. Depending on the mode we
# - provide the link to the online version for local documentation, or
# - create the language switch for the online publication.

function heading()
{  local section=$1
   local lang=$2
   local page=$3
   local variant=$4
   local file_prefix
   local file

   if [ $page == 0 ]
     then file_prefix=$section
     else file_prefix=$section$page
   fi

   if [ $variant != 0 ]
     then file_prefix="${file_prefix}_${variant}"
   fi

   file=${file_prefix}.html

   cat >> $file <<EOF
<table width="100%" cellpadding="0" border="0">
 <tr>
   <td align="left">
      <font size="+3"><b>$project_title</b></font>
EOF

      echo "<i>$trans_version $cooked_version</i>" >>$file

      if [ $major_mode == "local" ]; then
        echo "</td><td align=\"right\"><font size="+3">&nbsp;</font><a href=\"http://dvdisaster.sourceforge.net/$lang/\">$trans_to_internet</a>" >>$file
      fi
   cat >> $file <<EOF
   </td>
 </tr>
</table>

<table width="100%" cellpadding="0" border="0">
EOF
  if [ $major_mode != "local" ]; then
    echo "<tr bgcolor=\"#000000\"><td colspan=\"2\" width=\"100%\"><img width=1 height=1 alt=\"\"></td></tr>" >> $file
  fi

  if [ $major_mode != "local" ]; then
    echo "<tr>" >> $file

    case $major_mode in
      sf) trans_to_hoster="$trans_to_sourceforge" ;;
    esac

    case $lang in
      cs)  echo "   <td align=\"left\"><a href=\"$project_at_hoster\">$trans_to_hoster</a></td>" >> $file
           echo "<td align=\"right\">" >>$file
#           echo "&#268;esky &nbsp;&nbsp;&nbsp;" >>$file
#	   echo "<a href=\"../de/$file\" title=\"Deutsche Sprache\">Deutsch</a> &nbsp;&nbsp;&nbsp;" >>$file
	   echo "<a href=\"../en/$file\" title=\"English language\">English</a>" >>$file
	   echo "<a href=\"../en/$file\" title=\"Russian language\">Russian</a>" >>$file
           echo "</td>" >>$file
	   ;;

      de) echo "   <td align=\"left\"><a href=\"$project_at_hoster\">$trans_to_hoster</a></td>" >> $file
          echo "<td align=\"right\">" >>$file
          echo "<a href=\"../cs/$file\">&#268;esky</a> &nbsp;&nbsp;&nbsp;" >>$file
	  echo "Deutsch &nbsp;&nbsp;&nbsp;" >>$file
	  echo "<a href=\"../en/$file\">English</a> &nbsp;&nbsp;&nbsp;" >>$file
	  echo "<a href=\"../en/$file\">Russisch</a>" >>$file
          echo "</td>" >>$file
	  ;;

      ru) echo "   <td align=\"left\"><a href=\"$project_at_hoster\">$trans_to_hoster</a></td>" >> $file
          echo "<td align=\"right\">" >>$file
#          echo "<a href=\"../cs/$file\">&#268;esky</a> &nbsp;&nbsp;&nbsp;" >>$file
#	  echo "<a href=\"../de/$file\">Deutsch</a> &nbsp;&nbsp;&nbsp;" >>$file
	  echo "<a href=\"../en/$file\">English</a> &nbsp;&nbsp;&nbsp;" >>$file
	  echo "Russian" >>$file
          echo "</td>" >>$file
          ;;

      *)  echo "   <td align=\"left\"><a href=\"$project_at_hoster\">$trans_to_hoster</a></td>" >> $file
          echo "<td align=\"right\">" >>$file
#          echo "<a href=\"../cs/$file\">&#268;esky</a> &nbsp;&nbsp;&nbsp;" >>$file
#	  echo "<a href=\"../de/$file\">Deutsch</a> &nbsp;&nbsp;&nbsp;" >>$file
	  echo "English &nbsp;&nbsp;&nbsp;" >>$file
	  echo "<a href=\"../ru/$file\">Russian</a>" >>$file
          echo "</td>" >>$file
          ;;
    esac
    echo "</tr>" >>$file
  fi

  cat >> $file <<EOF
 <tr bgcolor="#000000"><td colspan="2" width="100%"><img width=1 height=1 alt=""></td></tr>
 <tr><td colspan="2" width="100%" height="10"><img width=1 height=1 alt=""></td></tr>
</table>

EOF
}

#
# Creates the footer bar.
# 
# Includes the copyright and redistribution terms.
#

function footer()
{  local section=$1
   local lang=$2
   local page=$3
   local variant=$4
   local file_prefix
   local file

   if [ $page == 0 ]
     then file_prefix=$section
     else file_prefix=$section$page
   fi

   if [ $variant != 0 ]
     then file_prefix="${file_prefix}_${variant}"
   fi

   file=${file_prefix}.html

   echo >> $file 

   cat >> $file <<EOF
<table width="100%" cellpadding="0" border="0">
 <tr><td colspan="2" width="100%" height="10"><img width=1 height=1 alt=""></td></tr>
 <tr bgcolor="#000000"><td colspan="2" width="100%"><img width=1 height=1 alt=""></td></tr>
 <tr>
  <td align="center">
   <font size="-1">
    <i> $trans_copyright<br>
EOF
if test "$modified_source" = "true"; then
  echo "        $trans_modified<br>" >>$file
fi
cat >> $file <<EOF
        $trans_fdl
    </i>
   </font>
  </td>
 </tr>
 <tr bgcolor="#000000"><td colspan="2" width="100%"><img width=1 height=1 alt=""></td></tr>
</table>
EOF
}

#
# Collects the contents part of a given page.
#
# Assembles the navigation and body part of the page.
# Convoluted. Mind-bending. Don't try this at home ;-)
#

function insert_news_column()
{  local file_base=$1
   local file=$2
   local lang=$3
   local variant=$4

cat >>$file <<EOF
  <td></td>
  <td $NEWSCOLOR valign="top" width="20%">
    <table width="100%" cellpadding="10"><tr><td>
      <font size="-1"><b>$trans_news</b></font>
      <table width="100%" cellpadding="0" cellspacing="0">
         <tr bgcolor="#000000" height=1><td width="100%" height=1><img width=1 height=1 alt=""></td></tr>
      </table><p>
      <table width="100%">
        <tr><td>
EOF
  eval "news_contents_${lang} $file_base headlines 0 $lang $variant"

cat >>$file <<EOF
        </td></tr>
      </table>
    </table>
  </td>
EOF
}

function contents()
{  local section=$1
   local lang=$2
   local page=$3
   local variant=$4
   local all_pages=$5
   local file_base
   local file_prefix
   local file
   local show_news=0
   local body_width="77%"

   if [ $page == 0 ]
     then file_base=$section
     else file_base=$section$page
   fi

   if [ $variant != 0 ]
     then file_prefix="${file_base}_${variant}"
     else file_prefix="${file_base}"
   fi

   if [ $file_prefix == "index" ]
     then show_news=1
          body_width="57%"
   fi

   file=${file_prefix}.html

# The Navigation and Contents field are nested tables.

cat >>$file <<EOF
<table width="100%" cellspacing=0>
 <tr>
  <td $IDXCOLOR valign="top" width="20%">
    <table width="100%" cellpadding="10"><tr><td>
      <font size="-1"><b>$trans_contents</b></font>
      <table width="100%" cellpadding="0" cellspacing="0">
         <tr bgcolor="#000000" height=1><td width="100%" height=1><img width=1 height=1 alt=""></td></tr>
      </table><p>
      <table width="100%">
EOF

# Collect the link list for the Navigation bar

for secidx in $SECTIONS; do
  eval "link_title=\"\$${secidx}_link\""

  if [ "$section" == "$secidx" ] # This is the active section; expand it
    then if [ "0" == "$page" ]   # Print the section title either as currently selected or as a link
           then echo "        <tr><td colspan=3><font size=\"-1\">$link_title</font></td></tr>" >> $file
           else echo "        <tr><td colspan=3><font size=\"-1\"><a href=\"${secidx}.html\">$link_title</a></font></td></tr>" >> $file
	 fi

	 for p in $pages; do     # Print all sub sections
	   if [ $p -gt 0 ]; then
	     local subsect=$(($p/10))     # sub section
	     local subsubsect=$(($p%10))  # sub sub section
	     local pagesect=$(($page/10)) # sub section of current page

	     eval "link_title=\"\$${secidx}${p}_link\""

	     if [ $subsubsect -gt 0 ]; then  # format as sub sub section
	       if [ $pagesect -eq $subsect ]; then  # include sub sub section list only for respective section
	         if [ "$p" == "$page" ]
		   then echo "        <tr><td valign=\"top\"></td><td>-</td><td><font size=\"-1\">${link_title}</font></td></tr>" >> $file 
	           else echo "        <tr><td valign=\"top\"></td><td>-</td><td><font size=\"-1\"><a href=\"${secidx}${p}.html\">$link_title</a></font></td></tr>" >> $file
		 fi
	       fi
	     else                            # format as sub section
	       if [ "$p" == "$page" ]
		 then echo "        <tr><td valign=\"top\" width=\"1%\">&middot;</td><td colspan=2><font size=\"-1\">${link_title}</font></td></tr>" >> $file 
	         else echo "        <tr><td valign=\"top\" width=\"1%\">&middot;</td><td colspan=2><font size=\"-1\"><a href=\"${secidx}${p}.html\">$link_title</a></font></td></tr>" >> $file
               fi
	     fi
	   fi
         done                    # Print all other sections unexpanded (title only)
    else echo "        <tr><td colspan=3><font size=\"-1\"><a href=\"${secidx}.html\">$link_title</a></font></td></tr>" >> $file
  fi
  echo "        <tr><td></td><td></td><td height=\"10\"></td></tr>" >> $file
done

cat >>$file <<EOF
      </table>
    </td></tr></table>
  </td>
EOF

# Paste in the text for the contents part

cat >> $file <<EOF
  <td></td>
  <td valign="top" width="$body_width" rowspan="2">
EOF

   eval "${prefix}_contents_${lang} $file_base content $page $lang $variant"

echo "  </td>" >>$file
if [ $show_news == 1 ]
  then insert_news_column $file_base $file $lang $variant 
fi

if [ $major_mode == "local" ]
  then
cat >> $file <<EOF
 </tr>
</table>
EOF
  else
cat >> $file <<EOF
 </tr>
 <tr valign="bottom">
   <td $IDXCOLOR align="center">
    <font size="-2">$trans_hosting</font><br>
EOF
    case $major_mode in
      sf)
cat >> $file <<EOF
      <a href="http://sourceforge.net">
         <img src="http://sflogo.sourceforge.net/sflogo.php?group_id=157550&amp;type=2" 
              width="125" height="37" border="0" alt="SourceForge.net Logo" />
      </a>
EOF
      ;;
    esac

cat >> $file <<EOF
   </td>
   <td></td><td></td>
 </tr>
</table>
EOF
fi
}

#
# Finish the HTML file
#
# The easy part: provide the closing </body></html> tags.
#

function finish()
{  local section=$1
   local lang=$2
   local page=$3
   local variant=$4
   local file_prefix
   local file

   if [ $page == 0 ]
     then file_prefix=$section
     else file_prefix=$section$page
   fi

   if [ $variant != 0 ]
     then file_prefix="${file_prefix}_${variant}"
   fi

   file=${file_prefix}.html

   cat >> $file <<EOF
</body>
</html>
EOF
}

###
### Helper functions
###
# Some more tricks to make static HTML appear as if it were dynamic.
#

#
# The news item function.
#

function news_item()
{  local file=$1
   local date=$2
   local headline=$3
   local mode=$4

   news_counter=$((news_counter + 1))

   if [ $mode == full ]; then
     echo "<table width=\"90%\"><tr>" >>$file
     echo "<td><a href="#item${news_counter}"></a><b>${headline}</b></td>">>$file
     echo "<td align=\"right\">$date</td>">>$file
     echo "</tr></table>">>$file
     cat >>$file <<EOF
      <table width="90%" cellpadding="0" cellspacing="0">
         <tr bgcolor="#000000" height=1><td width="100%" height=1><img width=1 height=1 alt=""></td></tr>
      </table>
      <table width="90%" cellpadding="0" cellspacing="0">
         <tr bgcolor="#ffffff" height=5><td width="100%" height=1><img width=1 height=1 alt=""></td></tr>
      </table>
      <table width="90%"><tr><td>
EOF
     cat >>$file
     echo "</td></tr></table><pre> </pre>">>$file
   else
     echo "<font size=\"-1\">$date</font> <br>" >>$file
     echo "<font size=\"-1\"><a href=\"news.html#item${news_counter}\">${headline}</a></font><p>" >>$file
   fi
}

#
# Subsections.
#

function subsection()
{ local file=$1
  local title=$2
  local subtitle=$3

cat >>$file <<EOF
  <table width="100%">
   <tr><td><font size="+1">$title</font></td></tr>
   <tr><td><i>$subtitle</i></td></tr>
   <tr bgcolor="#000000"><td width="100%"><img width=1 height=1 alt=""></td></tr>
  </table><p>
EOF
}

#
# The screenshot function.
#
# Each page can have multiple screenshot, which are initially shown
# in a blue thumbnail case to the right.
# The user can expand the thumbnail to full size; this will shrink any
# other expanded thumbnails back to original size. Also, clicking the expanded
# thumbnail will shrink it back.
# The expanded thumbnails are shown in the context of the originating page.

function screenshot()
{  local file_base="$1"
   local caption="$2"
   local image="$3"
   local expand="$4"
   local file

   if [ $expand != 0 ]; 
     then file="${file_base}_${expand}.html";
     else file="${file_base}.html";
   fi
   
   variant_counter=$((variant_counter + 1))
   if test $expand == 0; then
       variant_list="$variant_list $variant_counter"
   fi

   # Screen version (large image)

   if test $variant_counter == $expand; then
     cat >> $file <<EOF
       <table class="screenshot">
         <tr><td><a name="snap${variant_counter}"></a>
EOF
     cat >>$file

     cat >> $file <<EOF
	 </td></tr>
	 <tr><td><a href="${file_base}.html#snap${variant_counter}"><img src="images/$image"></a></td></tr>
	 <tr><td align="center">$trans_screen_shot: $caption</td></tr>
       </table>
       <pre> </pre>
EOF
   # Thumbnail version

   else  
     cat >> $file <<EOF
     <table>
       <tr>
       <td valign="top">
EOF
     cat >>$file

     cat >> $file <<EOF

       </td>
       <td><a name="snap${variant_counter}"></a>
         <table class="thumbnail" width="160px">
	   <tr><td><font size="-1">$caption</font></td></tr>
	   <tr><td><a href="${file_base}_${variant_counter}.html#snap${variant_counter}"><img src="thumbnails/$image"></a></td></tr>
	 </table>
       </td>
       </tr>
     </table><p>
EOF
   fi
}


###
### Documentation assembly
###

# Import the localized pages

#source create-cs.bash
source create-en.bash
#source create-de.bash
source create-ru.bash

# Assemble all pages together

function create_subpages()
{  local prefix=$1
   local lang=$2
   local pages=$3

   # First collect the information for the link panel

   eval "${prefix}_contents_${lang} $prefix link 0 $lang 0"
   eval "${prefix}_link=\"$link_title\""

   for page in $pages; do
     link_title=error
     eval "${prefix}_contents_${lang} ${prefix}$page link$page $page $lang 0"
     eval "${prefix}${page}_link=\"$link_title\""
   done

   # Now create the real pages

   for page in $pages; do
     variant_counter=0
     news_counter=0
     variant_list=""

     begin    ${prefix} $lang $page 0
     heading  ${prefix} $lang $page 0
     contents ${prefix} $lang $page 0 "$pages"
     footer   ${prefix} $lang $page 0
     finish   ${prefix} $lang $page 0

     for variant in $variant_list; do
	 variant_counter=0
	 news_counter=0
	 begin    ${prefix} $lang $page $variant
	 heading  ${prefix} $lang $page $variant
	 contents ${prefix} $lang $page $variant "$pages"
	 footer   ${prefix} $lang $page $variant
	 finish   ${prefix} $lang $page $variant
     done

   done

}

if test $major_mode = "local"
then SECTIONS="news index howtos download qa feedback"
else SECTIONS="news index howtos download qa feedback imprint"
fi

#if test $major_mode = "local"
#then SECTIONS="news index screenshots howtos download qa background"
#else SECTIONS="news index screenshots howtos download qa background imprint"
#fi


# Prepare entry links for each section

# English translation

dictionary_en

for sect in $SECTIONS; do
  link_title=error
  eval "${sect}_contents_en ignore link ignore en 0"
  eval "${sect}_link=\"$link_title\""
done

if ! test -e $workdir/en; then mkdir $workdir/en; fi
cd $workdir/en
create_subpages news en "0"
create_subpages index en "0 10 20 30"
create_subpages howtos en "0 10 20 21 22 30 40 50 90 91 92 93"
create_subpages download en "0 10 20 30 40"
create_subpages qa en "0 10 20"
create_subpages feedback en "0"

# create_subpages index en "0 10 20 30"
# create_subpages screenshots en "0 10 20 21 22 30 40 50"
# create_subpages howtos en "0 10 20 21 22 30 40 50"
# create_subpages download en "0 30 10 20"
# create_subpages background en "0 10 20 30 40 50 60 70"
# create_subpages background en "0 10 11 12 20 30"

if ! test $major_mode = "local"; then
  create_subpages imprint en "0"
fi

# Russian translation

dictionary_ru

for sect in $SECTIONS; do
  link_title=error
  eval "${sect}_contents_ru ignore link ignore ru 0"
  eval "${sect}_link=\"$link_title\""
done

if ! test -e $workdir/ru; then mkdir $workdir/ru; fi
cd $workdir/ru
create_subpages news ru "0"
create_subpages index ru "0 10 20 30"
create_subpages screenshots ru "0"
#create_subpages howtos ru "0 10 20 21 22 30 40 50"
create_subpages download ru "0 10 20 30 40"
create_subpages qa ru "0 10 20"
create_subpages feedback ru "0"
#create_subpages background ru "0 10 11 12 20 30"

if ! test $major_mode = "local"; then
  create_subpages imprint ru "0"
fi
