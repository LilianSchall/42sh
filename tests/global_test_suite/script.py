import os

def check_string(string):
    for c in string:
        if c not in [' ', '{', '}', '\n']:
            return False
    return True

def check_empty_line(file_path):
    with open(file_path, 'r') as f:
        lines = f.readlines()
        a = len(lines)
        b = len(lines[a-1])
        if(lines[a-1][b-1]  != "\n"):
            print(f'Error: File {file_path}, does not end with newline')

def check_function_length(file_path):
    with open(file_path, 'r') as f:
        lines = f.readlines()
        in_function = False
        function_line_count = 0
        open_bracket_count = 0
        closed_bracket_count = 0
        for line in lines:
            # Ignore empty lines
            if not line.strip():
                continue
            # Ignore commented lines
            if line.strip().startswith('//') or line.strip().startswith('/*') or line.strip().startswith('#'):
                continue
            # check if the line start with a character
            if line[len(line)-2] in [')'] and not in_function:
                in_function = True
                open_bracket_count = line.count('{')
                closed_bracket_count = line.count('}')
            elif in_function:
                if (not check_string(line)):
                    function_line_count += 1
                open_bracket_count += line.count('{')
                closed_bracket_count += line.count('}')
                if open_bracket_count == closed_bracket_count:
                    in_function = False
                    if function_line_count >= 40:
                        print(f'Error: Function in {file_path} has than {function_line_count} lines')
                    function_line_count = 0
                    open_bracket_count = 0
                    closed_bracket_count = 0

root_dir = '../../src'
for subdir, dirs, files in os.walk(root_dir):
    for file in files:
        if file.endswith('.c'):
            file_path = os.path.join(subdir, file)
            check_function_length(file_path)
            check_empty_line(file_path)
