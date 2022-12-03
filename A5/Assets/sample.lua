-- A simple scene with some miscellaneous geometry.

mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25)
mat4 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25)
mat4 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25)

soil = gr.material({0.4, 0.2, 0.0}, {0.04, 0.02, 0.01}, 25)
ocean = gr.material({0.4, 0.4, 1.0}, {0.2, 0.2, 0.2}, 70)
stone = gr.material({0.8, 0.7, 0.7}, {0.0, 0.0, 0.0}, 0)
hide = gr.material({0.84, 0.6, 0.53}, {0.3, 0.3, 0.3}, 20)

scene_root = gr.node('root')



s3 = gr.sphere('s3')

scene_root:add_child(s3)
s3:set_material(ocean)
s3:scale(150, 150, 150)
s3:translate(0, -100, 30)

land = gr.sphere('land')
s3:add_child(land)
land:set_material(soil)
land:scale(0.9, 0.9, 0.9)
land:translate(-0.07, 0.08, 0.09)


cow = gr.node('the_cow')
for _, spec in pairs({
	{'body', {0, 0, 0}, 1.0},
	{'head', {.9, .3, 0}, 0.6},
	{'tail', {-.94, .34, 0}, 0.2},
	{'lfleg', {.7, -.7, -.7}, 0.3},
	{'lrleg', {-.7, -.7, -.7}, 0.3},
	{'rfleg', {.7, -.7, .7}, 0.3},
	{'rrleg', {-.7, -.7, .7}, 0.3}
	 }) do
part = gr.nh_sphere(table.unpack(spec))
part:set_material(hide)
cow:add_child(part)
end

land:add_child(cow)
cow:scale(0.1, 0.1, 0.1)
cow:translate(0.15, 0.7, 0.8)

steldodec = gr.mesh( 'dodec', 'smstdodeca.obj' )
steldodec:set_material(mat3)
scene_root:add_child(steldodec)
steldodec:scale(0.5, 0.5, 0.5)
steldodec:translate(1.0, -50, 200.0)


white_light = gr.light({-200.0, 300.0, 800.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
magenta_light = gr.light({400.0, 100.0, 150.0}, {0.5, 0.5, 0.9}, {1, 0, 0})

gr.render(scene_root, 'sample.png', 512, 512,
	  {0, 0, 400}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.3, 0.3, 0.3}, {white_light})
