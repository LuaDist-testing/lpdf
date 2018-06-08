-- This file was automatically generated for the LuaDist project.

package = "lpdf"
version = "20130627.52-1"
-- LuaDist source
source = {
  tag = "20130627.52-1",
  url = "git://github.com/LuaDist-testing/lpdf.git"
}
-- Original source
-- source = {
-- 	url = "http://www.tecgraf.puc-rio.br/~lhf/ftp/lua/5.2/lpdf.tar.gz",
-- 	md5 = "ea3620317d653262d2a43977859b5785",
-- 	dir = "pdf",
-- }
description = {
	summary = "A library for generating PDF documents based on PDFlib",
	homepage = "http://www.tecgraf.puc-rio.br/~lhf/ftp/lua/#lpdf",
	license = "Public Domain",
}
dependencies = {
	"lua ~> 5.2"
}
external_dependencies = {
	PDFLIB = {
		header = "pdflib.h"
	}
}
build = {
	type = "builtin",
	modules = {
		pdf = {
			sources = {"lpdf.c",},
			libraries = {"pdf"},
			incdirs = {"$(PDFLIB_INCDIR)"},
			libdirs = {"$(PDFLIB_LIBDIR)"},
		},
	},
}