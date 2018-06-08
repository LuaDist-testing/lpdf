-- test pdflib binding

require"pdf"

print(pdf.version)

local function run(x)
	FILE=x
	print("",x)
	dofile(x)
end

pdf._begin_page=pdf.begin_page
function pdf.begin_page(...)
	pdf._begin_page(...)
	p:save()
end
pdf._end_page=pdf.end_page
function pdf.end_page(...)
	p:restore()
	p:setcolor("both","rgb",0.8,0.8,0.8)
	local W = p:get_value("pagewidth")
	local font = p:load_font("Helvetica","host")
	p:setfont(font,0.015*W)
	p:fit_textline(FILE,W-2,2,"position={right bottom} orientate=west")
	pdf._end_page(...)
end

p = assert(pdf.new())
--p:set_parameter("logging","filename=log remove")
--p:set_parameter("compatibility","1.3")
p:set_parameter("errorpolicy","exception")


p:begin_document("test.pdf")
p:set_info("Creator","test.lua")
p:set_info("Author","Luiz Henrique de Figueiredo")
p:set_info("Title","testing PDFlib binding for Lua")

run"logo.lua"
run"clock.lua"
run"lua-logo.lua"
run"image.lua"
run"graphics.lua"

p:end_document()

print(pdf.version)
