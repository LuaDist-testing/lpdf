-- test pdf binding

require"pdf"

local function run(x)
	print("-------",x)
	dofile(x)
end

print(pdf.version)

p = assert(pdf.new())
--p:set_parameter("compatibility","1.3")
assert(p:open_file("test.pdf"))

p:set_info("Creator", "test.lua")
p:set_info("Author", "Luiz Henrique de Figueiredo")
p:set_info("Title", "testing PDFlib binding for Lua")

run"logo.lua"
run"clock.lua"
run"lua-logo.lua"
run"image.lua"

p:close()

print(pdf.version)
