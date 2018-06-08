-- logo.lua

RESOLUTION=512

p:begin_page(RESOLUTION, RESOLUTION)

p:translate(RESOLUTION/2, RESOLUTION/2)
p:scale(RESOLUTION/2/math.sqrt(2), RESOLUTION/2/math.sqrt(2))

p:setcolor("both", "rgb", 1, 0.695, 0.06)
p:circle(0, 0, 1)
p:fill()

p:setlinewidth(0.02)
p:setcolor("both", "rgb", 0, 0, 0)
p:circle(0, 0, 1)
p:stroke()

font = p:load_font("Helvetica", "host")
p:setfont(font, 0.3)
p:setcolor("both", "rgb", 1, 1, 1)
p:show_xy("PDFlib", -0.5, 0.0)
p:continue_text("with Lua")

p:setfont(font, 0.05)
p:setcolor("both", "rgb", 0, 0, 0)
p:show_xy(p.version, -1.4, -1.4)

p:end_page()
