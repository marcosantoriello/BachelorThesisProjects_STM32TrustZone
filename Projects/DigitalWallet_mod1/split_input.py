def split_string(input_string):

    part1 = input_string[:254]
    part2 = input_string[254:508]
    part3 = input_string[508:]

    return part1, part2, part3

def main():
    str = input("Insert the message to split:")

    part1, part2, part3 = split_string(str)

    print(f'Part 1: {part1}\n')
    print(f'Part 2: {part2}\n')
    print(f'Part 3: {part3}\n')

    print(f'Part 1 Length: {len(part1)}\n')
    print(f'Part 2 Length: {len(part2)}\n')
    print(f'Part 3 Length: {len(part3)}\n')




if __name__ == '__main__':
    main()