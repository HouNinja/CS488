-- A simple scene with some miscellaneous geometry.

mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25, 1.0, 1.0, 1.0)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25, 1.0, 1.0, 1.0)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25, 1.0, 1.0, 1.0)
mat4 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25, 1.0, 1.0, 1.0)

mat5 = gr.material({0.9, 0.9, 0.9}, {0.5, 0.7, 0.5}, 25, 0.0, 0.0, 1.0)

stone = gr.material({0.8, 0.7, 0.7}, {0.0, 0.0, 0.0}, 0, 0.0, 0.0, 1.0)
gold = gr.material({1.0, 0.7, 0.0}, {0.0, 0.0, 0.0}, 20, 0.2, 0.0, 1.0)
glass = gr.material({0.9, 0.9, 0.9}, {0.0, 0.0, 0.0}, 20, 0.2, 0.7, 1.0)

tex1 = gr.texture(1, {0.9, 0.9, 0.9}, {0.4, 0.4, 0.4})
marble = gr.texture(2, {1.0, 1.0, 1.0}, {0.0, 0.0, 0.0})
flame = gr.texture(2, {1.0, 0.2, 0.2}, {1.0, 1.0, 0.3})

scene = gr.node('root')

trophy1 = gr.node('trophy1')
scene:add_child(trophy1)
trophy1:scale(0.7, 0.7, 0.7)
trophy1:translate(-1.5, 25.2, 23)

base1 = gr.cylinder('base1')
trophy1:add_child(base1)
base1:scale(1.5, 0.3, 1.5)
base1:set_material(mat5)
base1:set_texture(marble)

base2 = gr.cylinder('base2')
trophy1:add_child(base2)
base2:scale(1.2, 0.2, 1.2)
base2:translate(0.0, 0.5, 0.0)
base2:set_material(gold)
--base2:set_texture(marble)

support1 =  gr.cylinder('support1')
trophy1:add_child(support1)
support1:scale(0.1, 0.2, 0.1)
support1:translate(-0.5, 0.9, 0.4)
support1:set_material(gold)


support2 =  gr.cylinder('support2')
trophy1:add_child(support2)
support2:scale(0.1, 0.2, 0.1)
support2:translate(0.5, 0.9, 0.4)
support2:set_material(gold)


support3 =  gr.cylinder('support3')
trophy1:add_child(support3)
support3:scale(0.1, 0.2, 0.1)
support3:translate(0.0, 0.9, -0.75)
support3:set_material(gold)

ball1 = gr.sphere('ball1')
trophy1:add_child(ball1)
ball1:scale(1.1, 1.1, 1.1)
ball1:translate(0.0, 1.8, 0.0)
ball1:set_material(glass)


trophy2 = gr.node('trophy2')
scene:add_child(trophy2)
trophy2:scale(0.7, 0.7, 0.7)
trophy2:translate(1.5, 25.1, 23)

base3 = gr.cylinder('base3')
trophy2:add_child(base3)
base3:scale(1.7, 0.1, 1.7)
base3:set_material(gold)

base4 = gr.cylinder('base4')
trophy2:add_child(base4)
base4:scale(1.4, 0.1, 1.4)
base4:translate(0.0, 0.2, 0.0)
base4:set_material(gold)

body = gr.cone('body')
trophy2:add_child(body)
body:scale(1, 4, 1)
body:translate(0.4, -0.8, 0.0)
body:set_material(gold)

support4 =  gr.cylinder('support4')
trophy2:add_child(support4)
support4:scale(0.1, 0.2, 0.1)
support4:translate(0.8, 3.3, 0.0)
support4:set_material(gold)

ball2 = gr.sphere('ball2')
trophy2:add_child(ball2)
ball2:scale(1.1, 1.1, 1.1)
ball2:translate(1.0, 4.4, 0.0)
ball2:set_material(gold)

cube = gr.nh_box('cube', {-25,-25,-25}, 50)
scene:add_child(cube)
cube:set_material(stone)
cube:set_texture(tex1)
white_light = gr.light({-20.0, 0.0, 300.0}, {0.6, 0.6, 0.6}, {1, 0, 0})
top_left_light = gr.light({-25.0, 70.0, 40.0}, {0.6, 0.6, 0.6}, {1, 0, 0})

gr.render(scene, 'scene.png', 512, 512,
	  {2, 30, 31}, {-0.12, -0.5, -1}, {0, 1, 0}, 50,
	  {0.3, 0.3, 0.3}, {white_light, top_left_light})