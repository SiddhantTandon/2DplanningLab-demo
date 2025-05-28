
def load_map_file(filepath):
    file = filepath.split("/")[-1]
    if not file.lower().endswith('.txt'):
        print("Incorrect file format, please use txt")
        return ""
    else:
        size = file.split("_")[1]
        try:
            rowcol = size.split("x")
            print(f"row = {rowcol[0]}")
            print(f"col = {rowcol[1]}")
        except:
            print("Make sure map file is written in the format - map_16x32_*.txt")
            return ""
        
        with open(filepath, "r") as f:
            grid = [list(next(f).strip()) for _ in range(int(rowcol[0]))]
        
        return grid

def is_this_topleft_point(row, col, grid, obj_str):
    try:
        loc = grid[row-1][col]
    except IndexError:
        loc = 0
    
    if loc != obj_str:
        try:
            loc = grid[row][col-1]
        except IndexError:
            loc = 0
        
        if loc == obj_str:
            return False
    else:
        return False
    
    return True