-- A simple scene with some miscellaneous geometry.

mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25, 0.0, 0.0, 1.0)
--mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25, 0.0, 0.0, 4.0)
--mat4 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25)
--mat5 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25)
scene_root = gr.node('root')

--s1 = gr.nh_sphere('s1', {0, 0, -400}, 100)
--scene_root:add_child(s1)
--s1:set_material(mat1)

tex1 = gr.texture(1, {0.7, 1.0, 0.7}, {1.0, 0.7, 0.7})
marble = gr.texture(2, {1.0, 1.0, 1.0}, {0.0, 0.0, 0.0})
flame = gr.texture(2, {1.0, 0.2, 0.2}, {1.0, 1.0, 0.3})

s2 = gr.cylinder('s2')
scene_root:add_child(s2)
s2:scale(150, 150, 150)
s2:rotate('X', 45)
s2:rotate('Z', 45)
s2:translate(200, -100, -100)
s2:set_material(mat1)
s2:set_texture(marble)

s3 = gr.sphere('s3')
scene_root:add_child(s3)
s3:scale(150, 150, 150)
s3:translate(-150, 50, -100)
s3:set_material(mat1)
s3:set_texture(marble)

s4 = gr.cone('s4')
scene_root:add_child(s4)
s4:scale(50, 300, 50)
s4:rotate('X', 45)
s4:rotate('Y', 45)
s4:translate(-200, -200, 150)
s4:set_material(mat1)
s4:set_texture(flame)

--s3 = gr.nh_sphere('s3', {0, -1200, -500}, 1000)
--scene_root:add_child(s3)
--s3:set_material(mat2)

--b1 = gr.nh_box('b1', {-200, -225, 100}, 100)
--scene_root:add_child(b1)
--b1:set_material(mat4)

-- s4 = gr.nh_sphere('s4', {-100, 25, -300}, 50)
--s4 = gr.nh_sphere('s4', {-150, -085, 150}, 30)
--scene_root:add_child(s4)
--s4:set_material(mat3)

--s5 = gr.nh_sphere('s5', {0, 100, -250}, 25)
--scene_root:add_child(s5)
--s5:set_material(mat1)

-- A small stellated dodecahedron.

--steldodec = gr.mesh( 'dodec', 'smstdodeca.obj' )
--steldodec:set_material(mat3)
--scene_root:add_child(steldodec)

white_light = gr.light({-100.0, 150.0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
magenta_light = gr.light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0})

gr.render(scene_root, 'test5.png', 512, 512,
	  {0, 0, 800}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.3, 0.3, 0.3}, {white_light, magenta_light})
