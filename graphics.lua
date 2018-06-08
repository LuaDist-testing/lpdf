-- graphics.lua (based on starter_graphics.c)

local xt=20
local x = 210
local y=770
local dy=90

-- Load the font for PDFlib Lite: change "unicode" to "winansi"
font = p:load_font("Helvetica", "host")
-- Start an A4 page
p:begin_page(0, 0, "width=a4.width height=a4.height")
-- Set the font
p:setfont(font, 14)
-- Set the line width
p:setlinewidth(2.0)
-- Set the stroke color
p:setcolor("stroke", "rgb", 0.0, 0.5, 0.5, 0.0)
-- Set the fill color
p:setcolor("fill", "rgb", 0.0, 0.85, 0.85, 0.0)
-- Stroke a line
-- Set the current point for graphics output
p:moveto(x, y)
-- Draw a line from the current point to the supplied point
p:lineto(x+300, y+50)
-- Stroke the path using the current line width and stroke color, and
-- clear it
p:stroke(p)
-- Output some descriptive black text
p:fit_textline("lineto() and stroke()", xt, y, "fillcolor={gray 0}")
-- Stroke a curve
-- Set the current point for graphics output
y=y-dy
p:moveto(x, y)
-- Draw a Bézier curve from the current point to (x3, y3), using three* control points
p:curveto(x+50, y+40, x+200, y+80, x+300, y+30)
-- Stroke the path using the current line width and stroke color, and clear it
p:stroke(p)
-- Output some descriptive black text
p:fit_textline("curveto() and stroke()", xt, y, 
"fillcolor={gray 0}")
-- Stroke a circle
-- Draw a circle at position (x, y) with a radius of 40
y=y-dy
p:circle(x, y, 40)
-- Stroke the path using the current line width and stroke color, and clear it
p:stroke(p)
-- Output some descriptive black text
p:fit_textline("circle() and stroke()", xt, y,
"fillcolor={gray 0}")
-- Stroke an arc segment
-- Draw an arc segment counterclockwise at position (x, y) with a radius
-- of 40 starting at an angle of 90 degrees and ending at 180 degrees

y=y-dy-20
p:arc(x, y, 40, 90, 180)
-- Stroke the path using the current line width and stroke color, and
-- clear it

p:stroke(p)
-- Output some descriptive black text
p:fit_textline("arc() and stroke()", xt, y,
"fillcolor={gray 0}")
-- Stroke a rectangle
-- Draw a rectangle at position (x, y) with a width of 200 and a height
-- of 50

y=y-dy
p:rect(x, y, 200, 50)
-- Stroke the path using the current line width and stroke color, and
-- clear it

p:stroke(p)
-- Output some descriptive black text
p:fit_textline("rect() and stroke()", xt, y,
"fillcolor={gray 0}")
-- ----------------
-- Fill a rectangle
-- ----------------

-- Draw a rectangle at position (x, y) with a width of 200 and a height
-- of 50

y=y-dy
p:rect(x, y, 200, 50)
-- Fill the path using current fill color, and clear it
p:fill(p)
-- Output some descriptive black text
p:fit_textline("rect() and fill()", xt, y,
"fillcolor={gray 0}")
-- ---------------------------
-- Fill and stroke a rectangle
-- ---------------------------

-- Draw a rectangle at position (x, y) with a width of 200 and a height
-- of 50

y=y-dy
p:rect(x, y, 200, 50)
-- Fill and stroke the path using the current line width, fill color,
-- and stroke color, and clear it

p:fill_stroke(p)
-- Output some descriptive black text
p:fit_textline("rect() and fill_stroke()", xt, y,
"fillcolor={gray 0}")
-- -------------------------------------------------------------
-- Draw a line and an arc, close the path and fill and stroke it
-- -------------------------------------------------------------

-- Set the current point for graphics output
y=y-dy
p:moveto(x-40, y)
-- Draw a line from the current point to the supplied point
p:lineto(x, y)
-- Draw an arc segment counterclockwise at position (x, y) with a radius
-- of 40 starting at an angle of 90 degrees and ending at 180 degrees

p:arc(x, y, 40, 90, 180)
-- Close the path and stroke and fill it, i.e. close the current subpath
-- (add a straight line segment from the current point to the starting
-- point of the path), and stroke and fill the complete current path

p:closepath_fill_stroke(p)
-- Output some descriptive black text
p:fit_textline("lineto(), arc(), and", xt, y+20,
"fillcolor={gray 0}")
p:fit_textline("closepath_fill_stroke()", xt, y,
"fillcolor={gray 0}")
-- -----------------------------------------------------------------
-- Draw a rectangle and use it as the clipping a path. Draw and fill
-- a circle and clip it according to the clipping path defined.
-- -----------------------------------------------------------------

-- Save the current graphics state including the current clipping
-- path which is set to the entire page by default

p:save(p)
-- Draw a rectangle at position (x, y) with a width of 200 and a height
-- of 50

y=y-dy
p:rect(x, y, 200, 50)
-- Use the current path as the clipping path for subsequent operations

p:clip(p)
-- Draw a circle at position (x, y) with a radius of 100
p:circle(x, y, 80)
-- Fill the path with the current fill color and clear it

p:fill(p)
-- Restore the graphics state which has been saved above
p:restore(p)
-- Output some descriptive black text
p:fit_textline("rect(), clip(),", xt, y+20,
"fillcolor={gray 0}")
p:fit_textline("circle(), and fill()", xt, y,
"fillcolor={gray 0}")
p:end_page()
