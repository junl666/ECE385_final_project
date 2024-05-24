from PIL import Image, ImageDraw, ImageFont

# 创建一个200*200的新图片
img = Image.new('RGB', (200, 200), color = (73, 109, 137))

d = ImageDraw.Draw(img)

# 计算字体大小，使其占据图片宽度的40%
font_size = int(img.width * 0.4)

# 使用默认字体，但是设置字体大小
fnt = ImageFont.truetype('arial', font_size)

# 计算文本的宽度和高度
text = "WIN"
text_width, text_height = d.textsize(text, font=fnt)

# 计算文本的位置
x = (img.width - text_width) / 2
y = (img.height - text_height) / 2

# 在图片上绘制文本
d.text((x, y), text, font=fnt, fill=(255, 255, 255))

# 保存图片
img.save('win.jpg')


# 打开并调整"smile.jpg"的大小
smile = Image.open('smile.jpg')
smile = smile.resize((50, 50))

# 打开"win.jpg"
win = Image.open('win.jpg')

# 计算粘贴"smile.jpg"的位置
x = (win.width - smile.width) / 2
y = 0

# 将调整大小后的"smile.jpg"粘贴到"win.jpg"上
win.paste(smile, (int(x), int(y)))


# 保存合并后的图片
win.save('win_combined.jpg')