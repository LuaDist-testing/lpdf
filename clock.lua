-- clock.lua (based on pdfclock.c)

RADIUS=200
MARGIN=20

p:begin_page(2 * (RADIUS + MARGIN), 2 * (RADIUS + MARGIN))

p:translate(RADIUS + MARGIN, RADIUS + MARGIN)
p:setcolor("both", "rgb", 0, 0, 1)

p:save()
-- minute strokes
p:setlinewidth(2)
for alpha = 0,360-6,6 do
    p:rotate(6)
    p:moveto(RADIUS, 0)
    p:lineto(RADIUS-MARGIN/3, 0)
    p:stroke()
end
p:restore()

p:save()
-- 5 minute strokes
p:setlinewidth(3)
for alpha = 0,360-30,30 do
    p:rotate(30)
    p:moveto(RADIUS, 0)
    p:lineto(RADIUS-MARGIN, 0)
    p:stroke()
end
p:restore()

hour,min,sec=string.match(os.date"%H:%M:%S","(%d+):(%d+):(%d+)")

-- draw hour hand
p:save()
p:rotate(-((min/60) + hour - 3) * 30)
p:moveto(-RADIUS/10, -RADIUS/20)
p:lineto(RADIUS/2, 0)
p:lineto(-RADIUS/10, RADIUS/20)
p:closepath()
p:fill()
p:restore()

-- draw minute hand
p:save()
p:rotate(-((sec/60) + min - 15) * 6)
p:moveto(-RADIUS/10, -RADIUS/20)
p:lineto(RADIUS * 0.8, 0)
p:lineto(-RADIUS/10, RADIUS/20)
p:closepath()
p:fill()
p:restore()

-- draw second hand
p:setcolor("both", "rgb", 1, 0, 0)
p:setlinewidth(2)
p:save()
p:rotate(-((sec - 15) * 6))
p:moveto(-RADIUS/5, 0)
p:lineto(RADIUS, 0)
p:stroke()
p:restore()

-- draw little circle at center
p:circle(0, 0, RADIUS/30)
p:fill()

p:end_page()
