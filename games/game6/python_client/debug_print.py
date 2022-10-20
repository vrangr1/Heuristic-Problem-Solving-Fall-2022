def static_vars(**kwargs):
    def decorate(func):
        for k in kwargs:
            setattr(func, k, kwargs[k])
        return func
    return decorate

def open_file(filename, mode = 'r'):
    try:
        file = open(filename, mode)
        return file
    except FileNotFoundError:
        print_func(f'The file {filename} was not found. Exiting from the program.')
        exit(0)

@static_vars(outfile = None)
def print_func(message, end = "\n", init = None):
    message = str(message)
    if print_func.outfile is None and init is not None:
        print_func.outfile = open_file(init, 'w')
    if print_func.outfile is not None:
        print_func.outfile.write(message + end)
    else:
        print(message, end = end)