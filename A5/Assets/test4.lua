-- A simple scene with some miscellaneous geometry.

mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25, 0.0, 0.0, 1.0)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25, 1.0, 1.0, 1.0)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25, 1.0, 1.0, 1.0)
mat4 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25, 1.0, 1.0, 1.0)

mat5 = gr.material({0.9, 0.9, 0.9}, {0.5, 0.7, 0.5}, 25, 1.0, 1.0, 1.0)

stone = gr.material({0.8, 0.7, 0.7}, {0.0, 0.0, 0.0}, 0, 1.0, 1.0, 1.0)

tex1 = gr.texture(1, {0.7, 1.0, 0.7}, {1.0, 0.7, 0.7})
marble = gr.texture(2, {1.0, 1.0, 1.0}, {0.0, 0.0, 0.0})
flame = gr.texture(2, {1.0, 0.2, 0.2}, {1.0, 1.0, 0.3})

scene_root = gr.node('root')

c1 = gr.nh_sphere('c1', {0,0,0}, 1)
scene_root:add_child(c1);
c1:set_material(mat1)
--c1:set_texture(marble)


--base = gr.cylinder('base')
--trophy1:add_child(base)


white_light = gr.light({-20.0, 0.0, 300.0}, {0.6, 0.6, 0.6}, {1, 0, 0})
magenta_light = gr.light({100.0, -50.0, 100.0}, {0.6, 0.6, 0.6}, {1, 0, 0})

gr.render(scene_root, 'test4.png', 512, 512,
	  {0, 0, 5}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.3, 0.3, 0.3}, {white_light})
