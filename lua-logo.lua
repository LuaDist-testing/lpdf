-- lua-logo.lua

sqrt=math.sqrt
PI=math.pi

RESOLUTION=512

p:begin_page(RESOLUTION, RESOLUTION)

p:translate(RESOLUTION/2, RESOLUTION/2)
p:scale(RESOLUTION/2/sqrt(2), RESOLUTION/2/sqrt(2))

-- planet
p:setcolor("fill", "rgb", 0, 0, 0.5)
p:circle(0, 0, 1)
p:fill()

-- hole
r=1-sqrt(2)/2
p:setcolor("fill", "rgb", 1, 1, 1)
p:circle(1-2*r, 1-2*r, r)
p:fill()

-- moon
p:setcolor("fill", "rgb", 0, 0, 0.5)
p:circle(1, 1, r)
p:fill()

-- logo
font = p:load_font("Helvetica", "host")
p:setfont(font, 0.9)
x=p:stringwidth("Lua", font, 0.9)
p:setcolor("both", "rgb", 1, 1, 1, 0)
p:show_xy("Lua", -x/2, -0.5)

-- orbit
p:setlinewidth(0.03)
x=(1+r)*(PI/180*5)
p:setdash(x,x)
p:setcolor("stroke", "gray", 0.5)
p:arcn(0, 0, 1+r, 32, 57)
p:stroke()

p:end_page()
