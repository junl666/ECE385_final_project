from rembg import remove

# #待处理的图片路径
# input_path = 'char_still_r.jpg'
# #处理后存储的图片路径
# output_path = 'char_still_r_out.jpg'


input_paths = ["life_bar_5.jpg"]
output_paths = ["life_bar_5_out.jpg"]
for index in range(len(input_paths)):
    with open(input_paths[index], 'rb') as i:
        with open(output_paths[index], 'wb') as o:
            input = i.read()
            output = remove(input)
            o.write(output)