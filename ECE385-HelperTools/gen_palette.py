def generate_palette():
    num_list = ['F', 'C', '9', '6', '3', '0']
    with open('palette.txt', 'w') as f:
        for i in num_list:
            for j in num_list:
                for k in num_list:
                    f.write(f'{i}{j}{k}\n')
def generate_palette1():
    num_list = ['F', 'C', '9', '6', '3', '0']
    with open('palette1.txt', 'w') as f:
        for i in num_list:
            for j in num_list:
                for k in num_list:
                    f.write(f"'0x{i}0{j}0{k}0', ")

# def generate_palette2():
#     num_list = ['F', '0', '2', '4', '6', '8', 'A', 'C']
#     with open('palette2.txt', 'w') as f:
#         for i in num_list:
#             for j in num_list:
#                 for k in num_list:
#                     f.write(f'{i}0{j}0{k}0\n')
generate_palette()
generate_palette1()
# generate_palette2()
