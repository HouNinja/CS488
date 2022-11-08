rootnode = gr.node('root')

red = gr.material({1.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 10)
blue = gr.material({0.0, 0.0, 1.0}, {0.1, 0.1, 0.1}, 10)
green = gr.material({0.0, 1.0, 0.0}, {0.1, 0.1, 0.1}, 10)
white = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 10)
black = gr.material({0.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 10)

---------------torso-------------------
torso = gr.mesh('sphere','torso')
rootnode:add_child(torso)
torso:set_material(white)
torso:scale(0.8, 2.5, 0.8)

---------------torso_shoulder_joint----
torso_shoulder_joint = gr.joint('torso_shoulder_joint', {-5, 0, 5}, {-30, 0, 30});
torso:add_child(torso_shoulder_joint)

----------------shoulder---------------

shoulder = gr.mesh('sphere','shoulder')
torso_shoulder_joint:add_child(shoulder)
shoulder:set_material(white)
shoulder:scale(3.5, 0.3, 0.7)
shoulder:translate(0.0, 1.0, 0.0)


----------------neck_rotate----------------------
neck_rotate = gr.mesh('sphere','neck_rotate')
shoulder:add_child(neck_rotate)
neck_rotate:set_material(red)
neck_rotate:scale(0.25, 1.0, 1.0)
neck_rotate:scale(0.2, 0.2, 0.2)
neck_rotate:translate(0.0, 1.4, 0.0)

---------------shoulder_neck_joint----
shoulder_neck_joint = gr.joint('shoulder_neck_joint', {-5, 0, 5}, {-30, 0, 30});
neck_rotate:add_child(shoulder_neck_joint)

----------------neck-------------------

neck = gr.mesh('sphere','neck')
shoulder_neck_joint:add_child(neck)
neck:set_material(red)
neck:scale(4.0, 4.0, 4.0)
neck:translate(0.0, 1.0, 0.0)

----------------head_rotate---------------
head_rotate = gr.mesh('sphere','head_rotate')
neck:add_child(head_rotate)
head_rotate:set_material(red)
head_rotate:scale(0.25, 0.25, 0.25)
head_rotate:translate(0.0, 1.0, 0.0)

---------------neck_head_joint-------

neck_head_joint = gr.joint('neck_head_joint', {-30, 0, 60}, {-30, 0, 30});
head_rotate:add_child(neck_head_joint)

----------------head-------------------
head = gr.mesh('sphere','head')
neck_head_joint:add_child(head)
head:set_material(white)
head:scale(5, 5, 5)
head:scale(2, 2, 2)
head:translate(0.0, 5.5, 0.0)

--------------left_eye-------------------
left_eye = gr.mesh('sphere','left_eye')
head:add_child(left_eye)
left_eye:set_material(black)
left_eye:scale(0.17, 0.17, 0.17)
left_eye:translate(-0.3, 0.1, 0.9)

--------------right_eye-------------------
right_eye = gr.mesh('sphere','right_eye')
head:add_child(right_eye)
right_eye:set_material(black)
right_eye:scale(0.17, 0.17, 0.17)
right_eye:translate(0.3, 0.1, 0.9)


-----------left_shoulder-----------------
left_shoulder = gr.mesh('sphere','left_shoulder')
shoulder:add_child(left_shoulder)
left_shoulder:set_material(white)
left_shoulder:scale(0.3, 1.0, 1.2)
left_shoulder:scale(1.15, 1.15, 1.15)
left_shoulder:translate(-0.7, 0.0, 0.0)

--------shoulder_left_upper_arm_joint------
shoulder_left_upper_arm_joint = gr.joint('shoulder_left_upper_arm_joint', {-180, 0, 30}, {-30, 0, 80});
left_shoulder:add_child(shoulder_left_upper_arm_joint)


---------------left_upper_arm-----------
left_upper_arm = gr.mesh('sphere','left_upper_arm')
shoulder_left_upper_arm_joint:add_child(left_upper_arm)
left_upper_arm:set_material(white)
left_upper_arm:scale(0.8, 2.5, 0.8)
left_upper_arm:scale(0.95, 0.95, 0.95)
left_upper_arm:translate(0.0, -1.7, 0.0)

---------------left_elbow-------------------
left_elbow = gr.mesh('sphere','left_elbow')
left_upper_arm:add_child(left_elbow)
left_elbow:set_material(white)
left_elbow:scale(1.25, 0.4, 1.25)
left_elbow:scale(0.5, 0.5, 0.5)
left_elbow:translate(0.0, -1.0, 0.0)


----------left_upper_arm_forearm_joint-----
left_upper_arm_forearm_joint = gr.joint('left_upper_arm_forearm_joint', {-150, 0, 0}, {-110, 0, 110});
left_elbow:add_child(left_upper_arm_forearm_joint)


---------------left_fore_arm-----------
left_fore_arm = gr.mesh('sphere','left_fore_arm')
left_upper_arm_forearm_joint:add_child(left_fore_arm)
left_fore_arm:set_material(white)
left_fore_arm:scale(0.8, 2.5, 0.8)
left_fore_arm:scale(1.4, 1.4, 1.4)
left_fore_arm:translate(0.0, -3.0, 0.0)


---------------left_wrist-----------------
left_wrist = gr.mesh('sphere','left_wrist')
left_fore_arm:add_child(left_wrist)
left_wrist:set_material(white)
left_wrist:scale(1.25, 0.4, 1.25)
left_wrist:scale(0.5, 0.5, 0.5)
left_wrist:translate(0.0, -1.0, 0.0)


----------left_forearm_hand_joint-----
left_forearm_hand_joint = gr.joint('left_forearm_hand_joint', {-70, 0, 70}, {-70, 0, 70});
left_wrist:add_child(left_forearm_hand_joint)

----------left_hand------------------
left_hand = gr.mesh('sphere','left_hand')
left_forearm_hand_joint:add_child(left_hand)
left_hand:set_material(white)
left_hand:scale(1.0, 2.0, 1.0)
left_hand:translate(0.0, -2.0, 0.3)

-----------------------------------------left_arm_done here---------------

--------------------------------right arm start-----------------

-----------right_shoulder-----------------
right_shoulder = gr.mesh('sphere','right_shoulder')
shoulder:add_child(right_shoulder)
right_shoulder:set_material(white)
right_shoulder:scale(0.3, 1.0, 1.2)
right_shoulder:scale(1.15, 1.15, 1.15)
right_shoulder:translate(0.7, 0.0, 0.0)

--------shoulder_right_upper_arm_joint------
shoulder_right_upper_arm_joint = gr.joint('shoulder_left_upper_arm_joint', {-180, 0, 30}, {-80, 0, 30});
right_shoulder:add_child(shoulder_right_upper_arm_joint)


---------------right_upper_arm-----------
right_upper_arm = gr.mesh('sphere','left_upper_arm')
shoulder_right_upper_arm_joint:add_child(right_upper_arm)
right_upper_arm:set_material(white)
right_upper_arm:scale(0.8, 2.5, 0.8)
right_upper_arm:scale(0.95, 0.95, 0.95)
right_upper_arm:translate(0.0, -1.7, 0.0)


---------------right_elbow-------------------
right_elbow = gr.mesh('sphere','right_elbow')
right_upper_arm:add_child(right_elbow)
right_elbow:set_material(white)
right_elbow:scale(1.25, 0.4, 1.25)
right_elbow:scale(0.5, 0.5, 0.5)
right_elbow:translate(0.0, -1.0, 0.0)

----------right_upper_arm_forearm_joint-----
right_upper_arm_forearm_joint = gr.joint('right_upper_arm_forearm_joint', {-150, 0, 0}, {-110, 0, 110});
right_elbow:add_child(right_upper_arm_forearm_joint)


---------------right_fore_arm-----------
right_fore_arm = gr.mesh('sphere','right_fore_arm')
right_upper_arm_forearm_joint:add_child(right_fore_arm)
right_fore_arm:set_material(white)
right_fore_arm:scale(0.8, 2.5, 0.8)
right_fore_arm:scale(1.4, 1.4, 1.4)
right_fore_arm:translate(0.0, -3.0, 0.0)


---------------right_wrist-----------------
right_wrist = gr.mesh('sphere','right_wrist')
right_fore_arm:add_child(right_wrist)
right_wrist:set_material(white)
right_wrist:scale(1.25, 0.4, 1.25)
right_wrist:scale(0.5, 0.5, 0.5)
right_wrist:translate(0.0, -1.0, 0.0)


----------right_forearm_hand_joint-----
right_forearm_hand_joint = gr.joint('right_forearm_hand_joint', {-70, 0, 70}, {0, 0, 70});
right_wrist:add_child(right_forearm_hand_joint)

----------right_hand------------------
right_hand = gr.mesh('sphere','right_hand')
right_forearm_hand_joint:add_child(right_hand)
right_hand:set_material(white)
right_hand:scale(1.0, 2.0, 1.0)
right_hand:translate(0.0, -2.0, 0.3)

---------------------------right hand done here-----------------------------------------

-------------------------hips_rotate------------------
hips_rotate = gr.mesh('sphere','hips_rotate')
torso:add_child(hips_rotate)
hips_rotate:set_material(red)
hips_rotate:scale(1.5, 0.5, 1.5)
hips_rotate:scale(0.1, 0.1, 0.1)
hips_rotate:translate(0.0, -1.0, 0.0)


---------------torso_hips_joint------------------------------
torso_hips_joint = gr.joint('torso_hips_joint', {-110, 0, 5}, {-15, 0, 15});
hips_rotate:add_child(torso_hips_joint)


--------------------------hips---------------------
hips = gr.mesh('sphere','hips')
torso_hips_joint:add_child(hips)
hips:set_material(white)
hips:scale(10.0, 8.0, 10.0)

-------------------------------left leg starts here-----------------------------------

----------left_leg_rotate------------------
left_leg_rotate = gr.mesh('sphere','left_leg_rotate')
hips:add_child(left_leg_rotate)
left_leg_rotate:set_material(red)
left_leg_rotate:scale(0.1, 0.1, 0.1)
left_leg_rotate:translate(-0.7, -0.3, 0.0)

-------------------------left_hips_thigh_joint---------------------------------
left_hips_thigh_joint = gr.joint('left_hips_thigh_joint', {-40, 0, 40}, {-60, 0, 60});
left_leg_rotate:add_child(left_hips_thigh_joint)


----------left_thigh------------------
left_thigh = gr.mesh('sphere','left_thigh')
left_hips_thigh_joint:add_child(left_thigh)
left_thigh:set_material(white)
left_thigh:translate(-0.2, -0.8, 0)
left_thigh:scale(10.0, 10.0, 10.0)
left_thigh:scale(0.55, 1.8, 0.55)

----------left_kneel---------------------------------

left_kneel = gr.mesh('sphere','left_kneel')
left_thigh:add_child(left_kneel)
left_kneel:set_material(white)
left_kneel:scale(0.3, 0.3, 0.3)
left_kneel:scale(1.7, 0.6, 1.7)
left_kneel:translate(0.0, -1.0, 0)

-------------------------left_thigh_calf_joint---------------------------------
left_thigh_calf_joint = gr.joint('left_thigh_calf_joint', {0, 0, 150}, {-30, 0, 30});
left_kneel:add_child(left_thigh_calf_joint)

----------left_calf------------------
left_calf = gr.mesh('sphere','left_calf')
left_thigh_calf_joint:add_child(left_calf)
left_calf:set_material(white)
left_calf:translate(0.0, -1.0, 0)
left_calf:scale(3.0, 3.0, 3.0)
left_calf:scale(0.45, 2.0, 0.45)

----------left_ankle------------------
left_ankle = gr.mesh('sphere','left_ankle')
left_calf:add_child(left_ankle)
left_ankle:set_material(white)
left_ankle:translate(0.0, -6.2, 0)
left_ankle:scale(0.33, 0.33, 0.33)
left_ankle:scale(2.1, 0.5, 2.1)

-------------------------left_calf_foot_joint---------------------------------
left_calf_foot_joint = gr.joint('left_calf_foot_joint', {-110, -90, 0}, {-110, 0, 110});
left_ankle:add_child(left_calf_foot_joint)

----------left_foot------------------
left_foot = gr.mesh('sphere','left_calf')
left_calf_foot_joint:add_child(left_foot)
left_foot:set_material(white)
left_foot:translate(0.0, -1.0, 0)
left_foot:scale(1.0, 2.0, 1.0)

-------------------------------------left leg ends here-----------------------

-----------------------------right leg starts here----------------------------

----------right_leg_rotate------------------
right_leg_rotate = gr.mesh('sphere','right_leg_rotate')
hips:add_child(right_leg_rotate)
right_leg_rotate:set_material(red)
right_leg_rotate:scale(0.1, 0.1, 0.1)
right_leg_rotate:translate(0.7, -0.3, 0.0)

-------------------------right_hips_thigh_joint---------------------------------
right_hips_thigh_joint = gr.joint('right_hips_thigh_joint', {-40, 0, 40}, {-60, 0, 60});
right_leg_rotate:add_child(right_hips_thigh_joint)


----------right_thigh------------------
right_thigh = gr.mesh('sphere','right_thigh')
right_hips_thigh_joint:add_child(right_thigh)
right_thigh:set_material(white)
right_thigh:translate(-0.2, -0.8, 0)
right_thigh:scale(10.0, 10.0, 10.0)
right_thigh:scale(0.55, 1.8, 0.55)

----------right_kneel---------------------------------

right_kneel = gr.mesh('sphere','right_kneel')
right_thigh:add_child(right_kneel)
right_kneel:set_material(white)
right_kneel:scale(0.3, 0.3, 0.3)
right_kneel:scale(1.7, 0.6, 1.7)
right_kneel:translate(0.0, -1.0, 0)

-------------------------right_thigh_calf_joint---------------------------------
right_thigh_calf_joint = gr.joint('right_thigh_calf_joint', {0, 0, 150}, {-30, 0, 30});
right_kneel:add_child(right_thigh_calf_joint)

----------right_calf------------------
right_calf = gr.mesh('sphere','right_calf')
right_thigh_calf_joint:add_child(right_calf)
right_calf:set_material(white)
right_calf:translate(0.0, -1.0, 0)
right_calf:scale(3.0, 3.0, 3.0)
right_calf:scale(0.45, 2.0, 0.45)

----------right_ankle------------------
right_ankle = gr.mesh('sphere','right_ankle')
right_calf:add_child(right_ankle)
right_ankle:set_material(white)
right_ankle:translate(0.0, -6.2, 0)
right_ankle:scale(0.33, 0.33, 0.33)
right_ankle:scale(2.1, 0.5, 2.1)

-------------------------right_calf_foot_joint---------------------------------
right_calf_foot_joint = gr.joint('right_calf_foot_joint', {-110, -90, 0}, {-110, 0, 110});
right_ankle:add_child(right_calf_foot_joint)

----------right_foot------------------
right_foot = gr.mesh('sphere','right_calf')
right_calf_foot_joint:add_child(right_foot)
right_foot:set_material(white)
right_foot:translate(0.0, -1.0, 0)
right_foot:scale(1.0, 2.0, 1.0)









rootnode:translate(0.0, 0.0, -20.0)

return rootnode
