; German messages

; WARNING!!! the file encoding is Win-1252
; Copyright (C) 2004-2006 THE dvdisaster'S COPYRIGHT HOLDER
; Copyright 2006 Lubos Stanek
; This file is distributed under the same license as the dvdisaster package.

; this file must include specific dvdisaster setup's messages
; remember that string are formatted according to the NSIS spec


; Language
!define LANG_ID_GERMAN 1031

LangString LANG_PUBLISHER ${LANG_GERMAN} "Carsten Gnöerlich"
LangString LANG_WEB_SITE ${LANG_GERMAN} "http://www.dvdisaster.de/index.html"
LangString LANG_UPDATE_SITE ${LANG_GERMAN} "http://www.dvdisaster.de/download.html"
LangString LANG_PUBLISHER_SITE ${LANG_GERMAN} "http://www.dvdisaster.de/imprint.html"

LangString LANG_INSTALLTYPE_STANDARD ${LANG_GERMAN} "Standard"
LangString LANG_INSTALLTYPE_FULL ${LANG_GERMAN} "Complete"

LangString LANG_SECTION_APPLICATION ${LANG_GERMAN} "Application"
LangString LANG_SECTION_ERROR ${LANG_GERMAN} "The $\"%s$\" section were not installed properly!"
LangString LANG_SECTION_MANUAL ${LANG_GERMAN} "Manual"
; the following refers to previous; if it is not possible to translate it as it is,
; prepend the previous string into all localizations (e.g. Manual in Czech)
; and translate the full expression
LangString LANG_SECTION_MANUAL_CZECH ${LANG_GERMAN} "in Czech"
LangString LANG_SECTION_MANUAL_ENGLISH ${LANG_GERMAN} "in English"
LangString LANG_SECTION_MANUAL_GERMAN ${LANG_GERMAN} "in German"

LangString LANG_SECTION_SHORTCUTS ${LANG_GERMAN} "Start menu shortcuts"
LangString LANG_SECTION_SHORTCUTS_DESC ${LANG_GERMAN} "Zusätzliche Fehlerkorrektur für CD/DVD"

LangString LANG_SECTION_DESKTOP ${LANG_GERMAN} "Desktop shortcut"

LangString DESC_SectionApp ${LANG_GERMAN} "Installs the application, libraries, library settings, and message catalogs."
LangString DESC_SectionManual ${LANG_GERMAN} "Installs the localized manual."
LangString DESC_SectionShortcuts ${LANG_GERMAN} "Creates shortcuts."

LangString LANG_APPISRUNNING ${LANG_GERMAN} "${CFG_PACKAGE} is running!$\r$\rTerminate all instances of ${CFG_PACKAGE} and try again."
LangString LANG_NOINSTALLPERMS ${LANG_GERMAN} "You do not possess required rights for installations!$\r$\rThe installer will be terminated."
