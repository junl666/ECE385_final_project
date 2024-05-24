import cv2
import numpy as np

# # Create a blank image with the desired size
# image = np.zeros((480, 640, 3), dtype=np.uint8)

# # Set each pixel to the desired color
# image[:] = (0xF0, 0xF0, 0xF0)

# # Save the image as a JPG file
# cv2.imwrite('output.jpg', image)

sprite = np.zeros((200, 300, 3), dtype=np.uint8)
sprite[:] = (0xF0, 0xF0, 0xF0)

# still right&left
input_stillRL_ = "../photos_out/char_still_r_out.jpg"
image = cv2.imread(input_stillRL_)
sprite[50: 100, 0: 30, :] = image[:, :, :]
sprite[50: 100, 30: 60, :] = image[:, ::-1, :]

# moving right&left
input_movingRL_list = ["../photos_out/char_r1_out.jpg", "../photos_out/char_r2_out.jpg", "../photos_out/char_r3_out.jpg"]
for i in range(len(input_movingRL_list)):
    image = cv2.imread(input_movingRL_list[i])
    sprite[50: 100, 0+30*(i+2): 30+30*(i+2), :] = image[:, :, :]
for i in range(len(input_movingRL_list)):
    image = cv2.imread(input_movingRL_list[i])
    sprite[50: 100, 0+30*(i+5): 30+30*(i+5), :] = image[:, ::-1, :]
    
# jump
input_jump_list = ["../photos_out/char_jump_r1_out.jpg", "../photos_out/char_jump_r2_out.jpg"]
for i in range(len(input_jump_list)):
    image = cv2.imread(input_jump_list[i])
    sprite[50: 100, 0+30*(i+8): 30+30*(i+8), :] = image[:, :, :]
for i in range(len(input_jump_list)):
    image = cv2.imread(input_jump_list[i])
    sprite[100: 150, 0+30*(i): 30+30*(i), :] = image[:, ::-1, :]
    
# attack
input_attack_list = ["../photos_out/char_attack_r_out.jpg"]
image = cv2.imread(input_attack_list[0])
sprite[100: 150, 0+30*(2): 30+30*(2), :] = image[:, :, :]
sprite[100: 150, 0+30*(3): 30+30*(3), :] = image[:, ::-1, :]

# hurt
input_hurt_list = ["../photos_out/char_hurt_r_out.jpg"]
image = cv2.imread(input_hurt_list[0])
sprite[100: 150, 0+30*(4): 30+30*(4), :] = image[:, :, :]
sprite[100: 150, 0+30*(5): 30+30*(5), :] = image[:, ::-1, :]
    
# ball
input_ball_list = ["../photos_out/ball_attack_r_out.jpg"]
image = cv2.imread(input_ball_list[0])
sprite[100: 150, 0+30*(8): 30+30*(8), :] = image[:, :, :]
sprite[100: 150, 0+30*(9): 30+30*(9), :] = image[:, ::-1, :]

#enemy moving
input_enemy_list = ["../photos_out/enemy_r1_out.jpg", "../photos_out/enemy_r2_out.jpg"]
for i in range(len(input_enemy_list)):
    image = cv2.imread(input_enemy_list[i])
    sprite[150: 200, 0+30*(i): 30+30*(i), :] = image[:, :, :]
for i in range(len(input_enemy_list)):
    image = cv2.imread(input_enemy_list[i])
    sprite[150: 200, 0+30*(i+2): 30+30*(i+2), :] = image[:, ::-1, :]
    
#enemy attack
input_enemy_attack_list = ["../photos_out/enemy_attack_r_out.jpg"]
image = cv2.imread(input_enemy_attack_list[0])
sprite[150: 200, 0+30*(4): 30+30*(4), :] = image[:, :, :]
sprite[150: 200, 0+30*(5): 30+30*(5), :] = image[:, ::-1, :]

#enemy hurt
input_enemy_hurt_list = ["../photos_out/enemy_hurt_r_out.jpg"]
image = cv2.imread(input_enemy_hurt_list[0])
sprite[150: 200, 0+30*(6): 30+30*(6), :] = image[:, :, :]
sprite[150: 200, 0+30*(7): 30+30*(7), :] = image[:, ::-1, :]

#life bar
input_life_bar_list = ["../photos_out/life_bar_5_out.jpg", "../photos_out/life_bar_4_out.jpg", "../photos_out/life_bar_3_out.jpg", "../photos_out/life_bar_2_out.jpg", "../photos_out/life_bar_1_out.jpg", "../photos_out/life_bar_0_out.jpg"]
for i in range(len(input_life_bar_list)):
    image = cv2.imread(input_life_bar_list[i])
    sprite[0: 50, 0+30*(i+1): 30+30*(i+1), :] = image[:, :, :]
cv2.imwrite('../sprite_originals/sprite.png', sprite)
    
