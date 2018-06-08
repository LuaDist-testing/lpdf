-- image.lua (based on image.c)

p:set_parameter("imagewarning", "true")
image = assert(p:load_image("png", "image.png"))

width = p:get_value("imagewidth", image)
height = p:get_value("imageheight", image)

p:begin_page(width, height)
p:fit_image(image, 0, 0)
p:close_image(image)
p:end_page()
