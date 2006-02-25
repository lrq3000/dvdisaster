#! /bin/bash

#  dvdisaster: Homepage generation script
#  Copyright (C) 2004-2006 Carsten Gnörlich
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

BGCOLOR1="bgcolor=\"#e8e8e8\""    # used in example section
BGCOLOR2="bgcolor=\"#f0f0f0\""    # used in exmaple section

major_mode=$1
workdir=`pwd`

project_title=dvdisaster
project_version=$2
cooked_version=`echo $2 | sed -e "s/pl[0-9]/ \(&\)/"`
project_package=$3

# Change into the homepage directory

if ! test -d "$workdir"; then
  echo "$workdir does not exist"
  exit 1
fi

# Make sure we've got a valid mode

case $major_mode in
  local) echo "Creating local documentation for $project_title $cooked_version" ;;
  www)   echo "Creating WWW homepage for $project_title $cooked_version" ;;
  *)     echo "Error: mode $major_mode unknown. Use \"local\" or \"www\"." ;;
esac

# begin a new html file

function begin()
{  local section=$1
   local lang=$2
   local page=$3
   local file_prefix
   local file

   if [ $page == 0 ]
     then file_prefix=$section
     else file_prefix=$section$page
   fi

   file=${file_prefix}.html

   eval "${section}_contents_${lang} $file_prefix title ignore $lang"

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
  table.example {width:100%; border-width:1px; border-style:solid; border-color:#000000; background-color:#ffffe0; font-family:fixed}
  table.example-win {width:100%; border-width:1px; border-style:solid; border-color:#000000; background-color:#e0e0ff; font-family:fixed}
  table.example-bsd {width:100%; border-width:1px; border-style:solid; border-color:#000000; background-color:#ffe0e0; font-family:fixed}
  .command { font-weight:bold }
-->
  </style>
</head>

<body $BGCOLOR>
EOF
}

# Creates the head bar.

function heading()
{  local section=$1
   local lang=$2
   local page=$3
   local file_prefix
   local file

   if [ $page == 0 ]
     then file_prefix=$section
     else file_prefix=$section$page
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
        echo "</td><td align=\"right\"><font size="+3">&nbsp;</font><a href=\"http://developer.berlios.de/projects/dvdisaster/$lang/\">$trans_to_internet</a>" >>$file
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

    case $lang in
      cs)  echo "   <td align=\"left\"><a href=\"http://developer.berlios.de/projects/dvdisaster/\">$trans_to_hoster</a></td>" >> $file
           echo "<td align=\"right\">" >>$file
           echo "&#268;esky &nbsp;&nbsp;&nbsp;" >>$file
	   echo "<a href=\"../de/$file\" title=\"Deutsche Sprache\">Deutsch</a> &nbsp;&nbsp;&nbsp;" >>$file
	   echo "<a href=\"../en/$file\" title=\"English language\">English</a>" >>$file
           echo "</td>" >>$file
	   ;;

      de) echo "   <td align=\"left\"><a href=\"http://developer.berlios.de/projects/dvdisaster/\">$trans_to_hoster</a></td>" >> $file
          echo "<td align=\"right\">" >>$file
          echo "<a href=\"../cs/$file\">&#268;esky</a> &nbsp;&nbsp;&nbsp;" >>$file
	  echo "Deutsch &nbsp;&nbsp;&nbsp;" >>$file
	  echo "<a href=\"../en/$file\">English</a>" >>$file
          echo "</td>" >>$file
	  ;;

      *)  echo "   <td align=\"left\"><a href=\"http://developer.berlios.de/projects/dvdisaster/\">$trans_to_hoster</a></td>" >> $file
          echo "<td align=\"right\">" >>$file
          echo "<a href=\"../cs/$file\">&#268;esky</a> &nbsp;&nbsp;&nbsp;" >>$file
	  echo "<a href=\"../de/$file\">Deutsch</a> &nbsp;&nbsp;&nbsp;" >>$file
	  echo "English" >>$file
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

# Creates the footer bar.

function footer()
{  local section=$1
   local lang=$2
   local page=$3
   local file_prefix
   local file

   if [ $page == 0 ]
     then file_prefix=$section
     else file_prefix=$section$page
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
        $trans_fdl
    </i>
   </font>
  </td>
 </tr>
 <tr bgcolor="#000000"><td colspan="2" width="100%"><img width=1 height=1 alt=""></td></tr>
</table>
EOF
}

# Collects the contents part of a given page.

function contents()
{  local section=$1
   local lang=$2
   local page=$3
   local all_pages=$4
   local file_prefix
   local file

   if [ $page == 0 ]
     then file_prefix=$section
     else file_prefix=$section$page
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
         done                    # Print all other section unexpanded (title only)
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
  <td valign="top" width="77%" rowspan="2">
EOF

   eval "${prefix}_contents_${lang} $file_prefix content $page $lang"

if [ $major_mode == "local" ]
  then
cat >> $file <<EOF
  </td>
 </tr>
</table>
EOF
  else
cat >> $file <<EOF
  </td>
 </tr>
 <tr valign="bottom">
   <td $IDXCOLOR align="center">
    <font size="-2">$trans_hosting</font><br>
      <a href="http://developer.berlios.de" title="BerliOS Developer"> 
        <img src="http://developer.berlios.de/bslogo.php?group_id=2105" 
             width="124px" height="32px" border="0" alt="BerliOS Developer Logo">
      </a>
   </td>
   <td></td><td></td>
 </tr>
</table>
EOF
fi
}

# finish the HTML file

function finish()
{  local section=$1
   local lang=$2
   local page=$3
   local file_prefix
   local file

   if [ $page == 0 ]
     then file_prefix=$section
     else file_prefix=$section$page
   fi

   file=${file_prefix}.html

   cat >> $file <<EOF
</body>
</html>
EOF
}

# Inlined pages are only referenced from within another page, 
# but are not accessible via / do not provide the table of contents.

function create_inline()
{  local prefix=$1
   local lang=$2
   local page=$3
   local backlink=$4
   local backlink_text="$5"

   file=${prefix}${page}.html

   # Now create the real page

   begin    ${prefix} $lang $page
   heading  ${prefix} $lang $page

   echo "<table width=\"70%\" align=\"center\">" >> $file
   eval "${prefix}${page}${lang} $file"
   echo "<tr><td><pre> </pre><a href=\"$backlink\">$trans_back $backlink_text</a></td></tr>" >> $file    
   echo "</table>" >> $file

   footer   ${prefix} $lang $page
   finish   ${prefix} $lang $page
}

# Import the localized pages

source create-cs.bash
source create-en.bash
source create-de.bash

# Assemble all pages together --------------------------------------------------

function create_subpages()
{  local prefix=$1
   local lang=$2
   local pages=$3

   # First collect the information for the link panel

   eval "${prefix}_contents_${lang} $prefix link 0 $lang"
   eval "${prefix}_link=\"$link_title\""

   for page in $pages; do
     link_title=error
     eval "${prefix}_contents_${lang} ${prefix}$page link$page $page $lang"
     eval "${prefix}${page}_link=\"$link_title\""
   done

   # Now create the real pages

   for page in $pages; do
     begin    ${prefix} $lang $page
     heading  ${prefix} $lang $page
     contents ${prefix} $lang $page "$pages"
     footer   ${prefix} $lang $page
     finish   ${prefix} $lang $page
   done
}

SECTIONS="index example download qa background imprint"

# Prepare entry links for each section

# German translation

dictionary_de

for sect in $SECTIONS; do
  link_title=error
  eval "${sect}_contents_de ignore link ignore de"
  eval "${sect}_link=\"$link_title\""
done

if ! test -e $workdir/de; then mkdir $workdir/de; fi
cd $workdir/de
create_subpages index de "0 10 20 30"
create_subpages example de "0 10 20 21 22 30 40 50 80 81 82 83 90"
create_subpages download de "0 10 20"
create_subpages qa de "0 10 20"
create_subpages background de "0 10 20 30 40 50 60 70"
create_subpages imprint de "0"

# English translation

dictionary_en

for sect in $SECTIONS; do
  link_title=error
  eval "${sect}_contents_en ignore link ignore en"
  eval "${sect}_link=\"$link_title\""
done

if ! test -e $workdir/en; then mkdir $workdir/en; fi
cd $workdir/en
create_subpages index en "0 10 20 30"
create_subpages example en "0 10 20 21 22 30 40 50 80 81 82 83 90"
create_subpages download en "0 10 20"
create_subpages qa en "0 10 20"
create_subpages background en "0 10 20 30 40 50 60 70"
create_subpages imprint en "0"

# Czech translation

dictionary_cs

for sect in $SECTIONS; do
  link_title=error
  eval "${sect}_contents_cs ignore link ignore cs"
  eval "${sect}_link=\"$link_title\""
done

if ! test -e $workdir/cs; then mkdir $workdir/cs; fi
cd $workdir/cs
create_subpages index cs "0 10 20 30"
create_subpages example cs "0 10 20 21 22 30 40 50 80 81 82 83 90"
create_subpages download cs "0 10 20"
create_subpages qa cs "0 10 20"
create_subpages background cs "0 10 20 30 40 50 60 70"
create_subpages imprint cs "0"
