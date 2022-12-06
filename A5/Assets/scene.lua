-- A simple scene with some miscellaneous geometry.

//mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25, 1.0, 1.0, 1.0)
//mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25, 1.0, 1.0, 1.0)
//mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25, 1.0, 1.0, 1.0)
//mat4 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25, 1.0, 1.0, 1.0)

mat5 = gr.material({0.9, 0.9, 0.9}, {0.5, 0.7, 0.5}, 25, 0.0, 0.0, 0.0)

stone = gr.material({0.8, 0.7, 0.7}, {0.0, 0.0, 0.0}, 0, 1.0, 1.0, 1.0)

tex1 = gr.texture(1, {0.7, 1.0, 0.7}, {1.0, 0.7, 0.7})
marble = gr.texture(2, {1.0, 1.0, 1.0}, {0.0, 0.0, 0.0})
flame = gr.texture(2, {1.0, 0.2, 0.2}, {1.0, 1.0, 0.3})

scene = gr.node('root')

trophy1 = gr.node('trophy1')
scene:add_child(trophy1)

base1 = gr.cylinder('base1')
trophy1:add_child(base1)
base1:scale(1.5, 0.3, 1.5)
base1:set_material(mat5)
base1:set_texture(marble)

base2 = gr.cylinder('base2')
trophy1:add_child(base)
base1:scale(1.5, 0.3, 1.5)
base1:set_material(mat5)
base1:set_texture(marble)



white_light = gr.light({-20.0, 0.0, 300.0}, {0.6, 0.6, 0.6}, {1, 0, 0})
magenta_light = gr.light({100.0, -50.0, 100.0}, {0.6, 0.6, 0.6}, {1, 0, 0})

gr.render(scene, 'scene.png', 512, 512,
	  {0, 3, 10}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.3, 0.3, 0.3}, {white_light})